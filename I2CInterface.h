#ifndef _BEAGLEI2C_H_
#define _BEAGLEI2C_H_

#include <sys/ioctl.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_BUFFER_SIZE					64

class I2CInterface
{
public:
	I2CInterface();
	~I2CInterface();

	// Public Variables
	unsigned char I2C_WR_Buf[MAX_BUFFER_SIZE];			// Contains data you want to send
	unsigned char I2C_RD_Buf[MAX_BUFFER_SIZE];			// Contains data which was read

	// Initialize Functions
	bool i2cOpen();										// Opens i2cbus 3, done at the beginning
	void i2cClose();									// Closes i2cbus 3, done at the ending
	bool i2cSetAddress(unsigned char address);					// Changes device address

	// Sends a single byte <Data> to <DEVICE_ADDR> on the register <Reg_ADDR>
	bool Send_I2C_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, unsigned char Data);

	//Sends multiple bytes to <DEVICE_ADDR>
	bool Send_Multi_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, const unsigned char *Data, unsigned char Length);

	// Reads and returns a single byte from <DEVICE_ADDR> on the register <Reg_ADDR>
	unsigned char Read_I2C_Byte(unsigned char DEVICE_ADDR,unsigned char Reg_ADDR);

	// Reads multipes byte from <DEVICE_ADDR> starting from the register address <Reg_ADDR>.
	// Read the output from i2cptr->I2C_RD_Buf
	unsigned char Read_Multi_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, ssize_t n);

protected:
	int _i2cFile;
};
#endif /* BEAGLEI2C.H */
