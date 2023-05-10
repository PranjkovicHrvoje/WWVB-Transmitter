#include <ATTinyCore.h>
#define HIGH 66 
#define LOW 6 


volatile int timer;
int signal [59];

int main() {
  /* Initalize Fast PWM on OCR1A*/
  DDRB |= _BV(PB1); // Set PWM pin as output
  PLLCSR |= _BV(PLLE); // Start PLL
  //_delay_us(100);      // Wait till PLL stablizes p. 9
  PLLCSR |= _BV(PCKE); // Set Clock source to PLL
  OCR1C = 132; // Set OCR1C to top p. 88 (60kkHz)
  OCR1A = 66;  // Set beginning OCR1A value (50% duty cycle)
  TCCR1   |= _BV(CS12);    /* Set clock prescaler to 8   */ 
  TCCR1 |= _BV(PWM1A)    /* Enable PWM based on OCR1A  */ \
      |  _BV(COM1A0)   /* Set PWM compare mode p. 89 */ \
      ;

  /* Initalize CTC interupt on timer0 at 61hz */
  TCCR0A  |=  _BV(WGM01); //pg. 82  Mode 2 CTC OCR0A TOP
  OCR0A |=  127;    // 8mhz / ((127+1) * 1024 prescale) = 61hz
  TCCR0B  |=  _BV(CS00) | _BV(CS02);  // set prescaler to 1024
  TIMSK |=  _BV(OCIE0A);// enable compare match interrupt
  
  sei();  // Enable interupts
  
  while(1);
}

ISR(TIMER0_COMPA_vect){

  for(int i = 0; i < 60; i++){
    if(signal[i] == 0){
      gen_zero();
    }
    if(signal[i] == 1){
      gen_one();
    }
    if(signal[i] == 2){
      gen_mark();
    }
  }
}

void gen_zero(){
  OCR1A = LOW;
  delay(200);
  OCR1A = HIGH;
  delay(800);
}

void gen_one(){
  OCR1A = LOW;
  delay(500);
  OCR1A = HIGH;
  delay(500);
}

void gen_mark(){
  OCR1A = LOW;
  delay(800);
  OCR1A = HIGH;
  delay(200);
}
