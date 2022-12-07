
#include "BT_pretvorba.h"

int BT_pretvorba( int x1 , int x2 ){

  int x[2];
  int value;

  x[0] = dectobin(x1);
  x[1] = dectobin(x2);

  value = ((x[0] * 100000000) + x[1]);

  //Serial.println(value);

  value = bintodec(value);
  
  return value;
  
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
