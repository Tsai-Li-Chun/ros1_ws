/** ******************************************************
	* @file		Modbus_Handshake.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "ros/ros.h"
#include "Modbus_Handshake.hpp"
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

/** * @brief Constructor - TCP Mode
 	* @param IP(char*) Modbus Device IP Address
 	* @param port(int) communication port, deatusult 502
 	* @param slave(int) Modbus Device ID, default 1
 	* @return None
**	**/
Modbus_Handshake::Modbus_Handshake(const char* IP, int port, int slave)
{
	/* 初始化modbus通訊設定 */
	mb = modbus_new_tcp(IP,port);
	if( Modbus_slave_connect(slave) != 0 )
		ROS_INFO("Modbus TCP Mode Setup Failure!");
	// handshake.value_int = (-1);
	// run_RL();
}

/** * @brief Setup SlaveID & Connect
 	* @param slave(int) Modbus Device ID, default 1
 	* @return int 
**	**/
int Modbus_Handshake::Modbus_slave_connect(int slave)
{
	/* 宣告libmodbus-API的ReturnCode變數 */
	int rc;
	/* 初始化是否成功判斷 */
	if( mb == NULL )
	{	/* 若初始化失敗,打印訊息,退出程序 */
		ROS_ERROR("%s",modbus_strerror(errno));
	}
	else
	{	/* 若初始化成功,打印訊息 */
		ROS_INFO("Initialize modbus_t Structure Success");
	}

	/* 設定欲通訊的modbus-slaveID */
	rc = modbus_set_slave(mb,slave);
	if( rc != 0 )
	{	/* 若設定失敗,打印訊息,退出建構函數 */
		ROS_ERROR("modbus set slave Failure!");
		return rc;
	}
	else
	{	/* 若slaveID設定成功,打印訊息 */
		ROS_INFO("Setup Slave ID \"%d\" Success", slave);
	}
	
	/* 建立連結 */
	rc = modbus_connect(mb);
	if( rc != 0 )
	{	/* 若設定失敗,打印訊息,退出建構函數 */
		ROS_ERROR("modbus connect Failure!");
		return rc;
	}
	else
	{	/* 若連結成功,打印訊息 */
		ROS_INFO("Slave ID \"%d\" Connect Success", slave);
		ROS_INFO("%s",modbus_strerror(errno));
	}

	/* 程式正常結束,返回0 */
	return 0;
}

/** * @brief Destructor
 	* @param None
 	* @return None
**	**/
Modbus_Handshake::~Modbus_Handshake()
{
	modbus_write_register(mb,addr_runRL,3);
	modbus_write_register(mb,addr_setRL,0);
	/* 關閉連結 */
	modbus_close(mb);
	/* 釋放modbus通訊結構體的位址 */
	modbus_free(mb);
	/* 打印關閉訊息 */
	ROS_INFO("modbus connect close");
}


/** * @brief 執行RL檔指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::run_RL(void)
{
	int rc=0;
	rc = modbus_write_register(mb,addr_setRL,1);
	if( rc )
		ROS_INFO("delta arm robot RL addr_setRL Success!");
	else
		ROS_ERROR("delta arm robot RL addr_setRL Failure!");
	rc = modbus_write_register(mb,addr_runRL,6);
	if( rc )
		ROS_INFO("delta arm robot RL addr_runRL Success!");
	else
		ROS_ERROR("delta arm robot RL addr_runRL Failure!");
}

/** * @brief 回home指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::back_home(void)
{
	int rc=0; ROS_INFO("back_home");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_BackHome);
	ROS_INFO("rc = %d",rc);
	handshake.value_int = 0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{
		display_status_msg(handshake.value_int);
		handshake.value_int = 0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, BackHome Success!");
}

/** * @brief 抓取物料指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::grab_material(void)
{
	int rc=0; ROS_INFO("grab_material");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_PickStation);
	ROS_INFO("rc = %d",rc);
	handshake.value_int=0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{	
		display_status_msg(handshake.value_int);
		handshake.value_int=0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, grab_material Success!");
}

/** * @brief 放下物料指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::release_material(void)
{
	int rc=0; ROS_INFO("release_material");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_PlaceStation);
	ROS_INFO("rc = %d",rc);
	handshake.value_int = 0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{
		display_status_msg(handshake.value_int);
		handshake.value_int = 0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, release_material Success!");
}

/** * @brief 手眼辨識指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::handeye(void)
{
	int rc=0; ROS_INFO("handeye");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_HandEye);
	ROS_INFO("rc = %d",rc);
	handshake.value_int = 0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{
		display_status_msg(handshake.value_int);
		handshake.value_int = 0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, handeye Success!");
}

/** * @brief 手臂內縮待命指令
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::arm_standby(void)
{
	int rc=0; ROS_INFO("arm_standby");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_Standby);
	ROS_INFO("rc = %d",rc);
	handshake.value_int = 0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{
		display_status_msg(handshake.value_int);
		handshake.value_int = 0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, arm_standby Success!");
}

void Modbus_Handshake::backzero(void)
{
	int rc=0; ROS_INFO("backzero");
	rc = modbus_write_register(mb,addr_cmd,cmd_code_backzero);
	ROS_INFO("rc = %d",rc);
	handshake.value_int = 0;
	modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
	while( handshake.value_int!=0 )
	{
		display_status_msg(handshake.value_int);
		handshake.value_int = 0;
		modbus_read_registers(mb,addr_ret,1,handshake.value_u16);
		usleep(100000);
	}
	ROS_INFO("Task, backzero Success!");
}

/** * @brief 顯示目前robot狀態
 	* @param None
 	* @return None
**	**/
void Modbus_Handshake::display_status_msg(int status)
{
	ROS_INFO("addr_ret = %d",status);
	switch (status)
	{
		case 0: ROS_INFO("Task complete"); break;
		case 1: ROS_INFO("Task, PickStation runing"); break;
		case 2: ROS_INFO("Task, HandEye runing"); break;
		case 3: ROS_INFO("Task, PlaceStation runing"); break;
		case 4: ROS_INFO("Task, BackHome runing"); break;
		case 5: ROS_INFO("Task, Standby runing"); break;
		case 100: ROS_INFO("Task, backzero runing"); break;
	}
}

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF Modbus_Handshake.cpp ***** */