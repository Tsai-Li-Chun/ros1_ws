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

/** * @brief constructor, Using timer call
	* @param ros::NodeHandle*, NodeHandle ptr
	* @param float time1, time interval between each trigger for 3D scanner
	* @param float time2, time interval for reading shm data
 	* @return None
**	**/
pho_ros_controller::pho_ros_controller(ros::NodeHandle* n_ptr_, float time1, float time2)
{
	t2_work_time = 0.0f;
	shm_work_status = false;
	n_ = n_ptr_;
	pub_pho_results_ = n_->advertise<photoneo_controller_msg_srv::LocalizationPoseList_msgs>("pho_loc_results",10);
	timer_t1_ = n_->createTimer(ros::Duration(time1), &pho_ros_controller::time_t1_callback, this);
	timer_t2_ = n_->createTimer(ros::Duration(time2), &pho_ros_controller::time_t2_callback, this);
	ROS_INFO("pho_ros_controller constructor [Using timer call]");
	
	shm_all_zero();
	/* Create shared memory control object */
    if(shm_StartUp(shm_key, shm_size, shm_flg, shm_rw_twoway) == EXIT_SUCCESS)
		ROS_INFO("Create shared memory control object, and attach Success!");
	else
		ROS_ERROR("Create and attach shared memory control object Failed!");
	ROS_INFO("--------------------------------------------------------------------\n");
}
/** * @brief constructor, Using service call
	* @param ros::NodeHandle*, NodeHandle ptr
	* @param std::string, service_mode choose create server or client
 	* @return None
**	**/
pho_ros_controller::pho_ros_controller(ros::NodeHandle* n_ptr_, service_mode s_m)
{
	shm_work_status = false;
	n_ = n_ptr_;
	timer_t2_ = n_->createTimer(ros::Duration(0.01), &pho_ros_controller::time_t2_callback, this);
	if( s_m == service_mode::server )
	{
		server_pho_loc_ = n_->advertiseService("pho_loc_service", &pho_ros_controller::service_pho_loc_callback, this);
		ROS_INFO("Create [Server] Success!");
		shm_all_zero();
		/* Create shared memory control object */
		if(shm_StartUp(shm_key, shm_size, shm_flg, shm_rw_twoway) == EXIT_SUCCESS)
			ROS_INFO("Create shared memory control object, and attach Success!");
		else
			ROS_ERROR("Create and attach shared memory control object Failed!");
		ROS_INFO("--------------------------------------------------------------------\n");
	}
	else if( s_m == service_mode::client )
	{
		client_pho_loc_ = n_->serviceClient<photoneo_controller_msg_srv::pho_loc_service>("pho_loc_service");
		ROS_INFO("Create [Client] Success!");
	}
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
}

/** * @brief service callback function
	* @param pho_loc_service::Request, service request dara
	* @param pho_loc_service::Response, service response data
 	* @return bool, program result
**	**/
bool pho_ros_controller::service_pho_loc_callback(photoneo_controller_msg_srv::pho_loc_service::Request &req,
												  photoneo_controller_msg_srv::pho_loc_service::Response &res)
{
	if( (shm_result[0]==0.0f) && (shm_work_status==false) )
	{
		shm_work_status = true;
		get_MAX_TFtime(false);
		/* send request */
		send_shm_request();
		/* waiting for pho result to return */
		wait_pho_return();
		/* retrieve and convert recognition results from Photoneo */
		retrieve_convert_pho_results();
		/* publish to topic */
		pub_pho_results_.publish(pho_result_list_msgs_);
		/* send reset signal */
		send_shm_reset_command();
		/* retrieve transformation time */
		get_MAX_TFtime(true);
		ROS_INFO("------------Finish------------\n");
		shm_work_status = false;
	}
	return true;
}

/** * @brief timer ${time1}s callback function
	* @param TimerEvent event, timer event
 	* @return None
**	**/
void pho_ros_controller::time_t1_callback(const ros::TimerEvent &event)
{
	if( (shm_result[0]==0.0f) && (shm_work_status==false) )
	{
		shm_work_status = true;
		/* send request */
		send_shm_request();
		// ROS_INFO("Send request for data command");
		// shm_result[0] = 1.0f;
		// // for(i=1; i<17; i++) pose[i] = 0.0f;
		// write_shm(shm_result, 1*sizeof(float));
		ROS_INFO("Waiting for data to return ...");
	}
}

/** * @brief timer ${time2}s callback function
	* @param TimerEvent event, timer event
 	* @return None
**	**/
void pho_ros_controller::time_t2_callback(const ros::TimerEvent &event)
{
	get_MAX_TFtime(false);
	if( shm_work_status==true )
	{
		read_shm(shm_result, 2*sizeof(float));
		if(shm_result[0]==2.0f)
		{
			/* retrieve and convert recognition results from Photoneo */
			retrieve_convert_pho_results();
			/* publish to topic */
			pub_pho_results_.publish(pho_result_list_msgs_);
			/* send reset signal */
			send_shm_reset_command();
			/* retrieve longest transformation time */
			get_MAX_TFtime(true);
			ROS_INFO("------------Finish------------\n");
			shm_work_status = false;
		}
	}
	// if( shm_work_status==true )	read_shm(shm_result, 2*sizeof(float));
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
void pho_ros_controller::shm_all_zero(void)
{
	for(int i=0; i<shm_float_size; i++)
		shm_result[i] = 0;
}

/** * @brief send data request to shm
	* @param None
 	* @return None
**	**/
void pho_ros_controller::send_shm_request(void)
{
	ROS_INFO("Send request for data command");
	shm_result[0] = 1.0f;
	write_shm(shm_result, 1*sizeof(float));
}

/** * @brief waiting for pho result to return
	* @param None
 	* @return None
**	**/
void pho_ros_controller::wait_pho_return(void)
{
	ROS_INFO("Waiting for data to return ...");
	while( shm_result[0]!=2.0f ) {}
}

/** * @brief retrieve and convert recognition results from photoneo
	* @param None
 	* @return None
**	**/
void pho_ros_controller::retrieve_convert_pho_results(void)
{
	read_shm(shm_result, (size_t)shm_result[1]*sizeof(float));
	pho_results_quantity = (int)((shm_result[1]-2)/19);
	shm2rosmsg();
}

/** * @brief send command to reset data in shm
	* @param None
 	* @return None
**	**/
void pho_ros_controller::send_shm_reset_command(void)
{
	shm_result[0] = 0.0f; shm_result[1] = 0.0f;
	write_shm(shm_result, 2*sizeof(float));
}

/** * @brief retrieve longest transformation time
	* @param bool s_e, false retrieve start time,
					   true retrieve end time and
 	* @return None
**	**/
void pho_ros_controller::get_MAX_TFtime(bool s_e)
{
	if( s_e == false )
		t2_start_time = ros::Time::now().toSec();
	else
	{
		t2_end_time = ros::Time::now().toSec();
		if( t2_work_time<(t2_end_time-t2_start_time) )
			t2_work_time = t2_end_time-t2_start_time;
		ROS_INFO("time_t2_callback MAX work time: %lf",t2_work_time);
	}
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_controller.cpp ***** */
