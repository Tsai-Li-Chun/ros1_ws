/** ******************************************************
	* @file		AGVPOC_CANopen_ODtable.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __AGVPOC_CANopen_ODtable_HPP__
#define __AGVPOC_CANopen_ODtable_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <stdint.h>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

/* 建立CANopen Function Code Table表,並創建專有DataType */
enum class CANopenFunctionCode_Table : uint16_t
{
	NMT_Service	= 0x000,
	SYNC 	= 0x080,
	EMCY 	= 0x080,
	TxPDO1	= 0x180,
	RxPDO1	= 0x200,
	TxPDO2	= 0x280,
	RxPDO2	= 0x300,
	TxPDO3	= 0x380,
	RxPDO3	= 0x400,
	TxPDO4	= 0x480,
	RxPDO4	= 0x500,
	TxSDO	= 0x580,
	RxSDO	= 0x600,
	NMT_Err_control	= 0x700
};
typedef CANopenFunctionCode_Table CANopenFunctionCode_Table;

/* 建立CANopen SDO CommandCode Table表,並創建專有DataType */
enum class CANopenSDO_CommandCode_Table : uint8_t
{
	writeT_4byte= 0x23,
	writeT_2byte= 0x2B,
	writeT_1byte= 0x2F,
	writeR_Pass	= 0x60,
	writeR_Err	= 0x80,
	readT		= 0x40,
	readR_4byte	= 0x43,
	readR_2byte	= 0x4B,
	readR_1byte	= 0x4F,
	readR_Err	= 0x80
};
typedef CANopenSDO_CommandCode_Table CANopenSDO_CommandCode_Table;

// /* 未完成,待補 */
// /* 建立CANopen SDO AbortCode Table表,並創建專有DataType */
// enum class CANopenSDO_AbortCode_Table : uint8_t
// {
// };
// typedef CANopenSDO_AbortCode_Table CANopenSDO_AbortCode_Table;

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


#endif /*__AGVPOC_CANopen_ODtable_HPP__ */

/* ***** END OF AGVPOC_CANopen_ODtable.HPP ***** */
