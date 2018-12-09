/*
    File:       common.hpp
    Date:       9.12.2018
    Project:    IMS - Corn Growth Simulation
    Authors:
                xdovic01@stud.fit.vutbr.cz
                xmeryj00@stud.fit.vutbr.cz
*/

#pragma once
#include <stdexcept>
#include <string>
#include <set>
#include <iterator>
#include <vector>

constexpr auto PROGRAM_NAME = "cornsim"; 

namespace IMS {

    // use as return value from main
    enum ExitCode : int {

        OK = 0,
        ARG_ERROR = 1,
        INTERNAL_ERROR = 99
    };

    class FatalError : public std::runtime_error {

    public /* rule of 0 */:

        FatalError() = delete;
        FatalError(const std::string& message, const ExitCode exit_code);

    public /* methods */:

        inline ExitCode exit_code() const { return m_exit_code; } 

    private /* members */:

        ExitCode m_exit_code;
    };

    void message(const std::string& message, const bool verbose);
}