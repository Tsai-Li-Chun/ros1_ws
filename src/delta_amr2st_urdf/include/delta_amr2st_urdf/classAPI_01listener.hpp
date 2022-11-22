/** ******************************************************
	* @file		classAPI_01listener.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __classAPI_01listener_HPP__
#define __classAPI_01listener_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include <std_msgs/String.h>
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

class listener : public ros::NodeHandle
{
private:
	ros::Subscriber sub;
	void chatterCallback(const std_msgs::String&);

public:
	listener();
	~listener();
};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__classAPI_01listener_HPP__ */

/* ***** END OF classAPI_01listener.HPP ***** */
