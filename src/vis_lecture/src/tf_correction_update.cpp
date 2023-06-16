#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <tf/LinearMath/Vector3.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_correction_update");
    ros::NodeHandle node;
    // 创建 odometry 发布器
    ros::Publisher pose_pub;
    geometry_msgs::PoseWithCovarianceStamped init_pose;
    pose_pub = node.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 1);
    ros::Rate rate(1.0); // 更新频率为10Hz

    tf::TransformListener listener;

    std::string parent_frame = "map";
    std::string child_frame = "base_footprint";

    tf::StampedTransform map_to_base;
    tf::Vector3 map_to_base_origin;
    tf::Quaternion map_to_base_rotation;
    double roll, pitch, yaw;
    double rotation_angle = 0.05; // 變化角度

    while(ros::ok())
    {
        // listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(10.0));
        bool tfOK = listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(1.0));
        while(!tfOK)
        {
            tfOK = listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(1.0));
            ROS_INFO("tfOK: %d",tfOK);
        }

        try
        {
            listener.lookupTransform(parent_frame, child_frame, ros::Time(0), map_to_base);
            map_to_base_origin = map_to_base.getOrigin();
            map_to_base_rotation = map_to_base.getRotation();
        }
        catch (tf::TransformException& ex)
        {
            ROS_ERROR("Failed to obtain tf transformation: %s", ex.what());
        }
        // 获取旋转矩阵
        tf::Matrix3x3 rotation_matrix(map_to_base_rotation);
        // 计算旋转角度（欧拉角）
        rotation_matrix.getRPY(roll, pitch, yaw);
        ROS_INFO("x: %f, y: %f, z: %f",map_to_base_origin.getX(),map_to_base_origin.getY(),map_to_base_origin.getZ());
        ROS_INFO("Roll: %f, Pitch: %f, Yaw: %f", roll, pitch, yaw);

        // 根据旋转角度计算B座標軸相对于A座標軸的位移向量
        map_to_base_rotation.setRPY(roll, pitch, (yaw+rotation_angle));
        ROS_INFO("x: %f, y: %f, z: %f, w: %f",map_to_base_rotation.getX(), map_to_base_rotation.getY(), map_to_base_rotation.getZ(), map_to_base_rotation.getW());
        ROS_INFO("--------------------------------------------------------");
        double x = (map_to_base_origin.getX()*cos(rotation_angle))-(map_to_base_origin.getY()*sin(rotation_angle));
        double y = (map_to_base_origin.getX()*sin(rotation_angle))+(map_to_base_origin.getY()*cos(rotation_angle));
        double z = map_to_base_origin.getZ();
        init_pose.header.frame_id = "map";
        init_pose.header.stamp = ros::Time::now();
        init_pose.pose.pose.position.x = x;
        init_pose.pose.pose.position.y = y;
        init_pose.pose.pose.position.z = z;
        init_pose.pose.pose.orientation.x = map_to_base_rotation.x();
        init_pose.pose.pose.orientation.y = map_to_base_rotation.y();
        init_pose.pose.pose.orientation.z = map_to_base_rotation.z();
        init_pose.pose.pose.orientation.w = map_to_base_rotation.w();
        // 发布 initialpose 消息
        pose_pub.publish(init_pose);

        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
