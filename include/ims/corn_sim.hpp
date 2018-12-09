#pragma once


namespace IMS {

    // forward declarations
    struct ParsedArguments;

    namespace Defaults {

        constexpr int AREA = 100; // ares

        constexpr bool INITIAL_FERTILIZE = false;
        constexpr bool ZINC_FERTILIZE = false;

        constexpr bool INITIAL_WATERING = false;
        constexpr bool FIVE_LEAF_WATERING = false;
        constexpr bool TEN_LEAF_WATERING = false;

        constexpr float SOIL_PH = 5.0f;

        constexpr int IDEAL_YIELD = 150; // kg
    }

    class CornSim {

    public /* rule of 0 */:

        CornSim() = delete;
        CornSim(const int argc, const char* const argv[]);

    private /* structures and classes */:
        
        struct Options {

            int area = Defaults::AREA;
            float soil_ph = Defaults::SOIL_PH;
            int ideal_yield = Defaults::IDEAL_YIELD;

            bool initial_fertilize = Defaults::INITIAL_FERTILIZE;
            bool zinc_fertilize = Defaults::ZINC_FERTILIZE;
            
            bool initial_watering = Defaults::INITIAL_WATERING;
            bool five_leaf_watering = Defaults::FIVE_LEAF_WATERING;
            bool ten_leaf_watering = Defaults::TEN_LEAF_WATERING;

            bool print_help = false;
            bool verbose = false;
        };

    public /* methods */:

        void execute();

    private /* static methods */:

        static Options parse_options(const ParsedArguments& args);

        static void print_help();

        static inline std::string get_usage();
        static inline std::string get_help();

        static inline void options_all_on(Options& opts);

    private /* methods */:

    private /* members */:

        Options m_opts;

    };
}