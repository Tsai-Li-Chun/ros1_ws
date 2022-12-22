/** ******************************************************
	* @file		Oriental_BLVD_KRD.cpp
	* @author	Tsai,Li-chun
	******************************************************
**	**/


/* System Includes ------------------------------------------*/
/* System Includes Begin */
/* System Includes End */
/* User Includes --------------------------------------------*/
/* User Includes Begin */
#include "Oriental_BLVD_KRD.hpp"
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

#if 1 /* system setup */
/** * @brief Constructor - modbusRTU mode setup
 	* @param device(char*) Modbus Device File path
 	* @param BR(int) Baud Rate
 	* @param parity(int) even/odd parity, default 'N'
 	* @param data_bit(int) DataBit Length, default 8(bit)
 	* @param stop_bit(int) StopBit Length, default 1(bit)
 	* @param slave(int) Modbus Device ID, default 1
 	* @return None
**	**/
BLVD_KRD_Control::BLVD_KRD_Control(const char* device,int slave,
									 int BR, char parity,
									 int data_bit, int stop_bit )
{
	/* 初始化modbus通訊設定 */
	mb = modbus_new_rtu(device,BR,parity,data_bit,stop_bit);
	if( Modbus_slave_connect(slave) != 0 )
		printf("BLVD-KRD Modbus RTU Mode Setup Failure!\n");
}

/** * @brief Setup SlaveID & Connect
 	* @param slave(int) Modbus Device ID, default 1
 	* @return int 
**	**/
int BLVD_KRD_Control::Modbus_slave_connect(int slave)
{
	/* 宣告libmodbus-API的ReturnCode變數 */
	int rc;
	/* 初始化是否成功判斷 */
	if( mb == NULL )
	{	/* 若初始化失敗,打印訊息,退出程序 */
		printf("%s\n",modbus_strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	{	/* 若初始化成功,打印訊息 */
		printf("Initialize BLVD-KRD modbus_t Structure Success\n");
	}

	/* 設定欲通訊的modbus-slaveID */
	rc = modbus_set_slave(mb,slave);
	if( rc != 0 )
	{	/* 若設定失敗,打印訊息,退出建構函數 */
		printf("BLVD-KRD modbus set slave Failure!\n");
		return rc;
	}
	else
	{	/* 若slaveID設定成功,打印訊息 */
		printf("BLVD-KRD Setup Slave ID \"%d\" Success\n", slave);
	}

	/* 建立連結 */
	rc = modbus_connect(mb);
	if( rc != 0 )
	{	/* 若設定失敗,打印訊息,退出建構函數 */
		printf("BLVD-KRD modbus connect Failure!\n");
		return rc;
	}
	else
	{	/* 若連結成功,打印訊息 */
		printf("BLVD-KRD Slave ID \"%d\" Connect Success\n", slave);
		// printf("%s\n",modbus_strerror(errno));
	}

	/* 程式正常結束,返回0 */
	return 0;
}

/** * @brief Destructor
 	* @param None
 	* @return None
**	**/
BLVD_KRD_Control::~BLVD_KRD_Control()
{
	/* 關閉連結 */
	modbus_close(mb);
	/* 釋放modbus通訊結構體的位址 */
	modbus_free(mb);
	/* 打印關閉訊息 */
	printf("BLVD-KRD modbus connect close\n");
}
#endif /* system setup */

#if 1 /* setup configuration and simple test */
/** * @brief Direct data operation setup,
             事先將工作模式,加減速,觸發方式(-4)設定好,後續更新速度即可即時變化
	* @param OpType(uint8_t) Operation type
	* @param Acc(uint16_t) Acceleration rate
	* @param Dec(uint16_t) Deceleration rate
	* @param Trigger(uint8_t) Trigger
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_DirectDataOperation_setup(uint8_t OpType, uint16_t Acc, uint16_t Dec, uint8_t Trigger)
{

}

/** * @brief Motor Stop
	* @param None
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorStop(void)
{

}

/** * @brief Put the motor into an excitation state
	* @param None
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorSON(void)
{
	int rc;
	// BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_DriverInputCommand;
	// uint16Buffer[0]=0x00;
	// uint16Buffer[1]=0x00;
	// uint16Buffer[2]=0x00;
	// uint16Buffer[3]=0x01;
	// uint16Buffer[4]=0x00;
	// uint16Buffer[5]=0x00;
	// uint16Buffer[6]=0x00;
	// uint16Buffer[7]=0x01;
	// rc = writeRegisters(RAT,4,uint16Buffer);
	rc = writeQuery();
	return rc;
}

/** * @brief Put the motor into an non-excitation state
	* @param None
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorSOFF(void)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_DriverInputCommand;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x00;
	uint16Buffer[3]=0x00;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x00;
	uint16Buffer[7]=0x00;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief motor forward test
	* @param v(int) motor forward velocity
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorForward(int v)
{

}

/** * @brief motor reverse test
	* @param v(int) motor reverse velocity
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorReverse(int v)
{

}

/** * @brief motor L-turn test
	* @param w(int) motor L-turn velocity
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorLturn(int w)
{

}

/** * @brief motor R-turn test
	* @param w(int) motor R-turn velocity
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::motorRturn(int w)
{

}
#endif /* setup configuration and simple test END */

#if 1 /* read simple parameter */
/** * @brief 讀取速度設定值
	* @param speed(int32_t*) Demand speed
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readDemandVelocity(int32_t *speed)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_DemandVelocity;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	speed[1] = 0;
	speed[1] = (speed[1]|uint16Buffer[0])<<16;
	speed[1] |= uint16Buffer[1];
	// motor R
	speed[0] = 0;
	speed[0] = (speed[0]|uint16Buffer[3])<<16;
	speed[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取實際速度
	* @param speed(int32_t*) feedback speed
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readActualVelocity(int32_t *speed)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_ActualVelocity;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	speed[1] = 0;
	speed[1] = (speed[1]|uint16Buffer[0])<<16;
	speed[1] |= uint16Buffer[1];
	// motor R
	speed[0] = 0;
	speed[0] = (speed[0]|uint16Buffer[3])<<16;
	speed[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取目前運轉位置(累計)
	* @param position(int32_t*) Actual position
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readActualPosition(int32_t *position)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_ActualPosition;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	position[1] = 0;
	position[1] = (position[1]|uint16Buffer[0])<<16;
	position[1] |= uint16Buffer[1];
	// motor R
	position[0] = 0;
	position[0] = (position[0]|uint16Buffer[3])<<16;
	position[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取馬達轉矩負載率
	* @param torque(int32_t*) Load Torque
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readLoadTorque(int32_t *torque)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_PresentTorque;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	torque[1] = 0;
	torque[1] = (torque[1]|uint16Buffer[0])<<16;
	torque[1] |= uint16Buffer[1];
	// motor R
	torque[0] = 0;
	torque[0] = (torque[0]|uint16Buffer[3])<<16;
	torque[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取Driver溫度
	* @param temperature(int32_t*) Driver temperature
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readDriverTemperature(int32_t *temperature)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_DriverTemperature;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	temperature[1] = 0;
	temperature[1] = (temperature[1]|uint16Buffer[0])<<16;
	temperature[1] |= uint16Buffer[1];
	// motor R
	temperature[0] = 0;
	temperature[0] = (temperature[0]|uint16Buffer[3])<<16;
	temperature[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取Motor溫度
	* @param temperature(int32_t*) Motor temperature
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readMotorTemperature(int32_t *temperature)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_MotorTemperature;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	temperature[1] = 0;
	temperature[1] = (temperature[1]|uint16Buffer[0])<<16;
	temperature[1] |= uint16Buffer[1];
	// motor R
	temperature[0] = 0;
	temperature[0] = (temperature[0]|uint16Buffer[3])<<16;
	temperature[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取Driver耗能
	* @param consumption(int32_t*) Power Consumption
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readPowerConsumption(int32_t *consumption)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_PowerConsumption;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	consumption[1] = 0;
	consumption[1] = (consumption[1]|uint16Buffer[0])<<16;
	consumption[1] |= uint16Buffer[1];
	// motor R
	consumption[0] = 0;
	consumption[0] = (consumption[0]|uint16Buffer[3])<<16;
	consumption[0] |= uint16Buffer[4];
	return rc;
}

/** * @brief 讀取Driver輸出狀態
	* @param status(int32_t*) Driver Output Status
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readDriverOutputStatus(int32_t *status)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_r_DriverOutputStatus;
	rc = readRegisters(RAT,6,uint16Buffer);
	// motor L
	status[1] = 0;
	status[1] = (status[1]|uint16Buffer[0])<<16;
	status[1] |= uint16Buffer[1];
	// motor R
	status[0] = 0;
	status[0] = (status[0]|uint16Buffer[3])<<16;
	status[0] |= uint16Buffer[4];
	return rc;
}
#endif /* read simple parameter */

#if 1 /* write(set) simple parameter */
/** * @brief 寫入工作模式
	* @param mode(int32_t) operation type
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeSetupType(int32_t mode)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_SetupType;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x00;
	uint16Buffer[3]=0x30;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x00;
	uint16Buffer[7]=0x30;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入速度設定值
	* @param velocity(int32_t) Set the operating velocity
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeVelocity(int32_t velocity)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_Velocity;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x00;
	uint16Buffer[3]=0x00;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x00;
	uint16Buffer[7]=0x00;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入加速度設定值
	* @param acc(int32_t) Set the operating acceleration
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeAcceleration(int32_t acc)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_Acc;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x03;
	uint16Buffer[3]=0xe8;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x03;
	uint16Buffer[7]=0xe8;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入減速度設定值
	* @param dec(int32_t) Set the operating decelerate
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeDecelerate(int32_t dec)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_Dec;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x03;
	uint16Buffer[3]=0xe8;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x03;
	uint16Buffer[7]=0xe8;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入力矩
	* @param torque(int32_t) Set the operating torque
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeTorque(int32_t torque)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_Torque;
	uint16Buffer[0]=0x00;
	uint16Buffer[1]=0x00;
	uint16Buffer[2]=0x03;
	uint16Buffer[3]=0xe8;
	uint16Buffer[4]=0x00;
	uint16Buffer[5]=0x00;
	uint16Buffer[6]=0x03;
	uint16Buffer[7]=0xe8;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入觸發方式
	* @param trigger(int32_t) Set the operating trigger
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeTrigger(int32_t trigger)
{
	int rc;
	BLVD_KRD_RegAdr_Table RAT=BLVD_KRD_RegAdr_Table::ADRS_w_Trigger;
	uint16Buffer[0]=0xff;
	uint16Buffer[1]=0xff;
	uint16Buffer[2]=0xff;
	uint16Buffer[3]=0xfc;
	uint16Buffer[4]=0xff;
	uint16Buffer[5]=0xff;
	uint16Buffer[6]=0xff;
	uint16Buffer[7]=0xfc;
	rc = writeRegisters(RAT,4,uint16Buffer);
	return rc;
}

/** * @brief 寫入Driver輸入狀態
	* @param status(int32_t) Set the operating status
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeDriverInputCommand(int32_t status)
{

}

/** * @brief diagnose the communication between a master and a slave.
	* @param None
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeDiagnosis(void)
{

}

#endif /* write(set) simple parameter END */

#if 1 /* Alarm relevant */
/** * @brief Read Present Alarm.
	* @param alarm(int32_t*)
 	* @return uint8_t
**	**/
uint8_t BLVD_KRD_Control::readAlarm(int32_t *alarm)
{

}

/** * @brief Read Communication Error.
	* @param err(int32_t*)
 	* @return uint8_t
**	**/
uint8_t BLVD_KRD_Control::readCommunicationError(int32_t *err)
{

}

/** * @brief RESET Alarm.
	* @param None
 	* @return none
**	**/
uint8_t BLVD_KRD_Control::ResetAlarm(void)
{

}

/** * @brief AlarmCode to StringMessage.
	* @param alarm(uint8_t) AlarmCode
 	* @return (uint8_t)
**	**/
std::string BLVD_KRD_Control::getStrOfAlarm(uint8_t alarm)
{
  switch ((AlarmCode)alarm)
  {
    case AlarmCode::CommunicationSwitchSettingError:
      return "Communication switch setting error";
    case AlarmCode::EEPROMError:
      return "EEPROM error";
    case AlarmCode::ExternalStop:
      return "External stop";
    case AlarmCode::InitialSensorError:
      return "Internal sensor error";
    case AlarmCode::MainCicruitOutputError:
      return "Main circuit output error";
    case AlarmCode::MainCicruitOverheat:
      return "Main circuit overheat";
    case AlarmCode::NetworkBusError:
      return "Network bus error";
    case AlarmCode::NetworkConverterError:
      return "NetworkConvertError";
    case AlarmCode::None:
      return "None";
    case AlarmCode::Overcurrent:
      return "Overcurrent";
    case AlarmCode::Overload:
      return "Overload";
    case AlarmCode::Overspeed:
      return "Overspeed";
    case AlarmCode::Overvoltage:
      return "Overvoltage";
    case AlarmCode::PreventionOfOperationAtPowerOn:
      return "Prevention of operation at power on";
    case AlarmCode::RS485CommunicationError:
      return "RS485 communicaton error";
    case AlarmCode::RS485CommunicationTimeout:
      return "RS485 communication timeout";
    case AlarmCode::SensorError:
      return "Sensor error";
    case AlarmCode::Undervoltage:
      return "Undervoltage";
  }
  return "";
}
#endif /* Alarm relevant END */

#if 1 /* Read data relevant */
/** * @brief Read Int32t data(single register data)
	* @param Adr(uint16_t) Register start Address
	* @param v(int32_t*) Response Data Address
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::readInt32t(uint16_t Adr, int32_t *v)
{

}

/** * @brief Read data for multiple successive registers
	* @param Adr(uint16_t) Register start Address
	* @param L(uint16_t) Number of registers
	* @param Data(int32_t*) Value read from register address
 	* @return (int)
**	**/
int BLVD_KRD_Control::readRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data)
{
	int rc;
	rc = modbus_read_registers(mb, (int)Adr, L, uint16Buffer);
	return rc;
}
#endif /* Read data relevant END */

#if 1 /* Write data relevant */
/** * @brief Write Int32t data(single register data)
	* @param Adr(uint16_t) Register start Address
	* @param Data(int32_t*) Value written to the register address
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeInt32t(uint16_t Adr, int32_t *Data)
{

}

/** * @brief Write data to multiple successive registers
	* @param Adr(uint16_t) Register start Address
	* @param L(uint16_t) Number of registers
	* @param Data(int32_t*) Value written to the registers address
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeRegisters(BLVD_KRD_RegAdr_Table Adr, uint8_t L, uint16_t *Data)
{
	int rc;
	rc = modbus_write_registers(mb, (int)Adr, L, uint16Buffer);
	return rc;
}

/** * @brief Send modbus raw request
	* @param FC(uint8_t) modbus FunctionCode
	* @param L(uint16_t) Number of registers
	* @param Data(int32_t*) Value written to the registers address
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeQuery(uint8_t FC, uint16_t L, uint8_t *Data)
{
	int rc;
	uint8Buffer[0] = 0x0f;
	uint8Buffer[1] = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
	uint8Buffer[2] = 0x00;
	uint8Buffer[3] = 0x0c;
	uint8Buffer[4] = 0x00;
	uint8Buffer[5] = 0x04;
	uint8Buffer[6] = 0x08;
	uint8Buffer[7] = 0x00;
	uint8Buffer[8] = 0x00;
	uint8Buffer[9] = 0x00;
	uint8Buffer[10]= 0x01;
	uint8Buffer[11]= 0x00;
	uint8Buffer[12]= 0x00;
	uint8Buffer[13]= 0x00;
	uint8Buffer[14]= 0x01;
	rc = modbus_send_raw_request(mb, uint8Buffer, 15*sizeof(uint8_t));
	return rc;
}
/** * @brief Send modbus raw request
	* @param None
 	* @return (uint8_t)
**	**/
uint8_t BLVD_KRD_Control::writeQuery(void)
{
	int rc;
	uint8Buffer[0] = 0x0f;
	uint8Buffer[1] = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
	uint8Buffer[2] = 0x00;
	uint8Buffer[3] = 0x0c;
	uint8Buffer[4] = 0x00;
	uint8Buffer[5] = 0x04;
	uint8Buffer[6] = 0x08;
	uint8Buffer[7] = 0x00;
	uint8Buffer[8] = 0x00;
	uint8Buffer[9] = 0x00;
	uint8Buffer[10]= 0x01;
	uint8Buffer[11]= 0x00;
	uint8Buffer[12]= 0x00;
	uint8Buffer[13]= 0x00;
	uint8Buffer[14]= 0x01;
	rc = modbus_send_raw_request(mb, uint8Buffer, 15*sizeof(uint8_t));
	return rc;
}
#endif /* Write data relevant END */

/* Program End */
/* ---------------------------------------------------------*/
/* ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ Program ⇧⇧⇧⇧⇧⇧⇧⇧⇧⇧ ---------------------------*/
/* ---------------------------------------------------------*/


/* ***** END OF Oriental_BLVD_KRD.cpp ***** */
