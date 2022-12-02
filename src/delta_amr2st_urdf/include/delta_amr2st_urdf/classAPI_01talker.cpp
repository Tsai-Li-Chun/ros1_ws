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
	pub = this->advertise<delta_amr2st_urdf::delta_RICHIE_test>("chatter", 1);
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

void talker::publish(delta_amr2st_urdf::delta_RICHIE_test dRt)
{
	pub.publish(dRt);
}

void talker::delta_RICHIE_msg_setdata(delta_amr2st_urdf::delta_RICHIE_test dRt)
{
	dRt.header.seq = dRt.num;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF classAPI_01talker.cpp ***** */
