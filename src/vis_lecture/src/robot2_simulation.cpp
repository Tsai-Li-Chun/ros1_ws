#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

#define theta_to_rad(theta) theta*0.00872664611

int main(int argc, char **argv)
{

    ros::init(argc, argv, "robot1_simulation_joint_publisher");
    ros::NodeHandle n;
    ros::Publisher pub_joint = n.advertise<sensor_msgs::JointState>("/joint_states",1);
    ros::Rate rate(10);
    sensor_msgs::JointState wheel;
    double rad = 0;
    double theta = 0;

    while(ros::ok())
    {   
        rad = theta_to_rad(theta);
        wheel.header.stamp = ros::Time::now();
        wheel.name.resize(2);
        wheel.name[0] = "base_to_wheel_L";
        wheel.name[1] = "base_to_wheel_R";
        wheel.position.resize(2);
        for(uint8_t i=0;i<2;i++ )
            wheel.position[i] = rad;
        pub_joint.publish(wheel);
        theta = theta+1.0;
        ROS_INFO("%lf\t\t%lf\n",theta,rad);
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}