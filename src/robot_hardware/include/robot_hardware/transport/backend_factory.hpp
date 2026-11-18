#pragma once

#include <memory>
#include <string>

#include "robot_hardware/transport/backend.hpp"

namespace robot_hardware::transport
{
    std::unique_ptr<Backend> CreateBackend(
        const std::string &backend, const BackendConfig &config);
}