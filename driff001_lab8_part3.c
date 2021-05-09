/*	Author: Dylan
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab 8  Exercise 3
 *	Exercise Description: [optional - include for your own benefit]
 *      Video Link:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
int count = 0;
int count2 = 0;

void TimerOn(){
TCCR1B = 0x0B;
OCR1A = 125;
TIMSK1 = 0x02;
TCNT1 = 0;
_avr_timer_cntcurr = _avr_timer_M;
SREG |= 0x80;

}	

void TimerOff(){
TCCR1B = 0x00;
}	
void TimerISR(){
TimerFlag = 1;
}	
ISR(TIMER1_COMPA_vect){

    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0){
	TimerISR();
	_avr_timer_cntcurr = _avr_timer_M;
    
    }	    
}



void TimerSet(unsigned long M){
       _avr_timer_M = M;
       _avr_timer_cntcurr = _avr_timer_M;
}	


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


unsigned char freq = 0;

double Queue[18] = { 127.47,222.0,185.00,222.0,146.83,146.83,146.83,138.59,146.83,185.00,444.0,277.18,444.0,185.00,329.63,311.13,293.66,207.65};


enum SM_STATES { SM_Wait, SM_Song,SM_End} SM_STATE;

void Tick(){
switch(SM_STATE){
	case SM_Wait:
	if ( 255 - PINA == 0b01){
        SM_STATE = SM_Song;
	}
 	else {
        SM_STATE = SM_Wait;
	}
	break;

	case SM_Song:
        if (freq == 18){
	freq = 0;
	SM_STATE = SM_End;
	}
	else{
        ++freq;
        SM_STATE = SM_Song;
	}

	break;

	case SM_End:

	if ( 255 - PINA == 0b00){
	SM_STATE = SM_Wait;
	}
	else {
	SM_STATE = SM_End;
	}
	break;

}
switch(SM_STATE){
	case SM_Wait:
	break;

	case SM_Song:
	set_PWM(Queue[freq]);
	break;
	
	case SM_End:
	set_PWM(0);
	break;


}
}
int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; 
    PORTA = 0xFF; DDRB = 0xFF; PORTB = 0xFF;
    DDRD = 0xFF;  PORTD = 0x00;
    
    TimerSet(400);
    TimerOn();

    PWM_on();
    
    SM_STATE = SM_Wait;  
    /* Insert your solution below */
    while (1) {
   
    Tick();

    while(!TimerFlag);
    TimerFlag = 0;
    }
   
}
