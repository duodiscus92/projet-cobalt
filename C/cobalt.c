#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <wiringSerial.h>
#include <wiringPi.h>

#define GPIO27_BAT1POWER	2
#define GPIO24_BAT2POWER	5
#define GPIO26_BAT1ONOFF	25
#define GPIO25_BAT2ONOFF	6

/* For MQTT protocol */
#define BAT1POWER_IDX 2712
#define BAT2POWER_IDX 2713
#define BAT1ONOFF_IDX 2710
#define BAT2ONOFF_IDX 2711

int main (void)
{
  wiringPiSetup () ;
  pinMode (GPIO27_BAT1POWER, INPUT);
  pinMode (GPIO24_BAT2POWER, INPUT);
  pinMode (GPIO26_BAT1ONOFF, INPUT);
  pinMode (GPIO25_BAT2ONOFF, INPUT);

  pullUpDnControl(GPIO27_BAT1POWER, PUD_OFF);
  pullUpDnControl(GPIO24_BAT2POWER, PUD_OFF);
  pullUpDnControl(GPIO26_BAT1ONOFF, PUD_OFF);
  pullUpDnControl(GPIO25_BAT2ONOFF, PUD_OFF);

  
  for (;;)
  {
//     printf("Batterie 1 : %s\n", digitalRead (GPIO27_BAT1POWER) ? "OK" : "LOW POWER");
//    printf("Batteire 2 : %s\n", digitalRead (GPIO24_BAT2POWER) ? "OK" : "LOW POWER");
//     printf("Batterie 1 : %s\n", digitalRead (GPIO26_BAT1ONOFF) ? "OFF" : "ON");
//    printf("Batterie 2 : %s\n", digitalRead (GPIO25_BAT2ONOFF) ? "OFF" : "ON");
     printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT1POWER_IDX, digitalRead (GPIO27_BAT1POWER) ? "CHARGEE" : "LOW POWER");  fflush(stdout);
     printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT2POWER_IDX, digitalRead (GPIO24_BAT2POWER) ? "CHARGEE" : "LOW POWER");  fflush(stdout);
     printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT1ONOFF_IDX, digitalRead (GPIO26_BAT1ONOFF) ? "HORS SERVICE" : "EN SERVICE");  fflush(stdout);
     printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT2ONOFF_IDX, digitalRead (GPIO25_BAT2ONOFF) ? "HORS SERVICE" : "EN SERVICE");  fflush(stdout);
     sleep(10);
  }
  return 0 ;
}
