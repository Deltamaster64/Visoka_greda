#ifndef REGULACIJA_H
#define REGULACIJA_H
#include <Arduino.h>

int senzor_vlaga( float x ); //pretvorba za kapacitivni senzor vlage
int str2int(int x); //pretvorba iz stringa (ASCI) v int
int bintodec(int n); //pretvorba iz binarnega v desetiško
int dectobin(int n); //pretvorba iz desetiškega v binarno
int time_conv(char ch1,char ch2); //pretvorba časa iz ASCII v int
void sun_rise_set(int day,int year,int month,int *trise,int *tset); //funkcija za pridobitev časa vzhoda in zahoda
int day_of_year(int day,int year,int month); //izračun dneva v letu
#endif
