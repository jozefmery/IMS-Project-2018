/*
    File:       arg_parse.hpp
    Date:       9.12.2018
    Project:    IMS - Corn Growth Simulation
    Authors:
                xdovic01@stud.fit.vutbr.cz
                xmeryj00@stud.fit.vutbr.cz
*/
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace IMS {

    using ArgPair = std::pair<std::string, std::string>;
    using ShortOpts = std::string;
    using LongOpts = std::vector<std::string>;
    using Args = std::vector<std::string>;

    // structure holding parsed arguments
    // short and long arguments are stored as argument-value pairs
    struct ParsedArguments {

        std::vector<ArgPair> short_opts;
        std::vector<ArgPair> long_opts;
        std::vector<std::string> arbitary;
    };

    // command line argument parser
    ParsedArguments parse_arguments(const int argc, 
                                    const char* const argv[], 
                                    const ShortOpts& short_opts,
                                    const LongOpts& long_opts);
}

