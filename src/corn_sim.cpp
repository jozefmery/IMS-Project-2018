#include <sstream>
#include <iostream>
#include <cassert>

#include "ims/corn_sim.hpp"
#include "ims/arg_parse.hpp"
#include "ims/common.hpp"

void IMS::CornSim::options_all_on(Options& opts) {

    opts.initial_watering = true;
    opts.five_leaf_watering = true;
    opts.ten_leaf_watering = true;

    opts.initial_fertilize = true;
    opts.zinc_fertilize = true;
}   

IMS::CornSim::Options IMS::CornSim::parse_options(const ParsedArguments& args) {

    Options opts;

    // arbitary arguments are expected
    if(args.arbitary.size()) {

        throw FatalError("Unexpected argument: " + args.arbitary[0], ExitCode::ARG_ERROR);
    }

    // parse short arguments
    for(auto& pair : args.short_opts) {

        if(pair.first == "h") {
            
            opts.print_help = true;

            // no need to continue parsing
            return opts;

        } else if(pair.first == "v") {

            opts.verbose = true;

        } else if(pair.first == "a") {
            
            options_all_on(opts);
        
        } else {

            assert(false && "something went terribly wrong");
        }
    }

    // parse long arguments
    for(auto& pair : args.long_opts) {

        if(pair.first == "help") {

            opts.print_help = true;

            // no need to continue parsing
            return opts;

        } else if(pair.first == "verbose") {

            opts.verbose = true;

        } else if(pair.first == "all") {

            options_all_on(opts);

        } else if(pair.first == "area") {

            int area;
            
            try {
                // try to convert
                area = std::stoi(pair.second);

            } catch(std::invalid_argument& e) {

                throw FatalError("area argument expected a positive integer, got \"" + pair.second + "\" instead", ExitCode::ARG_ERROR);
            }   

            if(area <= 0) {
                
                throw FatalError("area cannot be 0 or a negative value", ExitCode::ARG_ERROR);
            }

            opts.area = area;

        } else if(pair.first == "soil-ph") {

            float pH;

            try {
                // try to convert
                pH = std::stof(pair.second);

            } catch(std::invalid_argument& e) {

                throw FatalError("soil-ph argument expected a float, got \"" + pair.second + "\" instead", ExitCode::ARG_ERROR);
            }   

            // check pH range
            if(pH < 0.0f || pH > 14.0f) {
                
                throw FatalError("soil-pH needs to be in <0.0, 14.0> range", ExitCode::ARG_ERROR);
            }

            opts.soil_ph = pH;

        } else if(pair.first == "ideal-yield") { 

            int yield;
            
            try {
                // try to convert
                yield = std::stoi(pair.second);

            } catch(std::invalid_argument& e) {

                throw FatalError("ideal-yield argument expected a positive integer, got \"" + pair.second + "\" instead", ExitCode::ARG_ERROR);
            }   

            // check range
            if(yield < 100 || yield > 200) {
                
                throw FatalError("ideal-yield should be in <100, 200> range", ExitCode::ARG_ERROR);
            }

            opts.ideal_yield = yield;

        } else if(pair.first == "fertilize") { 

            opts.initial_fertilize = true;

        } else if(pair.first == "zinc-fertilize") { 

            opts.zinc_fertilize = true;

        } else if(pair.first == "water") { 

            opts.initial_watering = true;

        } else if(pair.first == "water-5") { 

            opts.five_leaf_watering = true;

        } else if(pair.first == "water-10") { 

            opts.ten_leaf_watering = true;

        } else {

            assert(false && "something went terribly wrong");
        }
    }

    return opts;
}

std::string IMS::CornSim::get_usage() {

    std::stringstream ss;

    ss <<   PROGRAM_NAME <<
            " [-h | --help]"
            " [-v | --verbose]"
            " [-a | --all]"
            " [--area=VALUE]"
            " [--soil-ph=VALUE]"
            " [--ideal-yield=VALUE]"
            " [--fertilize]"
            " [--zinc-fertilize]"
            " [--water]"
            " [--water-5]"
            " [--water-10]";

    return ss.str();
}

std::string IMS::CornSim::get_help() {

    std::stringstream ss;

    // set float precision
    ss.precision(1);

    ss <<   "    -h, --help             Print this help message and exit."                                  "\n"
            "                           Ignores all other arguments."                                       "\n\n"

            "    -v, --verbose          Print various extra information to stdout."                         "\n\n"

            "    -a, --all              Turn ON all possible actions for the simulation."                   "\n\n"

            "    --area=VALUE           Specify the size of the simulated field in ares."                   "\n"
            "                           Positive integer expected."                                         "\n"
            "                           Default: " << Defaults::AREA << " ares."                            "\n\n"

            "    --soil-ph=VALUE        Specify the soil pH."                                               "\n"
            "                           Float in range <0.0, 14.0> expected."                               "\n" 
            "                           Default: " << std::fixed << Defaults::SOIL_PH << " pH"              "\n\n"

            "    --ideal-yield=VALUE    Specify the ideal yield of corn per are in kg."                     "\n"
            "                           Integer in range <100, 200> expected."                              "\n"
            "                           Default: " << Defaults::IDEAL_YIELD << " kg/are"                    "\n\n"

            "The following options are all OFF by default."                                                 "\n\n"

            "    --fertilize            Turn ON fertilizing before planting."                               "\n\n"

            "    --zinc-fertilize       Turn ON zinc fertilizing in the the eight leaf stage."              "\n\n"

            "    --water                Turn ON watering right after planting."                             "\n\n"

            "    --water-5              Turn ON watering in the five leaf stage."                           "\n\n"

            "    --water-10             Turn ON watering in the ten leaf stage.";

    return ss.str();
}

void IMS::CornSim::print_help() {

    std::cout << get_usage() << "\n\n" << get_help() << "\n\n";
}

IMS::CornSim::CornSim(const int argc, const char* const argv[]) {

    ShortOpts short_ops = "hva";
    LongOpts long_opts = {  "help",
                            "verbose",

                            "all",

                            "area=",
                            "soil-ph=",
                            "ideal-yield=",

                            "fertilize",
                            "zinc-fertilize",

                            "water",
                            "water-5",
                            "water-10"
                        };

    ParsedArguments opts;

    try {

        opts = parse_arguments(argc, argv, short_ops, long_opts);

    } catch(std::runtime_error& e) {

        throw FatalError(e.what(), ExitCode::ARG_ERROR);
    }

    // std::cout   << "shorts: [";

    // for(auto pair : opts.short_opts) {

    //     std::cout << "(" << pair.first << ", " << pair.second << "), ";
    // }

    // std::cout << "\b\b]\n";

    // std::cout   << "longs: [";

    // for(auto pair : opts.long_opts) {

    //     std::cout << "(" << pair.first << ", " << pair.second << "), ";
    // }

    // std::cout << "\b\b]\n";

    // std::cout << opts.arbitary << "\n";

    m_opts = parse_options(opts);
}

void IMS::CornSim::execute() {

    std::cout << m_opts.area << '\n';

    if(m_opts.print_help) {

        print_help();
        return;
    }
}