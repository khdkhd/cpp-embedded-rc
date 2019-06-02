#include "generator.hpp"
#include "io.hpp"
#include "utils.hpp"

#include <iostream>

#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

int
main(int argc, char const **argv) {
    using boost::format;

    po::options_description options("Available options");
    options.add_options()
        ("help,h", po::bool_switch(), "Display help message and exit.")
        ("verbose,h", po::bool_switch(), "Enable verbose mode.")
        ("base,b", po::value<fs::path>()->default_value(fs::current_path()), "Set base directory value.")
        ("prefix,p", po::value<fs::path>()->default_value("/"), "Set base directory value.")
        ("name", po::value<std::string>()->default_value("binary_data"), "Set the resource name")
        ("output-dir,d", po::value<fs::path>()->default_value(fs::current_path()), "Set output directory.");

    po::options_description hidden;
    hidden.add_options()
        ("input-files", po::value<std::vector<fs::path> >(), "Set input files.");

    po::positional_options_description args;
    args.add("input-files", -1);

    po::options_description cli;
    cli.add(options).add(hidden);

    po::variables_map vm;
    po::store(
        po::command_line_parser(argc, argv).options(cli).positional(args).run(),
        vm
    );
    po::notify(vm);

    const auto usage = [&](std::ostream &out) -> std::ostream & {
        return out << "Usage: " << fs::path(argv[0]).stem().string() << " [OPTIONS] file [file...]"
            << "\n\n"
            << options;
    };

    const auto exists = [&](const fs::path &path) {
        const auto res = fs::is_regular_file(path);
        if (!res) {
            std::cerr << format("File '%s' does not exists or is not a regular file!") << std::endl;
        }
        return res;
    };

    if (vm["help"].as<bool>()) {
        std::cout << usage << std::endl;
        return 0;
    }

    if (vm.count("input-files") == 0) {
        std::cerr
            << "Error: Missing files!\n\n"
            << usage
            << std::endl;
        return 1;
    }

    const auto input_files = vm["input-files"].as<std::vector<fs::path>>();
    if (!std::all_of(std::begin(input_files), std::end(input_files), exists)) {
        return 1;
    }

    const auto output_dir = vm["output-dir"].as<fs::path>();
    if (fs::exists(output_dir) && !fs::is_directory(output_dir)) {
        std::cerr
            << format("Error: %s exists and is not a directory.") % output_dir
            << usage
            << std::endl;
        return 1;
    }

    fs::create_directories(output_dir);

    const auto name = vm["name"].as<std::string>();
    const auto base = vm["base"].as<fs::path>();
    const auto prefix = vm["prefix"].as<fs::path>();
    const auto cpp = output_dir/str(format("%s.cpp") % name);

    std::fstream cpp_output(cpp.string(), std::ios_base::out|std::ios_base::trunc);
    if (cpp_output) {
        khdkhd::rc::Generator generator(cpp_output, khdkhd::rc::Generator::Config{
            .base = base,
            .name = name,
            .prefix = prefix,
            .verbose = vm["verbose"].as<bool>(),
        });

        std::copy(
            std::begin(input_files),
            std::end(input_files),
            std::back_inserter(generator)
        );
    } else {
        std::cerr << format("Error: fail to open '%s' to write resource content!") << std::endl;
        return 1;
    }

    return 0;
}
