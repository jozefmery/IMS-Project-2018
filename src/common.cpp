#include "ims/common.hpp"

IMS::FatalError::FatalError(const std::string& message, IMS::ExitCode exit_code) 
    : std::runtime_error(message), m_exit_code(exit_code)
{}