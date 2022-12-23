/** ******************************************************
	* @file		register_address_table.h
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __register_address_table_HPP__
#define __register_address_table_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <stdint.h>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */

#define FN_CODE_DIAGNOSIS 0x08
#define MOTOR_FORWARD_BIT B00001000
#define MOTOR_REVERSE_BIT B00010000
#define MOTOR_SLOW_CHANGE_BIT B00100000
#define MOTOR_FREE_ON_STOP_BIT B10000000
#define MOTOR_DIRECTOIN_STOP 0
#define MOTOR_DIRECTOIN_FORWARD 1
#define MOTOR_DIRECTOIN_REVERSE 2

/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

/* 建立AlarmCode表,並創建專有DataType */
enum class BLVD_KRD_Alarm : uint8_t
{
	None = 0,
	Overload = 0x30,
	SensorError = 0x28,
	InitialSensorError = 0x42,
	Overvoltage = 0x22,
	Undervoltage = 0x25,
	Overspeed = 0x31,
	Overcurrent = 0x20,
	EEPROMError = 0x41,
	MainCicruitOverheat = 0x21,
	ExternalStop = 0x6e,
	PreventionOfOperationAtPowerOn = 0x46,
	NetworkBusError = 0x81,
	CommunicationSwitchSettingError = 0x83,
	RS485CommunicationError = 0x84,
	RS485CommunicationTimeout = 0x85,
	NetworkConverterError = 0x8e,
	MainCicruitOutputError = 0x2d
};
typedef BLVD_KRD_Alarm AlarmCode;

/* 建立Register Address表,並創建專有DataType */
enum class BLVD_KRD_RegAdr_Table : uint16_t
{
	/* 一般暫存器位址 */
	ADR_ALARM_H = 0x0080,
	ADR_ALARM_L = 0x0081,
	ADR_COMMAND_SPEED_H = 0x00c8,
	ADR_COMMAND_SPEED_L = 0x00c9,
	ADR_FEEDBACK_SPEED_H = 0x00ce,
	ADR_FEEDBACK_SPEED_L = 0x00cf,
	ADR_LOAD_FACTOR_H = 0x0108,
	ADR_LOAD_FACTOR_L = 0x0109,
	ADR_RESET_ALARM_H = 0x0180,
	ADR_RESET_ALARM_L = 0x0181,
	ADR_SPEED0_H = 0x0480,
	ADR_SPEED0_L = 0x0481,
	ADR_TORQUE_H = 0x0700,
	ADR_TORQUE_L = 0x0701,
	ADR_TORQUE_LIMIT0_H = 0x0700,
	ADR_TORQUE_LIMIT0_L = 0x0701,
	ADR_MOTOR_CONTROL = 0x007d,
	ADR_MOTOR_OUTPUT = 0x007F,
	ADR_ANALOG_MODE_L = 0x10e3,
	ADR_DemandPosition_H = 0x00c6,
	ADR_DemandPosition_L = 0x00c7,
	ADR_ActualPosition_H = 0x00cc,
	ADR_ActualPosition_L = 0x00cd,
	ADR_CONFIG_H = 0x018c,
	ADR_CONFIG_L = 0x018d,
	/* 共享ID模式 讀取暫存器位址 */
	ADRS_r_DemandVelocity  = 0x0000,
	ADRS_r_ActualVelocity = 0x0002,
	ADRS_r_ActualPosition = 0x0004,
	ADRS_r_PresentTorque  = 0x0006,
	ADRS_r_PresentAlarm = 0x0008,
	ADRS_r_CommunicationError = 0x000a,
	ADRS_r_DriverTemperature = 0x000c,
	ADRS_r_MotorTemperature = 0x000e,
	ADRS_r_PowerConsumption = 0x0010,
	ADRS_r_DriverOutputStatus = 0x0012,
	/* 共享ID模式 寫入暫存器位址 */
	ADRS_w_SetupType = 0x0000,
	ADRS_w_Velocity = 0x0002,
	ADRS_w_Acc = 0x0004,
	ADRS_w_Dec = 0x0006,
	ADRS_w_Torque = 0x0008,
	ADRS_w_Trigger = 0x000a,
	ADRS_w_DriverInputCommand = 0x000c
};
typedef BLVD_KRD_RegAdr_Table BLVD_KRD_RegAdr_Table;

/* 建立Driver Input Command Table表,並創建專有DataType */
enum class DriverInputCommand_Table : uint32_t
{
	DriverInputCommand_AllOff	= 0x00000000,
	DriverInputCommand_SON 		= 0x00000001,
	DriverInputCommand_STOP		= 0x00000020,
	DriverInputCommand_ALM_RST	= 0x00000080
};
typedef DriverInputCommand_Table DriverInputCommand_Table;

/* 建立Driver Output Command Table表,並創建專有DataType */
enum class DriverOutputCommand_Table : uint32_t
{
	DriverOutputCommand_AllOff	= 0x00000000,
	DriverOutputCommand_SON 	= 0x00000001,
	DriverOutputCommand_RDY_DD 	= 0x00000008,
	DriverOutputCommand_STOP	= 0x00000020,
	DriverOutputCommand_ALM		= 0x00000080
};
typedef DriverOutputCommand_Table DriverOutputCommand_Table;

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


#endif /*__register_address_table_HPP__ */

/* ***** END OF register_address_table.HPP ***** */
