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
#include <ros/ros.h>
#include "Oriental_BLVD_KRD.hpp"
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

/* enc to mm parameter */
const double enc_to_mm = 0.000561414233148148;

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
int main(int argc, char **argv)
{
	int rc;

    ros::init(argc,argv,"main_motorController");

	/* 建立 BLVD-KRD Control物件*/
    BLVD_KRD_Control BKC("/dev/ttyUSB0",0x0F);
    // BLVD_KRD_Control BKC("/dev/ttyS0",0x0F);
	sleep(1);
	/* motor Init - 事先將觸發方式(-4)等參數set,後續更新速度即可即時變化 */
	rc = BKC.motorInit(48,1000,1000,(-4));
	sleep(1);

		rc = BKC.motorForward(100);
		printf("Forward1 : %d\n",rc);
		sleep(5);
		rc = BKC.motorForward(0);
		printf("Forward0 : %d\n",rc);
		sleep(5);
		rc = BKC.motorReverse(100);
		printf("Reverse1 : %d\n",rc);
		sleep(5);
		rc = BKC.motorReverse(0);
		printf("Reverse0 : %d\n",rc);
		sleep(5);

	rc = BKC.motorSOFF();
	printf("%d\n",rc);
	sleep(1);

	ros::spin();
	ros::shutdown();
	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
