#include "I2C_raspi5.h"
#include <stdio.h>
#include <time.h>

struct timespec t_start, t_cur;


//THis library is adapted from code on "https://www.bitbanging.space/posts/bitbang-i2c"

//Function definitions


void dly(){
	
	float nanos_per_second = 1000000000.0;

    clock_gettime(CLOCK_REALTIME, &t_start);
    float t_prev_frac = t_start.tv_nsec/nanos_per_second;
    
    clock_gettime(CLOCK_REALTIME, &t_cur);
    float t_cur_frac  = t_cur.tv_nsec/nanos_per_second;
    
    double sec_dif      = t_cur.tv_sec-t_start.tv_sec;
    float nano_sec_dif = t_cur_frac - t_prev_frac;
    
    float dt = (float)sec_dif + nano_sec_dif;
    
    //printf("DElay \n");
    while (dt <0.000001){
		
		clock_gettime(CLOCK_REALTIME, &t_cur);
		t_cur_frac  = t_cur.tv_nsec/nanos_per_second;
		
		sec_dif      = t_cur.tv_sec-t_start.tv_sec;
		nano_sec_dif = t_cur_frac - t_prev_frac;
    
		dt = (float)sec_dif + nano_sec_dif;
		
	}
	
	}; //optional delay

void i2cSetup(int sda_pin, int scl_pin){
	
	pinMode(sda_pin, OUTPUT);
	pinMode(scl_pin, OUTPUT);
	
	//make initial states the same as the end of the stop
	digitalWrite(scl_pin, 0); //SCL_OFF;
	digitalWrite(sda_pin, 1); //SDA_ON;
	
}


// Start I2C sequence
void start(int sda_pin, int scl_pin){

	digitalWrite(sda_pin, 1); //SDA_ON;
	dly();
	digitalWrite(scl_pin, 1); //SCL_ON;
	dly();
	digitalWrite(sda_pin, 0); //SDA_OFF;
	dly();
	digitalWrite(scl_pin, 0); //SCL_OFF;
	dly();

}


// stop I2C sequence
void stop(int sda_pin, int scl_pin) {

	digitalWrite(sda_pin, 0); //SDA_OFF;
	dly();
	digitalWrite(scl_pin, 1); //SCL_ON;
	dly();
	digitalWrite(sda_pin, 1); //SDA_ON;
	dly();

}

// transmit data from trunk to branch
bool Tx(uint8_t dat, int sda_pin, int scl_pin){
	//returns the acknowledgement bit as a bool
	for (uint8_t i=8; i; i--){
		
		(dat & 0x80) ? digitalWrite(sda_pin, 1) : digitalWrite(sda_pin, 0);
		dat <<= 1;
		dly();
		digitalWrite(scl_pin, 1); //SCL_ON;
		dly();
		digitalWrite(scl_pin, 0); //SCL_OFF;
		dly();
		
	}
	
	digitalWrite(sda_pin, 1); //SDA_ON;
	digitalWrite(scl_pin, 1); //SCL_ON;
	
	dly();
	
	bool ack = !digitalRead(sda_pin);
	digitalWrite(scl_pin, 0); //SCL_OFF;
	
	return ack;
	
}

// Recieve data from branch to trunk

uint8_t Rx(bool ack, int sda_pin, int scl_pin) {
	//Recieve 8-bit data and send ACK/NACK
	uint8_t dat=0;
	
	digitalWrite(sda_pin, 1); //SDA_ON;
	dly();
	
	for (uint8_t i=0; i<8; i++){
		
		dat <<=1;
		do {
			digitalWrite(scl_pin, 1); //SCL_ON;
		} while (digitalRead(scl_pin)==0); //clock stretching
		
		dly();
		
		if (digitalRead(sda_pin)) {
			dat |=1;
		}
		
		dly();
		digitalWrite(scl_pin, 0); //SCL_OFF;
		dly();
	}
	
	ack ? digitalWrite(sda_pin, 0) :  digitalWrite(sda_pin, 1);
	digitalWrite(scl_pin, 1); //SCL_ON;
	dly();
	digitalWrite(scl_pin, 0); //SCL_OFF;
	digitalWrite(sda_pin, 1); //SDA_ON;
	return(dat);
}
			
