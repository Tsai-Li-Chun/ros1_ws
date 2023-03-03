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

#define L_MOTOR (uint8_t)2
#define R_MOTOR (uint8_t)1

/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

/* 建立CANopen Function Code Table表,並創建專有DataType */
enum class FunctionCodeTable : uint16_t
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
typedef FunctionCodeTable FunctionCodeTable;

/* 建立CANopen SDO CommandCode Table表,並創建專有DataType */
enum class SDO_CommandCodeTable : uint8_t
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
typedef SDO_CommandCodeTable SDO_CommandCodeTable;

/* 宣告Delya IMS馬達 解析OD資訊用結構體 */
typedef struct _Delta_IMS_MOTOR_OD{
	bool read_write;
	uint16_t index_main;
	uint8_t index_sub_amount;
	uint8_t index_sub;
	uint8_t dataLen;
	int32_t data;
}DeltaMotorOD_Struct;

/* 建立DeltaIMS_Motor ObjectDictionary Table表,並創建專有DataType */
enum class ObjectDictionaryTable : uint32_t
{
	DeviveType 							= 0x10000004,
	ErrorRegister						= 0x10010001,
	PreDefinedError 					= 0x10030504,
		PreDefinedError_sub 				= 0x00044444,
	COB_ID_SYNC							= 0x10050004,
	CommunicationCyclePeriod 			= 0x10060004,
	GuardTime							= 0x100C0002,
	LifeTimeFactor 						= 0x100D0001,
	StoreParameters 					= 0x10100104,
		StoreParameters_Sub 				= 0x00000004,
	RestoreParameters 					= 0x10110104,
		RestoreParameters_Sub 				= 0x00000004,
	COB_ID_Emergency 					= 0x10140004,
	ConsumerHeartbeatTime				= 0x10160104,
		ConsumerHeartbeatTime_Sub			= 0x00000004,
	ProducerHeartbeatTime				= 0x10170002,
	IdentityObject						= 0x1018030B,
		IdentityObject_Sub					= 0x00000444,
	ErrorBehavior						= 0x10290101,
	ErrorBehavior_Sub					= 0x00000001,
	ServerSDO_Parameter					= 0x1200020B,
		ServerSDO_Parameter_Sub				= 0x00000044,
	ReceivePDO1_CommunicationParameter	= 0x1400050B,
	ReceivePDO2_CommunicationParameter	= 0x1401050B,
	ReceivePDO3_CommunicationParameter	= 0x1402050B,
	ReceivePDO4_CommunicationParameter	= 0x1403050B,
		ReceivePDO_CommunicationParameter_Sub= 0x00021214,
	ReceivePDO1_MappingParameter		= 0x1600080B,
	ReceivePDO2_MappingParameter		= 0x1601080B,
	ReceivePDO3_MappingParameter		= 0x1602080B,
	ReceivePDO4_MappingParameter		= 0x1603080B,
		ReceivePDO_MappingParameter_Sub		= 0x44444444,
	TransmitPDO1_CommunicationParameter	= 0x1800050B,
	TransmitPDO2_CommunicationParameter	= 0x1801050B,
	TransmitPDO3_CommunicationParameter	= 0x1802050B,
	TransmitPDO4_CommunicationParameter	= 0x1803050B,
		TransmitPDO_CommunicationParameter_Sub= 0x00021214,
	TransmitPDO1_MappingParameter		= 0x1A00080B,
	TransmitPDO2_MappingParameter		= 0x1A01080B,
	TransmitPDO3_MappingParameter		= 0x1A02080B,
	TransmitPDO4_MappingParameter		= 0x1A03080B,
		TransmitPDO_MappingParameter_Sub	= 0x44444444,

	ErrorCode 							= 0x60300002,
	ControlWord 						= 0x60400002,
	StatusWord 							= 0x60410002,
	ShutdownOptionCode					= 0x605B0006,
	ModesOfOperation					= 0x60600005,
	ModesOfOperationDisplay				= 0x60610005,
	PositionDemandValue_PUU				= 0x60620008,
	PositionActualInternalValue			= 0x60630008,
	PositionActualValue					= 0x60640008,
	FollowingErrorWindow				= 0x60650004,
	PositionWindow						= 0x60670004,
	PositionWindowTime					= 0x60680002,
	VelocityDemandValue					= 0x606B0008,
	VelocityActualValue					= 0x606C0008,
	VelocityWindow						= 0x606D0002,
	VelocityWindowTime					= 0x606E0002,
	VelocityThreshold					= 0x606F0002,
	TargetTorque						= 0x60710006,
	MaxTorque							= 0x60720002,
	TorqueDemandValue					= 0x60740006,
	MotorRatedCurrent					= 0x60750004,
	MotorRatedTorque					= 0x60760004,
	TorqueActualValue					= 0x60770006,
	CurrentActualValue					= 0x60780006,
	TargetPosition						= 0x607A0008,
	HomeOffset							= 0x607C0008,
	SoftwarePositionlimit				= 0x607D0208,
		SoftwarePositionlimit_Sub			= 0x00000088,
	MaxProfileVelocity					= 0x607F0004,
	MaxMotorSpeed						= 0x60800004,
	ProfileVelocity						= 0x60810004,
	ProfileAcceleration					= 0x60830004,
	ProfileDeceleration					= 0x60840004,
	QuickStopDeceleration				= 0x60850004,
	TorqueSlope							= 0x60870004,
	PositionFactor						= 0x60930204,
		PositionFactor_Sub					= 0x00000044,
	HomingMethod						= 0x60980005,
	HomingSpeeds						= 0x60990204,
		HomingSpeeds_Sub					= 0x00000044,
	HomingAcceleration					= 0x609A0004,
	InterpolationSubModeSelect			= 0x60C00006,
	InterpolationDataRecord				= 0x60C10108,
		InterpolationDataRecord_Sub			= 0x00000008,
	InterpolationTimePeriod				= 0x60C20201,
		InterpolationTimePeriod_Sub			= 0x00000015,
	MaxAcceleration						= 0x60C50004,
	MaxDeceleration						= 0x60C60004,
	FollowingErrorActualValue			= 0x60F40008,
	PositionDemandValue					= 0x60F40008,
	DigitalInputs						= 0x60FD0008,
	DigitalOutput						= 0x60FE0204,
		DigitalOutput_Sub					= 0x00000044,
	TargetVelocity						= 0x60FF0008,
	SupportedDriveModes					= 0x65020004
};
typedef ObjectDictionaryTable ObjectDictionaryTable;

/* 建立DeltaIMS_Motor OperationMode Table表,並創建專有DataType */
enum class OperationModeTable : uint8_t
{
	ProfilePositionMode 	= 0x01,
	ProfileVelocityMode 	= 0x03,
	ProfileTorqueMode		= 0x04,
	HomingMoode				= 0x06,
	InterpolatedPositionMode= 0x07
};
typedef OperationModeTable OperationModeTable;

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
