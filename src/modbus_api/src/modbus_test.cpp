/** ******************************************************
	* @file		modbus_test.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <stdio.h>
#include <errno.h>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <modbus.h>
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
int main(int argc, char **argv)
{
    modbus_t *ctx;
    // uint8_t raw_req[] = { 0x01, 06, 0x02, 0x87, 0x01, 0x23 };
    uint16_t raw_req[] = { 0, 48, 0, 0, 0x0000, 0x0, 0, 1000, 0, 2500, 0, 1000, 0, 1 };
    // uint16_t raw_req[] = { 0, 48, 0, 0, 0xFFFF, 0xFE0C, 0, 1000, 0, 2500, 0, 1000, 0, 1 };
    // uint16_t raw_req[] = { 0, 48, 0, 0, 0xFFFF, 0xFC18, 0, 1000, 0, 2500, 0, 1000, 0, 1 };
    // uint16_t raw_req[] = { 0, 0 };
    uint16_t rsp[64];
    int req_length,i;

	ros::init(argc, argv, "modbus_test");

    ctx = modbus_new_rtu("/dev/ttyS0", 115200, 'E', 8, 1);
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_slave(ctx, 1);

    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
    else
    {
        printf("OK\n");
    }

    // req_length = modbus_write_registers(ctx, 0x007C, 2, raw_req);
    req_length = modbus_write_registers(ctx, 0x005A, 0x000E, raw_req);
    printf("%d\n", req_length);
    // req_length = modbus_read_registers(ctx, 646, 2, rsp);
    // printf("%d\n", req_length);
    // if( req_length >0 )
    //     for(i=0; i<req_length; i++)
    //         printf("rsp[%d]= 0x%04X\n", i, rsp[i]);


    modbus_close(ctx);
    modbus_free(ctx);
	
	ros::shutdown();
	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
