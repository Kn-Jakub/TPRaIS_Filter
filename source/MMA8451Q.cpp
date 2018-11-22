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

#include "MMA8451Q.h"
#include "fsl_debug_console.h"


#define REG_WHO_AM_I      0x0DU
#define REG_CTRL_REG_1    0x2AU
#define REG_OUT_X_MSB     0x01
#define REG_OUT_Y_MSB     0x03
#define REG_OUT_Z_MSB     0x05
#define REG_XYZ			  0x0EU
#define REG_ACCEL_STATUS  0x00U

#define UINT14_MAX        16383

#define ACCEL_I2C_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)


MMA8451Q::MMA8451Q(int addr) : m_addr(addr),m_I2C((uint32_t)ACCEL_I2C_CLK_FREQ) {

}

MMA8451Q::~MMA8451Q() { }

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
	value = 0x0d;
	m_I2C.write(m_addr, REG_CTRL_REG_1, &value, 1);

}

uint8_t MMA8451Q::getWhoAmI() {
	uint8_t who_am_i = 0;
    m_I2C.read(m_addr, REG_WHO_AM_I , &who_am_i, 1);
	return who_am_i;
}

float MMA8451Q::getAccX()
{
	return (float(getAccAxis(REG_OUT_X_MSB))/4096.0);
}

float MMA8451Q::getAccY() {
    return (float(getAccAxis(REG_OUT_Y_MSB))/4096.0);
}

float MMA8451Q::getAccZ() {
    return (float(getAccAxis(REG_OUT_Z_MSB))/4096.0);
}

void MMA8451Q::getAccAllAxis(float * res) {
    res[0] = getAccX();
    res[1] = getAccY();
    res[2] = getAccZ();
}

int16_t MMA8451Q::getAccAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX/2)
        acc -= UINT14_MAX;

    return acc;
}

void MMA8451Q::readRegs(char registerAddress, uint8_t* data, int len)
{
	m_I2C.read(m_addr, registerAddress, data, len);
}

void MMA8451Q::writeRegs(uint8_t * data, int size) {
	m_I2C.write(m_addr, 255, data, size);

}
