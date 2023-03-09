/** ******************************************************
	* @file		Modbus_Handshake.h
	* @author	Tsai,Li-chun
	******************************************************
**	**/

/* Define to prevent recursive inclusi ----------------------*/
#ifndef __Modbus_Handshake_HPP__
#define __Modbus_Handshake_HPP__


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "modbus/modbus.h"
/* User Includes End */


/* Define ---------------------------------------------------*/
/* Define Begin */

#define addr_runRL	(uint16_t)0x0000
#define addr_cmd 	(uint16_t)0x1000
#define addr_ret	(uint16_t)0x1100
#define addr_status (uint16_t)0x1102

#define cmd_code_PickStation	(int)1
#define cmd_code_HandEye		(int)2
#define cmd_code_PlaceStation	(int)3
#define cmd_code_BackHome		(int)4
#define cmd_code_Standby		(int)5

/* Define End */


/* Extern Typedef -------------------------------------------*/
/* Extern Typedef Begin */

union modbus_32Format
{
    int value_int;
    uint16_t value_u16[2];
};

/* Extern Typedef End */


/* Extern Class -------------------------------------------*/
/* Extern Class Begin */

/* 宣告modbus通訊物件 */
class Modbus_Handshake
{
/* 私有成員 */
private:
	/* 建立modbus通訊結構體 */
	modbus_t* mb;
	/* 宣告*** */
	modbus_32Format handshake;

	/* 設定連結函數 */
	int Modbus_slave_connect(int slave);

/* 公有成員 */
public:
	/* 建構函數 - TCP模式 */
	Modbus_Handshake(const char* IP, int port=502, int slave=2);
	/* 建構函數 - RTU模式 */
	Modbus_Handshake(const char* device, int BR, char parity='N', int data_bit=8, int stop_bit=1, int slave=1);
	/* 解建構函數 */
	~Modbus_Handshake();

	/* 執行RL檔指令 */
	void run_RL(void);
	/* 回home指令 */
	void back_home(void);
	/* 抓取物料指令 */
	void grab_material(void);
	/* 放下物料指令 */
	void release_material(void);
	/* 手眼辨識指令 */
	void handeye(void);
	/* 手臂內縮待命指令 */
	void arm_standby(void);

	/* 顯示目前robot狀態 */
	void display_status_msg(int);

};

/* Extern Class End */


/* Extern Variables -----------------------------------------*/
/* Extern Variables Begin */
/* Extern Variables End */


/* Function -------------------------------------------------*/
/* Function Begin */
/* Function End */


#endif /*__ Modbus_Handshake_HPP__ */

/* ***** END OF Modbus_Handshake.HPP ***** */