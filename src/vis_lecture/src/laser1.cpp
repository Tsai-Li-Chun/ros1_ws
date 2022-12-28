#include <stdint.h>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

#define num_index 100

int main(int argc, char **argv)
{
    ros::init(argc,argv,"laser1");
    ros::NodeHandle n;
    ros::Publisher pub_laserscan = n.advertise<sensor_msgs::LaserScan>("/laser_link/scan",1);

    double laser_frequency = 40;
    double ranges[num_index];
    double intensities[num_index];

    int count = 0;
    ros::Rate r(10);
    
    while(ros::ok())
    {
        for( uint8_t i=0;i<num_index;i++ )
        {
            ranges[i] = count;
            intensities[i] = 100+count;
        }
        ros::Time scan_time = ros::Time::now();
        sensor_msgs::LaserScan laserscan;
        laserscan.header.stamp = scan_time;
        laserscan.header.frame_id = "laser_link";
        laserscan.angle_max = 0.53;
        laserscan.angle_min = 0;
        laserscan.angle_increment = 0.53 / num_index;
        laserscan.time_increment = (1/laser_frequency)/num_index;
        laserscan.range_max = 100;
        laserscan.range_min = 0;
        laserscan.ranges.resize(num_index);
        laserscan.intensities.resize(num_index);
        for( uint8_t i=0;i<num_index;i++ )
        {
            laserscan.ranges[i] = 10;
            laserscan.intensities[i] = 200;
        }
        pub_laserscan.publish(laserscan);
        ++count;
        r.sleep();
    }

    return 0;
}
