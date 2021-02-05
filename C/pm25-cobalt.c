#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <wiringSerial.h>
#include <wiringPi.h>

// définitions pour capteur pm25
#define  NB_VALUES  12
/* For pm25 MQTT protocol */
#define IDX_PART03 	1220
#define IDX_PART05 	1221
#define IDX_PART1  	1222
#define IDX_PART25 	1223
#define IDX_PART5  	1224
#define IDX_PART10 	1225
#define IDX_STD1	1227
#define IDX_STD25	1228
#define IDX_STD10	1229
#define IDX_ATM1	1230
#define IDX_ATM25	1231
#define IDX_ATM10	1232
//=======================================
//définition pour COBALT
#define GPIO27_BAT1POWER	2
#define GPIO24_BAT2POWER	5
#define GPIO26_BAT1ONOFF	25
#define GPIO25_BAT2ONOFF	6
/* For COBALT MQTT protocol */
#define BAT1POWER_IDX 2712
#define BAT2POWER_IDX 2713
#define BAT1ONOFF_IDX 2710
#define BAT2ONOFF_IDX 2711
//=======================================
/* interrupt handler  for low power */
static int batt1_low=1, batt2_low=1;
void batt1low_interrupthandler(void)
{
batt1_low = 0;
}

void batt2low_interrupthandler(void)
{
batt2_low = 0;
}

//======================================
int main(int argc, char ** argv)
{

   int fn, i, ctr=0;
   unsigned char buffer[40], c;

   int pm1_hb_std, pm1_lb_std, pm25_hb_std, pm25_lb_std, pm10_hb_std, pm10_lb_std;
   int pm1_hb_atm, pm1_lb_atm, pm25_hb_atm, pm25_lb_atm, pm10_hb_atm, pm10_lb_atm;
   int part_03_hb, part_03_lb, part_05_hb, part_05_lb, part_1_hb, part_1_lb, part_25_hb, part_25_lb, part_5_hb, part_5_lb, part_10_hb, part_10_lb;

   int pm1_std, pm25_std, pm10_std, pm1_atm,  pm25_atm, pm10_atm;
   int part_03, part_05, part_1, part_25, part_5, part_10;

   int h, min, s, day, mois, an;
   time_t now;
   struct tm *local;

// initialisations pour PM24
   if((fn = serialOpen("/dev/serial0", 9600)) == -1 ){
      fprintf(stderr, "Unable to open serial0\n");
      exit(1);
   }
   fprintf(stderr, "Open of serial0 succesful\n");

// initialisation pour COBALT
  wiringPiSetup () ;
  pinMode (GPIO27_BAT1POWER, INPUT);
  pinMode (GPIO24_BAT2POWER, INPUT);
  pinMode (GPIO26_BAT1ONOFF, INPUT);
  pinMode (GPIO25_BAT2ONOFF, INPUT);

  pullUpDnControl(GPIO27_BAT1POWER, PUD_OFF);
  pullUpDnControl(GPIO24_BAT2POWER, PUD_OFF);
  pullUpDnControl(GPIO26_BAT1ONOFF, PUD_OFF);
  pullUpDnControl(GPIO25_BAT2ONOFF, PUD_OFF);

//  wiringPiISR(GPIO26_BAT1ONOFF, INT_EDGE_RISING, batt1low_interrupthandler);
//  wiringPiISR(GPIO25_BAT2ONOFF, INT_EDGE_RISING, batt2low_interrupthandler);

   while(1){
// partie pm25
      //printf("\e[1;1H\e[2J");
      buffer[0] = 0;
      while(buffer[0] != 0x42)
         //read(fn, &buffer[0], 1);
	 buffer[0] = serialGetchar(fn);
      //read(fn, &buffer[1], 1);
      buffer[1] = serialGetchar(fn);
      if(buffer[1] != 0x4d)
         continue;

      time(&now);

      //read(fn, buffer+2, 30);
      for(i=2; i<30; i++)
         buffer[i] = serialGetchar(fn);
      /*for(i=0; i<32; i++)
        printf("%02x ", buffer[i]);
      printf("\n");*/
                //PM1, PM2.5 and PM10 values for standard particle in ug/m^3
                pm1_hb_std = buffer[4];
                pm1_lb_std = buffer[5];
                pm1_std = (pm1_hb_std * 256 + pm1_lb_std);
                pm25_hb_std = buffer[6];
                pm25_lb_std = buffer[7];
                pm25_std = (pm25_hb_std * 256 + pm25_lb_std);
                pm10_hb_std = buffer[8];
                pm10_lb_std = buffer[9];
                pm10_std = (pm10_hb_std * 256 + pm10_lb_std);

                // PM1, PM2.5 and PM10 values for atmospheric conditions in ug/m^3
                pm1_hb_atm = buffer[10];
                pm1_lb_atm = buffer[11];
                pm1_atm = (pm1_hb_atm * 256 + pm1_lb_atm);
                pm25_hb_atm = buffer[12];
                pm25_lb_atm = buffer[13];
                pm25_atm = (pm25_hb_atm * 256 + pm25_lb_atm);
                pm10_hb_atm = buffer[14];
                pm10_lb_atm = buffer[15];
                pm10_atm = (pm10_hb_atm * 256 + pm10_lb_atm);

                // Number of particles bigger than 0.3 um, 0.5 um, etc. in #/cm^3
                part_03_hb = buffer[16];
                part_03_lb = buffer[17];
                part_03 = (part_03_hb * 256 + part_03_lb);
                part_05_hb = buffer[18];
                part_05_lb = buffer[19];
                part_05 = (part_05_hb * 256 + part_05_lb);
                part_1_hb = buffer[20];
                part_1_lb = buffer[21];
                part_1 = (part_1_hb * 256 + part_1_lb);
                part_25_hb = buffer[22];
                part_25_lb = buffer[23];
                part_25 = (part_25_hb * 256 + part_25_lb);
                part_5_hb = buffer[24];
                part_5_lb = buffer[25];
                part_5 = (part_5_hb * 256 + part_5_lb);
                part_10_hb = buffer[26];
                part_10_lb = buffer[27];
                part_10 = (part_10_hb * 256 + part_10_lb);

		local = localtime(&now);
  		h = local->tm_hour;
  		min = local->tm_min;
  		s = local->tm_sec;
  		day = local->tm_mday;
  		mois = local->tm_mon + 1;
  		an = local->tm_year + 1900;
/*
		printf("======================|=====|=====|=====|\n");
		printf("%02d/%02d/%d - %02d:%02d:%02d |  PM1|PM2.5| PM10|\n", day, mois, an, h, min, s);
		printf("======================|=====|=====|=====|\n");
                printf("Standard particle     |%5d|%5d|%5d|\n", pm1_std, pm25_std, pm10_std);
		printf("======================|=====|=====|=====|\n");
                printf("Atmospheric conditions|%5d|%5d|%5d|\n", pm1_atm, pm25_atm, pm10_atm);
		printf("======================|=====|=====|=====|=====|=====|=====|\n");
		printf("                      | >0.3| >0.5| >1.0| >2.5|   >5|  >10|\n");
		printf("======================|=====|=====|=====|=====|=====|=====|\n");
                printf("Number of particles   |%5d|%5d|%5d|%5d|%5d|%5d|\n",  part_03, part_05, part_1, part_25, part_5, part_10);
		printf("======================|=====|=====|=====|=====|=====|=====|\n");
*/
		switch(ctr){
		case 0 : 
		   printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART03, part_03);  fflush(stdout);
		   break;
		case 1 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART05, part_05);  fflush(stdout);
		   break;
		case 2 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART1, part_1);  fflush(stdout);
		   break;
		case 3 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART25, part_25);  fflush(stdout);
		   break;
		case 4 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART5, part_5);  fflush(stdout);
		   break;
		case 5 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_PART10, part_10);  fflush(stdout);
		   break;
		case 6 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_STD1, pm1_std);  fflush(stdout);
		   break;
		case 7 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_STD25, pm25_std);  fflush(stdout);
		   break;
		case 8 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_STD10, pm10_std);  fflush(stdout);
		   break;
		case 9 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_ATM1, pm1_atm);  fflush(stdout);
		   break;
		case 10 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_ATM25, pm25_atm);  fflush(stdout);
		   break;
		case 11 : 
		printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%d\"}\n", IDX_ATM10, pm10_atm);  fflush(stdout);
		   break;
		}

      ctr++;
      ctr = ctr %12;
// fin partie pm25
// partie COBALT
//      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT1POWER_IDX, digitalRead (GPIO27_BAT1POWER) ? "CHARGEE" : "LOW POWER");  fflush(stdout);
//      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT2POWER_IDX, digitalRead (GPIO24_BAT2POWER) ? "CHARGEE" : "LOW POWER");  fflush(stdout);
//      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT1POWER_IDX, batt1_low ? "CHARGEE" : "LOW POWER");  fflush(stdout);
//      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT2POWER_IDX, batt2_low ? "CHARGEE" : "LOW POWER");  fflush(stdout);
      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT1ONOFF_IDX, digitalRead (GPIO26_BAT1ONOFF) ? "HORS SERVICE" : "EN SERVICE");  fflush(stdout);
      printf("{\"idx\" : %4d, \"nvalue\" : 0, \"svalue\" : \"%s\"}\n", BAT2ONOFF_IDX, digitalRead (GPIO25_BAT2ONOFF) ? "HORS SERVICE" : "EN SERVICE");  fflush(stdout);
//      if(digitalRead (GPIO26_BAT1ONOFF) == 1) batt1_low = 1;
//      if(digitalRead (GPIO25_BAT2ONOFF) == 1) batt2_low = 1;
// fin partie COBALT
      sleep(10);
  }
  //close(fn);
  serialClose(fn);
}

