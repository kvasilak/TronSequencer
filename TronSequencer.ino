#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

#define LeftLeg 2
#define LeftBody 3
#define LeftArm 4

#define RightLeg 5
#define RightArm 6
#define RightBody 7

#define Head 12 //todo get pins for sequencer
#define Eyes 13

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(9,10);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 2,3,4,5,6,7,8,13 };

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;
uint8_t state = 0;
void *StateFuncs[] = {AllOff,PowerUp,PowerDown,MotorCycle,KeithOn,GraceOn};

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

  AllOff();
  
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
        
        if(state != OldState)
        {
            if(state < sizeof(StateFuncs))
            {
                OldState = state;
                StateFuncs[state]; //call state function
                printf("State Change: %d\r\n);
            }
            else
            {
                printf("Unknown State: %d\r\n);
            }
                
        }
      }
    }
}

void AllOff(void)
{
    // Turn EL Off
    int i;
    for(i=0;i<sizeof(led_pins); i++;)
    {
      pinMode(led_pins[i],OUTPUT);
      digitalWrite(led_pins[i],HIGH);
    }
}

//lright leg on, right bodyon
// left leg on, right leg on
//arms on, head on, eyes on
void PowerUp(void)
{
    digitalWrite(led_pins[LeftLeg],LOW);
    delay(100);
    digitalWrite(led_pins[RightLeg],LOW);
    delay(200);
    
    digitalWrite(led_pins[LeftBody],LOW);
    delay(100);
    digitalWrite(led_pins[RightBody],LOW);
    delay(500);
    
    //arms
    digitalWrite(led_pins[LeftArm],LOW);
    delay(100);
    digitalWrite(led_pins[LeftArm],LOW);
    delay(100);
    digitalWrite(led_pins[LeftArm],HIGH);
    delay(100);
    digitalWrite(led_pins[LeftArm],HIGH);
    delay(100);
    digitalWrite(led_pins[LeftArm],LOW);
    digitalWrite(led_pins[LeftArm],LOW);
    delay(210);
    
    digitalWrite(led_pins[Head],LOW);
    delay(100);
    digitalWrite(led_pins[Eyes],LOW);
    delay(100);
    digitalWrite(led_pins[Eyes],HIGH);
    delay(100);
    digitalWrite(led_pins[Eyes],LOW);
}

void BodyPwrDwn(void)
{
    int i;
    
    for(i=0;i<10;i++)
    {
        digitalWrite(led_pins[LeftLeg],HIGH);
        digitalWrite(led_pins[RightLeg],HIGH);
       
        digitalWrite(led_pins[LeftBody],HIGH);
        digitalWrite(led_pins[RightBody],HIGH);
        digitalWrite(led_pins[LeftArm],HIGH);
        digitalWrite(led_pins[LeftArm],HIGH);
        delay(i*10); //on more and more each loop
        
        digitalWrite(led_pins[LeftLeg],LOW);
        digitalWrite(led_pins[RightLeg],LOW);
       
        digitalWrite(led_pins[LeftBody],LOW);
        digitalWrite(led_pins[RightBody],LOW);
        digitalWrite(led_pins[LeftArm],LOW);
        digitalWrite(led_pins[LeftArm],LOW);
        delay(100-(i*10));//off more and more each loop
    }
}

//flash everything except heard and eyes
//Start w high duty cycle and decrease
//At end turn hair / hat off
//then turn eyes off, delay, then back on briefly
void PowerDown(void)
{
  BodyPwrDwnr();
  
  digitalWrite(led_pins[Head],HIGH);
  delay(100);
  digitalWrite(led_pins[Head],LOW);
  delay(500);
  digitalWrite(led_pins[eyes],HIGH);
  delay(100);
  digitalWrite(led_pins[Eyes],LOW);
  delay(500);
  digitalWrite(led_pins[Head],HIGH);
  delay(500);
  digitalWrite(led_pins[Eyes],HIGH);
  delay(500);
  digitalWrite(led_pins[Eyes],LOW);
  delay(500);
  digitalWrite(led_pins[Eyes],HIGH);
  delay(1000);
  digitalWrite(led_pins[Eyes],LOW);
  delay(100);
  digitalWrite(led_pins[Eyes],HIGH);
}

void MotorCycle(void)
{
  KeithOff();
  GraceOn();
}

void KeithOn(void)
{
  
}

void GraceOn(void)
{
  
}

#define WaveDelay 100
//start all on, send off wave through us as we do the wave
void Wave(void)
{
    //starts on Graces Right
    digitalWrite(led_pins[RightArm],HIGH);
    delay(WaveDelay);
    digitalWrite(led_pins[RightArm],LOWH);
    delay(WaveDelay);
    digitalWrite(led_pins[RightLeg],HIGH);
    digitalWrite(led_pins[RightBody],HIGH);      
    delay(WaveDelay);
    digitalWrite(led_pins[RightLeg],LOW);
    digitalWrite(led_pins[RightBody],LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[Head],HIGH);
    digitalWrite(led_pins[EYES],HIGH); 
    delay(WaveDelay);
    digitalWrite(led_pins[Head],LOW);
    digitalWrite(led_pins[EYES],LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftLeg],HIGH);
    digitalWrite(led_pins[LeftBody],HIGH); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftLeg],LOW);
    digitalWrite(led_pins[LeftBody,LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftArm],HIGH);
    digitalWrite(led_pins[LeftArm],HIGH); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftArm],LOW);
    digitalWrite(led_pins[LeftArm],LOW); 
    
    //and back again
    delay(WaveDelay);
    digitalWrite(led_pins[LeftArm],HIGH);
    digitalWrite(led_pins[LeftArm],HIGH); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftArm],LOW);
    digitalWrite(led_pins[LeftArm],LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[LeftLeg],High);
    digitalWrite(led_pins[LeftBody],High);
    delay(WaveDelay);
    digitalWrite(led_pins[LeftLeg],Low);
    digitalWrite(led_pins[LeftBody],Low);
    delay(WaveDelay);
    digitalWrite(led_pins[Head],HIGH);
    digitalWrite(led_pins[EYES],HIGH); 
    delay(WaveDelay);
    digitalWrite(led_pins[Head],LOW);
    digitalWrite(led_pins[EYES],LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[RightLeg],HIGH);
    digitalWrite(led_pins[RightBody],HIGH);      
    delay(WaveDelay);
    digitalWrite(led_pins[RightLeg],LOW);
    digitalWrite(led_pins[RightBody],LOW); 
    delay(WaveDelay);
    digitalWrite(led_pins[RightArm],HIGH);
    delay(WaveDelay);
    digitalWrite(led_pins[RightArm],LOWH);
}


digitalWrite(led_pins[LeftLeg],HIGH);
        digitalWrite(led_pins[RightLeg],HIGH);
       
        digitalWrite(led_pins[LeftBody],HIGH);
        digitalWrite(led_pins[RightBody],HIGH);
        digitalWrite(led_pins[LeftArm],HIGH);
        digitalWrite(led_pins[LeftArm],HIGH);
