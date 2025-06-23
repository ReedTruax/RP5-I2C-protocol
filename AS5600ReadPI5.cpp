#include "AS5600ReadPI5.h"

uint16_t I2C_ReadAngle(uint8_t B_REGISTER, uint8_t B_ADDRESS, int sda_pin, int scl_pin){
	
	//"Data read with address pointer reload" mode from the AS5600 datasheet"
	start(sda_pin, scl_pin);
	bool ACK=Tx((B_ADDRESS << 1) | I2C_WRITE, sda_pin, scl_pin);
	ACK = Tx(B_REGISTER , sda_pin, scl_pin);
	start(sda_pin, scl_pin);
	ACK=Tx((B_ADDRESS << 1) | I2C_READ, sda_pin, scl_pin);
	uint8_t highByte = Rx(1, sda_pin, scl_pin); //sends a ACK to continue
	uint8_t lowByte  = Rx(0, sda_pin, scl_pin); //sends a NAck to end the transmission
	stop(sda_pin, scl_pin);
	return((highByte << 8) | lowByte);

}
