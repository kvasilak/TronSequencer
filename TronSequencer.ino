#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(9,10);

// Pins on the remote for buttons
//const uint8_t button_pins[] = { 2,3,4,5,6,7 };
//const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 2,3,4,5,6,7 };
//const uint8_t num_led_pins = sizeof(led_pins);

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

//uint8_t button_states[num_button_pins];
//uint8_t led_states[num_led_pins];

uint8_t state = 0;

void setup(void)
{
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/examples/led_remote/\n\r");
  printf("ROLE: Sequencer\n\r");

  radio.begin();

  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  radio.printDetails();

  // Turn EL Off
    int i = sizeof(led_pins);
    while(i--)
    {
      pinMode(led_pins[i],OUTPUT);
      //led_states[i] = LOW;
      digitalWrite(led_pins[i],LOW); //led_states[i]);
    }
}

void loop(void)
{
    delay(4);

    // if there is data ready
    if ( radio.available() )
    {
      // Read Data from the radio
      while (radio.available())
      {
        radio.read( &state, 1 );
        printf("State change %d\n\r", state);

        if(state==0)
        {
          digitalWrite(led_pins[0],LOW);
          digitalWrite(led_pins[1],LOW);
          digitalWrite(led_pins[2],LOW);
        }
        else if(state==1)
        {
          digitalWrite(led_pins[0],HIGH);
          digitalWrite(led_pins[1],HIGH);
          digitalWrite(led_pins[2],HIGH);
        }
      }
    }
}

void AllOff(void)
{
}

void PowerUp(void)
{

}
void PowerDown(void)
{
  
}

void MotorCycle(void)
{
  
}

void KeithOn(void)
{
  
}

void GraceOn(void)
{
  
}

