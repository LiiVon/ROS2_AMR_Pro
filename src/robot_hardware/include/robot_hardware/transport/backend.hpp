#pragma once

#include <optional>
#include <string>

#include "robot_hardware/protocol/packet.hpp"

namespace robot_hardware::transport
{
    using robot_hardware::protocol::Command;

    // 后端配置结构体，包含串口和 UDP 的配置参数
    struct BackendConfig
    {
        // 串口配置 通过 USB 转串口连接底盘
        std::string serial_device{"dev/ttyUSB0"};
        int serial_baud{115200};

        // UDP 配置 通过网络向底盘发送数据
        std::string udp_host{"192.168.1.10"};
        int udp_port{9000};

        // 底盘数据协议：
        // "text"       : 文本协议
        // "mick_binary": Mick 二进制协议
        std::string protocol{"text"};
    };

    // 后端类型枚举类，表示不同的通信后端类型
    enum class BackendKind
    {
        kMock,
        kSerial,
        kUdp,
    };

    // 将 BackendKind 枚举类型转换为字符串表示
    inline const char *ToString(BackendKind kind)
    {
        switch (kind)
        {
        case BackendKind::kMock:
            return "mock";
        case BackendKind::kSerial:
            return "serial";
        case BackendKind::kUdp:
            return "udp";
        default:
            return "unknown";
        }
    }

    // 后端抽象基类，定义了底盘通信的接口
    class Backend
    {
    public:
        virtual ~Backend() = default;

        virtual std::string Name() const = 0;
        virtual BackendKind Kind() const = 0;

        virtual bool Open(std::string *error) = 0;
        virtual void Close() = 0;
        virtual bool IsOpen() const = 0;

        // 向底盘发送运动指令。
        virtual bool WriteCommand(const Command &command, std::string *error) = 0;

        // 读取底盘通信数据。
        virtual std::optional<std::string> Read(std::string *error) = 0;
    };
}