/** ******************************************************
	* @file		pho_ros_node_server.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <thread>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "shm_controller.hpp"
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
    ros::init(argc, argv, "pho_ros_node_server");
    ros::NodeHandle n;

    // pho_ros_controller pho_ros_ctl(&n,10.0f,0.01f);
    pho_ros_controller pho_ros_ctl(&n, service_mode::server);

    ros::spin();

    return EXIT_SUCCESS;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_node_server.cpp ***** */
