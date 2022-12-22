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

const double enc_to_mm = 0.000561414233148148;

/* Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


union modbus_32Format
{
    int f;	//float-4byte, u16-2byte, ch[2]
    int16_t ch[ (sizeof(f))/sizeof(int16_t) ];
};


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
	int32_t p[2];
	double p_mm[2];

    ros::init(argc,argv,"main_motorController");

    BLVD_KRD_Control BKC("/dev/ttyUSB0",0x0F);
    // BLVD_KRD_Control BKC("/dev/ttyS0",0x0F);
	sleep(1);

	rc = BKC.motorSON();
	printf("%d\n",rc);
	sleep(5);
	BKC.motorSOFF();
	printf("%d\n",rc);
	sleep(1);


	// while(1)
	// {
	// 	// printf("aaa\n");
	// 	rc = BKC.readDriverTemperature(p);
	// 	printf("%d, %d, %d\n",rc,p[1],p[0]);
	// 	// p_mm[0] = ((double)p[0])*enc_to_mm;
	// 	// p_mm[1] = ((double)p[1])*enc_to_mm;
	// 	// printf("%d, %lf, %lf\n",rc,p_mm[1],p_mm[0]);
	// 	sleep(1);
	// }


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
