/** ******************************************************
	* @file		main.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "Modbus_Handshake.hpp"
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
/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
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
	/* ros init */
    ros::init(argc,argv,"arm_robot");
	/* 建立modbus通訊物件 */
	Modbus_Handshake robot_arm("192.168.1.10");
	ROS_INFO("robot_arm init OK"); sleep(1);

	ROS_INFO("argv[1] = %s",argv[1]);
	int number = atoi(argv[1]);
	ROS_INFO("argv[1] = %d",number);

	if( number == 1 )
	{	ROS_INFO("robot_arm.grab_material()");
		robot_arm.grab_material();
	}
	else if( number == 2 )
	{	ROS_INFO("robot_arm.handeye()");
		robot_arm.handeye();
	}
	else if( number == 3 )
	{	ROS_INFO("robot_arm.release_material()");
		robot_arm.release_material();
	}
	else if( number == 4 )
	{	ROS_INFO("robot_arm.back_home()");
		robot_arm.back_home();
	}
	else if( number == 5 )
	{	ROS_INFO("robot_arm.arm_standby()");
		robot_arm.arm_standby();
	}

	while (ros::ok())
	{
		ros::spinOnce();
	}

	ros::shutdown();
	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF main.cpp ***** */