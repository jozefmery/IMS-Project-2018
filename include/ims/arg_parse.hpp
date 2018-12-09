#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace IMS {

    using ArgPair = std::pair<std::string, std::string>;

    struct ParsedArguments {

        std::vector<ArgPair> short_opts;
        std::vector<ArgPair> long_opts;
        std::vector<std::string> arbitary;
    };

    using ShortOpts = std::string;
    using LongOpts = std::vector<std::string>;
    using Args = std::vector<std::string>;

    ParsedArguments parse_arguments(const int argc, 
                                    const char* const argv[], 
                                    const ShortOpts& short_opts,
                                    const LongOpts& long_opts);
}

