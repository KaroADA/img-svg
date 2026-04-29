/**
 * @file main.cpp
 * @brief Entry point for the CLI Raster Vectorizer tool.
 */

#include <boost/program_options.hpp>
#include <iostream>
// TODO: Delete example
#include "example.hpp"

namespace po = boost::program_options;

/**
 * @brief Main function handling CLI arguments and program flow.
 */
int main(int argc, char *argv[]) {
  try {
    // Define available CLI options
    po::options_description desc(
        "img🦍svg Raster Graphics Vectorizer CLI tool\nAllowed options");
    desc.add_options()("help,h", "Print help message")(
        "output,o", po::value<std::string>()->default_value("output.svg"),
        "Path to output SVG file")("colors",
                                   po::value<int>()->default_value(16),
                                   "Target number of colors")(
        "min-area", po::value<int>()->default_value(10),
        "Minimum spot area in pixels")(
        "tolerance", po::value<double>()->default_value(2.0),
        "Error tolerance determining allowed curve deviation from edge pixels")(
        "corner-threshold", po::value<double>()->default_value(60.0),
        "Threshold angle determining whether to keep a sharp corner or smooth "
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

    po::notify(vm); // Validates required arguments

    // TODO: Implement the actual raster to SVG conversion logic here using the
    // parsed options

    // TODO: Delete example
    uint32_t num = 5;
    std::cout << num << "! = " << factorial(num) << "\n";

  } catch (const po::error &e) {
    std::cerr << "CLI Parsing Error: " << e.what() << std::endl;
    std::cerr << "Use -h or --help to display the help message." << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Unexpected Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
