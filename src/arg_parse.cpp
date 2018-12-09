#include <cassert>
#include <cctype>   
#include <algorithm>
#include <iostream>

#include "ims/common.hpp"
#include "ims/arg_parse.hpp"

enum ValueType : char {

    None, Optional = '?', Required = '='
};

using ParsedHelper = std::pair<std::vector<std::string>, std::vector<ValueType>>;

ParsedHelper parse_short_otps(const IMS::ShortOpts& opts) {

    ParsedHelper arg_set;

    for(size_t i = 0u; i < opts.size(); i++) {

        auto c = opts[i];

        // assert proper argument definition
        assert(isalpha(c) && "Invalid argument definition");
        
        // do not allow duplicate characters
        assert(opts.substr(i + 1).find(c) == std::string::npos && "Duplicate characted in argument definition");

        // default to none
        ValueType value = ValueType::None;

        // always safe to check + 1 thanks to null terminator 
        if(opts[i + 1] == '=' || opts[i + 1] == '?') {

            value = static_cast<ValueType>(opts[i + 1]);
            // advance index, skip parsing control characted
            i++;
        }

        arg_set.first.emplace_back(1, c);
        arg_set.second.emplace_back(value);
    }

    return arg_set;
}

ParsedHelper parse_long_opts(const IMS::LongOpts& opts) {

    ParsedHelper arg_set;

    for(auto str : opts) {

        // default to none
        ValueType value = ValueType::None;

        auto last_c = str[str.size() - 1];

        // check for control characters
        if(last_c == '=' || last_c == '?') {

            value = static_cast<ValueType>(last_c);
            str = str.substr(0, str.size() - 1);
        } 

        arg_set.first.emplace_back(std::move(str));
        arg_set.second.emplace_back(value);
    }

    return arg_set;
}

IMS::ParsedArguments IMS::parse_arguments(const int argc, 
                                    const char* const argv[], 
                                    const IMS::ShortOpts& short_opts,
                                    const IMS::LongOpts& long_opts) {                                  

    ParsedArguments parsed;                                

    Args args;

    // skip program path
    for(auto i = 1; i < argc; i++) {

        args.emplace_back(argv[i]);
    }

    auto parsed_short = parse_short_otps(short_opts);
    auto parsed_long  = parse_long_opts(long_opts);

    for(auto it = args.begin(); it != args.end(); it++) {

        // long argument
        if(it->size() > 1 && it->substr(0, 2) == "--") {
            
            // find equals in the argument
            auto eq_pos = it->find('=');

            std::string arg;
            std::string value;

            if(eq_pos == std::string::npos) {
                // extract argument
                arg = it->substr(2);

            } else {

                // extract argument
                arg = it->substr(2, eq_pos - 2);
                // extract value
                value = it->substr(eq_pos + 1);
            }

            auto arg_it = std::find(parsed_long.first.begin(), parsed_long.first.end(), arg);

            // handle invalid argument
            if(arg_it == parsed_long.first.end()) {

                throw std::runtime_error("Unknown argument: \"" + *it + "\"");
            }

            // get argument value type 
            auto value_type = parsed_long.second[arg_it - parsed_long.first.begin()];

            // validate argument value
            switch(value_type) {

                case ValueType::None:

                    // argument recieved an unexpected value
                    if(eq_pos != std::string::npos) {

                        throw std::runtime_error("Argument \"--" + arg + "\" does not take any value");
                    }
                    break;

                case ValueType::Required:

                    // argument did not recieve required value
                    if(eq_pos == std::string::npos || value.size() == 0u) {

                        throw std::runtime_error("Argument \"--" + arg + "\" did not recieve required value");
                    }
                    break;

                case ValueType::Optional:
                    
                    // argument recieved an empty value
                    if(eq_pos != std::string::npos && value.size() == 0u) {

                        throw std::runtime_error("Argument \"--" + arg + "\" recieved an empty value");
                    }
                    break;

                default:

                    assert(false && "Something went wrong! This should not have happened");
            }

            // emplace proper argument + value
            parsed.long_opts.emplace_back(std::move(arg), std::move(value));

        // short argument
        } else if((*it)[0] == '-') {
            
            if(it->size() == 1u) {

                throw std::runtime_error("Unkown argument: \"-\"");
            }

            // skip '-'
            for(size_t i = 1u; i < it->size(); i++) {
                
                std::string value;

                // create a string from char 
                auto arg = std::string(1, (*it)[i]);

                auto arg_it = std::find(parsed_short.first.begin(), parsed_short.first.end(), arg);

                // handle invalid argument
                if(arg_it == parsed_short.first.end()) {

                    throw std::runtime_error("Unkown argument: \"-" + arg + "\"");
                }

                // get argument value type 
                auto value_type = parsed_short.second[arg_it - parsed_short.first.begin()];

                switch(value_type) {

                    case ValueType::None:
                        // don't do anything
                        break;
                    
                    case ValueType::Required:
                    case ValueType::Optional: // optional is greedy

                        // extract value 
                        value = it->substr(i + 1);

                        // value is not part of this argument
                        if(value.size() == 0u) {
                            
                            // check if the next argument is a value
                            if(it + 1 != args.end() && (*(it + 1))[0] != '-') {
                                
                                // advance iterator
                                // don't parse next argument
                                it++;
                                // save next argument as value
                                value = *it;
                            }

                            // check value again
                            if(value.size() == 0u && value_type == ValueType::Required) {
                                
                                // handle missing value
                                throw std::runtime_error("Argument \"-" + arg + "\" did not recieve required value");
                            }
                        }

                        // stop parsing current argument
                        i = it->size();
                        break;


                    default:

                        assert(false && "Something went wrong! This should not have happened");
                }

                parsed.short_opts.emplace_back(std::move(arg), std::move(value));
            }

        // arbitary argument
        } else {

            parsed.arbitary.emplace_back(*it);
        }
    }

    return parsed;
}