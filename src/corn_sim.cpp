#include "ims/corn_sim.hpp"
#include "ims/arg_parse.hpp"

IMS::CornSim::CornSim(const int argc, const char* const argv[]) {

    ShortOpts short_ops = "";
    LongOpts long_opts = {};

    auto opts = parse_arguments(argc, 
                                argv,
                                short_ops,
                                long_opts);


}

void IMS::CornSim::execute() {


}