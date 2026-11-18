#include "robot_hardware/transport/backend_factory.hpp"

#include <stdexcept>

#include "robot_hardware/transport/mock_backend.hpp"
#include "robot_hardware/transport/serial_backend.hpp"
#include "robot_hardware/transport/udp_backend.hpp"

namespace robot_hardware::transport
{
    std::unique_ptr<Backend> CreateBackend(
        const std::string &backend,
        const BackendConfig &config)
    {
        if (backend == "mock")
        {
            return std::make_unique<MockBackend>();
        }

        if (backend == "serial")
        {
            return std::make_unique<SerialBackend>(config);
        }

        if (backend == "udp")
        {
            return std::make_unique<UdpBackend>(config);
        }

        throw std::invalid_argument(
            "Unsupported chassis backend: '" + backend +
            "'. Supported backends: mock, serial, udp.");
    }
}