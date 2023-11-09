/** ******************************************************
	* @file		pho_ros_controller.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __pho_ros_controller_HPP__
#define __pho_ros_controller_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "shm_controller.hpp"
#include "photoneo_controller_msg_srv/LocalizationPose_msgs.h"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

/* shared memory control object - ROS version */
class pho_ros_controller : public shared_memory_controller
{
/* private members */
private:
	ros::NodeHandle n_;
	ros::Publisher pub_pho_results_;

/* public members */
public:
	/* constructor */
	pho_ros_controller(ros::NodeHandlePtr n_ptr_ );
	/* destructor */
	~pho_ros_controller();



};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__pho_ros_controller_HPP__ */

/* ***** END OF pho_ros_controller.HPP ***** */
