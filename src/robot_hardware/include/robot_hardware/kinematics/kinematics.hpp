#pragma once

#include <array>
#include <string>

#include "robot_hardware/protocol/packet.hpp"

namespace robot_hardware::kinematics
{
    using protocol::Command;

    // 机器人在世界坐标系（例如 odom）中的二维位姿。
    struct Pose2D
    {
        double x_m = 0.0;     // x坐标，单位米
        double y_m = 0.0;     // y坐标，单位米
        double yaw_rad = 0.0; // 航向角，单位弧度
    };

    // 四个轮子的转速，单位 rpm
    struct WheelSpeeds
    {
        // 0 = 左前轮，1 = 右前轮，
        // 2 = 左后轮，3 = 右后轮。
        std::array<double, 4> rpm{0.0, 0.0, 0.0, 0.0};
    };

    //  校验并返回规范的运动学模型名称。
    std::string NormalizeModel(const std::string &model_name);

    // 根据机器人当前位姿、机体坐标系速度和时间间隔，
    // 估算新的世界坐标系位姿。
    Pose2D IntegratePose(const Pose2D &pose, const Command &command, double dt_seconds);

    // 根据底盘速度计算四个车轮的转速。
    // wheel_base_m ：轴距：前后两个车轮轴线之间的距离。
    // track_width_m：轮距：左侧车轮和右侧车轮之间的距离。
    WheelSpeeds EstimateWheelSpeeds(const Command &command, const std::string &model_name,
                                    double wheel_diameter_m, double wheel_base_m, double track_width_m);

    // 根据四个车轮的转速，估算底盘线速度与角速度。
    Command EstimateChassisCommand(const WheelSpeeds &speeds, const std::string &model_name,
                                   double wheel_diameter_m, double wheel_base_m, double track_width_m);
}