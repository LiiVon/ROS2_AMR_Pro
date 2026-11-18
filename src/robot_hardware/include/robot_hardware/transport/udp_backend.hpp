#pragma once

#include <netinet/in.h>

#include "robot_hardware/transport/backend.hpp"

namespace robot_hardware::transport
{
    using robot_hardware::protocol::Command;

    class UdpBackend final : public Backend
    {
    public:
        UdpBackend() = default;
        std::string Name() const override;
        BackendKind Kind() const override;

        bool Open(std::string *error) override;
        void Close() override;
        bool IsOpen() const override;

        bool WriteCommand(const Command &command, std::string *error) override;
        std::optional<std::string> Read(std::string *error) override;

    private:
        bool open_ = false;       // UDP 是否已打开
        int fd_ = -1;             // UDP socket 文件描述符
        sockaddr_in remote_addr_{}; // UDP 目标地址
        BackendConfig config_;    // 后端配置
    };
}