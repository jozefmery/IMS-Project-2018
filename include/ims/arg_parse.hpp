#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

namespace IMS {

    struct ParsedArguments {

        std::unordered_map<std::string, std::string> short_opts;
        std::unordered_map<std::string, std::string> long_opts;
        std::set<std::string> arbitary;
    };

    using ShortOpts = std::string;
    using LongOpts = std::vector<std::string>;

    ParsedArguments parse_arguments(const int argc, 
                                    const char* const argv[], 
                                    const ShortOpts short_opts,
                                    const LongOpts long_opts);
}

