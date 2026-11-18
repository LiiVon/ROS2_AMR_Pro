//  chassis_packet 机器人底盘硬件的 通信数据包定义与协议编解码
#pragma once

#include <cstdint>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace robot_hardware::protocol
{
    // 底盘指令
    struct Command
    {
        double linear_x_mps = 0.0;    // x方向线速度 m/s
        double linear_y_mps = 0.0;    // y方向线速度 m/s
        double angular_z_radps = 0.0; // 绕z轴角速度 rad/s
    };

    // 底盘里程计数据
    struct OdometryPacket
    {
        double x_m = 0.0;     // x坐标
        double y_m = 0.0;     // y坐标
        double yaw_rad = 0.0; // 航向角  x为正前方，y为左侧90方向，yaw为逆时针旋转角度

        double linear_x_mps = 0.0;    // x方向线速度 m/s
        double linear_y_mps = 0.0;    // y方向线速度 m/s
        double angular_z_radps = 0.0; // 绕z轴角速度 rad/s

        std::optional<double> battery_voltage; // 电池电压 V
    };

    // 底盘状态数据
    struct StatePacket
    {
        std::string status;                    // 底盘状态描述
        std::optional<double> battery_voltage; // 电池电压 V
    };

    // 数据包类型
    // 强类型枚举，没有指定底层类型，C++ 标准规定其底层类型默认为 int，0 1 2 开始
    enum class PacketKind : int
    {
        kOdometry = 0, // 里程计数据包
        kState = 1,    // 状态数据包
    };

    // 统一的数据包容器
    struct Packet
    {
        PacketKind kind = PacketKind::kState; // 数据包类型
        OdometryPacket odometry;              // 里程计数据包
        StatePacket state;                    // 状态数据包
    };

    // ======================== 文本协议的编码与解码 =========================
    // 内部数据结构  <-->  文本协议
    std::string EncodeCommand(const Command &command);
    std::string EncodeOdometry(const OdometryPacket &odometry);
    std::string EncodeState(const StatePacket &state);

    std::optional<Command> DecodeCommand(const std::string &text);
    std::optional<Packet> DecodePacket(const std::string &text);

    // ======================== 二进制协议的编码与解码 =========================
    // 内部数据结构  <-->  二进制协议数据
    struct BinaryFrame
    {
        uint8_t type = 0;          // 数据包类型
        std::vector<uint8_t> data; // 二进制数据包内容
    };

    using robot_hardware::protocol::Command;

    // 将帧类型和数据内容组装成二进制数据帧
    std::vector<uint8_t> EncodeBinaryFrame(const uint8_t type, const std::vector<uint8_t> &data);

    // 将底盘速度指令转换成 Mick 协议所需要的二进制数据
    std::vector<uint8_t> EncodeMickSpeedCommand(const Command &command);

    // 生成清零里程计的二进制控制指令
    std::vector<uint8_t> EncodeMickClearOdometryCommand();

    // 将解析好的二进制帧解码成统一的 Packet 对象
    std::optional<Packet> DecodeMickBinaryPacket(const BinaryFrame &frame);

    // ======================== 文本数据流解析器 =========================
    class PacketStream
    {
    public:
        explicit PacketStream(std::size_t max_buffer_size = 4096);

        // 向解析器中追加数据
        void Append(const std::string &data);
        void Append(const char *data, std::size_t size);

        // 取出完整数据包
        std::vector<Packet> DrainPackets();

        // 检查是否发生缓冲区溢出
        bool Overflowed() const;

        // 获取解析过程中发生错误的行数
        std::size_t InvalidLineCount() const;

        // 清除溢出标记、清除无效帧计数，清空整个解析器状态
        void ClearOverflow();
        void ClearInvalidLineCount();
        void Clear();

    private:
        std::size_t max_buffer_size_;        // 最大缓冲区大小
        std::string buffer_;                 // 缓冲区，用于存储未解析的文本数据
        bool overflowed_ = false;            // 是否发生缓冲区溢出
        std::size_t invalid_line_count_ = 0; // 解析过程中发生错误的行数
    };

    // ======================== 二进制数据流解析器 =========================
    class BinaryPacketStream
    {
    public:
        explicit BinaryPacketStream(std::size_t max_buffer_size = 4096);

        // 向解析器中追加数据
        void Append(const char *data, std::size_t size);
        void Append(const std::string &data);

        // 取出完整数据包
        std::vector<Packet> DrainPackets();

        // 检查是否发生缓冲区溢出
        bool Overflowed() const;

        // 获取解析过程中发生错误的帧数
        std::size_t InvalidFrameCount() const;

        // 清除溢出标记、清除无效帧计数，清空整个解析器状态
        void ClearOverflow();
        void ClearInvalidFrameCount();
        void Clear();

    private:
        std::size_t max_buffer_size_;         // 最大缓冲区大小
        std::vector<uint8_t> buffer_;         // 缓冲区，用于存储未解析的二进制数据
        bool overflowed_ = false;             // 是否发生缓冲区溢出
        std::size_t invalid_frame_count_ = 0; // 解析过程中发生错误的帧数
    };
}