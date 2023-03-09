/** ******************************************************
	* @file		Delta_IMS_AGVPOC.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
#include <string.h>
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "Delta_IMS_AGVPOC.hpp"
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

#define system_setup
/* class system setup, Constructor and Destructor */
#ifdef system_setup /* class system setup */

/** * @brief Constructor - can_obj_TxRx init
 	* @param None
 	* @return None
**	**/
Delta_IMS_Motor_Control::Delta_IMS_Motor_Control()
{
	Waiting_Takeout_Data = 0;
	/* canalystii struct initialization */
	can_obj_init.ID=0;
    can_obj_init.SendType=1;
    can_obj_init.RemoteFlag=0;
    can_obj_init.ExternFlag=0;
    can_obj_init.DataLen=8;
	for(int i=0; i<can_obj_init.DataLen; i++) can_obj_init.Data[i]=0;
	can_obj_Tx = can_obj_init;
	can_obj_Rx = can_obj_init;
	/* Delta IMS Motor CANopen OD struct initialization */
	OD_struct_init.index_main = 0;
	OD_struct_init.index_sub_amount = 0;
	OD_struct_init.index_sub = 0;
	OD_struct_init.dataLen = 0;
	OD_struct_init.data = 0;
	OD_struct_init.read_write = 0;
	OD_struct_Rx = OD_struct_init;
	OD_struct_Tx = OD_struct_init;
}
/** * @brief Destructor
 	* @param None
 	* @return None
**	**/
Delta_IMS_Motor_Control::~Delta_IMS_Motor_Control()
{
	if( can_node.close_device() == 1 )
		ROS_INFO("CANalystii close Success");
	else if( can_node.close_device() == 0 )
		ROS_WARN("CANalystii close Failure");

}

#endif /* system setup */


#define CANalystii_SetUp_Operation
/* CANalystii setup operation */
/* involve drive start/close and channel open */
#ifdef CANalystii_SetUp_Operation

/** * @brief CANalystii 初始化函數
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::CANalystii_SetUp(void)
{
	/* CANalystii drive start and channel open */
	return CANalystii_open();
}

/** * @brief CANalystii drive start and channel open
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::CANalystii_open(void)
{
	/* CANalystii drive starts */
	if( can_node.start_device() == 1)
		ROS_INFO("CANalystii drive Starts Success");
	else if( can_node.start_device() == 0)
	{
		ROS_ERROR("CANalystii drive Starts Error");
		return -1;
	}

	/* CANalystii channel init&open */
    vci_conf.AccCode = 0x00000000;
    vci_conf.AccMask = 0xFFFFFFFF;
    vci_conf.Filter = 1;//receive all frames
    vci_conf.Timing0 = 0x00;
    vci_conf.Timing1 = 0x1C;//baudrate 500kbps
    vci_conf.Mode = 0;//normal mode
    if( can_node.init_can_interface(0,vci_conf) == 1 )
		ROS_INFO("CANalystii channel(0) init Success");
    else
	{
		ROS_ERROR("CANalystii channel(0) init Error");
		return -1;
	}
	if( can_node.init_can_interface(1,vci_conf) == 1 )
		ROS_INFO("CANalystii channel(1) init Success");
    else
	{
		ROS_ERROR("CANalystii channel(1) init Error");
		return -1;
	}

	return 1;
}

/** * @brief CANalystii 設備關閉函數
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::CANalystii_close(void)
{
	if( can_node.close_device() == 1 )
		ROS_INFO("CANalystii channel(1) init Success");
    else
	{
		ROS_ERROR("CANalystii channel(1) init Error");
		return -1;
	}
	return 1;
}
#endif /* CANalystii_SetUp_Operation */


#define Delta_IMS_Motor_Operation 
/* Delta IMS AGV Motor operation and write command relevant */
/* involve Operation Mode set, ServoON/OFF, write motion parameters, etc...  */
#ifdef Delta_IMS_Motor_Operation

/** * @brief Delta IMS AGV-Motor Operation setting function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::set_OperationMode(uint8_t mode)
{
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::ModesOfOperation;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, &mode);
	/* send data */
	SDO_transmit(true, R_MOTOR);
	SDO_transmit(true, L_MOTOR);
	return 0;
}

/** * @brief Delta IMS AGV-Motor clear receive Buffer
 	* @param None
 	* @return None
**	**/
void Delta_IMS_Motor_Control::clear_RxBuffer(void)
{
	ROS_INFO("[clear RxBuffer] start...");
	for(int i=0; i<2048; i++)
		can_node.receive_can_frame(0,can_obj_Rx,2048,100);
	ROS_INFO("[clear RxBuffer] Over");
}

/** * @brief Delta IMS AGV-Motor Servo ON function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::motorSON(void)
{
	uint8_t StateMachine_value[2] = {0x06,0x00};
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	StateMachine_value[0] = 0x07;
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	StateMachine_value[0] = 0x0F;
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	return 0;
}

/** * @brief Delta IMS AGV-Motor Servo OFF function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::motorSOFF(void)
{
	uint8_t StateMachine_value[2] = {0x07,0x00};
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	StateMachine_value[0] = 0x06;
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	StateMachine_value[0] = 0x00;
	writeStateMachineChange(StateMachine_value);
	usleep(100000);
	ROS_INFO("motorSOFF");
	return 0;
}

/** * @brief Delta IMS AGV-Motor StateMachine Change function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::writeStateMachineChange(uint8_t* value)
{
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::ControlWord;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, value);
	/* send data */
	SDO_transmit(true, R_MOTOR);
	SDO_transmit(true, L_MOTOR);
	return 0;
}

/** * @brief Delta IMS AGV-Motor set Acceleration(0-3000rpm) function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::writeAcceleration(uint32_t acc)
{
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::ProfileAcceleration;
	/* 32Format to 8Format transform union type */
	FormatTo_8_16_32 FormatTo_8;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	FormatTo_8.ud32 = acc;
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, FormatTo_8.ud8);
	/* send data */
	SDO_transmit(true, L_MOTOR);	
	SDO_transmit(true, R_MOTOR);	
	return 0;
}

/** * @brief Delta IMS AGV-Motor set Decelerate(0-3000rpm) function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::writeDecelerate(uint32_t dec)
{
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::ProfileDeceleration;
	/* 32Format to 8Format transform union type */
	FormatTo_8_16_32 FormatTo_8;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	FormatTo_8.ud32 = dec;
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, FormatTo_8.ud8);
	/* send data */
	SDO_transmit(true, L_MOTOR);	
	SDO_transmit(true, R_MOTOR);	
	return 0;
}

/** * @brief 寫入速度設定值
	* @param Lvelocity(int32_t) Set the operating Lmotor velocity
	* @param Rvelocity(int32_t) Set the operating Rmotor velocity
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::writeVelocity(int32_t Lvelocity,int32_t Rvelocity)
{
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::TargetVelocity;
	/* 32Format to 8Format transform union type */
	FormatTo_8_16_32 FormatTo_8;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	FormatTo_8.d32 = Lvelocity;
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, FormatTo_8.ud8);
	/* send data */
	SDO_transmit(true, L_MOTOR);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	FormatTo_8.d32 = Rvelocity;
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, FormatTo_8.ud8);
	/* send data */
	SDO_transmit(true, R_MOTOR);

	return 0;
}

#endif	/* Delta_IMS_Motor_Operation */


#define Delta_IMS_Motor_Information
/* Get Delta IMS AGV Motor Information,read command relevant */
/* involve read motion parameters, read AlarmCode, etc... */
#ifdef Delta_IMS_Motor_Information

/** * @brief 讀取速度設定值
	* @param speed(int32_t*) Demand speed
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::readDemandVelocity(int32_t *speed)
{
	return 0;
}

/** * @brief 讀取實際速度
	* @param speed(int32_t*) feedback speed
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::readActualVelocity(int32_t *speed)
{
		/* 建立解析OD資訊用結構體 */
	DeltaMotorOD_Struct OD_analyze;
	/* 取得對應的OD列表 VelocityActualValue */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::VelocityActualValue;
	/* 解析OD資訊用結構體 */
	set_DeltaMotorOD_Struct(false,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	DeltaMotorOD_To_CANalystiiOBJ((uint16_t)FunctionCodeTable::RxSDO, nullptr);
	/* send data */
	SDO_transmit(false, R_MOTOR);
	SDO_transmit(false, L_MOTOR);
	return 0;
}

/** * @brief 讀取目前運轉位置(累計)
	* @param position(int32_t*) Actual position
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::readActualPosition(int32_t *position)
{
	return 0;
}

/** * @brief 讀取馬達轉矩負載率
	* @param torque(int32_t*) Load Torque
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::readLoadTorque(int32_t *torque)
{
	return 0;
}

/** * @brief 讀取Driver耗能
	* @param consumption(int32_t*) Power Consumption
 	* @return (int)
**	**/
int Delta_IMS_Motor_Control::readPowerConsumption(int32_t *consumption)
{
	return 0;
}


#endif	/* Delta_IMS_Motor_Information */



/** * @brief CANalystii SendData SDO 函數
 	* @param rw(bool) R/W mode select, 0(read), 1(write)
 	* @param id(uint8_t) Motor CANopen id
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::SDO_transmit(bool rw, uint8_t id)
{
	uint16_t tmp = can_obj_Tx.ID;
	can_obj_Tx.ID+=id; 
	// ROS_INFO("TX - ID    : 0x%02x",can_obj_Tx.ID);
	// ROS_INFO("TX - Len   : 0x%02x",can_obj_Tx.DataLen);
	// ROS_INFO("TX - CC    : 0x%02x",can_obj_Tx.Data[0]);
	// ROS_INFO("TX - indexL: 0x%02x",can_obj_Tx.Data[1]);
	// ROS_INFO("TX - indexH: 0x%02x",can_obj_Tx.Data[2]);
	// ROS_INFO("TX - indexS: 0x%02x",can_obj_Tx.Data[3]);
	// ROS_INFO("TX - data0 : 0x%02x",can_obj_Tx.Data[4]);
	// ROS_INFO("TX - data1 : 0x%02x",can_obj_Tx.Data[5]);
	// ROS_INFO("TX - data2 : 0x%02x",can_obj_Tx.Data[6]);
	// ROS_INFO("TX - data3 : 0x%02x",can_obj_Tx.Data[7]);
	// ROS_INFO("---------------------------");
	if(can_node.send_can_frame(0,can_obj_Tx,1))
	{
		// usleep(1);
		// ROS_INFO("CANalystii channel(0) send Success");
		// if(rw==false) SDO_receive(rw, id);
	}
	else
	{
		ROS_ERROR("[SDO_transmit] Failure");
	}
	can_obj_Tx.ID = tmp;
	return 0;
}

/** * @brief CANalystii ReceiveData SDO 函數
 	* @param rw(bool) R/W mode select, 0(read), 1(write)
 	* @param id(uint8_t) Motor CANopen id
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::SDO_receive(bool rw, uint8_t id)
{
	if(can_node.receive_can_frame(0,can_obj_Rx,1))
	{	
		CANalystiiOBJ_To_DeltaMotorOD(rw, id);
	}

	return 0;
}

/** * @brief 解析OD資訊用結構體function
 	* @param RW(bool) read/write operation select
 	* @param od(uint32_t) Delta IMS Motor Object Dictionary buffer
 	* @return None
**	**/
void Delta_IMS_Motor_Control::set_DeltaMotorOD_Struct(bool RW, uint32_t od)
{
	OD_struct_Tx = OD_struct_init;
	/* get r/w operation */
	OD_struct_Tx.read_write = RW;
	/* get index */
	OD_struct_Tx.index_main = (uint16_t)(od>>16);
	/* get sub index amount */
	OD_struct_Tx.index_sub_amount = (uint8_t)((od&0x0000FF00)>>8);
	/* get sub index, get data type (data length) */
	if(OD_struct_Tx.index_sub_amount==0)
	{	/* have no sub index */
		OD_struct_Tx.index_sub = 0;
		OD_struct_Tx.dataLen = (uint8_t)(od&0x000000FF);
	}
	else
	{	/* have sub index */ /* undone */
		OD_struct_Tx.index_sub = 1;
		OD_struct_Tx.dataLen = (uint8_t)(od&0x000000FF);
	}
}

/** * @brief OD資訊用結構體 to VCI_CAN_OBJ結構體 function
 	* @param fc(uint16_t) CANopen Function Code
 	* @param od(DeltaMotorOD_Struct) Delta Motor OD_Analyze Struct
 	* @param data(uint8_t*) CANopen 4byte data
 	* @return (VCI_CAN_OBJ) CANalystii Struct
**	**/
void Delta_IMS_Motor_Control::DeltaMotorOD_To_CANalystiiOBJ(uint16_t fc, uint8_t *data)
{
	can_obj_Tx = can_obj_init;
	uint16_t index_tmp;

	/* set CANopen COB-ID : function code */
	can_obj_Tx.ID = fc;
	/* set CANopen 8byte data : byte0 command code */
	if(OD_struct_Tx.dataLen>4) OD_struct_Tx.dataLen=OD_struct_Tx.dataLen-4;
	// can_obj_Tx.DataLen = OD_struct_Tx.dataLen;
	can_obj_Tx.DataLen = 8;
	if(OD_struct_Tx.read_write == true)
		switch (OD_struct_Tx.dataLen)
		{	/* write operation */
			case 1:  can_obj_Tx.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_1byte; break;
			case 2:  can_obj_Tx.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_2byte; break;
			case 4:  can_obj_Tx.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_4byte; break;
			default: ROS_ERROR("[ODAnalyzeToVCICANOBJ] data dength to command code Failure"); break;
		}
	else	/* read operation */
		can_obj_Tx.Data[0] = (uint8_t)SDO_CommandCodeTable::readT ;
	/* set CANopen 8byte data : byte1-2 Index address */
	index_tmp = __builtin_bswap16(OD_struct_Tx.index_main);
	can_obj_Tx.Data[1] = (uint8_t)((index_tmp&0xFF00)>>8);
	can_obj_Tx.Data[2] = (uint8_t)(index_tmp&0x00FF);
	/* set CANopen 8byte data : byte3 Sub Index address */
	can_obj_Tx.Data[3] = OD_struct_Tx.index_sub;
	/* if write mode, set CANopen 8byte data : byte4-7 data */
	if(OD_struct_Tx.read_write == true)
		if( (OD_struct_Tx.dataLen<=4) && (OD_struct_Tx.dataLen>=0) )
			for(int i=0; i<OD_struct_Tx.dataLen; i++)	
				can_obj_Tx.Data[i+4] = *(data+i);
}

/** * @brief OD資訊用結構體 to VCI_CAN_OBJ結構體 function
 	* @param rw(bool) R/W mode select, 0(read), 1(write)
 	* @param id(uint8_t) Motor CANopen id
 	* @return (VCI_CAN_OBJ) CANalystii Struct
**	**/
void Delta_IMS_Motor_Control::CANalystiiOBJ_To_DeltaMotorOD(bool rw, uint8_t id)
{
	uint8_t read_cc_check = 0;	/* read CommandCode check */
	uint8_t command_code = 0;
	uint8_t source_id = (uint8_t)(can_obj_Rx.ID&0x007F);
	uint16_t OD_index_main = 0;
	uint32_t SDO_AbortCode = 0;
	OD_struct_Rx = OD_struct_init;
	if(can_obj_Rx.ExternFlag == 1)
		ROS_ERROR("The SourceDevice-ExternFlag is Extern mode");
	if(can_obj_Rx.RemoteFlag == 1)
		ROS_ERROR("The SourceDevice-RemoteFlag is Remote frame");
	command_code = can_obj_Rx.Data[0];
	OD_struct_Rx.index_sub = can_obj_Rx.Data[3];
	OD_struct_Rx.index_main = (((uint16_t)can_obj_Rx.Data[2])<<8);
	OD_struct_Rx.index_main |= (uint16_t)(can_obj_Rx.Data[1]);

	if(rw == false)
	{	/* if read operation */
		read_cc_check = 0;
		if( command_code == ((uint8_t)SDO_CommandCodeTable::readR_1byte) ) {read_cc_check++;OD_struct_Rx.dataLen=1;}
		if( command_code == ((uint8_t)SDO_CommandCodeTable::readR_2byte) ) {read_cc_check++;OD_struct_Rx.dataLen=2;}
		if( command_code == ((uint8_t)SDO_CommandCodeTable::readR_4byte) ) {read_cc_check++;OD_struct_Rx.dataLen=4;}
		if( command_code == ((uint8_t)SDO_CommandCodeTable::readR_Err) )
		{	/* Display SDO AbortCode Message */
			memcpy(&SDO_AbortCode, can_obj_Rx.Data+4, 4);
			ROS_ERROR("CANopen SDO [receive] Failure, SDO_AbortCode = 0x%08xh",SDO_AbortCode);
		}
		else if( read_cc_check != 1 )
			ROS_ERROR("CANopen SDO [receive] CommandCode(%d) does not meet the standard",command_code);
		else if( OD_struct_Rx.index_main == 0x606C )
		{
			memcpy(&OD_struct_Rx.data, can_obj_Rx.Data+4, 4);
			Actual_Speed[id-1] = OD_struct_Rx.data;
			// ROS_INFO("COBID=0x%04xh, sourceID=0x%02xh",can_obj_Rx.ID ,source_id);
			// ROS_INFO("OD   =0x%04xh",OD_struct_Rx.index_main);
			// ROS_INFO("ODsub=0x%04xh",OD_struct_Rx.index_sub);
			// ROS_INFO("Len  =%d",OD_struct_Rx.dataLen);
			// ROS_INFO("data =%d",OD_struct_Rx.data);
		}
	}
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF Delta_IMS_AGVPOC.cpp ***** */
