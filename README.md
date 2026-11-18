``` text
source /opt/ros/jazzy/setup.bash

# 编译整个工作空间
colcon build

# 编译并创建符号链接（开发时推荐）
colcon build --symlink-install  

# 只编译指定功能包
colcon build --packages-select robot_hardware --cmake-args -DPython3_EXECUTABLE=/usr/bin/python3

# 编译指定包及其依赖
colcon build --packages-up-to robot_hardware

# 只编译发生变化的包及其依赖
colcon build --packages-up-to robot_hardware

# 显示详细编译信息
colcon build --event-handlers console_direct+  

# 测试整个工作空间
colcon test  

# 测试指定功能包
colcon test --packages-select robot_hardware

# 查看测试结果
colcon test-result --verbose

# 查看失败的测试
colcon test-result --all --verbose  

#############################################################
# 编译完后，加载工作空间环境，进行运行
source install/setup.bash

# 运行指定功能包中的节点
ros2 run <功能包名> <可执行文件名>

# 运行节点并设置参数
ros2 run <功能包名> <可执行文件名> --ros-args -p 参数名:=参数值

# 运行 Launch 文件
ros2 launch <功能包名> <launch文件名>
```