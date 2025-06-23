the #include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <math.h>
#include <time.h>
#include "I2C_raspi5.h"
#include "AS5600ReadPI5.h"

struct timespec tstart, cur_time;

const float NS_PER_SECOND = 1000000000.0;

int sda_pin = 0;
int scl_pin = 2;



float t_diff(){

    float t_prev_frac = tstart.tv_nsec/NS_PER_SECOND;

    clock_gettime(CLOCK_REALTIME, &cur_time);

    float t_cur_frac  = cur_time.tv_nsec/NS_PER_SECOND;
    
    double sec_dif      = cur_time.tv_sec-tstart.tv_sec;
    float nano_sec_dif = t_cur_frac - t_prev_frac;
    
    float dt = (float)sec_dif + nano_sec_dif;
    
    return dt;

}



int main (int argc, char **argv){
	
	int fd = wiringPiSetup();
    
   i2cSetup(sda_pin ,scl_pin);
    
    int i=1;
    float dt=1.0;
    
    clock_gettime(CLOCK_REALTIME, &tstart);
    
    while (i<10000){
        /*
        digitalWrite(sda_pin,0);
        delay(100);
        digitalWrite(scl_pin,0);
        delay(100);
        digitalWrite(sda_pin,1);
        delay(100);
        digitalWrite(scl_pin,1);
        delay(100);
        */
        
        uint16_t angle=I2C_ReadAngle(0x0C, 0x36, sda_pin, scl_pin);
        
        float temp_angle = angle *((2*3.14159) / 4096.0);
        
        dt=t_diff();
        printf("%3.4f, %3.4f, %10d\n", dt, temp_angle, angle);
        i++;
        
    }
    
    dt=t_diff();
    
    printf( "%3.4f, \n", dt);
 
    printf("Test done \n");

    return 0;
}
