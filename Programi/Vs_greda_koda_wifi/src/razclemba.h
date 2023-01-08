#ifndef RAZCLEMBA_H
#define RAZCLEMBA_H
#include <Arduino.h>

void Razclemba(unsigned long i_vrednost, boolean *o_RA1, boolean *o_RA2, boolean *o_RA3, int *o_zeljenavlaga1, int *o_zeljenavlaga2, int *o_zeljenavlaga3, int *o_zcas1,
 int *o_zcas2, int *o_zcas3, int *o_kcas1, int *o_kcas2, int *o_kcas3);
 
boolean manual_auto(unsigned long i_vrednost);
int zeljena_vaga(unsigned long i_vrednost);
int cas_min(unsigned long i_vrednost);
#endif