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

/* enc to mm parameter */
const double enc_to_mm = 0.000561414233148148;
/*  */
geometry_msgs::Twist twist_last;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

void twist_callback(const geometry_msgs::Twist& twist_msg);

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
	/* 建立topic-Subscriber物件 */
	ros::Subscriber twist_sub = nh.subscribe("/cmd_vel", 100, twist_callback);
	/* 建立delay用物件 */
	ros::Rate loop_rate(10);

	/* 建立 BLVD-KRD Control物件*/
    BLVD_KRD_Control BKC("/dev/ttyUSB0",0x0F);
    // BLVD_KRD_Control BKC("/dev/ttyS0",0x0F);
	sleep(1);
	/* motor Init - 事先將觸發方式(-4)等參數set,後續更新速度即可即時變化 */
	rc = BKC.motorInit(48,1000,1000,(-4));
	sleep(1);

	while (ros::ok())
	{
		velL = twist_last.linear.x - twist_last.angular.z ;
		velR = twist_last.linear.x + twist_last.angular.z ;
		printf("%d , %d\n",velL,velR);
		BKC.writeVelocity(velL,velR);
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

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
