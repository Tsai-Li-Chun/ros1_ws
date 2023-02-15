/** ******************************************************
	* @file		modbus_test.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "Oriental_BLVD_KRD.hpp"
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

/* 建立 BLVD-KRD Control物件*/
BLVD_KRD_Control BKC("/dev/USBport-motor",0x0F);
/* 建立topic-Publisher物件 */
ros::Publisher motor_fb;
/* 建立message Twist物件 */
geometry_msgs::Twist twist_last;
/* 宣告存放左右兩輪累計距離用變數 */
int32_t position[2]={0};
/* 宣告存放左右兩輪實際速度用變數 */
int32_t velocity_A[2]={0};
/* 宣告存放左右兩輪理論速度用變數 */
int32_t velocity_D[2]={0};
/* 建立message Inertia物件 */
motor_feedback_msgs::motor_feedback mf;
/* 宣告左右輪速度 */
int32_t velL=0,velR=0;

const double ms_to_rpm = 2976.40418132187;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

void twist_callback(const geometry_msgs::Twist& twist_msg);
void timer_callback(const ros::TimerEvent& e);

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
	/* 宣告libmodbus-API的Return用變數 */
	int rc;
	/* 建立ROS time物件 */
	ros::Time current_time, last_time;
	/* delta time(s) */
	double dt;
	/* ros init */
    ros::init(argc,argv,"main_motorController");

	/* 建立NodeHandle物件 */
	ros::NodeHandle nh;
	/* 建立topic-Subscriber物件並初始化 */
	ros::Subscriber twist_sub = nh.subscribe("/cmd_vel", 100, twist_callback);
	/* 建立計時物件並初始化計時中斷 */
	ros::Timer timer = nh.createTimer(ros::Duration(0.01), timer_callback);
	/* 初始化motor_fb物件 */
	motor_fb = nh.advertise<motor_feedback_msgs::motor_feedback>("/motor_feedback",100);
	/* 建立delay用物件 */
	ros::Rate loop_rate(100);

	sleep(1);
	/* motor Init - 事先將觸發方式(-4)等參數set,後續更新速度即可即時變化 */
	rc = BKC.motorInit(48,1000,1000,(-4));
	sleep(1);

	current_time = ros::Time::now();
	last_time = current_time;

	while (ros::ok())
	{
		current_time = ros::Time::now();
		dt = (current_time - last_time).toSec();
		// ROS_INFO("%lf\n",dt);
		BKC.readActualPosition(position);
		BKC.readActualVelocity(velocity_A);
		BKC.readDemandVelocity(velocity_D);
		// mf.header.frame_id = "motor_feedback";
		// mf.header.seq = 0;
		// mf.header.stamp = ros::Time::now();
		mf.positionL = position[0];
		mf.positionR = position[1];
		mf.AvelocityL = velocity_A[0];
		mf.AvelocityR = velocity_A[1];
		mf.DvelocityL = velocity_D[0];
		mf.DvelocityR = velocity_D[1];
		motor_fb.publish(mf);
		last_time = current_time;

		ros::spinOnce();
		loop_rate.sleep();
	}
	// ros::spin();

	rc = BKC.motorSOFF();
	ROS_INFO("rc = %d , motor SOFF",rc);
	sleep(1);

	ros::shutdown();
	/* main quit */
	return 0;
}

void twist_callback(const geometry_msgs::Twist& twist_msg)
{
	twist_last = twist_msg;
	double velLtmp,velRtmp;
	velLtmp = twist_last.linear.x - twist_last.angular.z ;
	velRtmp = twist_last.linear.x + twist_last.angular.z ;
	velL = (int32_t)(velLtmp*ms_to_rpm);
	velR = (int32_t)(velRtmp*ms_to_rpm);
	if(velL>890) velL=890;
	else if(velL<(-890)) velL=(-890);
	if(velR>890) velR=890;
	else if(velR<(-890)) velR=(-890);
	// printf("%d , %d\n",velL,velR);
	BKC.writeVelocity(velL,velR);
}

void timer_callback(const ros::TimerEvent& e)
{

}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
