/*
    File:       common.cpp
    Date:       9.12.2018
    Project:    IMS - Corn Growth Simulation
    Authors:
                xdovic01@stud.fit.vutbr.cz
                xmeryj00@stud.fit.vutbr.cz
*/
#include <iostream>

#include "ims/common.hpp"


IMS::FatalError::FatalError(const std::string& message, IMS::ExitCode exit_code) 
    : std::runtime_error(message), m_exit_code(exit_code)
{}

void IMS::message(const std::string& message, const bool verbose) {
    
    if(verbose) {

        std::cout << "[SIM] " << message << "\n";
    }
}
