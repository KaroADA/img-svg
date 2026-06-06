/**
 * @mainpage img🦍svg — Raster Graphics Vectorizer
 *
 * @section intro_sec Introduction
 * A command-line utility designed to convert raster images
 * into scalable and optimized vector graphics.
 *
 * @section pipeline_sec Processing Pipeline
 * The vectorization process consists of the following consecutive stages:
 * - @b Resize: Proportionally scales down images exceeding the maximum width.
 * - @b KMeans: Reduces image color depth through pixel clustering.
 * - @b UnionFind: Groups connected pixels of identical color and filters noise.
 * - @b EdgeDetection: Traces region boundaries to extract discrete polygons.
 * - @b PathOptimization: Reduces vertex count using the RDP algorithm.
 * - @b BezierSmoothing: Converts polygon edges into continuous Cubic Bezier curves.
 * - @b SvgOutput: Exports computed shapes into a formatted SVG file.
 */

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

#include "bezier_smoothing.hpp"
#include "edge_detection.hpp"
#include "kmeans.hpp"
#include "mock.hpp"
#include "path_optimization.hpp"
#include "resize.hpp"
#include "svg_output.hpp"
#include "types.hpp"
#include "union_find.hpp"

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
        "tolerance", po::value<double>()->default_value(1.0),
        "Error tolerance determining allowed curve deviation from "
        "edge pixels")(
        "corner-threshold", po::value<double>()->default_value(125.0),
        "Threshold angle determining whether to "
        "keep a sharp corner or smooth "
        "the path")("smoothing", po::value<double>()->default_value(1.0),
                    "Multiplier for curve smoothing intensity")(
        "max-width", po::value<int>()->default_value(1200),
        "Maximum image width, larger images will be scaled down")(
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
                  .max_width = vm["max-width"].as<int>(),
                  .colors = vm["colors"].as<int>(),
                  .min_area = vm["min-area"].as<int>(),
                  .tolerance = vm["tolerance"].as<double>(),
                  .corner_threshold = vm["corner-threshold"].as<double>(),
                  .smoothing = vm["smoothing"].as<double>(),
                  .verbose = vm["verbose"].as<bool>()};

    if (config.verbose) {
      std::println("Image: {}, Size: {}x{}, Original channels: {}", img_path,
                   img.w, img.h, original_channel_count);
    }

    Stage::Resize::process(img, config);
    QuantizedImage qimg = Stage::KMeans::process(img, config);
    ImageRegions regions = Stage::UnionFind::process(qimg, config);

    // Temporary mocks
    // ImageRegions mock1 = mock_segmentation(img);
    // ImageRegions mock2 = mock_segmentation_geom(img);

    auto polygons = Stage::EdgeDetection::process(regions, config);

    auto optimized_polygons =
        Stage::PathOptimization::process(polygons, config);

    auto smoothed_shapes =
        Stage::BezierSmoothing::process(optimized_polygons, config);

    Stage::SvgOutput::process(smoothed_shapes, img.w, img.h, config);

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
