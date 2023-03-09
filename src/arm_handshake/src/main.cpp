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
	if( argc!=2 ) 
	{	/* 若程式輸入參數數量錯誤 */
		ROS_ERROR("Input parameter quantity Error");
		ros::shutdown();
		return 1;
	}
	/* 宣告libmodbus-API的Return用變數 */
	int rc;
	/* ros init */
    ros::init(argc,argv,"arm_robot");
	/* 建立modbus通訊物件 */
	Modbus_Handshake robot_arm("192.168.1.10");

	if( argv[1] == "1" )
	{
		robot_arm.grab_material();
	}
	else if( argv[1] == "2" )
	{
		robot_arm.handeye();
	}
	else if( argv[1] == "3" )
	{
		robot_arm.release_material();
	}
	else if( argv[1] == "4" )
	{
		robot_arm.back_home();
	}
	else if( argv[1] == "5" )
	{
		robot_arm.arm_standby();
	}

	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF main.cpp ***** */