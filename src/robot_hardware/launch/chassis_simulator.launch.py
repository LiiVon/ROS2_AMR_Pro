from launch import LaunchDescription                    # 描述整个启动文件要执行哪些操作
from launch.actions import DeclareLaunchArgument        # 声明启动参数，允许用户从命令行传入
from launch.substitutions import LaunchConfiguration    # 获取启动参数的实际值
from launch_ros.actions import Node                     # 定义需要启动的 ROS 2 节点


def generate_launch_description():
    bind_host = LaunchConfiguration("bind_host")
    port = LaunchConfiguration("port")
    battery_voltage = LaunchConfiguration("battery_voltage")
    return LaunchDescription(
        [
            DeclareLaunchArgument("bind_host", default_value="0.0.0.0"),
            DeclareLaunchArgument("port", default_value="9000"),
            DeclareLaunchArgument("battery_voltage", default_value="24.0"),
            Node(
                package="robot_hardware",
                executable="chassis_simulator_node",
                parameters=[
                    {
                        "bind_host": bind_host,
                        "port": port,
                        "battery_voltage": battery_voltage,
                    }
                ],
                output="screen",
            ),
        ]
    )
