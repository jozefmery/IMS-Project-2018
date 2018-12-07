#pragma once


namespace IMS {

    // forward declarations
    struct ParsedArguments;

    namespace Defaults {

        constexpr int AREA = 100; // ares

        constexpr bool INITIAL_FERTILIZE = true;
        constexpr bool ZINC_FERTILIZE = true;

        constexpr bool INITIAL_WATERING = true;
        constexpr bool FIVE_LEAF_WATERING = true;
        constexpr bool TEN_LEAF_WATERING = true;

        constexpr float SOIL_PH = 6.0f;
    }

    class CornSim {

    public /* rule of 0 */:

        CornSim() = delete;
        CornSim(const int argc, const char* const argv[]);

    private /* structures and classes */:
        
        struct Options {

            int area = Defaults::AREA;

            bool initial_fertilize = Defaults::INITIAL_FERTILIZE;
            bool zinc_fertilize = Defaults::ZINC_FERTILIZE;
            
            bool initial_watering = Defaults::INITIAL_WATERING;
            bool five_leaf_watering = Defaults::FIVE_LEAF_WATERING;
            bool ten_leaf_watering = Defaults::TEN_LEAF_WATERING;

            float soil_ph = Defaults::SOIL_PH;
        };

    public /* methods */:

        void execute();

    private /* static methods */:

        static Options parse_options(const ParsedArguments& args);

        static void print_help();

        static inline std::string get_usage();
        static inline std::string get_help();
        static inline std::string get_examples();

    private /* methods */:

    private /* members */:

        Options m_opts;

    };
}