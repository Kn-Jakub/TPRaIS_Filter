/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "fsl_debug_console.h"

#include "../include/MMA8451Q.h"


#define REG_WHO_AM_I      0x0DU	// [R]   : 00011010 Device ID (0x1A)
#define REG_CTRL_REG_1    0x2AU // [R/W] : 0	ODR = 800 Hz, standby mode
#define REG_OUT_X_MSB     0x01	// [R]
#define REG_OUT_Y_MSB     0x03	// [R]
#define REG_OUT_Z_MSB     0x05	// [R]
#define REG_XYZ			  0x0EU // [R/W] : 00000000 Data config
#define REG_ACCEL_STATUS  0x00U	// [R] : 00000000 	Status register

//REGISTERS FOR FREE FALL
#define REG_INT_SOURCE 	0x0C 	// 	[R]   : 00000000 Interrupt status
#define REG_FF_CFG		0x15	//  [R/W] : 0		FF functional block configuration
#define REG_FF_SRC		0x16	//  [R/W] : 0 		FF event source register
#define REG_FF_THS      0x17 	//  [R/W] : 0 		FF threshold register
#define REG_CTRL_REG_4  0x2D    //  [r/w] : 0		Interrupt enable register
#define REG_CTFL_REG_5  0x2E    //  [R\W] : 0		Interrupt pin map (INT1/INT2)


#define UINT14_MAX        		16383

#define ACCEL_I2C_CLK_FREQ   	CLOCK_GetFreq(I2C0_CLK_SRC)

MMA8451Q::MMA8451Q(int addr) : m_addr(addr),m_I2C((uint32_t)ACCEL_I2C_CLK_FREQ) {}

MMA8451Q::~MMA8451Q() {}

void MMA8451Q::init()
{
	/*  please refer to the "example FXOS8700CQ Driver Code" in FXOS8700 datasheet. */
	/*  write 0000 0000 = 0x00 to accelerometer control register 1 */
	/*  standby */
	/*  [7-1] = 0000 000 */
	/*  [0]: active=0 */
	uint8_t value = 0;
	m_I2C.write(m_addr, REG_CTRL_REG_1, &value, 1);
	/*  write 0000 0001= 0x01 to XYZ_DATA_CFG register */
	/*  [7]: reserved */
	/*  [6]: reserved */
	/*  [5]: reserved */
	/*  [4]: hpf_out=0 */
	/*  [3]: reserved */
	/*  [2]: reserved */
	/*  [1-0]: fs=01 for accelerometer range of +/-4g range with 0.488mg/LSB */
	/*  [1-0]: fs=00 for accelerometer range of +/-2g range  */
	/*  databyte = 0x01; */
	value = 0x00;
	m_I2C.write(m_addr, REG_XYZ , &value, 1);
    /*  write 0000 1101 = 0x0D to accelerometer control register 1 */
	/*  [7-6]: aslp_rate=00 */
	/*  [5-3]: dr=001 for 200Hz data rate (when in hybrid mode) */
	/*  [2]: lnoise=1 for low noise mode */
	/*  [1]: f_read=0 for normal 16 bit reads */
	/*  [0]: active=1 to take the part out of standby and enable sampling */
	/*   databyte = 0x0D; */
	/*   databyte = 0x01; active mode only */
	value = 0x01;
	m_I2C.write(m_addr, REG_CTRL_REG_1, &value, 1);

}

void MMA8451Q::freefall()
{

}

uint8_t MMA8451Q::getWhoAmI()
{
	uint8_t who_am_i = 0;
    m_I2C.read(m_addr, REG_WHO_AM_I , &who_am_i, 1);
	return who_am_i;
}

float MMA8451Q::getX()
{
	return (float(getAxis(REG_OUT_X_MSB))/4096.0);
}

float MMA8451Q::getY() {
    return (float(getAxis(REG_OUT_Y_MSB))/4096.0);
}

float MMA8451Q::getZ() {
    return (float(getAxis(REG_OUT_Z_MSB))/4096.0);
}

void MMA8451Q::getAllAxis(float * res) {
    res[0] = getX();
    res[1] = getY();
    res[2] = getZ();
}

int16_t MMA8451Q::getAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    m_I2C.read(m_addr, addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;

    return acc;
}
