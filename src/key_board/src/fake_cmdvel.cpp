/** ******************************************************
	* @file		fake_cmdvel.cpp
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
    bool dir=false;

    ros::init(argc,argv,"fake_cmdvel");
    ros::NodeHandle n;

    ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("cmd_vel",1);

    ros::Rate loop_rate(10);

    cmd_vel.linear.x = 0;
    cmd_vel.angular.z = 0;

    while( ros::ok() )
    {
        if(!dir)
        {
            cmd_vel.angular.z += 0.01;
            if(cmd_vel.angular.z >= 1.0) dir=true;
        }
        else
        {
            cmd_vel.angular.z -= 0.01;
            if(cmd_vel.angular.z <= -1.0) dir=false;           
        }

        pub_cmd_vel.publish(cmd_vel);
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF fake_cmdvel.cpp ***** */
