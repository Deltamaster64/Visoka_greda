
#include "regulacija.h"
#include "EEPROM.h"

int senzor_vlaga( float x ){

  int value;

  value = (1 - ((x - 1500) / (4095.0 - 1500.0))) * 100;

  return value;
  
}

int str2int( int x ){

  return(x - 48);
  
}

int bintodec(int n) {
  int dec = 0, i = 0, rem;

  while (n!=0) {
    rem = n % 10;
    n /= 10;
    dec += rem * pow(2, i);
    ++i;
  }

  return dec;
}

int dectobin(int n) {
  int bin = 0;
  int rem, i = 1;

  while (n!=0) {
    rem = n % 2;
    n /= 2;
    bin += rem * i;
    i *= 10;
  }

  return bin;
}


int time_conv(char ch1,char ch2){

  int a;
  
  a = (str2int(ch1) * 10) + str2int(ch2);
  
  return a;
}


void sun_rise_set(int day,int year,int month,int *trise,int *tset){

  int doy = day_of_year(day,year,month);

  int avg1 = 407; //povprečni čas sončnega vzhoda
  int dif1 = 154; //razlika najhitrejšega in najkasnejšega časa vzhoda

  *trise = avg1 + 0.5 * dif1 * cos((doy+8) / 58.09);

  int avg2 = 1134;
  int dif2 = 281;

  *tset = avg2 - 0.5 * dif2 * cos((doy+8) / 58.09);
}

int day_of_year(int day,int year,int month){

  int x = floor(275 * month / 9) - ((floor((month + 9)/ 12)) * (1 + floor(2000 + year - 4 * floor((2000 + year)/4)+2)/3)) + day - 30 +1;

  return x;
}
