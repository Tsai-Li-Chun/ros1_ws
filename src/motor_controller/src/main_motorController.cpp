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
#include <geometry_msgs/Inertia.h>
#include "Oriental_BLVD_KRD.hpp"
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
BLVD_KRD_Control BKC("/dev/ttyUSB0",0x0F);
/* 建立topic-Publisher物件 */
ros::Publisher enc_pub;
/* 建立message Twist物件 */
geometry_msgs::Twist twist_last;
/* 宣告存放左右兩輪累計距離用變數 */
int32_t position[2]={0};
/* 宣告存放左右兩輪實際速度用變數 */
int32_t velocity_A[2]={0};
/* 宣告存放左右兩輪理論速度用變數 */
int32_t velocity_D[2]={0};
/* 建立message Inertia物件 */
geometry_msgs::Inertia enc;

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
	/* 宣告左右輪速度 */
	int32_t velL=0,velR=0;
	/* ros init */
    ros::init(argc,argv,"main_motorController");

	/* 建立NodeHandle物件 */
	ros::NodeHandle nh;
	/* 建立topic-Subscriber物件並初始化 */
	ros::Subscriber twist_sub = nh.subscribe("/cmd_vel", 100, twist_callback);
	/* 建立計時物件並初始化計時中斷 */
	ros::Timer timer = nh.createTimer(ros::Duration(0.01), timer_callback);
	/* 初始化enc_pub物件 */
	enc_pub = nh.advertise<geometry_msgs::Inertia>("/motor_encoder",100);
	/* 建立delay用物件 */
	ros::Rate loop_rate(10);

	sleep(1);
	/* motor Init - 事先將觸發方式(-4)等參數set,後續更新速度即可即時變化 */
	// rc = BKC.motorInit(48,1000,1000,(-4));
	sleep(1);

	while (ros::ok())
	{
		// velL = twist_last.linear.x - twist_last.angular.z ;
		// velR = twist_last.linear.x + twist_last.angular.z ;
		// // printf("%d , %d\n",velL,velR);
		// BKC.writeVelocity(velL,velR);
		ros::spinOnce();
		loop_rate.sleep();
	}

	rc = BKC.motorSOFF();
	printf("%d\n",rc);
	sleep(1);

	ros::shutdown();
	/* main quit */
	return 0;
}

void twist_callback(const geometry_msgs::Twist& twist_msg)
{
	twist_last = twist_msg;
}

void timer_callback(const ros::TimerEvent& e)
{
	BKC.readActualPosition(position);
	BKC.readActualVelocity(velocity_A);
	BKC.readDemandVelocity(velocity_D);
	enc.ixx = position[0];
	enc.ixy = position[1];
	enc.ixz = velocity_A[0];
	enc.iyy = velocity_A[1];
	enc.iyz = velocity_D[0];
	enc.izz = velocity_D[1];
	enc_pub.publish(enc);
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
