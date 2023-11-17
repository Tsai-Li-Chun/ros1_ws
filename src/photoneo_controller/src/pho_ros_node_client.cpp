/** ******************************************************
	* @file		pho_ros_node_client.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "pho_ros_controller.hpp"
/* User Includes End */

/* namespace ------------------------------------------------*/
/* namespace Begin */

// using namespace pho::api;

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

/** * @brief  Program entry point.
	* @param argc(int) Number of input parameters
 	* @param argv(int) input parameters
 	* @return (int) Program Error.
**	**/
int main(int argc,char **argv)
{
	char key;
	ros::init(argc, argv, "pho_ros_node_client");
	ros::NodeHandle n;
	photoneo_controller_msg_srv::LocalizationPoseList_msgs pho_result_list_msgs_;

	// pho_ros_controller pho_ros_ctl(&n,10.0f,0.01f);
	pho_ros_controller pho_ros_ctl(&n, service_mode::client);

	while( (ros::ok()) && (key!='q') )
	{
		// std::cout << "input 'q' to exit program, input 's' trigger the 3Dscanner: ";
		// std::cin >> key;
		sleep(15);
		key = 's';
		if( key=='s' )
		{
			if( pho_ros_ctl.client_call() )                             
			{
				ROS_INFO("call to the service");
				pho_ros_ctl.pub_pho_results();
				pho_result_list_msgs_ = pho_ros_ctl.get_pho_results();
				ROS_INFO("Success to call service");
			}
			else
			{
				ROS_ERROR("Failed to call service");
			}
		}
	}
	

	return EXIT_SUCCESS;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_node_client.cpp ***** */
