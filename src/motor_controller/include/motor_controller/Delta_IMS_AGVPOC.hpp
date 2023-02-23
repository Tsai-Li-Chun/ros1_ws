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
#include "canalystii_node.h"
#include "AGVPOC_CANopen_ODtable.hpp"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

/* 32Format to 8Format transform union type */
union modbus_32FormatTo8Format
{
    int32_t d32;
    uint16_t d8[ (sizeof(d32))/sizeof(uint8_t) ];
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

	/* CANalystii 初始化函數 */
	int CANalystii_SetUp(void);
	/* CANalystii 設備開啟函數 */
	int CANalystii_open(void);
	/* CANalystii 設備關閉函數 */
	int CANalystii_close(void);

	/* 設定工作模式 */
	int set_OperationMode(uint8_t mode);
	/* SDO發送函式 */
	int SDO_transmit(VCI_CAN_OBJ data);
	/* 馬達激磁函數 */
	int motorSON();
	/* 馬達解磁函數 */
	int motorSOFF();

	/* 寫入加速度設定值 */
	int writeAcceleration(int32_t acc);
	/* 寫入減速度設定值 */
	int writeDecelerate(int32_t dec);
	/* 寫入速度設定值 */
	int writeVelocity(int32_t Lvelocity, int32_t Rvelocity);
	/* 寫入狀態機切換值 */
	int writeStateMachineChange(int32_t dec);

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

private:
	/* 建立CANalystii通訊物件 */
	CANalystii_node can_node;
	/* 建立can設備初始化結構體 */
	VCI_INIT_CONFIG vci_conf;
	/* 建立can通訊資料結構體 */
	VCI_CAN_OBJ can_obj_init,can_obj_Rx,can_obj_Tx;

	/* uint16 temp buffer */
	uint16_t uint16Buffer[20];
	/* uint8 temp buffer */
	uint8_t uint8Buffer[20];

	/* 解析OD資訊用結構體function */
	DeltaMotorOD_Analyze_Struct DeltaMotorOD_Analyze(bool RW, uint32_t);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 function */
	VCI_CAN_OBJ ODAnalyzeToVCICANOBJ(uint16_t, DeltaMotorOD_Analyze_Struct, uint8_t*);

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
