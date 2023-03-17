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
#include "sensor_msgs/Joy.h"
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

bool arm_action = false;
/* 建立modbus通訊物件 */
Modbus_Handshake robot_arm("192.168.1.10");

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

void joy_callback(const sensor_msgs::Joy&);

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
	ros::NodeHandle n;
    ros::Subscriber sub_joy    = n.subscribe("joy",100,joy_callback);

	ROS_INFO("robot_arm init OK"); sleep(1);

	ROS_INFO("argv[1] = %s",argv[1]);
	int number = atoi(argv[1]);
	ROS_INFO("argv[1] = %d",number);

	ros::Rate loop_rate(100);

	robot_arm.run_RL();

	// if( number == 1 )
	// {	ROS_INFO("robot_arm.grab_material()");
	// 	robot_arm.grab_material();
	// }
	// else if( number == 2 )
	// {	ROS_INFO("robot_arm.handeye()");
	// 	robot_arm.handeye();
	// }
	// else if( number == 3 )
	// {	ROS_INFO("robot_arm.release_material()");
	// 	robot_arm.release_material();
	// }
	// else if( number == 4 )
	// {	ROS_INFO("robot_arm.back_home()");
	// 	robot_arm.back_home();
	// }
	// else if( number == 5 )
	// {	ROS_INFO("robot_arm.arm_standby()");
	// 	robot_arm.arm_standby();
	// }

	while (ros::ok())
	{
		ros::spinOnce();
        loop_rate.sleep();

	}

	ros::shutdown();
	/* main quit */
	return 0;
}

void joy_callback( const sensor_msgs::Joy &joy_msg )
{
	if( arm_action == false )
	{
    	if(joy_msg.buttons[3]==1)
		{
			arm_action = true;
			ROS_INFO("robot_arm.grab_material()");
			robot_arm.grab_material();
			arm_action = false;
		}
		else if(joy_msg.buttons[1]==1)
		{
			arm_action = true;
			ROS_INFO("robot_arm.release_material()");
			robot_arm.release_material();
			arm_action = false;
		}
		else if(joy_msg.buttons[0]==1)
		{
			arm_action = true;
			ROS_INFO("robot_arm.back_home()");
			robot_arm.back_home();
			arm_action = false;
		}
		else if(joy_msg.buttons[2]==1)
		{
			arm_action = true;
			ROS_INFO("robot_arm.backzero()");
			robot_arm.backzero();
			arm_action = false;
		}
	}

}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF main.cpp ***** */