#pragma once
#include <stdexcept>
#include <string>

namespace IMS {

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
}