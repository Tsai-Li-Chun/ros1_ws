#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
#include "stdio.h"

geometry_msgs::Twist cmd_vel;

void joy_callback(const sensor_msgs::Joy&);

int main(int argc,char **argv)
{

    ros::init(argc,argv,"joy_control");
    ros::NodeHandle n;

    ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/cmd_vel",1);

    ros::Subscriber sub_joy    = n.subscribe("/joy",1,joy_callback);

    ros::Rate loop_rate(10);

    while( ros::ok() )
    {
        pub_cmd_vel.publish(cmd_vel);
        
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

void joy_callback( const sensor_msgs::Joy &joy_msg )
{
    cmd_vel.linear.x = joy_msg.axes[1];
    cmd_vel.linear.y = joy_msg.axes[0];
    cmd_vel.angular.z= joy_msg.axes[2];
}