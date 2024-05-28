/** ******************************************************
	* @file		modbus_test.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include "Oriental_BLVD_KRD.hpp"
#include "motor_feedback_msgs/msg/motor_feedback.hpp"
#include <chrono>

using namespace std::chrono_literals;
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

/* 建立 BLVD-KRD Control物件*/
BLVD_KRD_Control BKC("/dev/USBport-motor",0x0F);
/* 建立topic-Publisher物件 */
//auto motor_fb;
/* 建立message twist物件 */
geometry_msgs::msg::Twist twist_last;
/* 宣告存放左右兩輪累計距離用變數 */
int32_t position[2]={0};
/* 宣告存放左右兩輪實際速度用變數 */
int32_t velocity_A[2]={0};
/* 宣告存放左右兩輪理論速度用變數 */
int32_t velocity_D[2]={0};
/* 建立message Inertia物件 */
motor_feedback_msgs::msg::MotorFeedback mf;
/* 宣告左右輪速度 */
int32_t velL=0,velR=0;

const double ms_to_rpm = 2976.40418132187;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */

void twist_callback(const geometry_msgs::msg::Twist& twist_msg);
void timer_callback( );

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
int main(int argc, char **argv)
{
	/* 宣告libmodbus-API的Return用變數 */
	int rc;
	/* 建立ROS time物件 */
	rclcpp::Time current_time, last_time;
	/* delta time(s) */
	double dt;
	/* ros init */
    //ros::init(argc,argv,"main_motorController");
	rclcpp::init(argc, argv);

	/* 建立NodeHandle物件 */
	//ros::NodeHandle nh;
	auto node = rclcpp::Node::make_shared("main_motorController");

	/* 建立topic-Subscriber物件並初始化 */
	//ros::Subscriber twist_sub = nh.subscribe("/cmd_vel", 100, twist_callback);
	auto twist_sub = node->create_subscription<geometry_msgs::msg::Twist>("/cmd_vel", 100, twist_callback);
	/* 建立計時物件並初始化計時中斷 */
	rclcpp::TimerBase::SharedPtr timer = node->create_wall_timer(0.01s, std::bind(timer_callback));
	/* 初始化motor_fb物件 */
	auto motor_fb = node->create_publisher<motor_feedback_msgs::msg::MotorFeedback>("/motor_feedback",100);
	/* 建立delay用物件 */
	rclcpp::Rate loop_rate(100);

	sleep(1);
	/* motor Init - 事先將觸發方式(-4)等參數set,後續更新速度即可即時變化 */
	rc = BKC.motorInit(48,300,300,(-4));
	sleep(1);

	current_time = node->now();
	last_time = current_time;

	while (rclcpp::ok())
	{
		current_time = node->now();
		dt = (current_time - last_time).seconds();
		// ROS_INFO("%lf\n",dt);
		// BKC.readActualPosition(position);
		// BKC.readActualVelocity(velocity_A);
		BKC.readDemandVelocity(velocity_D);

		mf.header.stamp = current_time;
		mf.position_l = 0;
		mf.position_r = 0;
		mf.a_velocity_l = velocity_D[0];
		mf.a_velocity_r = velocity_D[1];
		// mf.DvelocityL = velocity_D[0];
		// mf.DvelocityR = velocity_D[1];
		// printf("%010d(%010d) , %010d(%010d)\n",velocity_D[0],velocity_A[0],velocity_D[1],velocity_A[1]);

		motor_fb->publish(mf);
		last_time = current_time;

		rclcpp::spin_some(node);
		loop_rate.sleep();
	}
	// ros::spin();

	rc = BKC.motorSOFF();
	RCLCPP_INFO(node->get_logger(),"rc = %d , motor SOFF",rc);
	sleep(1);

	rclcpp::shutdown();
	/* main quit */
	return 0;
}

void twist_callback(const geometry_msgs::msg::Twist& twist_msg)
{
	twist_last = twist_msg;
	double velLtmp,velRtmp;
	//if(twist_last.linear.x>1.2) twist_last.linear.x=1.2;
	//else if(twist_last.linear.x<(-1.2)) twist_last.linear.x=(-1.2);
	//if(twist_last.angular.z>1.0) twist_last.angular.z=1.0;
	//else if(twist_last.angular.z<(-1.0)) twist_last.angular.z=(-1.0);
	velLtmp = twist_last.linear.x - (twist_last.angular.z/2) ;
	velRtmp = twist_last.linear.x + (twist_last.angular.z/2) ;
	// printf("%lf , %lf\n",velLtmp,velRtmp);
	velL = (int32_t)(velLtmp*ms_to_rpm);
	velR = (int32_t)(velRtmp*ms_to_rpm);
	BKC.writeVelocity(velL,velR);
}

void timer_callback( )
{

}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
