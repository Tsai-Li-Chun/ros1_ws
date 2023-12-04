/** ******************************************************
	* @file		pho_ros_node_server.cpp
	* @author	BRUCE & Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <iostream>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "CoordinateTransformation.h"
#include "M5Manipulator.h"
#include "pho_ros_controller.hpp"
/* User Includes End */

/* namespace ------------------------------------------------*/
/* namespace Begin */
using namespace std;
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

void ShowMatrix(const Matrix4d matrix) {
	printf("\
		%10.4lf , %10.4lf , %10.4lf , %10.4lf\n\
		%10.4lf , %10.4lf , %10.4lf , %10.4lf\n\
		%10.4lf , %10.4lf , %10.4lf , %10.4lf\n\
		%10.4lf , %10.4lf , %10.4lf , %10.4lf\n\n",
		matrix(0,0), matrix(0,1), matrix(0,2), matrix(0,3),
		matrix(1,0), matrix(1,1), matrix(1,2), matrix(1,3), 
		matrix(2,0), matrix(2,1), matrix(2,2), matrix(2,3), 
		matrix(3,0), matrix(3,1), matrix(3,2), matrix(3,3));
}

/** * @brief  Program entry point.
	* @param argc(int) Number of input parameters
 	* @param argv(int) input parameters
 	* @return (int) Program Error.
**	**/
int main(int argc,char **argv)
{
	ros::init(argc, argv, "pho_ros_node_client");
	ros::NodeHandle n;
	photoneo_controller_msg_srv::LocalizationPoseList_msgs pho_result_list_msgs_;
	photoneo_controller_msg_srv::matrix4x4_f32_msgs pho_result_m4x4_;
	pho_ros_controller pho_ros_ctl(&n, service_mode::client);

	CoordinateTransformation coordinate_transformation;
	/* set eye-in-hand parameter */
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
	Matrix4d camera_robot_flange_to_camera = Matrix4d::Identity();
	camera_robot_flange_to_camera(0,0) = 0.0200651;  camera_robot_flange_to_camera(0,1) = -0.999785;  camera_robot_flange_to_camera(0,2) =  -0.00532307; camera_robot_flange_to_camera(0,3) = 83.8534;
	camera_robot_flange_to_camera(1,0) =  0.965313;  camera_robot_flange_to_camera(1,1) = 0.020759;  camera_robot_flange_to_camera(1,2) = -0.260269; camera_robot_flange_to_camera(1,3) =  114.71;
	camera_robot_flange_to_camera(2,0) =  0.260323; camera_robot_flange_to_camera(2,1) =  0.00008388; camera_robot_flange_to_camera(2,2) =  0.965522;  camera_robot_flange_to_camera(2,3) =   88.206;
	coordinate_transformation.SetCameraRobotFlangeToCameraTransMatrix(camera_robot_flange_to_camera);
	cout << "(camera_robot) flange_to_camera :" << endl;
	ShowMatrix(camera_robot_flange_to_camera);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

	/* set (camera_robot) pose, root to flange */
	//--------------------------------------------------------------------------------------//
	Pose camera_robot_root_to_flange;
	camera_robot_root_to_flange.SetData(556.354, -130.180, 844.813, -168.980, -43.248, 25.138); // x,y,z,Rx,Ry,Rz
	cout << "(camera_robot) root_to_flange :" << endl;
	ShowMatrix(camera_robot_root_to_flange.GetData());
	//--------------------------------------------------------------------------------------//

	/* set object information in camera frame */
	//--------------------------------------------------------------------------------------------------------//
	/* get photoneo results */
	if( pho_ros_ctl.client_call() )                             
	{
		ROS_INFO("call to the service");
		pho_ros_ctl.pub_pho_results();
		pho_result_list_msgs_ = pho_ros_ctl.get_pho_results();
		ROS_INFO("Success to call service");
	}
	else
	{	/* if fail, program exits. */
		ROS_ERROR("Failed to call service");
		return EXIT_FAILURE;
	}
	pho_result_m4x4_ = pho_result_list_msgs_.LocalizationPoseList.at(0).Transformation;
	Matrix4d T = Matrix4d::Identity();
	for( int i=0; i<3; i++ )
		for( int j=0; j<4; j++ )
			T(i,j) = pho_result_m4x4_.row.at(i).col.at(j);
	Pose object_in_camera_frame;
	object_in_camera_frame.SetData(T); // x,y,z,qw,qx,qy,qz
	cout << "object_in_camera_frame :" << endl;
	ShowMatrix(object_in_camera_frame.GetData());
	//--------------------------------------------------------------------------------------------------------//

	// calculate object information in camera_robot root frame
	//-----------------------------------------------------------------------------------------------------------------------------------------//
	Pose camera_robot_root_to_object;
	coordinate_transformation.CalculateObjectInCameraRobotRoot(camera_robot_root_to_flange, object_in_camera_frame, camera_robot_root_to_object);
	cout << "(camera_robot) root_to_object :" << endl;
	ShowMatrix(camera_robot_root_to_object.GetData());
	double x,y,z,rx,ry,rz;
	camera_robot_root_to_object.GetData(x,y,z,rx,ry,rz);
	printf("  %10.4lf , %10.4lf , %10.4lf , %10.4lf , %10.4lf , %10.4lf\n",x,y,z,rx,ry,rz);
	//-----------------------------------------------------------------------------------------------------------------------------------------//


	// set gripper object information
	//----------------------------------------------------------------------------------------------------------//
	Matrix4d guide_robot_flange_to_gripper_object = Matrix4d::Identity();
	guide_robot_flange_to_gripper_object(2,3) = 100;
	coordinate_transformation.SetGuideRobotFlangeToGripperObjectTransMatrix(guide_robot_flange_to_gripper_object);
	//----------------------------------------------------------------------------------------------------------//

	/*****************************************************************************************************************************/

	//M5Manipulator m5;
	//Pose m5_robot_root_to_flange;

	//m5_robot_root_to_flange.SetData(1379.956, 289.447, 572.48);

	return EXIT_SUCCESS;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF pho_ros_node_server.cpp ***** */