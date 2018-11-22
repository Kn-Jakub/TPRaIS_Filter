/*
 * 		I2C.cpp
 *      Author: Jakub Pekar
 */

#include <I2C.h>

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define SIZE_OF_BUFFER   25
#define I2C_RELEASE_BUS_COUNT 100U
#define I2C_RELEASE_SDA_PORT PORTE
#define I2C_RELEASE_SCL_PORT PORTE
#define I2C_RELEASE_SDA_GPIO GPIOE
#define I2C_RELEASE_SDA_PIN 25U
#define I2C_RELEASE_SCL_GPIO GPIOE
#define I2C_RELEASE_SCL_PIN 24U

#define I2C_BAUDRATE 100000U

I2C::I2C(uint32_t clockRate):
m_MasterTransfer(),
m_masterConfig()
//m_handle()
{
	I2C_MasterGetDefaultConfig(&m_masterConfig);
	m_masterConfig.baudRate_Bps = I2C_BAUDRATE;

	I2C_MasterInit(I2C0, &m_masterConfig, clockRate);

	//I2C_MasterTransferCreateHandle(I2C0, &m_handle, master_callback, NULL);

	//this->ConfigurePins();



}

uint8_t I2C::write(uint8_t device_addr, uint8_t reg_addr, uint8_t*  value, uint8_t size, uint32_t flags )
{
//	i2c_master_transfer_t masterXfer;
//	memset(&masterXfer, 0, sizeof(masterXfer));

	m_MasterTransfer.slaveAddress = device_addr;
	m_MasterTransfer.direction = kI2C_Write;
	m_MasterTransfer.subaddress = reg_addr;
	if(reg_addr != 255)
	{
		m_MasterTransfer.subaddressSize = 1;
	} else {
		m_MasterTransfer.subaddressSize = 0;
	}
	m_MasterTransfer.data = value;
	m_MasterTransfer.dataSize = size;
	m_MasterTransfer.flags = flags;

	I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer);

	/*  direction=write : start+device_write;cmdbuff;xBuff; */
	/*  direction=recive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */

	//I2C_MasterTransferNonBlocking(I2C0, &m_handle, &m_MasterTransfer);


	/*  wait for transfer completed. */
//	while ((!nakFlag) && (!completionFlag))
//	{
//	}
//
//	nakFlag = false;
//
//	if (completionFlag == true)
//	{
//		completionFlag = false;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}

}

uint8_t I2C::read(uint8_t device_addr, uint8_t reg_addr, uint8_t*  buff, uint8_t size, uint32_t flags)
{
	m_MasterTransfer.slaveAddress = device_addr;
	m_MasterTransfer.direction = kI2C_Read;

	if(reg_addr != 255)
	{
		m_MasterTransfer.subaddress = reg_addr;
		m_MasterTransfer.subaddressSize = 1;
	} else {
		m_MasterTransfer.subaddress = 0;
		m_MasterTransfer.subaddressSize = 0;
	}
	m_MasterTransfer.data = buff;
	m_MasterTransfer.dataSize = size;
	m_MasterTransfer.flags = flags;

	I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer);

	//I2C_MasterTransferNonBlocking(I2C0, &m_handle, &m_MasterTransfer);
	/*  wait for transfer completed. */
//	while ((!nakFlag) && (!completionFlag))
//	{
//	}
//
//		nakFlag = false;
//
//	if (completionFlag == true)
//	{
//		completionFlag = false;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
}

//void I2C::release_bus_delay(void)
//{
//	uint32_t i = 0;
//	for (i = 0; i < I2C_RELEASE_BUS_COUNT; i++)
//	{
//		__NOP();
//	}
//}

//void I2C::ReleaseBus(void)
//{
//
//    uint8_t i = 0;
//    gpio_pin_config_t pin_config;
//    port_pin_config_t i2c_pin_config = {0};
//
//    /* Config pin mux as gpio */
//    i2c_pin_config.pullSelect = kPORT_PullUp;
//    i2c_pin_config.mux = kPORT_MuxAsGpio;
//
//    pin_config.pinDirection = kGPIO_DigitalOutput;
//    pin_config.outputLogic = 1U;
//    CLOCK_EnableClock(kCLOCK_PortE);
//    PORT_SetPinConfig(I2C_RELEASE_SCL_PORT, I2C_RELEASE_SCL_PIN, &i2c_pin_config);
//    PORT_SetPinConfig(I2C_RELEASE_SDA_PORT, I2C_RELEASE_SDA_PIN, &i2c_pin_config);
//
//    GPIO_PinInit(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, &pin_config);
//    GPIO_PinInit(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, &pin_config);
//
//    /* Drive SDA low first to simulate a start */
//    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
//    this->release_bus_delay();
//
//    /* Send 9 pulses on SCL and keep SDA low */
//    for (i = 0; i < 9; i++)
//    {
//        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
//        this->release_bus_delay();
//
//        GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
//        this->release_bus_delay();
//
//        GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
//        this->release_bus_delay();
//        this->release_bus_delay();
//    }
//
//    /* Send stop */
//    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 0U);
//    this->release_bus_delay();
//
//    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 0U);
//    this->release_bus_delay();
//
//    GPIO_WritePinOutput(I2C_RELEASE_SCL_GPIO, I2C_RELEASE_SCL_PIN, 1U);
//    this->release_bus_delay();
//
//    GPIO_WritePinOutput(I2C_RELEASE_SDA_GPIO, I2C_RELEASE_SDA_PIN, 1U);
//    this->release_bus_delay();
//
//}

//void I2C::ConfigurePins(void)
//{
//	CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */
//
//	  const port_pin_config_t porte24_pin24_config = {
//	    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
//	    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
//	    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
//	    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
//	    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SCL */
//	  };
//	  PORT_SetPinConfig(PORTE, BOARD_INITPINS_ACCEL_SCL_PIN, &porte24_pin24_config); /* PORTE24 (pin 24) is configured as I2C0_SCL */
//	  const port_pin_config_t porte25_pin25_config = {
//	    kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
//	    kPORT_SlowSlewRate,                                      /* Slow slew rate is configured */
//	    kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
//	    kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
//	    kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SDA */
//	  };
//	  PORT_SetPinConfig(PORTE, BOARD_INITPINS_ACCEL_SDA_PIN, &porte25_pin25_config); /* PORTE25 (pin 25) is configured as I2C0_SDA */
//}

//static void master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
//{
//	/* Signal transfer success when received success status. */
//	if (status == kStatus_Success)
//	{
//		completionFlag = true;
//	}
//	/* Signal transfer success when received success status. */
//	if ((status == kStatus_I2C_Nak) || (status == kStatus_I2C_Addr_Nak))
//	{
//		nakFlag = true;
//	}
//}



I2C::~I2C() {
	// TODO Auto-generated destructor stub
}


//uint8_t I2C::write(int addr, uint8_t* buff, uint8_t size)
//{
//	m_MasterTransfer.slaveAddress = addr;
//	m_MasterTransfer.direction = kI2C_Write;
//	m_MasterTransfer.subaddress =(uint32_t) NULL;
//	m_MasterTransfer.subaddressSize = 0;
//	m_MasterTransfer.data = buff;
//	m_MasterTransfer.dataSize = size;
//	m_MasterTransfer.flags = kI2C_TransferRepeatedStartFlag;
//
//	status_t returned = I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer);
//	return 1;
//}
//uint8_t I2C::read(int addr, uint8_t*  buff, uint8_t size)
//{
//	m_MasterTransfer.slaveAddress = addr;
//	m_MasterTransfer.direction = kI2C_Read;
//	m_MasterTransfer.subaddress =(uint32_t) NULL;
//	m_MasterTransfer.subaddressSize = 0;
//	m_MasterTransfer.data = buff;
//	m_MasterTransfer.dataSize = size;
//	m_MasterTransfer.flags = kI2C_TransferDefaultFlag;
//
//	status_t returned = I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer);
//
//	if(returned == kStatus_Success)
//	{
//		/*Chyba pri prenose dat*/
//		return 2;
//	} else if(returned == kStatus_Fail){
//		return 3;
//	} else if(returned == kStatus_ReadOnly){
//		return 3;
//	} else if(returned == kStatus_OutOfRange){
//		return 3;
//	} else if(returned == kStatus_InvalidArgument){
//		return 3;
//	} else if(returned == kStatus_Timeout){
//		return 3;
//	} else if(returned == kStatus_NoTransferInProgress){
//		return 3;
//	}
//}



