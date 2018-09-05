#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"

/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example LED Remote
 *
 * This is an example of how to use the RF24 class to control a remote
 * bank of LED's using buttons on a remote control.
 *
 * On the 'remote', connect any number of buttons or switches from
 * an arduino pin to ground.  Update 'button_pins' to reflect the
 * pins used.
 *
 * On the 'led' board, connect the same number of LED's from an
 * arduino pin to a resistor to ground.  Update 'led_pins' to reflect
 * the pins used.  Also connect a separate pin to ground and change
 * the 'role_pin'.  This tells the sketch it's running on the LED board.
 *
 * Every time the buttons change on the remote, the entire state of
 * buttons is send to the led board, which displays the state.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(9,10);

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2,3,4,5,6,7 };
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 2,3,4,5,6,7 };
const uint8_t num_led_pins = sizeof(led_pins);

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];

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

  // Turn LED's ON until we start getting keys
    int i = num_led_pins;
    while(i--)
    {
      pinMode(led_pins[i],OUTPUT);
      led_states[i] = HIGH;
      digitalWrite(led_pins[i],led_states[i]);
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
        // Fetch the payload, and see if this was the last one.
        radio.read( &state, 1 );

        // Spew it
        printf("State change %d\n\r", state);
      }
    }

    
    
//    // if there is data ready
//    if ( radio.available() )
//    {
//      // Read Data from the radio
//      while (radio.available())
//      {
//        // Fetch the payload, and see if this was the last one.
//        radio.read( button_states, num_button_pins );
//
//        // Spew it
//        printf("Got buttons\n\r");
//
//        // For each button, if the button now on, then toggle the LED
//        int i = num_led_pins;
//        while(i--)
//        {
//          if ( button_states[i] )
//          {
//            led_states[i] ^= HIGH;
//            digitalWrite(led_pins[i],led_states[i]);
//          }
//        }
//      }
//    }

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

