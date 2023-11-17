/** ******************************************************
	* @file		define.h
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __define_H__
#define __define_H__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */

#define shm_key (key_t)15
#define shm_size (size_t)524288 /* 512KB */
#define shm_float_size (shm_size/sizeof(float))
#define shm_flg (int)(IPC_CREAT|0666)
#define shm_rw_twoway (int)0
#define shm_rw_readonly (int)SHM_RDONLY

#define pho_results_MAXquantity 10
#define loc_base_quantity 19
#define loc_offset_ID 2
#define loc_offset_Oc 3
#define loc_offset_VO 4
#define loc_offset_TF(i,j) (5+(4*i)+j)

/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */
/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /* __define_H__ */

/* ***** END OF main.HPP ***** */