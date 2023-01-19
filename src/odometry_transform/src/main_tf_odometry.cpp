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
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "motor_feedback_msgs/motor_feedback.h"
/* User Includes End */

/* namespace ------------------------------------------------*/
/* namespace Begin */
/* namespace End */


/* Define ---------------------------------------------------*/
/* Define Begin */
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
motor_feedback_msgs::motor_feedback motor_info;
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
/* 建立ROS time物件 */
ros::Time current_time, last_time;
/* 建立計算旋轉勁度物件 */
geometry_msgs::Quaternion odom_quat;
/* 建立tf廣播用變數物件 */
geometry_msgs::TransformStamped odom_trans;
/* 建立odom里程計變數物件 */
nav_msgs::Odometry odom;
// /* 建立odom里程計topic發布物件 */
// ros::Publisher odom_pub;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

/* motor feedback message object init */
void motor_info_init(void);
/* topic subscribe interrupt callback */
void motor_feedback_callback(const motor_feedback_msgs::motor_feedback&);

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
	ros::init(argc, argv, "main_tf_odometry");
	
	ros::NodeHandle nh;
	ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 100);
	ros::Subscriber sub_enc = nh.subscribe("/motor_feedback", 100, motor_feedback_callback);
	/* 建立tf廣播物件 */
	tf::TransformBroadcaster odom_broadcaster;
	ros::Rate sleep(100);

	
	motor_info_init();

 	while(nh.ok())
    {
		ros::spinOnce();
		/* compute odometry in a typical way given the velocities of the robot */
		current_time = ros::Time::now();
		dt = (current_time - last_time).toSec();
		// ROS_INFO("%lf\n",dt);
		delta_x = (vx * cos(th) - vy * sin(th)) * dt;
		delta_y = (vx * sin(th) + vy * cos(th)) * dt;
		delta_th = vth * dt;
		x += delta_x;
		y += delta_y;
		th += delta_th;
		odom_quat = tf::createQuaternionMsgFromYaw(th);
		odom_trans.header.stamp = current_time;
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_footprint";
		odom_trans.transform.translation.x = x;
		odom_trans.transform.translation.y = y;
		odom_trans.transform.translation.z = 0;
		odom_trans.transform.rotation = odom_quat;
		/* send the transform */
		odom_broadcaster.sendTransform(odom_trans);

		/* publish the odometry message over ROS */
		nav_msgs::Odometry odom;
		odom.header.stamp = current_time;
		odom.header.frame_id = "odom";
		/* set the position */
		odom.pose.pose.position.x = x;
		odom.pose.pose.position.y = y;
		odom.pose.pose.position.z = 0;
		odom.pose.pose.orientation = odom_quat;
		/* set the velocity */
		odom.child_frame_id = "base_footprint";
		odom.twist.twist.linear.x = vx;
		odom.twist.twist.linear.y = vy;
		odom.twist.twist.angular.z = vth;
		/* publish the message */
		odom_pub.publish(odom);
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
void motor_feedback_callback(const motor_feedback_msgs::motor_feedback& mf_last)
{
	/* ]get motor feedback message */
	motor_info = mf_last;
	// ROS_INFO("position : %d , %d", motor_info.positionL, motor_info.positionR);
	// ROS_INFO("Avelocity : %d , %d", motor_info.AvelocityL, motor_info.AvelocityR);
	// ROS_INFO("Dvelocity : %d , %d", motor_info.DvelocityL, motor_info.DvelocityR);
	// ROS_INFO(" ");
	AvelocityL_M = motor_info.AvelocityL*enc_to_mm*mm_to_M;
	AvelocityR_M = motor_info.AvelocityR*enc_to_mm*mm_to_M;
	vx = ((AvelocityL_M+AvelocityR_M)/2);
	vth = ((AvelocityR_M-AvelocityL_M)/AMR_width);
	// ROS_INFO("%lf , %lf",vx ,vth);
}

/** * @brief  motor info init.
	* @param none
 	* @return none.
**	**/
void motor_info_init(void)
{
	vx = 0;
	vy = 0;
	vth = 0;
	x = 0;
	y = 0;
	th = 0;
	motor_info.header.frame_id = "motor_feedback";
	// motor_info.header.seq = 0;
	// motor_info.header.stamp = ros::Time::now();
	motor_info.positionL = 0;
	motor_info.positionR = 0;
	motor_info.AvelocityL = 0;
	motor_info.AvelocityR = 0;
	motor_info.DvelocityL = 0;
	motor_info.DvelocityR = 0;
	current_time = ros::Time::now();
	last_time = current_time;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF main_tf_odometry.cpp ***** */
