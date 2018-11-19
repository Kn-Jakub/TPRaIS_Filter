/*
 * I2C.h
 *
 *  Created on: 5. 11. 2018
 *      Author: jakub
 */

#ifndef I2C_H_
#define I2C_H_

#include "fsl_i2c.h"

static volatile  bool completionFlag;
static volatile bool nakFlag;
static void master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData);


class I2C {
public:
	I2C(uint32_t clockRate);
	~I2C();
	uint8_t write(uint8_t device_adrr, uint8_t reg_addr, uint8_t value, uint8_t size, uint32_t flags );
	uint8_t read(uint8_t device_adrr, uint8_t reg_addr, uint8_t*  buff, uint8_t size, uint32_t flags );
private:
	void ReleaseBus(void);
	void ConfigurePins(void);
	void release_bus_delay(void); //static
private:
	i2c_master_transfer_t m_MasterTransfer;
	i2c_master_config_t m_masterConfig;
	i2c_master_handle_t m_handle;
};



#endif /* I2C_H_ */
