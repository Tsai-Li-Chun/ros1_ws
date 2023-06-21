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
    /* set tf child is truetagX */
    std::string truetag0_child = "/true_tag0";
    std::string truetag1_child = "/true_tag1";
    /* set tf child is tagX */
    std::string tag0_child = "/tag_0";
    std::string tag1_child = "/tag_1";

    tf::StampedTransform map_to_base;
    tf::StampedTransform tag0_true,tag1_true,tag0_car,tag1_car;
    tf::Vector3 map_to_base_origin;
    tf::Vector3 translation_tag0_true;
    tf::Vector3 translation_tag1_true;
    tf::Vector3 translation_tag0_car;
    tf::Vector3 translation_tag1_car;
    tf::Quaternion map_to_base_rotation;
    double roll, pitch, yaw;
    double rotation_angle = 0.05; // 變化角度
    uint8_t tfOK=0;

    while(ros::ok())
    {
        // listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(10.0));
        tfOK=0;
        tfOK |= (listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
        tfOK |= (listener.waitForTransform(parent_frame, truetag0_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
        tfOK |= (listener.waitForTransform(parent_frame, truetag1_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
        tfOK |= (listener.waitForTransform(parent_frame, tag0_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
        tfOK |= (listener.waitForTransform(parent_frame, tag1_child, ros::Time(0), ros::Duration(1.0)));
        ROS_INFO("tfOK: %02x",tfOK);
        while( tfOK!=(0x1F) )
        {
            tfOK=0;
            tfOK |= (listener.waitForTransform(parent_frame, child_frame, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
            tfOK |= (listener.waitForTransform(parent_frame, truetag0_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
            tfOK |= (listener.waitForTransform(parent_frame, truetag1_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
            tfOK |= (listener.waitForTransform(parent_frame, tag0_child, ros::Time(0), ros::Duration(1.0)));tfOK=(tfOK<<1);
            tfOK |= (listener.waitForTransform(parent_frame, tag1_child, ros::Time(0), ros::Duration(1.0)));
            ROS_INFO("tfOK: %02x",tfOK);
        }

        try
        {
            /* get tag tf transform */
            listener.lookupTransform(parent_frame, child_frame, ros::Time(0), map_to_base);
            listener.lookupTransform(parent_frame, truetag0_child, ros::Time(0), tag0_true);
            listener.lookupTransform(parent_frame, tag0_child, ros::Time(0), tag0_car);
            listener.lookupTransform(parent_frame, truetag1_child, ros::Time(0), tag1_true);
            listener.lookupTransform(parent_frame, tag1_child, ros::Time(0), tag1_car);

            /* 取得tag的xyz3軸對map的座標 */
            translation_tag0_true = tag0_true.getOrigin();
            translation_tag1_true = tag1_true.getOrigin();
            translation_tag0_car = tag0_car.getOrigin();
            translation_tag1_car = tag1_car.getOrigin();
            map_to_base_origin = map_to_base.getOrigin();
            map_to_base_rotation = map_to_base.getRotation();
        }
        catch (tf::TransformException& ex)
        {
            ROS_ERROR("Failed to obtain tf transformation: %s", ex.what());
        }

        /* 計算 truetag & cartag 方向向量 */
        tf::Vector3 truetag_direction = translation_tag1_true - translation_tag0_true;
        tf::Vector3 cartag_direction  = translation_tag1_car - translation_tag0_car;
        /* 計算 truetag & cartag 角度 */
        double truetag_angle = atan2(truetag_direction.getY(), truetag_direction.getX());
        double cartag_angle = atan2(cartag_direction.getY(), cartag_direction.getX());
        /* 計算角度誤差 */
        double angle_error = truetag_angle - cartag_angle;
        if (angle_error > 3.1415926536)        angle_error -= 6.2831853072;
        else if (angle_error < -3.1415926536)  angle_error += 6.2831853072;

        /* 打印計算結果 */
        ROS_INFO("Line 1 Angle: %f degrees", truetag_angle);
        ROS_INFO("Line 2 Angle: %f degrees", cartag_angle);
        ROS_INFO("Angle Error: %f degrees", angle_error);

        // 获取旋转矩阵
        tf::Matrix3x3 rotation_matrix(map_to_base_rotation);
        // 计算旋转角度（欧拉角）
        rotation_matrix.getRPY(roll, pitch, yaw);
        ROS_INFO("x: %f, y: %f, z: %f",map_to_base_origin.getX(),map_to_base_origin.getY(),map_to_base_origin.getZ());
        ROS_INFO("Roll: %f, Pitch: %f, Yaw: %f", roll, pitch, yaw);

        // 根据旋转角度计算B座標軸相对于A座標軸的位移向量
        map_to_base_rotation.setRPY(roll, pitch, (yaw+angle_error));
        ROS_INFO("x: %f, y: %f, z: %f, w: %f",map_to_base_rotation.getX(), map_to_base_rotation.getY(), map_to_base_rotation.getZ(), map_to_base_rotation.getW());
        ROS_INFO("--------------------------------------------------------");
        double x = (map_to_base_origin.getX()*cos(angle_error))-(map_to_base_origin.getY()*sin(angle_error));
        double y = (map_to_base_origin.getX()*sin(angle_error))+(map_to_base_origin.getY()*cos(angle_error));
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
