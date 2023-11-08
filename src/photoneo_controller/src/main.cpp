/** ******************************************************
	* @file		main.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "shm_define.h"
#include "shm_controller.hpp"
/* User Includes End */

/* namespace ------------------------------------------------*/
/* namespace Begin */

// using namespace pho::api;

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

// /* print Devices Information function */
// void printDeviceInfoList(const pho::api::PhoXiDeviceInformation);
// /* software trigger single scan function */
// bool SoftwareTrigger_SingleScan(const pho::api::PPhoXi);
// /* Print out profiles */
// void printProfilesList(const std::vector<pho::api::PhoXiProfileDescriptor> &ProfilesInfo);

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
    int i;
    char wait_key;
    float pose[shm_float_size]={0};

    ros::init(argc, argv, "photoneo_controller");
    ros::NodeHandle n;
    ros::Rate loop_rate(100);

	/* Create shared memory control object */
    shared_memory_controller shm_ctl;
    /* create and get shared memory segment */
    int id = shm_ctl.get_shm(shm_key, shm_size, shm_flg);
    if( id == (-1) ) return EXIT_FAILURE;
    /* attach to the shared memory segment */
    void *ptr = shm_ctl.attach_shm(id, nullptr, shm_rw_twoway);
    if( ptr == (void*)(-1) ) return EXIT_FAILURE;
	/* display information of the shm */
    shm_ctl.shmds_information(); 
    std::cout << "----------------------------------" << std::endl << std::endl;

	while ( wait_key != 'q' )
	{
        printf("Press Q to exit the program, press R to read from shared memory: ");
        std::cin >> wait_key;
        if( wait_key == 'r' )
        {
            /* send request */
            printf("Send request for data command\n");
            pose[0] = 1.0f;
            // for(i=1; i<17; i++) pose[i] = 0.0f;
            shm_ctl.write_shm(pose, 1*sizeof(float));
            /* wait for response */
            printf("Waiting for data to return ...\n");
            while( pose[0] != 2.0f )
            {
                shm_ctl.read_shm(pose, 2*sizeof(float));
                // printf("pose[0]=%10.4f , pose[1]=%10.4f\n",pose[0],pose[1]);
                sleep(1);
            }
            shm_ctl.read_shm(pose, (size_t)pose[1]*sizeof(float));
            for(i=0; i<(int)pose[1]; i++) printf("(%10.4f) ",pose[i]);
            printf("\n");
            /* send reset signal */
            printf("Response received. Send reset signal ... ");
            for(i=0; i<shm_float_size; i++) pose[i] = 0.0f;
            shm_ctl.write_shm(pose, sizeof(pose));
            printf("Finish\n\n");
        }
		ros::spinOnce();
		loop_rate.sleep();
	}

    // /* send remove shared memory signal */
    // printf("send remove shared memory signal.\n");
    // pose[0] = 4.0f;
    // shm_ctl.write_shm(pose, 1*sizeof(float));

    /* detach and Remove shared memory segment */
    if( shm_ctl.detach_shm(ptr) == (-1) )
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF fake_cmdvel.cpp ***** */
