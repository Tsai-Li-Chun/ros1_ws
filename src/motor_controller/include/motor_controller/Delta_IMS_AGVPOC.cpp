/** ******************************************************
	* @file		Delta_IMS_AGVPOC.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
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
	can_obj_init.ID=0;
    can_obj_init.SendType=1;
    can_obj_init.RemoteFlag=0;
    can_obj_init.ExternFlag=0;
    can_obj_init.DataLen=8;
	for(int i=0; i<can_obj_init.DataLen; i++) can_obj_init.Data[i]=0;
	can_obj_Tx = can_obj_init;
	can_obj_Rx = can_obj_init;
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


/** * @brief Delta IMS AGV-Motor Operation setting function
 	* @param None
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::set_OperationMode(uint8_t mode)
{
	/* 建立解析OD資訊用結構體 */
	DeltaMotorOD_Analyze_Struct OD_analyze;
	/* 取得對應的OD列表 */
	uint32_t OD_tmp = (uint32_t)ObjectDictionaryTable::ModesOfOperation;
	/* 解析OD資訊用結構體 */
	OD_analyze = DeltaMotorOD_Analyze(true,OD_tmp);
	/* OD資訊用結構體 to VCI_CAN_OBJ結構體 */
	can_obj_Tx = ODAnalyzeToVCICANOBJ((uint16_t)FunctionCodeTable::RxSDO, OD_analyze, &mode);
	/* send data */
	can_obj_Tx.ID+=1;	// R-Motor
	SDO_transmit(can_obj_Tx);
	can_obj_Tx.ID+=1;	// L-Motor
	SDO_transmit(can_obj_Tx);
	return 0;
}


/** * @brief CANalystii SendData SDO 函數
 	* @param RW(bool) R/W mode select, 0(read), 1(write)
 	* @return (int) Program Error.
**	**/
int Delta_IMS_Motor_Control::SDO_transmit(VCI_CAN_OBJ data)
{
	// ROS_INFO("ID    : 0x%02x",can_obj_Tx.ID);
	// ROS_INFO("Len   : 0x%02x",can_obj_Tx.DataLen);
	// ROS_INFO("CC    : 0x%02x",can_obj_Tx.Data[0]);
	// ROS_INFO("indexL: 0x%02x",can_obj_Tx.Data[1]);
	// ROS_INFO("indexH: 0x%02x",can_obj_Tx.Data[2]);
	// ROS_INFO("indexS: 0x%02x",can_obj_Tx.Data[3]);
	// ROS_INFO("data0 : 0x%02x",can_obj_Tx.Data[4]);
	// ROS_INFO("data1 : 0x%02x",can_obj_Tx.Data[5]);
	// ROS_INFO("data2 : 0x%02x",can_obj_Tx.Data[6]);
	// ROS_INFO("data3 : 0x%02x",can_obj_Tx.Data[7]);
	// ROS_INFO("---------------------------");
	if(can_node.send_can_frame(0,data,1))
	{
		ROS_INFO("CANalystii channel(0) send Success");
	}
	return 0;
}

/** * @brief 解析OD資訊用結構體function
 	* @param od(uint32_t) define in ObjectDictionaryTable OD data
 	* @return (DeltaMotorOD_Analyze_Struct) OD Analyze Struct
**	**/
DeltaMotorOD_Analyze_Struct Delta_IMS_Motor_Control::DeltaMotorOD_Analyze(bool RW, uint32_t od)
{
	DeltaMotorOD_Analyze_Struct ODA;
	/* get index */
	ODA.index_main = (uint16_t)(od>>16);
	/* get sub index amount */
	ODA.index_sub_amount = (uint8_t)((od&0x0000FF00)>>8);
	/* get sub index, get data type (data length) */
	if(ODA.index_sub_amount==0)
	{	/* have no sub index */
		ODA.index_sub = 0;
		ODA.dataLen = (uint8_t)(od&0x000000FF);
	}
	else
	{	/* have sub index */ /* undone */
		ODA.index_sub = 1;
		ODA.dataLen = (uint8_t)(od&0x000000FF);
	}

	return ODA;
}

/** * @brief OD資訊用結構體 to VCI_CAN_OBJ結構體 function
 	* @param od(DeltaMotorOD_Analyze_Struct) Delta Motor OD_Analyze Struct
 	* @param fc(uint16_t) CANopen Function Code
 	* @return (VCI_CAN_OBJ) CANalystii Struct
**	**/
VCI_CAN_OBJ Delta_IMS_Motor_Control::ODAnalyzeToVCICANOBJ(uint16_t fc, DeltaMotorOD_Analyze_Struct od, uint8_t *data)
{
	VCI_CAN_OBJ canalystii_obj = can_obj_init;
	uint16_t index_tmp;

	/* set CANopen COB-ID : function code */
	canalystii_obj.ID = fc;
	/* set CANopen 8byte data : byte0 command code */
	if(od.dataLen>4) od.dataLen=od.dataLen-4;
	// canalystii_obj.DataLen = od.dataLen;
	canalystii_obj.DataLen = 8;
	switch (od.dataLen)
	{
		case 1:  canalystii_obj.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_1byte ; break;
		case 2:  canalystii_obj.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_2byte ; break;
		case 4:  canalystii_obj.Data[0] = (uint8_t)SDO_CommandCodeTable::writeT_4byte ; break;
		default: ROS_ERROR("[ODAnalyzeToVCICANOBJ] data dength to command code Failure"); break;
	}
	/* set CANopen 8byte data : byte1-2 Index address */
	index_tmp = __builtin_bswap16(od.index_main);
	canalystii_obj.Data[1] = (uint8_t)((index_tmp&0xFF00)>>8);
	canalystii_obj.Data[2] = (uint8_t)(index_tmp&0x00FF);
	/* set CANopen 8byte data : byte3 Sub Index address */
	canalystii_obj.Data[3] = od.index_sub;
	/* set CANopen 8byte data : byte4-7 data */
	if( (od.dataLen<=4) && (od.dataLen>=0) )
		for(int i=0; i<od.dataLen; i++)	
			canalystii_obj.Data[i+4] = *data+i;

	return canalystii_obj;
}


/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF Delta_IMS_AGVPOC.cpp ***** */
