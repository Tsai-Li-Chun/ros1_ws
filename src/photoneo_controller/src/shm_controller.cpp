/** ******************************************************
	* @file		shm_controller.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "shm_controller.hpp"
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

/** * @brief Constructor
 	* @param None
 	* @return None
**	**/
shared_memory_controller::shared_memory_controller()
{
	shm_id = 0;
	return_code = 0;
	shm_void_ptr = nullptr;
}
/** * @brief Destructor
 	* @param None
 	* @return None
**	**/
shared_memory_controller::~shared_memory_controller()
{
}

/** * @brief create and get shared memory segment
 	* @param key_t key, identification key
 	* @param size_t size, shared memory size
 	* @param int flg, control flag value
 	* @return int8_t, unique identification ID number
**	**/
int shared_memory_controller::shm_StartUp(key_t key, size_t size, int ctlflg, int rwflg)
{
    /* create and get shared memory segment */
    if( get_shm(key, size, ctlflg) == (-1) ) return EXIT_FAILURE;
    /* attach to the shared memory segment */
    if( attach_shm(rwflg) == (void*)(-1) ) return EXIT_FAILURE;
	/* display information of the shm */
    shmds_information(); 
    // std::cout << "----------------------------------" << std::endl << std::endl;
	return EXIT_SUCCESS;
}

/** * @brief create and get shared memory segment
 	* @param key_t key, identification key
 	* @param size_t size, shared memory size
 	* @param int flg, control flag value
 	* @return int8_t, unique identification ID number
**	**/
int shared_memory_controller::get_shm(key_t key, size_t size, int flg)
{
	shm_id = shmget(key, size, flg);
	if( shm_id != (-1) )
        printf("Create shared memory Success. shm_id=: %d\n",shm_id);
    else
    {
        printf("Create shared meeory Failed.\n");
        perror(" -> shmget error code: ");
    }
	return shm_id;
}

/** * @brief attach to the shared memory segment
 	* @param int id, unique identification ID number
 	* @param void* ptr, specify address to this program
 	* @param int flg, control flag value(R/W)
 	* @return void*, address linked to this program
**	**/
void* shared_memory_controller::attach_shm(int flg)
{
	shm_void_ptr = shmat(shm_id, NULL, flg);
	if( shm_void_ptr != (void*)(-1) )
		printf("Attach shared memroy Success. shm_ptr=: %p\n",shm_void_ptr);
	else
	{
		printf("Create shared meeory Failed.\n");
		perror(" -> shmat error code: ");
	}
	return shm_void_ptr;
}

/** * @brief detach from shared memory segment
 	* @param void* ptr, specify address to this program
 	* @return void*, error code
**	**/
int8_t shared_memory_controller::detach_shm(void)
{
	printf("Recevied detach shared memory command.\n");
	return_code = shmdt(shm_void_ptr);
	if( return_code != (-1) )
        printf("Detach shared meeory Success.\n");
    else
    {
        printf("Detach shared meeory Failed.\n");
        perror(" -> shmdt error code: ");
    }
	return return_code;
}

/** * @brief remove from shared memory segment
 	* @param void* ptr, specify address to this program
 	* @return void*, error code
**	**/
int8_t shared_memory_controller::remove_shm(void)
{
	printf("Recevied remove shared memory command.\n");
	return_code = shmctl(shm_id, IPC_RMID, NULL);
	if( return_code != (-1) )
        printf("Remove shared meeory Success.\n");
    else
    {
        printf("Remove shared meeory Failed.\n");
        perror(" -> shmctl error code: ");
    }
	return return_code;
}

/** * @brief display information of the shm
 	* @param None
 	* @return None
**	**/
int shared_memory_controller::shmds_information(void)
{
	return_code = shmctl(shm_id, IPC_STAT, &shm_ds);
	if( return_code == 0 )
    {
        // printf("shm_ds.shm_perm.uid = %d\n",shm_ds.shm_perm.uid);
        // printf("shm_ds.shm_perm.mode= %d\n",shm_ds.shm_perm.mode);
        // printf("shm_ds.shm_perm.gid = %d\n",shm_ds.shm_perm.gid);
        // printf("shm_ds.shm_perm.cuid= %d\n",shm_ds.shm_perm.cuid);
        // printf("shm_ds.shm_perm.cgid= %d\n",shm_ds.shm_perm.cgid);
        printf("shm_ds.shm_segsz    = %ld\n",shm_ds.shm_segsz);
        printf("shm_ds.shm_nattch   = %ld\n",shm_ds.shm_nattch);
        // printf("shm_ds.shm_lpid     = %d\n",shm_ds.shm_lpid);
        // printf("shm_ds.shm_cpid     = %d\n",shm_ds.shm_cpid);
        // printf("shm_ds.shm_ctime    = %ld\n",shm_ds.shm_ctime);
        // printf("shm_ds.shm_atime    = %ld\n",shm_ds.shm_atime);
        // printf("shm_ds.shm_dtime    = %ld\n",shm_ds.shm_dtime);
    }
    else
        perror("shmctl error code: ");
	return (int)return_code;
}

/** * @brief write data to shared memory
 	* @param void* source, source of Data
 	* @param size_t size, length of bytes to be copy
 	* @return void* , address of the destination, null if unsuccessful
**	**/
void* shared_memory_controller::write_shm(void* source ,size_t size)
{
	return_code_ptr = memcpy(shm_void_ptr, source, size);
	return return_code_ptr;
}

/** * @brief read data from shared memory
 	* @param void* destination, destination to copy to
 	* @param size_t size, length of bytes to be copy
 	* @return void* , address of the destination, null if unsuccessful
**	**/
void* shared_memory_controller::read_shm(void* destination, size_t size)
{
	return_code_ptr = memcpy(destination, shm_void_ptr, size);
	return return_code_ptr;
}


/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF shm_controller.cpp ***** */
