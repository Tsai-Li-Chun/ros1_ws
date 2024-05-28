#include <termios.h>
#include <signal.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/poll.h>
#include <chrono>
#include <functional>
#include <memory>

#include <boost/thread/thread.hpp>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#define KEYCODE_W 0x77
#define KEYCODE_A 0x61
#define KEYCODE_S 0x73
#define KEYCODE_D 0x64

#define KEYCODE_A_CAP 0x41
#define KEYCODE_D_CAP 0x44
#define KEYCODE_S_CAP 0x53
#define KEYCODE_W_CAP 0x57

using namespace std::chrono_literals;

int kfd = 0;
struct termios cooked, raw;
bool done;


class SmartCarKeyboardTeleopNode : public rclcpp::Node
{
    public:
        SmartCarKeyboardTeleopNode()
        : Node("SmartCarKeyboardTeleopNode")
        {
            this->declare_parameter<double>("walk_vel", 0.5);
            this->declare_parameter<double>("run_vel", 1.0);
            this->declare_parameter<double>("yaw_rate", 0.4);
            this->declare_parameter<double>("yaw_rate_run", 0.8);

            pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 100);
        
            timer_ = this->create_wall_timer(1s, std::bind(&SmartCarKeyboardTeleopNode::timer_callback, this));
        }
                
        double walk_vel_;
        double run_vel_;
        double yaw_rate_;
        double yaw_rate_run_;
        geometry_msgs::msg::Twist cmdvel_;
        void timer_callback();
        //~SmartCarKeyboardTeleopNode() {  }
        void keyboardLoop();
        void stopRobot();
    private:
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
        rclcpp::TimerBase::SharedPtr timer_;
};

//SmartCarKeyboardTeleopNode* tbk;

int main(int argc, char** argv)
{
    //rclcpp::init(argc,argv,"tbk", rclcpp::init_options::AnonymousName | rclcpp::init_options::NoSigintHandler);
    rclcpp::init(argc, argv);
    //SmartCarKeyboardTeleopNode tbk;

    //boost::thread t = boost::thread(boost::bind(&SmartCarKeyboardTeleopNode::keyboardLoop, &tbk));
    
    rclcpp::spin(std::make_shared<SmartCarKeyboardTeleopNode>());
    //t.interrupt();
    //t.join();
    //tbk.stopRobot();
    //tcsetattr(kfd, TCSANOW, &cooked);
    rclcpp::shutdown();
    return(0);
}

void SmartCarKeyboardTeleopNode::timer_callback()
{
    timer_->cancel();
    
    walk_vel_ = this->get_parameter("walk_vel").get_value<double>();
    run_vel_ = this->get_parameter("run_vel").get_value<double>();
    yaw_rate_ = this->get_parameter("yaw_rate").get_value<double>();
    yaw_rate_run_ = this->get_parameter("yaw_rate_run").get_value<double>();

    keyboardLoop();
    stopRobot();
    tcsetattr(kfd, TCSANOW, &cooked);
}

void SmartCarKeyboardTeleopNode::stopRobot()
{
    cmdvel_.linear.x = 0.0;
    cmdvel_.angular.z = 0.0;
    pub_->publish(cmdvel_);
}

void SmartCarKeyboardTeleopNode::keyboardLoop()
{
    char c;

    walk_vel_ = this->get_parameter("walk_vel").get_value<double>();
    run_vel_ = this->get_parameter("run_vel").get_value<double>();
    yaw_rate_ = this->get_parameter("yaw_rate").get_value<double>();
    yaw_rate_run_ = this->get_parameter("yaw_rate_run").get_value<double>();

    double max_tv = walk_vel_;
    double max_rv = yaw_rate_;
    bool dirty = false;
    int speed = 0;
    int turn = 0;

    // get the console in raw mode
    tcgetattr(kfd, &cooked);
    memcpy(&raw, &cooked, sizeof(struct termios));
    // raw.c_lflag &=~ (ICANON | ECHO);
    raw.c_lflag &=~ (ICANON);
    raw.c_cc[VEOL] = 1;
    raw.c_cc[VEOF] = 2;
    tcsetattr(kfd, TCSANOW, &raw);

    RCLCPP_INFO(this->get_logger(),"Reading from keyboard");
    RCLCPP_INFO(this->get_logger(),"Use WASD keys to control the robot");
    RCLCPP_INFO(this->get_logger(),"Press Shift to move faster");

    struct pollfd ufd;
    ufd.fd = kfd;
    ufd.events = POLLIN;

    for(;;)
    {
        //boost::this_thread::interruption_point();

        // get the next event from the keyboard
        int num;

        if ((num = poll(&ufd, 1, 250)) < 0)
        {
            perror("poll():");
            return;
        }
        else if(num > 0)
        {
            if(read(kfd, &c, 1) < 0)
            {
                perror("read():");
                return;
            }
        }
        else
        {
            if (dirty == true)
            {
                stopRobot();
                dirty = false;
            }

            continue;
        }

        switch(c)
        {
            case KEYCODE_W:
                max_tv = walk_vel_;
                speed = 1;
                turn = 0;
                dirty = true;
                break;
            case KEYCODE_S:
                max_tv = walk_vel_;
                speed = -1;
                turn = 0;
                dirty = true;
                break;
            case KEYCODE_A:
                max_rv = yaw_rate_;
                speed = 0;
                turn = 1;
                dirty = true;
                break;
            case KEYCODE_D:
                max_rv = yaw_rate_;
                speed = 0;
                turn = -1;
                dirty = true;
                break;

            case KEYCODE_W_CAP:
                max_tv = run_vel_;
                speed = 1;
                turn = 0;
                dirty = true;
                break;
            case KEYCODE_S_CAP:
                max_tv = run_vel_;
                speed = -1;
                turn = 0;
                dirty = true;
                break;
            case KEYCODE_A_CAP:
                max_rv = yaw_rate_run_;
                speed = 0;
                turn = 1;
                dirty = true;
                break;
            case KEYCODE_D_CAP:
                max_rv = yaw_rate_run_;
                speed = 0;
                turn = -1;
                dirty = true;
                break;
            default:
                max_tv = walk_vel_;
                max_rv = yaw_rate_;
                speed = 0;
                turn = 0;
                dirty = false;
        }
        cmdvel_.linear.x = (speed*max_tv);
        cmdvel_.angular.z = (turn*max_rv);
        RCLCPP_INFO(this->get_logger(),"linear_x = %f",cmdvel_.linear.x);
        RCLCPP_INFO(this->get_logger(),"angular_z = %f",cmdvel_.angular.z);
        pub_->publish(cmdvel_);        
    }
}