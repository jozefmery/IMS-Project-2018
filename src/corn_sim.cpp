#include <sstream>
#include <iostream>
#include <cassert>
#include <unordered_map>
#include <sstream>
#include <limits>

#include <simlib.h>

#include "ims/corn_sim.hpp"
#include "ims/arg_parse.hpp"
#include "ims/common.hpp"


class Field : public Process {

public /* rule of 0 */:

    Field() = delete;

    Field(const IMS::CornSim::Options& opts) 
        : Process(), 
        m_opts(opts),
        m_corn_yield(0u),
        m_min_grow_time(IMS::MIN_LEAF_GROW_TIME), 
        m_total_growth_time(0u)
        {}

public /* methods */:

    void Behavior() override {

        IMS::message("Entered behavior of Field object.", m_opts.verbose);

        initial_fertilize();

        IMS::message("Waiting 10 days before planting", m_opts.verbose);
        // wait to plant
        Wait(10 * IMS::DAY);
        m_total_growth_time += 10u;

        plant();

        // initial watering
        water(m_opts.initial_watering, 0.3);

        grow_leafs(5);

        // 5 leaf watering
        water(m_opts.five_leaf_watering, 0.2);

        grow_leafs(3);

        zinc_fertilize();

        grow_leafs(2);

        water(m_opts.ten_leaf_watering, 0.2);

        // wait before harvesting
        auto wait_harvest = static_cast<unsigned>(Uniform(m_min_grow_time * 2u, m_min_grow_time * 3u));

        std::stringstream ss;
        
        ss << "Waiting " << wait_harvest << " day(s) before harvesting.";

        IMS::message(ss.str(), m_opts.verbose);

        Wait(wait_harvest * IMS::DAY);
        m_total_growth_time += wait_harvest;

        ss.str("");
        ss << "Total growth time: " << m_total_growth_time << " days. Final corn yield: " << m_corn_yield << " kg.";

        IMS::message(ss.str(), true);
    }  

private /* methods */:

    void zinc_fertilize() {

        std::stringstream ss;

        IMS::message("Beginning zinc fertilization.", m_opts.verbose);

        ss << "Zinc fertilization is set to: " << ((m_opts.zinc_fertilize) ? "true" : "false") << ".";

        IMS::message(ss.str(), m_opts.verbose);

        auto initial_state = m_corn_yield;
        bool good_ph = m_opts.soil_ph >= 6.0f && m_opts.soil_ph <= 7.0f;

        int change = 0;
        double chance = 0.0;

        if(good_ph && m_opts.zinc_fertilize) {

            change = 1;
            chance = 0.2;

        } else if(good_ph || m_opts.zinc_fertilize) {

            change = 0;
            chance = 0.0;

        } else {

            change = -1;
            chance = 0.4;
        }

        for(unsigned long i = 0u; i < initial_state; i++) {

            if(Random() <= chance) {

                m_corn_yield += change;
            }
        }

        IMS::message("Zinc fertilization done.", m_opts.verbose);

        // clear string stream
        ss.str("");

        ss << "Expected corn yield after zinc fertilization: " << m_corn_yield << " kg.";

        IMS::message(ss.str(), m_opts.verbose);
    }

    void initial_fertilize() {

        std::stringstream ss;

        IMS::message("Beginning initial fertilization.", m_opts.verbose);

        ss << "Initial fertilization is set to: " << ((m_opts.initial_fertilize) ? "true" : "false") << ".";

        IMS::message(ss.str(), m_opts.verbose);

        for(int i = 0; i < m_opts.area; i++) {

            if(m_opts.initial_fertilize || Random() <= 0.5) {

                m_corn_yield++;
            }
        }

        IMS::message("Initial fertilization done.", m_opts.verbose);

        // clear string stream
        ss.str("");

        ss << "Fertile area after fertilization: " << m_corn_yield << " ares.";

        IMS::message(ss.str(), m_opts.verbose);
    }

    void grow_leafs(const int leafs) {

        for(int i = 0; i < leafs; i++) {
            
            auto one_leaf_time = Uniform(m_min_grow_time, m_min_grow_time + IMS::GROW_TIME_OFFSET);

            Wait(IMS::DAY * one_leaf_time);
            m_total_growth_time += one_leaf_time;
        } 

        std::stringstream ss;

        ss << "Current day of the process: " << m_total_growth_time << ".";

        IMS::message(ss.str(), m_opts.verbose);
    }

    void plant() {

        std::stringstream ss;

        IMS::message("Beginning corn planting.", m_opts.verbose);

        m_corn_yield *= Uniform(m_opts.ideal_yield - 15, m_opts.ideal_yield + 15);

        IMS::message("Corn planting done.", m_opts.verbose);

        ss << "Expected corn yield after planting: " << m_corn_yield << " kg.";

        IMS::message(ss.str(), m_opts.verbose);
    }

    void water(const bool should_water, const double destroy_chance) {

        IMS::message("Beginning watering.", m_opts.verbose);

        std::stringstream ss;

        ss << "Current watering is set to: " << ((should_water) ? "true" : "false") << ".";

        IMS::message(ss.str(), m_opts.verbose);

        ss.str("");

        ss.precision(1);

        ss << "Current minimal growth time is: " << std::fixed << m_min_grow_time << ".";

        IMS::message(ss.str(), m_opts.verbose);

        auto initial_state = m_corn_yield;

        if(!should_water) {
            
            // worsen yield
            for(unsigned long i = 0u; i < initial_state; i++) {

                if(Random() <= destroy_chance) {

                    m_corn_yield--;
                }
            }

            // increase grow time
            m_min_grow_time += IMS::GROW_TIME_DELTA;

        } else {

            // decrease grow time
            m_min_grow_time -= IMS::GROW_TIME_DELTA;
        }

        IMS::message("Watering done.", m_opts.verbose);

        // clear string stream
        ss.str("");

        ss << "Expected corn yield after watering: " << m_corn_yield << " kg.";

        IMS::message(ss.str(), m_opts.verbose);

        // clear string stream
        ss.str("");

        ss.precision(1);

        ss << "Current minimal growth time is: "  << std::fixed << m_min_grow_time << ".";

        IMS::message(ss.str(), m_opts.verbose);
    }

private /* members */:

    IMS::CornSim::Options m_opts;
    unsigned long m_corn_yield;

    float m_min_grow_time;

    unsigned m_total_growth_time;
};

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

        } else if(pair.first == "a") {
            
            options_all_on(opts);
        
        } else if(pair.first == "v") {
            
            opts.verbose = true;
        
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

            "    -v, --verbose          Print various information to stdout."                               "\n\n"

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

    m_opts = parse_options(opts);
}

void IMS::CornSim::run() {

    if(m_opts.print_help) {

        print_help();
        return;
    }

    init();

    message("Starting simulation...", true);

    // simlib Run
    Run();
}

void IMS::CornSim::init() {

    message("Initializing simulation.", m_opts.verbose);

    // randomize
    RandomSeed(time(NULL));

    // set simulation time
    Init(0, SIM_TIME_DAYS * DAY);

    std::stringstream ss;

    ss << "Setting simulation time to " << SIM_TIME_DAYS << " day(s).";

    message(ss.str(), m_opts.verbose);

    message("Creating Field object.", m_opts.verbose);

    (new Field(m_opts))->Activate();
}