/** ******************************************************
	* @file		main_tf_odometry.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include "tf2/LinearMath/Quaternion.h"
#include <nav_msgs/msg/odometry.hpp>
#include "motor_feedback_msgs/msg/motor_feedback.hpp"
/* User Includes End */

/* namespace ------------------------------------------------*/
/* namespace Begin */
/* namespace End */


/* Define ---------------------------------------------------*/
/* Define Begin */

#define low_pass_filter 0.6f

/* Define End */


/* Typedef --------------------------------------------------*/
/* Typedef Begin */
/* Typedef End */


/* Class --------------------------------------------------*/
/* Class Begin */
/* Class End */


/* Variables ------------------------------------------------*/
/* Variables Begin */

/* enc to mm to M parameter */
const double enc_to_mm = 0.0005599597921296297;
const double mm_to_M = 0.001;
const double AMR_width = 0.5476;
/* motor feedback message object */
motor_feedback_msgs::msg::MotorFeedback mf_last;
/* delta time(s) */
double dt;
/* X,Y,th 距離 */
double x = 0.0;
double y = 0.0;
double th = 0.0;
/* X,Y,th 距離變化量 */
double delta_x = 0;
double delta_y = 0;
double delta_th = 0;
/* X,Y,th 速率 */
double vx = 0;
double vy = 0;
double vth = 0;
double AvelocityL_M= 0;
double AvelocityR_M = 0;
double AvelocityL_lpf = 0;
double AvelocityR_lpf = 0;
/* 建立ROS time物件 */
rclcpp::Time current_time, last_time, init_time;
/* 建立計算旋轉勁度物件 */
geometry_msgs::msg::Quaternion odom_quat;
tf2::Quaternion q;
/* 建立tf廣播用變數物件 */
geometry_msgs::msg::TransformStamped odom_trans;
/* 建立odom里程計變數物件 */
nav_msgs::msg::Odometry odom;
// /* 建立odom里程計topic發布物件 */
// ros::Publisher odom_pub;
motor_feedback_msgs::msg::MotorFeedback encodom;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

/* motor feedback message object init */
void motor_info_init(void);
/* topic subscribe interrupt callback */
void motor_feedback_callback(const motor_feedback_msgs::msg::MotorFeedback&);

/* Function End */



/* ---------------------------------------------------------*/
/* ⇩⇩⇩⇩⇩⇩⇩⇩⇩⇩ Program ⇩⇩⇩⇩⇩⇩⇩⇩⇩⇩ ---------------------------*/
/* ---------------------------------------------------------*/
/* Program Begin */

/** * @brief  Program entry point.
	* @param argc(int) Number of input parameters
 	* @param argv(int) input parameters
 	* @return (int) Program Error.
**	**/
int main(int argc, char **argv)
{
	//ros::init(argc, argv, "main_tf_odometry");
	//ros::NodeHandle nh;
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("main_tf_odometry");

	//ros::Publisher odom_pub = nh.advertise<nav_msgs::msg::Odometry>("odom", 100);
	auto odom_pub = node->create_publisher<nav_msgs::msg::Odometry>("odom",100);

	//ros::Publisher encodom_pub = nh.advertise<motor_feedback_msgs::msg::MotorFeedback>("encodom", 100);
	auto encodom_pub = node->create_publisher<motor_feedback_msgs::msg::MotorFeedback>("encodom",100);

	//ros::Subscriber sub_enc = nh.subscribe("/motor_feedback", 100, motor_feedback_callback);
	auto sub_enc = node->create_subscription<motor_feedback_msgs::msg::MotorFeedback>("/motor_feedback", 100, motor_feedback_callback);
	
	/* 建立tf廣播物件 */
	std::shared_ptr<tf2_ros::TransformBroadcaster> odom_broadcaster;
	rclcpp::Rate sleep(100);

	init_time = node->now();
	motor_info_init();

 	while(rclcpp::ok())
    {
		rclcpp::spin_some(node);
		/* compute odometry in a typical way given the velocities of the robot */
		current_time = node->now();
		dt = (current_time - last_time).seconds();
		// ROS_INFO("%lf\n",dt);
		delta_x = (vx * cos(th) - vy * sin(th)) * dt;
		delta_y = (vx * sin(th) + vy * cos(th)) * dt;
		delta_th = vth * dt;
		x += delta_x;
		y += delta_y;
		th += delta_th;

		q.setRPY(0, 0, th);
		odom_quat.x = q.x();
		odom_quat.y = q.y();
		odom_quat.z = q.z();
		odom_quat.w = q.w();

		odom_trans.header.stamp = current_time;
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_footprint";
		odom_trans.transform.translation.x = x;
		odom_trans.transform.translation.y = y;
		odom_trans.transform.translation.z = 0;
		odom_trans.transform.rotation = odom_quat;
		/* send the transform */
		// odom_broadcaster.sendTransform(odom_trans);

		/* publish the odometry message over ROS */
		nav_msgs::msg::Odometry odom;
		odom.header.stamp = current_time;
		odom.header.frame_id = "odom";
		odom.child_frame_id = "base_footprint";
		/* set the position */
		odom.pose.pose.position.x = x;
		odom.pose.pose.position.y = y;
		odom.pose.pose.position.z = 0;
		odom.pose.pose.orientation = odom_quat;
		odom.pose.covariance = {1e-3, 0, 0, 0, 0, 0, 
                        0, 1e-3, 0, 0, 0, 0,
                        0, 0, 1e6, 0, 0, 0,
                        0, 0, 0, 1e6, 0, 0,
                        0, 0, 0, 0, 1e6, 0,
                        0, 0, 0, 0, 0, 1e3};
		/* set the velocity */
		odom.twist.twist.linear.x = vx;
		odom.twist.twist.linear.y = vy;
		odom.twist.twist.angular.z = vth;
		odom.twist.covariance = {1e-3, 0, 0, 0, 0, 0, 
                         0, 1e-3, 0, 0, 0, 0,
                         0, 0, 1e6, 0, 0, 0,
                         0, 0, 0, 1e6, 0, 0,
                         0, 0, 0, 0, 1e6, 0,
                         0, 0, 0, 0, 0, 1e3};
		/* publish the message */
		odom_pub->publish(odom);

		encodom.header.stamp = current_time;
		encodom.vx = vx;
		encodom.vy = vy;
		encodom.vth = vth;
		encodom.delta_x = delta_x;
		encodom.delta_y = delta_y;
		encodom.delta_th = delta_th;
		encodom.x = x;
		encodom.y = y;
		encodom.th = th;
		encodom_pub->publish(encodom);

		/* updata last time */
		last_time = current_time;
		sleep.sleep();
	}
	// ros::spin();

	/* main quit */
	return 0;
}


/** * @brief  topic "/motor_feedback" subscribe interrupt callback.
	* @param mf_last(motor_feedback) motor feedback message
 	* @return none.
**	**/
void motor_feedback_callback(const motor_feedback_msgs::msg::MotorFeedback& motor_info)
{
	// double Lacctemp,Racctemp;

	// Lacctemp = (motor_info.AvelocityL-mf_last.AvelocityL)*enc_to_mm*mm_to_M;
	// Racctemp = (motor_info.AvelocityR-mf_last.AvelocityR)*enc_to_mm*mm_to_M;
	// if( (Lacctemp<=1.0) || (Lacctemp>=-1.0) )
	// 		AvelocityL_M = motor_info.AvelocityL*enc_to_mm*mm_to_M;
	// else 	AvelocityL_M = mf_last.AvelocityL*enc_to_mm*mm_to_M;

	encodom.a_velocity_l = motor_info.a_velocity_l;
	encodom.a_velocity_r = motor_info.a_velocity_r;

	AvelocityL_M = motor_info.a_velocity_l*enc_to_mm*mm_to_M;
	AvelocityR_M = motor_info.a_velocity_r*enc_to_mm*mm_to_M;

	AvelocityL_lpf = (low_pass_filter*AvelocityL_M)+((1.0-low_pass_filter)*AvelocityL_lpf);
	AvelocityR_lpf = (low_pass_filter*AvelocityR_M)+((1.0-low_pass_filter)*AvelocityR_lpf);

	encodom.a_velocity_l_m = AvelocityL_M;
	encodom.a_velocity_r_m = AvelocityR_M;
	encodom.a_velocity_l_lpf = AvelocityL_lpf;
	encodom.a_velocity_r_lpf = AvelocityR_lpf;

	vx = ((AvelocityL_M+AvelocityR_M)/2);
	vth = ((AvelocityR_M-AvelocityL_M)/AMR_width);

	mf_last = motor_info;
}

/** * @brief  motor info init.
	* @param none
 	* @return none.
**	**/
void motor_info_init(void)
{
	current_time = init_time;
	last_time = current_time;
	vx = 0;
	vy = 0;
	vth = 0;
	x = 0;
	y = 0;
	th = 0;
	AvelocityL_M = 0;
	AvelocityR_lpf = 0;
	AvelocityL_M = 0;
	AvelocityR_lpf = 0;
	mf_last.header.frame_id = "motor_feedback";
	// mf_last.header.seq = 0;
	mf_last.header.stamp = current_time;
	mf_last.position_l = 0;
	mf_last.position_r = 0;
	mf_last.a_velocity_l = 0;
	mf_last.a_velocity_r = 0;
	mf_last.d_velocity_l = 0;
	mf_last.d_velocity_r = 0;
}


/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF main_tf_odometry.cpp ***** */
