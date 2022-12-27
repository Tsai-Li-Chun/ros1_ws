/** ******************************************************
	* @file		01_listener.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include <std_msgs/String.h>
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

/** * @brief  Program entry point.
	* @param argc(int) Number of input parameters
 	* @param argv(int) input parameters
 	* @return (int) Program Error.
**	**/

void chatterCallback0(const std_msgs::String::ConstPtr& msg){}
void chatterCallback1(const std_msgs::String::ConstPtr& msg){}
void chatterCallback2(const std_msgs::String::ConstPtr& msg){}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "node_handle_test");
    ros::NodeHandle nh0("~");
    ros::NodeHandle nh1("ns1");
    ros::NodeHandle nh2("ns2");

    ros::Subscriber sub0 = nh0.subscribe("chatter0",10,chatterCallback0);
    ros::Subscriber sub1 = nh1.subscribe("chatter0",10,chatterCallback1);
    ros::Subscriber sub2 = nh2.subscribe("chatter0",10,chatterCallback2);

    ros::spin();

	
	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF 01_listener.cpp ***** */
