#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <tf/LinearMath/Vector3.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_correction_update");
    ros::NodeHandle node;

    tf::TransformBroadcaster broadcaster;
    tf::TransformListener listener;

    std::string parent_frame = "odom";
    std::string child_frame = "base_footprint";

    ros::Rate rate(1.0); // 更新频率为10Hz

    double rotation_angle = 0.1; // 變化角度
    tf::StampedTransform map_to_base;
    tf::Vector3 map_to_base_origin;
    tf::Quaternion map_to_base_rotation;
    double roll, pitch, yaw;


    // 创建旋转变换
    tf::Transform transform;

    while (node.ok())
    {
        listener.waitForTransform(parent_frame, child_frame, ros::Time(), ros::Duration(1.0));
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
        double x = (map_to_base_origin.getX()*cos(rotation_angle))-(map_to_base_origin.getY()*sin(rotation_angle));
        double y = (map_to_base_origin.getX()*sin(rotation_angle))+(map_to_base_origin.getY()*cos(rotation_angle));
        double z = map_to_base_origin.getZ();
        tf::Vector3 translation(x, y, z);
        map_to_base_rotation.setRPY(roll, pitch, (yaw+rotation_angle));
        transform.setRotation(map_to_base_rotation);
        transform.setOrigin(translation);
        // 广播新的座標軸转换关系
        broadcaster.sendTransform(tf::StampedTransform(transform, ros::Time::now(), parent_frame, child_frame));

        ROS_INFO("--------------------------------------------------------");
        rate.sleep();
        ros::spinOnce();
    }

    return 0;
}
