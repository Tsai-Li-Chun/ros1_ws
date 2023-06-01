#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

/** function declarations **/
bool moveToGoal(double xGoal, double yGoal);

/** declare the coordinates of interest **/
double waypoint1_x = 3.4;
double waypoint1_y = 0.0;
double waypoint2_x = 3.26;
double waypoint2_y = 0.0;
double waypoint3_x = 3.26;
double waypoint3_y = 0.0;
double waypoint4_x = 0.0;
double waypoint4_y = 0.0;

int main(int argc, char** argv)
{
	if( argc!=2 )
	{
		ROS_ERROR("Input Error!");
		return 0;
	}
	int waypoint = atoi(argv[1]);
	// ROS_INFO("%d,%d\n",argc,waypoint);

	/* ros node handle init */
	ros::init(argc, argv, "send_goal");
	ros::NodeHandle n;

	/* send goal */
	switch(waypoint)
	{
		case 0:
			moveToGoal(waypoint1_x, waypoint1_y);
		break;
		case 1:
			moveToGoal(waypoint2_x, waypoint2_y);
		break;
		case 2:
			moveToGoal(waypoint3_x, waypoint3_y);
		break;
		case 3:
			moveToGoal(waypoint4_x, waypoint4_y);
		break;
	}
	ros::spinOnce();
	return 0;
}

bool moveToGoal(double xGoal, double yGoal)
{

	//define a client for to send goal requests to the move_base server through a SimpleActionClient
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

	//wait for the action server to come up
	while(!ac.waitForServer(ros::Duration(5.0)))
	{
		ROS_INFO("Waiting for the move_base action server to come up");
	}

	move_base_msgs::MoveBaseGoal goal;

	//set up the frame parameters
	goal.target_pose.header.frame_id = "map";
	goal.target_pose.header.stamp = ros::Time::now();

	/* moving towards the goal */
	goal.target_pose.pose.position.x =  xGoal;
	goal.target_pose.pose.position.y =  yGoal;
	goal.target_pose.pose.position.z =  0.0;
	goal.target_pose.pose.orientation.x = 0.0;
	goal.target_pose.pose.orientation.y = 0.0;
	goal.target_pose.pose.orientation.z = 0.0;
	goal.target_pose.pose.orientation.w = 1.0;

	ROS_INFO("Sending goal location ...");
	ac.sendGoal(goal);

	ac.waitForResult();

	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
	{
		ROS_INFO("You have reached the destination");
		return true;
	}
	else
	{
		ROS_INFO("The robot failed to reach the destination");
		return false;
	}
}