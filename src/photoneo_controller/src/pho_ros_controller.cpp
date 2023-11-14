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
	pub_pho_results_ = n_->advertise<photoneo_controller_msg_srv::LocalizationPoseList_msgs>("pho_loc_results",10);
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

	pub_pho_results_.publish(pho_result_list_msgs_);
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
			pub_pho_results_.publish(pho_result_list_msgs_);
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
	pho_result_list_msgs_.LocalizationPoseList.clear();
	for( k=0; k<pho_results_quantity; k++ )
	{
		if(k<pho_results_MAXquantity)
		{
			pho_result_msgs_.ID = shm_result[k*loc_base_quantity+loc_offset_ID];
			pho_result_msgs_.Occluded = shm_result[k*loc_base_quantity+loc_offset_Oc];
			pho_result_msgs_.VisibleOverlap = shm_result[k*loc_base_quantity+loc_offset_VO];
			for(i=0; i<4; i++)
				for(j=0; j<4; j++)
					pho_result_msgs_.Transformation.row.at(i).col.at(j) = shm_result[k*loc_base_quantity+loc_offset_TF(i,j)];
			pho_result_list_msgs_.LocalizationPoseList.push_back(pho_result_msgs_);
			ROS_INFO("Reading target object: %ld / %d",pho_result_list_msgs_.LocalizationPoseList.size(),pho_results_quantity);
		}
		else
		{
			k=pho_results_MAXquantity;
			ROS_WARN("result quantity exceeds the limit(pho_results_MAXquantity), retrieve only the first 10.");
		}
	}
	for( i=0; i<shm_float_size; i++)
		shm_result[i] = 0;
	
}

/** * @brief setup pho_results_msgs_ data to 0 (initialization)
	* @param None
 	* @return None
**	**/
void pho_ros_controller::pho_results_msgs_Initialization(void)
{
	int i,j;
	
	// pho_results_msgs_.LocalizationPoseList.resize(pho_results_MAXquantity);
	// for( i=0; i<shm_float_size; i++)
	// 	pho_results_msgs_.LocalizationPoseList.at(i).header.frame_id = "pho_ros_controller";
	for( i=0; i<shm_float_size; i++)
		shm_result[i] = 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_controller.cpp ***** */
