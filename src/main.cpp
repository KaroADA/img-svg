/**
 * @file main.cpp
 * @brief Entry point for the CLI Raster Vectorizer tool.
 */

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <print>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "kmeans.hpp"
#include "mock.hpp"
#include "types.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

/**
 * @brief Main function handling CLI arguments and program flow.
 */
int main(int argc, char* argv[]) {
  try {
    // Define available CLI options
    po::options_description desc(
        "img🦍svg Raster Graphics Vectorizer CLI tool\nAllowed "
        "options");
    desc.add_options()("help,h", "Print help message")(
        "output,o", po::value<std::string>()->default_value("output.svg"),
        "Path to output SVG file")("colors",
                                   po::value<int>()->default_value(16),
                                   "Target number of colors")(
        "min-area", po::value<int>()->default_value(10),
        "Minimum spot area in pixels")(
        "tolerance", po::value<double>()->default_value(2.0),
        "Error tolerance determining allowed curve deviation from "
        "edge pixels")(
        "corner-threshold", po::value<double>()->default_value(60.0),
        "Threshold angle determining whether to "
        "keep a sharp corner or smooth "
        "the path")("optimize", po::value<double>()->default_value(1.0),
                    "Controls the strength of the control point reduction "
                    "algorithm on generated curves")(
        "verbose,v", po::bool_switch()->default_value(false),
        "Enable verbose logging");

    // Positional argument for the input file
    po::options_description hidden("Hidden options");
    hidden.add_options()("input-file", po::value<std::string>()->required(),
                         "Input image");

    po::options_description all_options;
    all_options.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("input-file", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv)
                  .options(all_options)
                  .positional(p)
                  .run(),
              vm);

    if (vm.count("help") || argc == 1) {
      std::cout << "Usage: img-svg [INPUT_FILE] [OPTIONS]\n"
                << desc << std::endl;
      return 0;
    }

    po::notify(vm);  // Validates required arguments

    auto img_path = vm["input-file"].as<std::string>();
    if (!fs::exists(img_path)) {
      std::cerr << "Error: File '" << img_path << "' doesn't exist."
                << std::endl;
      return 1;
    }
    if (!fs::is_regular_file(img_path)) {
      std::cerr << "Error: '" << img_path << "' is a directory." << std::endl;
      return 1;
    }
    Image img;
    int original_channel_count;
    img.data = stbi_load(img_path.c_str(), &img.w, &img.h,
                         &original_channel_count, STBI_rgb);
    if (img.data == nullptr) {
      std::cerr << "Error loading image: " << stbi_failure_reason()
                << std::endl;
      return 1;
    }

    Config config{.output_path = vm["output"].as<std::string>(),
                  .colors = vm["colors"].as<int>(),
                  .min_area = vm["min-area"].as<int>(),
                  .tolerance = vm["tolerance"].as<double>(),
                  .corner_threshold = vm["corner-threshold"].as<double>(),
                  .optimize = vm["optimize"].as<double>(),
                  .verbose = vm["verbose"].as<bool>()};

    if (config.verbose) {
      std::println("Image: {}, Size: {}x{}, Original channels: {}", img_path,
                   img.w, img.h, original_channel_count);
    }

    Stage::KMeans::process(img, config);

    // Temporary mocks
    ImageRegions mock1 = mock_segmentation(img);
    ImageRegions mock2 = mock_segmentation_geom(img);

    stbi_image_free(img.data);

  } catch (const po::error& e) {
    std::cerr << "CLI Parsing Error: " << e.what() << std::endl;
    std::cerr << "Use -h or --help to display the help message." << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Unexpected Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
