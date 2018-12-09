/*
    File:       main.cpp
    Date:       9.12.2018
    Project:    IMS - Corn Growth Simulation
    Authors:
                xdovic01@stud.fit.vutbr.cz
                xmeryj00@stud.fit.vutbr.cz
*/

#include <iostream>
#include <utility>

#include "ims/common.hpp"
#include "ims/corn_sim.hpp"

int main(int argc, char *argv[]) {

    try {

        IMS::CornSim sim(std::as_const(argc), std::as_const(argv));

        sim.run();

    } catch(IMS::FatalError& e) {

        std::cerr << "ERROR: " << e.what() << "\n";

        return e.exit_code();

    } catch(...) {

        std::cerr << "ERROR: Unknown error occured!\n";

        return IMS::ExitCode::INTERNAL_ERROR;
    }

    return 0;
}