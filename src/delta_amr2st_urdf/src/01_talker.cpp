/** ******************************************************
	* @file		01_talker.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <iostream>
#include <memory>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "classAPI_01talker.hpp"
#include "delta_amr2st_urdf/delta_RICHIE_test.h"
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
	int64_t count = 0;
	std_msgs::String msg;
	delta_amr2st_urdf::delta_RICHIE_test dRt;

	ros::init(argc, argv, "talker");
	std::shared_ptr<talker> tk = std::make_shared<talker>();
	ros::Rate loop_rate(1);

	while (ros::ok())
	{
		msg.data = std::to_string(count);
		// ROS_INFO("publish: %s", msg.data.c_str());
		dRt.num = count;
		dRt.child_frame_id = msg.data.c_str();;
		tk->delta_RICHIE_msg_setdata(dRt);
		tk->publish(dRt);
		ros::spinOnce();
		loop_rate.sleep();
		count++;
	}

	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF 01_talker.cpp ***** */
