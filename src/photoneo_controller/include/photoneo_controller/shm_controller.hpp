/** ******************************************************
	* @file		shm_controller.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __shm_controller_HPP__
#define __shm_controller_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <iostream>
#include <sys/shm.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "shm_define.h"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

/* shared memory control object */
class shared_memory_controller
{
/* private members */
private:
	shmid_ds shm_ds;
    int8_t shm_id;
	int8_t return_code;
    void *shm_void_ptr;
    void *return_code_ptr;

/* public members */
public:
	/* Constructor */
	shared_memory_controller(void);
	/* Destructor */
	~shared_memory_controller();

	/* shm start up */
	int shm_StartUp(key_t key, size_t size, int ctlflg, int rwflg);
	/* create and get shared memory segment */
	int get_shm(key_t key, size_t size, int flg);
	/* attach to the shared memory segment */
	void* attach_shm(int flg);
	/* detach from shared memory segment */
	int8_t detach_shm(void);
	/* remove from shared memory segment */
	int8_t remove_shm(void);
	/* display information of the shm */
	int shmds_information(void);

	/* write data to shared memory. */
	void* write_shm(void* source, size_t size);
	/* read data from shared memory. */
	void* read_shm(void* destination, size_t size);
};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__shm_controller_HPP__ */

/* ***** END OF shm_controller.HPP ***** */
