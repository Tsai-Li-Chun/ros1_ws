/** ******************************************************
	* @file		modbus_test.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
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
    const double enc_to_mm = 0.000561414233148148;
    modbus_t *MD_L, *MD_R, *MD;
    // uint16_t raw_req_SON[]={ 0, 0, 0, 0 };
    uint8_t raw_req_SON[]={0x0F, MODBUS_FC_WRITE_MULTIPLE_REGISTERS, 0x00, 0x0c, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01 };
    uint8_t raw_req_SOFF[]={0x0F, MODBUS_FC_WRITE_MULTIPLE_REGISTERS, 0x00, 0x0c, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // uint16_t raw_req_Z[] = { 0,  0, 0, 0, 0,    0, 0, 2000, 0, 2000, 0, 1000, 0, 1 };
    // uint16_t raw_req_F[] = { 0, 48, 0, 0, 0, 1000, 0, 2000, 0, 2000, 0, 1000, 0, 1 };
    // uint16_t raw_req_B[] = { 0, 48, 0, 0, 0xFFFF, 0xFE0C, 0, 1000, 0, 2500, 0, 1000, 0, 1 };
    uint16_t raw_req[] = { 0, 48, 0xFFFF, 0xFE0C, 0, 1000, 0, 1000, 0, 1000, 0, 1, 0, 48, 0xFFFF, 0xFE0C, 0, 1000, 0, 1000, 0, 1000, 0, 1 };
    uint16_t Actual_Position[2] = {0};
    uint8_t rsp[10];
    uint8_t rc;
    int wheel_L_mm,wheel_R_mm;
    int wheel_L_enc,wheel_R_enc;
    int req_length,i,j;

	ros::init(argc, argv, "modbus_test");

    // MD_L = modbus_new_rtu("/dev/ttyUSB0", 115200, 'E', 8, 1);
    // if (MD_L == NULL)
    // {
    //     fprintf(stderr, "Unable to create the libmodbus context\n");
    //     return -1;
    // }
    // modbus_set_slave(MD_L, 1);
    // if (modbus_connect(MD_L) == -1)
    // {
    //     fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    //     modbus_free(MD_L);
    //     return -1;
    // }
    // else printf("MD_L OK\n");

    // MD_R = modbus_new_rtu("/dev/ttyS0", 115200, 'E', 8, 1);
    // if (MD_R == NULL)
    // {
    //     fprintf(stderr, "Unable to create the libmodbus context\n");
    //     return -1;
    // }
    // modbus_set_slave(MD_R, 2);
    // if (modbus_connect(MD_R) == -1)
    // {
    //     fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    //     modbus_free(MD_R);
    //     return -1;
    // }
    // else printf("MD_R OK\n");

    MD = modbus_new_rtu("/dev/ttyUSB0", 115200, 'E', 8, 1);
    if (MD == NULL)
    {
        fprintf(stderr, "Unable to create the libmodbus context\n");
        return -1;
    }
    modbus_set_slave(MD, 15);
    if (modbus_connect(MD) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(MD);
        return -1;
    }
    else printf("MD OK\n");


    // req_length = modbus_send_raw_request(MD, raw_req_SON, 15*sizeof(uint8_t));
    req_length = modbus_send_raw_request(MD, raw_req_SOFF, 15*sizeof(uint8_t));
    rc = modbus_receive_confirmation(MD, rsp);
    printf("%d\n", req_length);
    printf("%d\n", rc);
    sleep(1);
    // req_length = modbus_write_registers(MD, 0x0000, 0x0018, raw_req);
    // printf("%d\n", req_length);
    // sleep(1);

    while(1)
    {
        req_length = modbus_read_registers(MD_L, 154, 2, Actual_Position);
        printf("%02d, %04x %04x  |  ", req_length, Actual_Position[0], Actual_Position[1]);
        wheel_L_enc = 0;
        wheel_L_enc = (wheel_L_enc|Actual_Position[0])<<16;
        wheel_L_enc |= Actual_Position[1];
        // for(j=0; j<20; j++)for(i=0; i<65535; i++){}
        sleep(1);
        req_length = modbus_read_registers(MD_R, 154, 2, Actual_Position);
        printf("%02d, %04x %04x\n", req_length, Actual_Position[0], Actual_Position[1]);
        wheel_R_enc = 0;
        wheel_R_enc = (wheel_R_enc|Actual_Position[0])<<16;
        wheel_R_enc |= Actual_Position[1];

        printf("%10d, %10d\n", wheel_L_enc, wheel_R_enc);
        printf("%10.2lf, %10.2lf\n", enc_to_mm*wheel_L_enc, enc_to_mm*wheel_R_enc);



        sleep(1);
    }

    // req_length = modbus_write_registers(MD_L, 0x007C, 2, raw_req_SON);
    // req_length = modbus_write_register(MD_L, 0x007D, 0);
    // printf("%d\n", req_length);
    // sleep(1);

    // req_length = modbus_write_registers(MD_L, 0x005A, 0x000E, raw_req_B);
    // printf("%d\n", req_length);
    // sleep(1);

    // req_length = modbus_write_registers(MD_L, 0x005A, 0x000E, raw_req_Z);
    // printf("%d\n", req_length);
    // sleep(1);

    // req_length = modbus_read_registers(MD_L, 646, 2, rsp);
    // printf("%d\n", req_length);
    // if( req_length >0 )
    //     for(i=0; i<req_length; i++)
    //         printf("rsp[%d]= 0x%04X\n", i, rsp[i]);


    modbus_close(MD);
    modbus_free(MD);
    // modbus_close(MD_L);
    // modbus_free(MD_L);
    // modbus_close(MD_R);
    // modbus_free(MD_R);
	
	ros::shutdown();
	/* main quit */
	return 0;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF modbus_test.cpp ***** */
