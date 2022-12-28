#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_simulation1");
    ros::NodeHandle n;
    tf::TransformBroadcaster broadcaster;
    ros::Rate r(1);
    double x=0;
    while(n.ok())
    {
        broadcaster.sendTransform(
            tf::StampedTransform(
                tf::Transform(
                    tf::Quaternion(x, 0, 0, 1), 
                    tf::Vector3(0.1, 0.3, 0.2)),
                ros::Time::now(),
                "base_link", 
                "body1_link"));
        broadcaster.sendTransform(
            tf::StampedTransform(
                tf::Transform(
                    tf::Quaternion(0, 0, 0, 1), 
                    tf::Vector3(0.1, 0.1, 0.1)),
                ros::Time::now(),
                "body1_link", 
                "body2_link"));
        x = x + 0.02;
        r.sleep();
    }
}