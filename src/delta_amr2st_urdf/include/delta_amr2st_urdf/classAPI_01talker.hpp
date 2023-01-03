/** ******************************************************
	* @file		classAPI_01talker.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __classAPI_01talker_HPP__
#define __classAPI_01talker_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include "motor_feedback_msgs/motor_feedback.h"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

class talker : public ros::NodeHandle
{
private:
	bool hparam=false;
	ros::Publisher pub;
	motor_feedback_msgs::motor_feedback mfm;
public:
	talker();
	~talker();
	void publish(void);
	void motor_feedback_setdata(int32_t);
};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__classAPI_01talker_HPP__ */

/* ***** END OF classAPI_01talker.HPP ***** */
