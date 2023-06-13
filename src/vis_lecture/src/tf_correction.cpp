#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "tf_correction");
    ros::NodeHandle node;

    tf::TransformListener listener;

    /* set tf parent is map */
    std::string map_parent = "/map";
    /* set tf child is truetagX */
    std::string truetag0_child = "/true_tag0";
    std::string truetag1_child = "/true_tag1";
    /* set tf child is tagX */
    std::string tag0_child = "/tag_0";
    std::string tag1_child = "/tag_1";

    /* wait tf transform is effective */
    listener.waitForTransform(map_parent, truetag0_child, ros::Time(), ros::Duration(1.0));
    listener.waitForTransform(map_parent, truetag1_child, ros::Time(), ros::Duration(1.0));
    listener.waitForTransform(map_parent, tag0_child, ros::Time(), ros::Duration(1.0));
    listener.waitForTransform(map_parent, tag1_child, ros::Time(), ros::Duration(1.0));

    tf::StampedTransform tag0_true,tag1_true,tag0_car,tag1_car;
    try
    {
        /* get tag tf transform */
        listener.lookupTransform(map_parent, truetag0_child, ros::Time(0), tag0_true);
        listener.lookupTransform(map_parent, truetag1_child, ros::Time(0), tag1_true);
        listener.lookupTransform(map_parent, tag0_child, ros::Time(0), tag0_car);
        listener.lookupTransform(map_parent, tag1_child, ros::Time(0), tag1_car);

        /* 取得tag的xyz3軸對map的座標 */
        tf::Vector3 translation_tag0_true = tag0_true.getOrigin();
        tf::Vector3 translation_tag1_true = tag1_true.getOrigin();
        tf::Vector3 translation_tag0_car = tag0_car.getOrigin();
        tf::Vector3 translation_tag1_car = tag1_car.getOrigin();

        /* 打印座標 */
        ROS_INFO("------------------------------------------------------");
        ROS_INFO("tag0_true : x = %f, y = %f, z = %f",translation_tag0_true.getX(), translation_tag0_true.getY(), translation_tag0_true.getZ());
        ROS_INFO("tag1_true : x = %f, y = %f, z = %f",translation_tag1_true.getX(), translation_tag1_true.getY(), translation_tag1_true.getZ());
        ROS_INFO("tag0_car  : x = %f, y = %f, z = %f",translation_tag0_car.getX(), translation_tag0_car.getY(), translation_tag0_car.getZ());
        ROS_INFO("tag1_car  : x = %f, y = %f, z = %f",translation_tag1_car.getX(), translation_tag1_car.getY(), translation_tag1_car.getZ());
        ROS_INFO("------------------------------------------------------");

        /* 計算 truetag & cartag 方向向量 */
        tf::Vector3 truetag_direction = translation_tag1_true - translation_tag0_true;
        tf::Vector3 cartag_direction  = translation_tag1_car - translation_tag0_car;

        /* 計算 truetag & cartag 角度 */
        double truetag_angle = atan2(truetag_direction.getY(), truetag_direction.getX()) * 180.0 / M_PI;
        double cartag_angle = atan2(cartag_direction.getY(), cartag_direction.getX()) * 180.0 / M_PI;

        /* 計算角度誤差 */
        double angle_error = truetag_angle - cartag_angle;
        if (angle_error > 180.0)        angle_error -= 360.0;
        else if (angle_error < -180.0)  angle_error += 360.0;

        /* 打印計算結果 */
        ROS_INFO("Line 1 Angle: %f degrees", truetag_angle);
        ROS_INFO("Line 2 Angle: %f degrees", cartag_angle);
        ROS_INFO("Angle Error: %f degrees", angle_error);

    }
    catch (tf::TransformException& ex)
    {
        ROS_ERROR("Failed to obtain tf transformation: %s", ex.what());
    }

    return 0;
}
