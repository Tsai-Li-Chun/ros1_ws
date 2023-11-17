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
#include "photoneo_controller_msg_srv/pho_loc_service.h"
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

enum class service_mode:char
{
	server = 's',
	client = 'c'
};

/* photoneo control object - ROS to shm version */
class pho_ros_controller : public shared_memory_controller
{
/* private members */
private:
	ros::NodeHandle* n_;
	ros::Publisher pub_pho_results_;
	ros::ServiceServer server_pho_loc_;
	ros::ServiceClient client_pho_loc_;
	ros::Timer timer_t1_,timer_t2_;

	double t2_start_time,t2_end_time,t2_work_time;
	/* false(sleeping), true(working) */
	bool shm_work_status,this_status,server_return;
	/* calculated number of target object */
	int pho_results_quantity;

	float shm_result[shm_float_size]={0};
	photoneo_controller_msg_srv::LocalizationPose_msgs pho_result_msgs_;
	photoneo_controller_msg_srv::LocalizationPoseList_msgs pho_result_list_msgs_;
	photoneo_controller_msg_srv::pho_loc_service pho_loc_srv_;

	/* setup shm data to 0 */
	void shm_all_zero(void);
	/* Convert 1D array in SHM to ROS message format */
	void shm2rosmsg(void);
	/* send data request to shm */
	void send_shm_request(void);
	/* waiting for pho result to return */
	void wait_pho_return(void);
	/* retrieve and convert recognition results from Photoneo */
	void retrieve_convert_pho_results(void);
	/* send command to reset data in shm */
	void send_shm_reset_command(void);
	/* retrieve longest transformation time */
	void get_MAX_TFtime(bool s_e);

	/* service callback function */
	bool service_pho_loc_callback(photoneo_controller_msg_srv::pho_loc_service::Request &req,
								  photoneo_controller_msg_srv::pho_loc_service::Response &res);
	/* timer ${time1}s callback function */
	void time_t1_callback(const ros::TimerEvent &event);
	/* timer ${time2}s callback function */
	void time_t2_callback(const ros::TimerEvent &event);

/* public members */
public:
	/* constructor */
	pho_ros_controller(ros::NodeHandle* n_ptr_, float time1, float time2 );
	pho_ros_controller(ros::NodeHandle* n_ptr_, service_mode s_m);
	/* destructor */
	~pho_ros_controller();

	/* calling the service provider */
	bool client_call(void);
	/* publish to topic (photoneo results) */
	void pub_pho_results(void);
	/* retrieve photoneo localization results */
	photoneo_controller_msg_srv::LocalizationPoseList_msgs get_pho_results(void);
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
