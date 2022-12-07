
#include "regulacija.h"

int senzor_vlaga( float x ){

  int value;

  value = (1 - ((x - 1500) / (4095.0 - 1500.0))) * 100;

  return value;
  
}
