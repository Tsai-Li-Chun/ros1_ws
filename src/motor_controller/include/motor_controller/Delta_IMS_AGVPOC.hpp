/** ******************************************************
	* @file		Delta_IMS_AGVPOC.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __Delta_IMS_AGVPOC_HPP__
#define __Delta_IMS_AGVPOC_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <iostream>
#include <string>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "canalystii.h"
#include "AGVPOC_CANopen_ODtable.hpp"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

/* 32Format to 8Format transform union type */
union FormatTo_8_16_32
{
    int32_t		d32;
    uint32_t	ud32;
    int16_t		d16[2];
    uint16_t	ud16[2];
    int8_t		d8[4];
    uint8_t		ud8[4];
};

/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

/*  */
class Delta_IMS_Motor_Control
{
public:
	/* 建構函數 */
	Delta_IMS_Motor_Control();
	/* 解建構函數 */
	~Delta_IMS_Motor_Control();

	int32_t Actual_Speed[2];

	/* CANalystii 初始化函數 */
	int CANalystii_SetUp(void);
	/* CANalystii 設備開啟函數 */
	int CANalystii_open(void);
	/* CANalystii 設備關閉函數 */
	int CANalystii_close(void);

	/* 設定工作模式 */
	int set_OperationMode(uint8_t mode);
	void clear_RxBuffer(void);
	/* 馬達激磁函數 */
	int motorSON();
	/* 馬達解磁函數 */
	int motorSOFF();
	/* 寫入狀態機切換值 */
	int writeStateMachineChange(uint8_t*);

	/* 寫入加速度設定值 */
	int writeAcceleration(uint32_t acc);
	/* 寫入減速度設定值 */
	int writeDecelerate(uint32_t dec);
	/* 寫入速度設定值 */
	int writeVelocity(int32_t Lvelocity, int32_t Rvelocity);


	/* 讀取速度設定值 */
	int readDemandVelocity(int32_t *speed);
	/* 讀取實際速度 */	  
	int readActualVelocity(int32_t *speed);
	/* 讀取目前運轉位置(累計) */
	int readActualPosition(int32_t *position);
	/* 讀取馬達轉矩負載率 */
	int readLoadTorque(int32_t *torque);
	/* 讀取Driver耗能 */
	int readPowerConsumption(int32_t *consumption);

	/* 讀取目前AlarmCode */
	int readAlarm(int32_t* alarm);
	/* reset目前Alarm */
	int ResetAlarm(void);
	/* AlarmCode轉string */
	std::string getStrOfAlarm(uint8_t alarm);
	
	/* SDO發送函式 */
	int SDO_transmit(bool, uint8_t);
	/* SDO接收函式 */
	int SDO_receive(bool, uint8_t);

private:
	/* 是否有資料要取用旗標 */
	uint8_t Waiting_Takeout_Data;
	/* 建立CANalystii通訊物件 */
	CANalystii can_node;
	/* 建立can設備初始化結構體 */
	VCI_INIT_CONFIG vci_conf;
	/* 建立can通訊資料結構體 */
	VCI_CAN_OBJ can_obj_init,can_obj_Rx,can_obj_Tx;
	/* 建立解析OD資訊用結構體 */
	DeltaMotorOD_Struct OD_struct_init,OD_struct_Rx,OD_struct_Tx;

	/* uint16 temp buffer */
	uint16_t uint16Buffer[20];
	/* uint8 temp buffer */
	uint8_t uint8Buffer[20];

	/* 解析OD資訊用結構體function */
	void set_DeltaMotorOD_Struct(bool RW, uint32_t);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 function */
	void DeltaMotorOD_To_CANalystiiOBJ(uint16_t, uint8_t*);
	/* VCI_CAN_OBJ結構體 to OD資訊用結構體 function */
	void CANalystiiOBJ_To_DeltaMotorOD(bool, uint8_t);

	/* 檢查SDOTx回覆函式 */
	bool SDOTx_receive_check(void);
	/* 檢查SDORx回覆函式 */
	bool SDORx_receive_check(void);

	/* int32轉uint16[2] function */
	uint16_t* int32_to_4uint8(int32_t data32, uint8_t *data8);

};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__Delta_IMS_AGVPOC_HPP__ */

/* ***** END OF Delta_IMS_AGVPOC.HPP ***** */
