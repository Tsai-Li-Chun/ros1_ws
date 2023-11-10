/** ******************************************************
	* @file		pho_ros_controller.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "pho_ros_controller.hpp"
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

/** * @brief constructor
	* @param None
 	* @return None
**	**/
pho_ros_controller::pho_ros_controller(ros::NodeHandle* n_ptr_)
{
	shm_work_status = false;
	n_ = n_ptr_;
	pub_pho_results_ = n_->advertise<photoneo_controller_msg_srv::LocalizationPose_msgs>("pho_loc_results",10);
	timer_50_ = n_->createTimer(ros::Duration(10.0), &pho_ros_controller::time_50_callback, this);
	timer_01_ = n_->createTimer(ros::Duration(0.01), &pho_ros_controller::time_01_callback, this);
	
	pho_results_msgs_Initialization();
	/* Create shared memory control object */
    if(shm_StartUp(shm_key, shm_size, shm_flg, shm_rw_twoway) == EXIT_SUCCESS)
		ROS_INFO("Create shared memory control object, and attach Success!");
	else
		ROS_ERROR("Create and attach shared memory control object Failed!");
}

/** * @brief destructor
	* @param None
 	* @return None
**	**/
pho_ros_controller::~pho_ros_controller()
{
	if(detach_shm() == 0)
		ROS_INFO("Detach shared meeory Success");
	else if(detach_shm() == (-1))
		ROS_ERROR("Detach shared meeory Failed");
}

/** * @brief photoneo controller object entry point
	* @param None
 	* @return None
**	**/
void pho_ros_controller::Run(void)
{

	pub_pho_results_.publish(pho_results_msgs_);
}

/** * @brief timer 5s callback function
	* @param TimerEvent event, timer event
 	* @return None
**	**/
void pho_ros_controller::time_50_callback(const ros::TimerEvent &event)
{
	ROS_INFO("time_50_callback: %lf",ros::Time::now().toSec());
	if( (shm_result[0]==0.0f) && (shm_work_status==false) )
	{
		shm_work_status = true;
		/* send request */
		ROS_INFO("Send request for data command");
		shm_result[0] = 1.0f;
		// for(i=1; i<17; i++) pose[i] = 0.0f;
		write_shm(shm_result, 1*sizeof(float));
		ROS_INFO("Waiting for data to return ...");
	}
}

/** * @brief timer 0.01s callback function
	* @param TimerEvent event, timer event
 	* @return None
**	**/
void pho_ros_controller::time_01_callback(const ros::TimerEvent &event)
{
	if( shm_work_status==true )
	{
		read_shm(shm_result, 2*sizeof(float));
		if(shm_result[0]==2.0f)
		{
			read_shm(shm_result, (size_t)shm_result[1]*sizeof(float));
			pho_results_quantity = (int)((shm_result[1]-2)/19);
			shm2rosmsg();
			pub_pho_results_.publish(pho_results_msgs_);
			/* send reset signal */
			shm_result[0] = 0.0f;
            write_shm(shm_result, sizeof(shm_result));
			shm_work_status = false;
			ROS_INFO("------------Finish------------\n");
		}
	}
}

/** * @brief Convert 1D array in SHM to ROS message format
	* @param None
 	* @return None
**	**/
void pho_ros_controller::shm2rosmsg(void)
{
	int i,j,k;
	for( k=0; k<pho_results_quantity; k++ )
	{
		pho_results_msgs_.ID = shm_result[k*19+2];
		pho_results_msgs_.Occluded = shm_result[k*19+3];
		pho_results_msgs_.VisibleOverlap = shm_result[k*19+4];
		for(i=0; i<4; i++)
			for(j=0; j<4; j++)
				pho_results_msgs_.Transformation.row.at(i).col.at(j) = shm_result[k*19+5+(4*i)+j];
		for( i=0; i<shm_float_size; i++)
			shm_result[i] = 0;
	}
	
}

/** * @brief setup pho_results_msgs_ data to 0 (initialization)
	* @param None
 	* @return None
**	**/
void pho_ros_controller::pho_results_msgs_Initialization(void)
{
	int i,j;
	pho_results_msgs_.header.frame_id = "pho_ros_controller";
	pho_results_msgs_.header.stamp = ros::Time::now();
	pho_results_msgs_.header.seq = 0;
	pho_results_msgs_.ID = 0;
	pho_results_msgs_.Occluded = 0;
	pho_results_msgs_.VisibleOverlap = 0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			pho_results_msgs_.Transformation.row.at(i).col.at(j) = 0;
	for( i=0; i<shm_float_size; i++)
		shm_result[i] = 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_controller.cpp ***** */
