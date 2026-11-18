#pragma once

#include <string>

#include "robot_hardware/transport/backend.hpp"

namespace robot_hardware::transport
{
    using robot_hardware::protocol::Command;

    class SerialBackend final : public Backend
    {
    public:
        explicit SerialBackend(BackendConfig config);
        ~SerialBackend() override;
        
        std::string Name() const override;
        BackendKind Kind() const override;

        bool Open(std::string *error) override;
        void Close() override;
        bool IsOpen() const override;

        bool WriteCommand(const Command &command, std::string *error) override;
        std::optional<std::string> Read(std::string *error) override;

    private:
        bool open_ = false;    // 串口是否已打开
        int fd_ = -1;          // 串口文件描述符
        BackendConfig config_; // 后端配置
    };
}