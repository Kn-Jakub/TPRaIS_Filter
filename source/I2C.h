/*
 * I2C.h
 *
 *  Created on: 5. 11. 2018
 *      Author: jakub
 */

#ifndef I2C_H_
#define I2C_H_

#include "fsl_i2c.h"

#define SIZE_OF_BUFFER   25

class I2C {
public:
	I2C();
	~I2C();
	void init(uint8_t slaveAddress);
	uint8_t write(int addr, uint8_t*  buff, uint8_t size);
	uint8_t read(int addr, uint8_t*  buff, uint8_t size);

private:
	i2c_master_transfer_t m_MasterTransfer;
	i2c_master_config_t m_masterConfig;
	uint8_t m_buffer[SIZE_OF_BUFFER];
};

#endif /* I2C_H_ */
