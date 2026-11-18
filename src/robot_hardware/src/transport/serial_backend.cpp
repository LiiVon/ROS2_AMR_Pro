#include "robot_hardware/transport/serial_backend.hpp"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <utility>
#include <stdexcept>

namespace robot_hardware::transport
{
    using robot_hardware::protocol::Command;
    using robot_hardware::protocol::EncodeCommand;
    using robot_hardware::protocol::EncodeMickSpeedCommand;

    namespace
    {
        // 自己的 baud rate → Linux termios baud rate
        speed_t baudToTermios(int baud)
        {
            switch (baud)
            {
            case 9600:
                return B9600;
            case 19200:
                return B19200;
            case 38400:
                return B38400;
            case 57600:
                return B57600;
            case 115200:
                return B115200;
            case 230400:
                return B230400;
            case 460800:
                return B460800;
            case 921600:
                return B921600;
            default:
                throw std::invalid_argument("Unsupported baud rate: " + std::to_string(baud));
            }
        }

        // 配置 Linux 串口设备，让它按照指定的波特率、数据位、停止位等规则进行通信
        bool ConfigureSerialPort(const int fd, const int baud, std::string *error)
        {
            // Linux 保存串口配置的一张“配置表”， 创建一份串口配置
            termios tty{};

            // 把当前串口的配置读取出来，放进 tty
            if (tcgetattr(fd, &tty) != 0)
            {
                if (error)
                {
                    *error = "Failed to get serial port attributes: " + std::string(std::strerror(errno));
                }
                return false;
            }

            // 把串口设置成“原始数据模式”，不进行任何处理
            cfmakeraw(&tty);
            const speed_t speed = baudToTermios(baud);

            // 设置波特率 ,输入输出的波特率
            cfsetispeed(&tty, speed);
            cfsetospeed(&tty, speed);

            // 设置串口格式
            tty.c_cflag |= static_cast<tcflag_t>(CLOCAL | CREAD);
            tty.c_cflag &= static_cast<tcflag_t>(~PARENB);
            tty.c_cflag &= static_cast<tcflag_t>(~CSTOPB);
            tty.c_cflag &= static_cast<tcflag_t>(~CSIZE);
            tty.c_cflag |= CS8;

            // 控制 read() 等待数据的行为，
            tty.c_cc[VMIN] = 0;
            tty.c_cc[VTIME] = 1;

            // 在内存里修改配置,把配置写回串口
            if (tcsetattr(fd, TCSANOW, &tty) != 0)
            {
                if (error != nullptr)
                {
                    *error = std::strerror(errno);
                }
                return false;
            }
            return true;
        }
    }
}