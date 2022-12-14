/** ******************************************************
	* @file		classAPI_01talker.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/time.h"
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

talker::talker()
{
	pub = this->advertise<motor_feedback_msgs::motor_feedback>("chatter", 1);
	hparam = this->hasParam("/pub/content");
	if(hparam)
	{
		ROS_INFO("param is have");
	}
	else ROS_WARN("param is not have");
}
talker::~talker()
{
	this->shutdown();
}

void talker::publish(void)
{
	pub.publish(mfm);
}

void talker::motor_feedback_setdata(int32_t i)
{
	mfm.positionL = i;
	mfm.positionR = i;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF classAPI_01talker.cpp ***** */
