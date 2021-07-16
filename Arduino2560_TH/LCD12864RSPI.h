//Demo LCD12864 spi
#ifndef _LCD12864RSPI_H
#define _LCD12864RSPI_H

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <inttypes.h>


class LCD12864RSPI {
typedef unsigned char uchar;


public:

LCD12864RSPI();

void Initialise(void);
void delayns(void);

void WriteByte(int dat);
void WriteCommand(int CMD);
void WriteData(int CMD);


void CLEAR(void);
void DisplayString(int X,int Y,uchar *ptr,int dat);
void DisplaySig(int M,int N,int sig);
void DrawFullScreen(uchar *p);

int delaytime;
int DEFAULTTIME;


static const int latchPin = A10; //rs
static const int clockPin = A12; //e
static const int dataPin = A11;  //r/w

};
extern LCD12864RSPI LCDA;    
#endif
