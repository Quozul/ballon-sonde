#include <stdio.h>
#include <stdlib.h>



void main() 
{
  // Create I2C bus
  int file;
  char *bus = "/dev/i2c-1";
  if ((file = open(bus, O_RDWR)) < 0) 
  {
    printf("Failed to open the bus. \n");
    exit(1);
  }
  // Get I2C device, ADC121C_MQ131 I2C address is 0x50(80)
  ioctl(file, I2C_SLAVE, 0x50);

  // Read 2 bytes of data from register(0x00)
  // raw_adc msb, raw_adc lsb
  char reg[1] = {0x00};
  write(file, reg, 1);
  char data[2] = {0};
  if(read(file, data, 2) != 2)
  {
    printf("Erorr : Input/output Erorr \n");
    exit(1);
  }
  else
  {
    // Convert the data to 12-bits
    int raw_adc = (data[0] & 0x0F) * 256 + data[1];
    float ppm = (1.99 * raw_adc) / 4095.0 + 0.01;

    // Output data to screen
    printf("Ozone Concentration : %.2f ppm \n", ppm);
  }
}
