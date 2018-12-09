#pragma once
#include <stdexcept>
#include <string>
#include <set>
#include <iterator>
#include <vector>

constexpr auto PROGRAM_NAME = "cornsim"; 

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

    void message(const std::string& message, const bool verbose);
}

template <typename T>
std::ostream& operator<< (std::ostream& stream, const std::vector<T>& vec) {
    
    if(vec.empty()) {

        stream << "[]";
    }
    else {
    
        stream << '[';
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<T>(stream, ", "));
        stream << "\b\b]";
    }

    return stream;
}

template <typename T>
std::ostream& operator<< (std::ostream& stream, const std::set<T>& set) {
    
    if(set.empty()) {

        stream << "{}";
    }
    else {
    
        stream << '{';
        std::copy(set.begin(), set.end(), std::ostream_iterator<T>(stream, ", "));
        stream << "\b\b}";
    }

    return stream;
}