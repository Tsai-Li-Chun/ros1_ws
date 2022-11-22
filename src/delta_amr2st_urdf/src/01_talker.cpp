/** ******************************************************
	* @file		01_talker.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <memory>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "classAPI_01talker.hpp"
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
	ros::init(argc, argv, "talker");
	std::shared_ptr<talker> tk = std::make_shared<talker>();
	ros::Rate loop_rate(10);

	while (ros::ok())
	{
		std_msgs::String msg;
		msg.data = "hello world!";
		ROS_INFO("publish: %s", msg.data.c_str());
		tk->publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}

	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF 01_talker.cpp ***** */
