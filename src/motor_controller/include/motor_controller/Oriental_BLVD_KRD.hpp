/** ******************************************************
	* @file		Oriental_BLVD_KRD.hpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __Oriental_BLVD_KRD_HPP__
#define __Oriental_BLVD_KRD_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <iostream>
#include <string>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include <ros/ros.h>
#include "modbus/modbus.h"
#include "register_address_table.hpp"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */
/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */
/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

class BLVD_KRD_Control 
{
public:
	/* 建構函數 */
	BLVD_KRD_Control(const char* device, int slave,
						   int BR=115200,
						   char parity='E',
						   int data_bit=8,
						   int stop_bit=1	);
	/* 解建構函數 */
	~BLVD_KRD_Control();

	/* 直接資料傳輸設定輸出化 */
	uint8_t BLVD_KRD_DirectDataOperation_setup(uint8_t OpType, uint16_t Acc, uint16_t Dec, uint8_t Trigger);

	/* 馬達停止函數 */
	uint8_t motorStop();
	/* 馬達激磁函數 */
	uint8_t motorSON();
	/* 馬達解磁函數 */
	uint8_t motorSOFF();
	/* 前進函數 */
	uint8_t motorForward(int);
	/* 後退函數 */
	uint8_t motorReverse(int);
	/* 左轉函數 */
	uint8_t motorLturn(int);
	/* 右轉函數 */
	uint8_t motorRturn(int);

	/* 讀取速度設定值 */
	uint8_t readDemandVelocity(int32_t *speed);
	/* 讀取實際速度 */	  
	uint8_t readActualVelocity(int32_t *speed);
	/* 讀取目前運轉位置(累計) */
	uint8_t readActualPosition(int32_t *position);
	/* 讀取馬達轉矩負載率 */
	uint8_t readLoadTorque(int32_t *torque);
	/* 讀取Driver溫度 */
	uint8_t readDriverTemperature(int32_t *temperature);
	/* 讀取Motor溫度 */
	uint8_t readMotorTemperature(int32_t *temperature);
	/* 讀取Driver耗能 */
	uint8_t readPowerConsumption(int32_t *consumption);
	/* 讀取Driver輸出狀態 */
	uint8_t readDriverOutputStatus(int32_t *status);

	/* 寫入工作模式 */
	uint8_t writeSetupType(int32_t mode);
	/* 寫入速度設定值 */
	uint8_t writeVelocity(int32_t velocity);
	/* 寫入加速度設定值 */
	uint8_t writeAcceleration(int32_t acc);
	/* 寫入減速度設定值 */
	uint8_t writeDecelerate(int32_t dec);
	/* 寫入力矩 */
	uint8_t writeTorque(int32_t torque);
	/* 寫入觸發方式 */
	uint8_t writeTrigger(int32_t trigger);
	/* 寫入Driver輸入狀態 */
	uint8_t writeDriverInputCommand(int32_t status);

	/* 診斷測試 */
	uint8_t writeDiagnosis(void);

	/* 讀取目前AlarmCode */
	uint8_t readAlarm(int32_t* alarm);
	/* 讀取目前CommunicationError */
	uint8_t readCommunicationError(int32_t *err);
	/* reset目前Alarm */
	uint8_t ResetAlarm(void);
	/* AlarmCode轉string */
	std::string getStrOfAlarm(uint8_t alarm);

private:
	/* 建立modbus通訊結構體 */
	modbus_t* mb;
	/* Setup SlaveID & Connect */
	int Modbus_slave_connect(int);

	BLVD_KRD_RegAdr_Table RegAdr_T;

	/* 讀取Int32t資料 */
	uint8_t readInt32t(uint16_t Adr, int32_t *v);
	/* 讀取n筆暫存器資料 */
	int readRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data);

	/* 寫入Int32t資料 */
	uint8_t writeInt32t(uint16_t Adr, int32_t *Data);
	/* 寫入n筆暫存器資料 */
	uint8_t writeRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data);
	/* 寫入Query */
	uint8_t writeQuery(uint8_t FC, uint16_t L, uint8_t *Data);
	uint8_t writeQuery(void);

	/* uint16 temp buffer */
	uint16_t uint16Buffer[40];
	/* uint8 temp buffer */
	uint8_t uint8Buffer[20];
};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__Oriental_BLVD_KRD_HPP__ */

/* ***** END OF Oriental_BLVD_KRD.HPP ***** */
