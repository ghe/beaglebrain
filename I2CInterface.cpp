#include "I2CInterface.h"
#include "Log.h"
#include "i2cbusses.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "i2c-dev.h"
#ifdef __cplusplus
}
#endif


I2CInterface::I2CInterface()
{
	i2cOpen();
}

I2CInterface::~I2CInterface()
{
	i2cClose();
}

// open the Linux device
bool I2CInterface::i2cOpen()
{
	Log::log("beagle-i2c opening /dev/i2c-1 \n");
	int i2cbus = lookup_i2c_bus("1");
  char buffer[256] = {0};

	_i2cFile = open_i2c_dev(i2cbus, buffer, sizeof(buffer), 0);

	if (_i2cFile < 0) {
		Log::log("ERROR: i2cOpen in I2CInterface::i2cOpen\n");
		return false;
	}
	return true;
}

// close the Linux device
void I2CInterface::i2cClose()
{
	//if open doesn't happen in this class, neither should close
	close(_i2cFile);
}

// set the I2C slave address for all subsequent I2C device transfers
bool I2CInterface::i2cSetAddress(unsigned char address)
{
	if (set_slave_addr(_i2cFile, address, 0))
	{
		Log::log("ERROR: i2cSetAddress error in I2CInterface::i2cSetAddress\n");
		return false;
	}
	return true;
}


bool I2CInterface::Send_I2C_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, unsigned char Data)
{
	i2cSetAddress(DEVICE_ADDR);

	if(i2c_smbus_write_byte_data(_i2cFile, Reg_ADDR, Data) < 0) {
		Log::log("ERROR: Write Error in I2CInterface::Send_I2C_Byte\n");
		return false;
	}
	return true;
}

bool I2CInterface::Send_Multi_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, const unsigned char *Data, unsigned char Length)
{
	i2cSetAddress(DEVICE_ADDR);
  if(i2c_smbus_write_i2c_block_data(_i2cFile, Reg_ADDR, Length, Data) < 0) {
		Log::log("ERROR: Write Error in I2CInterface::Send_Mutli_Byte\n");
	}
	return true;
}



unsigned char I2CInterface::Read_I2C_Byte(unsigned char DEVICE_ADDR,unsigned char Reg_ADDR)
{
	i2cSetAddress(DEVICE_ADDR);
	I2C_RD_Buf[0] = i2c_smbus_read_byte_data(_i2cFile, Reg_ADDR);
	return I2C_RD_Buf[0];
}

unsigned char I2CInterface::Read_Multi_Byte(unsigned char DEVICE_ADDR, unsigned char Reg_ADDR, ssize_t n)
{

	i2cSetAddress(DEVICE_ADDR);

	ssize_t t = i2c_smbus_read_i2c_block_data(_i2cFile, Reg_ADDR, n, I2C_RD_Buf);
	if( t != n)
	{
		Log::log("ERROR: Read Error in I2CInterface::Read_Multi_Byte: read %d bytes instead of %d\n", t, n);
	}
	return t;
}
