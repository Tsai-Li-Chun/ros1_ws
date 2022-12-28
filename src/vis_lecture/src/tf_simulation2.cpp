#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>

#define theta_to_rad(theta) theta*0.0174533

struct pose
{
    double origin_x;
    double origin_y;
    double origin_z;
    double rotation_x;
    double rotation_y;
    double rotation_z;
};

void set_joint_tf(pose, const char*, const char*);
pose set_pose(double,double,double,double,double,double);

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_simulation2");
    ros::NodeHandle n;
    tf::TransformBroadcaster broadcaster;
    ros::Rate r(10);
    double theta = 0;
    double x=0;
    while(ros::ok())
    {
        set_joint_tf(set_pose(0, x, 0.075, 0, 0, 0), "base_link", "body_link");
        set_joint_tf(set_pose(0, 0.125, -0.05, 0, 0, 0), "body_link", "ball_link");
        set_joint_tf(set_pose(0, 0.125, -0.05, 0, 0, 0), "body_link", "ball_link");
        set_joint_tf(set_pose(0, 0.13, 0.07, 0, 0, 90), "body_link", "laser_link");
        set_joint_tf(set_pose(0.125, -0.1, -0.025, -90, 0, theta), "body_link", "wheel_R");
        set_joint_tf(set_pose(-0.125, -0.1, -0.025, -90, 0, theta), "body_link", "wheel_L");
        theta = theta + 0.4;
        x=x+0.001;
        r.sleep();
    }
}

void set_joint_tf(pose p,const char *parent,const char *child)
{
    tf::TransformBroadcaster broadcaster;
    tf::Transform tf;
    tf::Vector3 v3;
    v3.setValue( p.origin_x, p.origin_y, p.origin_z );
    tf::Quaternion q;
    q.setEuler( theta_to_rad(p.rotation_x), theta_to_rad(p.rotation_y), theta_to_rad(p.rotation_z) );
    tf.setOrigin(v3);
    tf.setRotation(q);  
    broadcaster.sendTransform(
        tf::StampedTransform(
            tf,
            ros::Time::now(),
            parent, 
            child));
}

pose set_pose(double ox,double oy,double oz,double rx,double ry,double rz)
{
    pose p;
    p.origin_x = ox;
    p.origin_y = oy;
    p.origin_z = oz;
    p.rotation_x = rx;
    p.rotation_y = ry;
    p.rotation_z = rz;
    return p;
}