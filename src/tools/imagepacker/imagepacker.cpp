#include "bufffileclass.h"
#include "rtsutils.h"
#include <cxxopts.hpp>

int main(int argc, char **argv)
{
    cxxopts::Options options("Imagepacker", "A commandline tool to pack multiple images");

    options.allow_unrecognised_options();

    // clang-format off
    options.add_options()
    ("i,input", "input folder", cxxopts::value<std::string>())
    ("o,output", "output folder", cxxopts::value<std::string>())
    ("x,xdim", "the width of the generated texture packs", cxxopts::value<int>()->default_value("1024"))
    ("y,ydim", "the height of the generated texture packs", cxxopts::value<int>()->default_value("1024"))
    ("h,help", "print usage")
    ("v,verbose", "verbose output", cxxopts::value<bool>()->default_value("false"))
    ;
    // clang-format on

    const auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (result.count("input") != 1) {
        std::cerr << "Please specify an input folder" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("output") != 1) {
        std::cerr << "Please specify an output folder" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
