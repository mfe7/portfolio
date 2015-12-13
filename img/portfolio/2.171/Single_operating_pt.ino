/*
 * MagLev Code
 * 
 * Michael Everett
 * Wyatt Ubellacker
 * 12-10-2015
 * 2.171 Final Project
 * 
 * Using Arduino Nano board
 * 
 */

#include <Average.h>

Average<float> ave(99);
Average<int> last5(5);

const int magnetPin = 10; // Pin for PWM output to magnet opamp
const int sensorPin = A2; // Pin to read light sensor opamp
const int ref_lightBits = 283; // bits, operating height
float old_error = 0; // initialize the old error
const float Kp = 1600; // comes from MATLAB for lead compens.
const int magnetOffset = 100; // bits, feedforward term
int oldLightReadingBits = ref_lightBits;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  analogWrite(magnetPin, 0);
  pinMode(7,OUTPUT);
  
  // PWM for approx. analog output to Magnet
  // on 16-bit Timer1
  pinMode(magnetPin,OUTPUT);
  TCCR1A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR1B = _BV(CS20);
  OCR1B = 512; //change this register for duty cycle

  // 8-bit Timer0 causes 4kHz interrupt
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 62;// = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();
  
  
}

void loop() {
  // Nothing happens here. Everything is via interrupts
}

int magnetBitsToOutput(float force_desired)
{
  // This function takes in the number of bits
  // needed to compensate for error
  // and adds it to the offset a.k.a. feedforward
  // term.
  int bits;
  bits = force_desired + magnetOffset;

  // make sure no overflow (only 0-1023 range for 10-bit pwm)
  if (bits > 1023) {
    bits = 1023;
  }
  if (bits < 0)
  {
    bits = 0;
  }
  return bits;
}

ISR(TIMER0_COMPA_vect) {
  // Control Loop
  // Runs every 4kHz
  // Toggle pin 7 to confirm loop is running at 4kHz
  int oldLightReadingBits;
  digitalWrite(7,LOW);
  digitalWrite(7,HIGH);  

  // read height sensor
  int lightReadingBits = analogRead(sensorPin);

  // Lead Compensator
  float error = -(ref_lightBits - lightReadingBits);
  float force_desired = Kp*(error-0.997*old_error);
  int magnetBits = magnetBitsToOutput(force_desired);
  
  // Set PWM duty cycle via register OCR1B
  OCR1B = magnetBits; //essentially analogWrite(magnetPin,magnetBits)
  old_error = error;

/*
  Serial.print("Current Height: ");
  Serial.println(lightReadingBits);
  Serial.print("Current Error: ");
  Serial.println(error);
  Serial.print("Force Desired: ");
  Serial.println(force_desired);
  Serial.print("Magnet Bits: ");
  Serial.println(magnetBits);
  Serial.println("-----------");
  */
}

