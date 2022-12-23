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

/* 32Format to 16Format transform union type */
union modbus_32FormatTo16Format
{
    int32_t d32;
    uint16_t d16[ (sizeof(d32))/sizeof(uint16_t) ];
};

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

	/* motor initialize */
	int motorInit(uint32_t OpType, uint32_t Acc, uint32_t Dec, int32_t Trigger);
	/* 直接資料傳輸設定初始化 */
	int BLVD_KRD_DirectDataOperation_setup(uint32_t OpType, uint32_t Acc, uint32_t Dec, int32_t Trigger);
	/* 確認輸出狀態 */
	bool BLVD_KRD_DriverOutputStatus_check(void);

	/* 馬達停止函數 */
	int motorStop();
	/* 馬達激磁函數 */
	int motorSON();
	/* 馬達解磁函數 */
	int motorSOFF();
	/* 前進函數 */
	int motorForward(int32_t);
	/* 後退函數 */
	int motorReverse(int32_t);
	/* 左轉函數 */
	int motorLturn(int32_t);
	/* 右轉函數 */
	int motorRturn(int32_t);

	/* 讀取速度設定值 */
	int readDemandVelocity(int32_t *speed);
	/* 讀取實際速度 */	  
	int readActualVelocity(int32_t *speed);
	/* 讀取目前運轉位置(累計) */
	int readActualPosition(int32_t *position);
	/* 讀取馬達轉矩負載率 */
	int readLoadTorque(int32_t *torque);
	/* 讀取Driver溫度 */
	int readDriverTemperature(int32_t *temperature);
	/* 讀取Motor溫度 */
	int readMotorTemperature(int32_t *temperature);
	/* 讀取Driver耗能 */
	int readPowerConsumption(int32_t *consumption);
	/* 讀取Driver輸出狀態 */
	int readDriverOutputStatus(void);

	/* 寫入工作模式 */
	int writeSetupType(int32_t mode);
	/* 寫入速度設定值 */
	int writeVelocity(int32_t Lvelocity, int32_t Rvelocity);
	/* 寫入加速度設定值 */
	int writeAcceleration(int32_t acc);
	/* 寫入減速度設定值 */
	int writeDecelerate(int32_t dec);
	/* 寫入力矩 */
	int writeTorque(int32_t torque);
	/* 寫入觸發方式 */
	int writeTrigger(int32_t trigger);
	/* 寫入Driver輸入狀態 */
	int writeDriverInputCommand(int32_t status);

	/* 診斷測試 */
	int writeDiagnosis(void);

	/* 讀取目前AlarmCode */
	int readAlarm(int32_t* alarm);
	/* 讀取目前CommunicationError */
	int readCommunicationError(int32_t *err);
	/* reset目前Alarm */
	int ResetAlarm(void);
	/* AlarmCode轉string */
	std::string getStrOfAlarm(uint8_t alarm);

private:
	/* 宣告libmodbus-API的Return用變數 */
	int rc;
	/* 建立modbus通訊結構體 */
	modbus_t* mb;
	/* 建立AlarmCode結構體 */
	BLVD_KRD_RegAdr_Table RegAdr_T;
	/* uint16 temp buffer */
	uint16_t uint16Buffer[20];
	/* uint8 temp buffer */
	uint8_t uint8Buffer[20];
	/*  */
	int32_t motorStatus[2];
	// /* 32bit to 16bit[2] 轉換結構體  */
	// modbus_32FormatTo16Format buffer_32to16;


	/* Setup SlaveID & Connect */
	int Modbus_slave_connect(int);

	/* 讀取Int32t資料 */
	int readInt32t(uint16_t Adr, int32_t *v);
	/* 讀取n筆暫存器資料 */
	int readRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data);

	/* 寫入Int32t資料 */
	int writeInt32t(uint16_t Adr, int32_t *Data);
	/* 寫入n筆暫存器資料 */
	int writeRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data);
	/* 寫入Query */
	int writeQuery(uint16_t L, uint8_t *Data);

	/* int32轉uint16[2] function */
	uint16_t* int32_to_2uint16(int32_t data32, uint16_t *data16);

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
