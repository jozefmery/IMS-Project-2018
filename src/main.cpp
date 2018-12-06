#include <iostream>
#include <utility>

#include "ims/common.hpp"
#include "ims/corn_sim.hpp"

int main(int argc, char *argv[]) {

    try {

        IMS::CornSim sim(std::as_const(argc), std::as_const(argv));

        sim.execute();

    } catch(IMS::FatalError& e) {

        std::cerr << "ERROR: " << e.what() << "\n";

        return e.exit_code();

    } catch(...) {

        std::cerr << "ERROR: Unknown error occured!\n";

        return IMS::ExitCode::INTERNAL_ERROR;
    }

    return 0;
}