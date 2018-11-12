/*
 * I2C.cpp
 *
 *  Created on: 5. 11. 2018
 *      Author: jakub
 */

#include <I2C.h>

I2C::I2C()
{
	// TODO Auto-generated constructor stub
	I2C_MasterGetDefaultConfig(&m_masterConfig);

	I2C_MasterInit(I2C0, &m_masterConfig, kCLOCK_BusClk);

}
// Mozno nebude treba
void I2C::init(uint8_t slaveAddress)
{
	m_MasterTransfer.slaveAddress = slaveAddress;
	m_MasterTransfer.direction = kI2C_Write;
	m_MasterTransfer.subaddress = (uint32_t) NULL;
	m_MasterTransfer.subaddressSize = 0;
	m_MasterTransfer.data = m_buffer;
	m_MasterTransfer.dataSize = SIZE_OF_BUFFER;
	m_MasterTransfer.flags = kI2C_TransferDefaultFlag;
}

uint8_t I2C::write(int addr, uint8_t* buff, uint8_t size)
{
	m_MasterTransfer.slaveAddress = addr;
	m_MasterTransfer.direction = kI2C_Write;
	m_MasterTransfer.subaddress =(uint32_t) NULL;
	m_MasterTransfer.subaddressSize = 0;
	m_MasterTransfer.data = buff;
	m_MasterTransfer.dataSize = size;
	m_MasterTransfer.flags = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer);
	return 1;
}
uint8_t I2C::read(int addr, uint8_t*  buff, uint8_t size)
{
	m_MasterTransfer.slaveAddress = addr;
	m_MasterTransfer.direction = kI2C_Read;
	m_MasterTransfer.subaddress =(uint32_t) NULL;
	m_MasterTransfer.subaddressSize = 0;
	m_MasterTransfer.data = buff;
	m_MasterTransfer.dataSize = size;
	m_MasterTransfer.flags = kI2C_TransferDefaultFlag;

	if(I2C_MasterTransferBlocking(I2C0, &m_MasterTransfer) != kStatus_Success)
	{
		/*Chyba pri prenose dat*/
		return 2;
	}
		return 1;
}

I2C::~I2C() {
	// TODO Auto-generated destructor stub
}

