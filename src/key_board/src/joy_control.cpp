/** ******************************************************
	* @file		joy_control.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include "stdio.h"
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"
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

double enlarge=0;
geometry_msgs::Twist cmd_vel;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

void joy_callback(const sensor_msgs::Joy&);

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

    ros::init(argc,argv,"joy_control_node");
    ros::NodeHandle n;

    ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("cmd_vel",1);

    ros::Subscriber sub_joy    = n.subscribe("joy",1,joy_callback);

    ros::Rate loop_rate(10);

    while( ros::ok() )
    {
        pub_cmd_vel.publish(cmd_vel);
        // char vcx_chptr[10],vcr_chptr[10],chptr[20];
        // //float vcx = 0,vcr = 0;
        // sprintf( vcr_chptr , "VCX%4.2lf", cmd_vel.linear.x/2.0 );
        // sprintf( vcr_chptr , "VCR%4.2lf", cmd_vel.angular.z/2.0 );
        // sprintf( chptr , "VCX%4.2lf\tVCR%4.2lf\n", cmd_vel.linear.x/2.0, cmd_vel.angular.z/2.0);
        // ROS_INFO("%s",chptr);
        // //ROS_INFO("VCX%4.2lf\tVCR%4.2lf\n", cmd_vel.linear.x/2.0, cmd_vel.angular.z/2.0);

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

void joy_callback( const sensor_msgs::Joy &joy_msg )
{
    enlarge = 0.5;
    if(joy_msg.buttons[4]==1) enlarge = 1;
    cmd_vel.linear.x = (joy_msg.axes[4])*enlarge*(1);
    // cmd_vel.linear.y = joy_msg.axes[0];
    cmd_vel.angular.z= (joy_msg.axes[0])*(enlarge*0.8);
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF joy_control.cpp ***** */
