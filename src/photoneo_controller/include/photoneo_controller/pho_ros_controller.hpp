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
#include "photoneo_controller_msg_srv/LocalizationPoseList_msgs.h"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

/* photoneo control object - ROS to shm version */
class pho_ros_controller : public shared_memory_controller
{
/* private members */
private:
	ros::NodeHandle* n_;
	ros::Publisher pub_pho_results_;
	ros::Timer timer_50_,timer_01_;

	/* false(sleeping), true(working) */
	bool shm_work_status;
	/* calculated number of target object */
	int pho_results_quantity;

	float shm_result[shm_float_size]={0};
	photoneo_controller_msg_srv::LocalizationPose_msgs pho_results_msgs_;

	/* setup pho_results_msgs_ data to 0 (initialization) */
	void pho_results_msgs_Initialization(void);
	/* Convert 1D array in SHM to ROS message format */
	void shm2rosmsg(void);

	/* timer 5s callback function */
	void time_50_callback(const ros::TimerEvent &event);
	/* timer 0.01s callback function */
	void time_01_callback(const ros::TimerEvent &event);

/* public members */
public:
	/* constructor */
	pho_ros_controller(ros::NodeHandle* n_ptr_ );
	/* destructor */
	~pho_ros_controller();

	/* photoneo control object entry point */
	void Run(void);
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
