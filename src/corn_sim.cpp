#include <sstream>

#include "ims/corn_sim.hpp"
#include "ims/arg_parse.hpp"
#include "ims/common.hpp"

IMS::CornSim::Options IMS::CornSim::parse_options(const ParsedArguments& args) {


}

std::string IMS::CornSim::get_usage() {

    std::stringstream ss;

    ss <<   PROGRAM_NAME <<
            " [-h | --help]"
            " [-v | --verbose]"
            " [-a | --all]"
            " [-n | --none]"
            " [--area=FIELD_AREA]"
            " [--fertilize]"
            " [--zinc-fertilize]"
            " [--water]"
            " [--water-5]"
            " [--water-10]"
            " [--soil-ph=SOIL_PH]";

    return ss.str();
}

std::string IMS::CornSim::get_help() {

}

std::string IMS::CornSim::get_examples() {

}

IMS::CornSim::CornSim(const int argc, const char* const argv[]) {

    ShortOpts short_ops = "hvan";
    LongOpts long_opts = {  "help",
                            "verbose",

                            "all",
                            "none",

                            "area=",

                            "fertilize",
                            "zinc-fertilize",

                            "water",
                            "water-5",
                            "water-10",

                            "soil-ph="
                        };

    ParsedArguments opts;

    try {

        opts = parse_arguments(argc, argv, short_ops, long_opts);

    } catch(std::runtime_error& e) {

        throw FatalError(e.what(), ExitCode::ARG_ERROR);
    }

    m_opts = parse_options(opts);
}

void IMS::CornSim::execute() {


}