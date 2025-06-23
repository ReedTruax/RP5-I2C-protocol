# RP5-I2C-protocol
This repository is a fast I2C script specific for the Raspberry Pi 5. 

When i first started using the Rasberry Pi 5 i noticed there wasn't a lot of support for using the onboard I2C pins.  A few libraries exist, but many exisiting libraries haven't yet been updated for to account for the new chip set.  Those that do exist all seem to use the same base library installed in the linux environment.  I tried using WiringPi which provides a library for using I2C communication and uses this I2C communication protocol.  However, after testing this communication proved to be too slow for my needs. 

WiringPi was averaging 0.6-0.8ms to read a block of data consiting of two 8 bit registers.  As my application required readings from multiple devices this was leading to a few milliseconds delay and choking the speed of my control loop.

Changing the i2c_arm_baurate in the firmware file seemed to have no affect on process time as has been noted on mutiple forum posts.

Instead I wrote a library based on the wonderful work from bitbanging. https://www.bitbanging.space/posts/bitbang-i2c

The implementation is almost a direct copy from them.  However, their implementation for Arduino's does not include any delay between toggling bits.  Using a data analyzer this method on the PI produced inconsistant results so a delay function was added and a few additional delays needed to be added to the function to get the proper data output.  

This new implementation was able to reduce read time to ~0.2ms for two 8 bit registers.

This code is fairly limited in scope so I encourage others to use it as a starting point for their code.  

This library requires the installation of:
  - WiringPi which is used to turn on and off the pins
  - Time, used for the delay function

I appologize but this is not setup as a library you can install rather as a normal h file you can include.

The provided example is for reading data from an AS5600 encoder https://www.digikey.com/htmldatasheets/production/1647438/0/0/1/as5600-datasheet.html?gclsrc=aw.ds&gad_source=1&gad_campaignid=120565755&gbraid=0AAAAADrbLlgBuXW0To8pc3gfjRi0ZFFsw&gclid=EAIaIQobChMIqbjbgOGHjgMVGFxHAR1ZrzLzEAAYASAAEgKd2_D_BwE
