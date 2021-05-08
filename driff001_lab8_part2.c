/*	Author: Dylan Riffel
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 8  Exercise 2
 *	Exercise Description: [optional - include for your own benefit]
 *      Video Link: https://www.youtube.com/watch?v=Slx4PjQdqkE
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency){ TCCR3B &= 0x08;}
	        else { TCCR3B |= 0x03; }

		if( frequency < 0.954) { OCR3A = 0xFFFF;}

		else if (frequency > 31250) { OCR3A = 0x0000; }

		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);

}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;

}

unsigned int count = 0;
unsigned int count1 = 0;
double freq = 0;


enum S_STATES { S_Wait, S_On,S_Off} S_STATE;

void On(){

switch(S_STATE){
case S_Wait:
     if ((255 - PINA == 0b01)){
     ++count;
     S_STATE = S_On;
     } 
     else { 
     S_STATE = S_Wait;
     }
     break;
case S_On:
     if ((255 - PINA == 0b01)){
     S_STATE = S_Off;   
     }
     else {
     S_STATE = S_On; 
     }
     break;
case S_Off:
     if (255 - PINA == 0b01 && count == 0){
     S_STATE = S_On;
     }
     else {
     S_STATE = S_Off;
     }
     break;
}

switch(S_STATE){

case S_Wait:
set_PWM(0);
break;

case S_On:
set_PWM(freq);
break;


case S_Off:
count = 0;
set_PWM(0);
break;

}
}


enum SM_STATES { SM_Wait, SM_PressInc,SM_PressDec,SM_PressWait} SM_STATE;

void Tick(){
switch(SM_STATE){

case SM_Wait:

if(255 - PINA == 0b100){
++count1;
SM_STATE = SM_PressInc;
}
else {
SM_STATE = SM_Wait;
}
break;

case SM_PressInc:

if  ( 255 - PINA == 0b00){  
SM_STATE = SM_PressWait;
}

else if  ( (255 - PINA == 0b10) && (count1 > 0 ) ) {
--count1;
SM_STATE = SM_PressDec;	
}

break;

case SM_PressDec:

if  (255 - PINA == 0b00){
SM_STATE = SM_PressWait;
}

else if  ( (255 - PINA == 0b100) && (count1 < 8 ) ) {
++count1;
SM_STATE = SM_PressInc;
}

break;

case SM_PressWait:

if ( (255 - PINA == 0b100) && count1 < 8   ) {
++count1;
SM_STATE = SM_PressInc;
}
else if  ( (255 - PINA == 0b10) && (count1 > 0 ) ) {
--count1;
SM_STATE = SM_PressDec;
}
else {
SM_STATE = SM_PressWait; 
}
break;
}


switch(SM_STATE){
case S_Wait:
set_PWM(261.63);
break;

case SM_PressInc:

if(count1 == 0){
freq = 261.63;
set_PWM(261.63);
}
if(count1 == 1){
freq = 293.66;
set_PWM(293.66);
}
if(count1 == 2){
freq = 329.63;
set_PWM(329.63);
}
else if(count == 3){
freq = 349.23;
set_PWM(349.23);
}
else if(count1 == 4){
freq = 392.00;
set_PWM(392.00);
}
else if(count1 == 5){
freq = 444.00;
set_PWM(440.00);
}
else if(count1 == 6){
freq = 493.88;
set_PWM(493.88);
}
else if(count1 == 7){
freq = 523.25;
set_PWM(523.25);
}
break;

case SM_PressDec:

if(count1 == 0){
freq = 261.63;
set_PWM(261.63);
}

if(count1 == 1){
freq = 293.66;
set_PWM(293.66);
}
else if(count1 == 2){
freq = 329.63;
set_PWM(329.63);
}
else if(count == 3){
freq = 349.23;
set_PWM(349.23);
}
else if(count1 == 4){
freq = 392.00;
set_PWM(392.00);
}
else if(count1 == 5){
freq = 444.00;
set_PWM(440.00);
}
else if(count1 == 6){
freq = 493.88;
set_PWM(493.88);
}
else if(count1 == 7){
freq = 523.25;
set_PWM(523.25);
}

break;


case SM_PressWait:

if(count1 == 0){
freq = 261.63;
set_PWM(261.63);
}

if(count1 == 1){
freq = 293.66;
set_PWM(293.66);
}
else if(count1 == 2){
freq = 329.63;
set_PWM(329.63);
}
else if(count == 3){
freq = 349.23;
set_PWM(349.23);
}
else if(count1 == 4){
freq = 392.00;
set_PWM(392.00);
}
else if(count1 == 5){
freq = 444.00;
set_PWM(440.00);
}
else if(count1 == 6){
freq = 493.88;
set_PWM(493.88);
}
else if(count1 == 7){
freq = 523.25;
set_PWM(523.25);
}

break;

}
}










int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; 
    PORTA = 0xFF; DDRB = 0xFF; PORTB = 0xFF;
    DDRD = 0xFF;  PORTD = 0x00;
  
    PWM_on();
    
    SM_STATE = SM_Wait;  
    S_STATE = S_Wait;
    /* Insert your solution below */
    while (1) {
    On();
    Tick();
    }
   
}
