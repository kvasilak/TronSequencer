#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

#define LeftLeg 0
#define LeftBody 1
#define LeftArm 2

#define RightLeg 3
#define RightArm 4
#define RightBody 5

#define Head 6 
#define Eyes 7

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(9,10);

// EL Enable Pins on the EL Sequencer
const uint8_t ElWire[] = { 2,3,4,5,6,7,8,13 };//todo get pins for sequencer

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t state = 0;
uint8_t OldState = 0;

void AllOff(void);
void PowerUp(void);
void PowerDown(void);
void MotorCycle(void);
void KeithOn(void);
void GraceOn(void);
void Wave(void);

typedef void (* GenericFP)(void);
GenericFP StateFuncs[] = {AllOff,PowerUp,PowerDown,MotorCycle,KeithOn,GraceOn, Wave};

const uint8_t Grace = 1;
const uint8_t Keith = 2;
const uint8_t me = Grace;

void setup(void)
{
  Serial.begin(115200);
  printf_begin();
  printf("\n\rRF24/EL wire TRON Sequencer/\n\r");
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
                StateFuncs[state](); //call state function
                printf("State Change: %d\r\n");
            }
            else
            {
                printf("Unknown State: %d\r\n");
            }
                
        }
      }
    }
}

void AllOff(void)
{
    // Turn EL Off
    int i;
    for(i=0;i<sizeof(ElWire); i++)
    {
      pinMode(ElWire[i],OUTPUT);
      digitalWrite(ElWire[i],HIGH);
    }
}

//lright leg on, right bodyon
// left leg on, right leg on
//arms on, head on, eyes on
//
// the power up sequence is 3 seconds long
void PowerUp(void)
{
    digitalWrite(ElWire[LeftLeg],LOW);
    delay(100);
    digitalWrite(ElWire[RightLeg],LOW);
    delay(200);
    
    digitalWrite(ElWire[LeftBody],LOW);
    delay(100);
    digitalWrite(ElWire[RightBody],LOW);
    delay(500);
    
    //arms
    digitalWrite(ElWire[LeftArm],LOW);
    delay(100);
    digitalWrite(ElWire[LeftArm],LOW);
    delay(100);
    digitalWrite(ElWire[LeftArm],HIGH);
    delay(100);
    digitalWrite(ElWire[LeftArm],HIGH);
    delay(100);
    digitalWrite(ElWire[LeftArm],LOW);
    digitalWrite(ElWire[LeftArm],LOW);
    delay(210);
    
    digitalWrite(ElWire[Head],LOW);
    delay(100);
    digitalWrite(ElWire[Eyes],LOW);
    delay(100);
    digitalWrite(ElWire[Eyes],HIGH);
    delay(100);
    digitalWrite(ElWire[Eyes],LOW);
}

void BodyPwrDwn(void)
{
    int i;
    
    for(i=0;i<10;i++)
    {
        digitalWrite(ElWire[LeftLeg],HIGH);
        digitalWrite(ElWire[RightLeg],HIGH);
       
        digitalWrite(ElWire[LeftBody],HIGH);
        digitalWrite(ElWire[RightBody],HIGH);
        digitalWrite(ElWire[LeftArm],HIGH);
        digitalWrite(ElWire[LeftArm],HIGH);
        delay(i*10); //on more and more each loop
        
        digitalWrite(ElWire[LeftLeg],LOW);
        digitalWrite(ElWire[RightLeg],LOW);
       
        digitalWrite(ElWire[LeftBody],LOW);
        digitalWrite(ElWire[RightBody],LOW);
        digitalWrite(ElWire[LeftArm],LOW);
        digitalWrite(ElWire[LeftArm],LOW);
        delay(100-(i*10));//off more and more each loop
    }
}

//flash everything except heard and eyes
//Start w high duty cycle and decrease
//At end turn hair / hat off
//then turn eyes off, delay, then back on briefly
void PowerDown(void)
{
  BodyPwrDwn();
  
  digitalWrite(ElWire[Head],HIGH);
  delay(100);
  digitalWrite(ElWire[Head],LOW);
  delay(500);
  digitalWrite(ElWire[Eyes],HIGH);
  delay(100);
  digitalWrite(ElWire[Eyes],LOW);
  delay(500);
  digitalWrite(ElWire[Head],HIGH);
  delay(500);
  digitalWrite(ElWire[Eyes],HIGH);
  delay(500);
  digitalWrite(ElWire[Eyes],LOW);
  delay(500);
  digitalWrite(ElWire[Eyes],HIGH);
  delay(1000);
  digitalWrite(ElWire[Eyes],LOW);
  delay(100);
  digitalWrite(ElWire[Eyes],HIGH);
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

void KeithOff(void)
{
  
}

void GraceOff(void)
{
  
}

#define WaveDelay 100

void WaveRight(bool show)
{
    if( show)digitalWrite(ElWire[RightArm],HIGH);
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[RightArm],LOW);
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[RightLeg],HIGH);
    if( show)digitalWrite(ElWire[RightBody],HIGH);      
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[RightLeg],LOW);
    if( show)digitalWrite(ElWire[RightBody],LOW); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[Head],HIGH);
    if( show)digitalWrite(ElWire[Eyes],HIGH); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[Head],LOW);
    if( show)digitalWrite(ElWire[Eyes],LOW); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[LeftLeg],HIGH);
    if( show)digitalWrite(ElWire[LeftBody],HIGH); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[LeftLeg],LOW);
    if( show)digitalWrite(ElWire[LeftBody],LOW); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[LeftArm],HIGH);
    if( show)digitalWrite(ElWire[LeftArm],HIGH); 
    delay(WaveDelay);
    if( show)digitalWrite(ElWire[LeftArm],LOW);
    if( show)digitalWrite(ElWire[LeftArm],LOW);
}

void WaveLeft(bool show)
{
    delay(WaveDelay);
    if( show)(ElWire[LeftArm],HIGH);
    if( show)(ElWire[LeftArm],HIGH); 
    delay(WaveDelay);
    if( show)(ElWire[LeftArm],LOW);
    if( show)(ElWire[LeftArm],LOW); 
    delay(WaveDelay);
    if( show)(ElWire[LeftLeg],HIGH);
    if( show)(ElWire[LeftBody],HIGH);
    delay(WaveDelay);
    if( show)(ElWire[LeftLeg],LOW);
    if( show)(ElWire[LeftBody],LOW);
    delay(WaveDelay);
    if( show)(ElWire[Head],HIGH);
    if( show)(ElWire[Eyes],HIGH); 
    delay(WaveDelay);
    if( show)(ElWire[Head],LOW);
    if( show)(ElWire[Eyes],LOW); 
    delay(WaveDelay);
    if( show)(ElWire[RightLeg],HIGH);
    if( show)(ElWire[RightBody],HIGH);      
    delay(WaveDelay);
    if( show)(ElWire[RightLeg],LOW);
    if( show)(ElWire[RightBody],LOW); 
    delay(WaveDelay);
    if( show)(ElWire[RightArm],HIGH);
    delay(WaveDelay);
    if( show)(ElWire[RightArm],LOW);
    
}

//start all on, send off wave through us as we do the wave
void Wave(void)
{
    //starts on Graces Right
    WaveRight(me == Grace);//Graces version
    WaveRight(me == Keith);//Keiths version
    
    //and back again
    WaveLeft(me == Grace);
    WaveLeft(me == Keith);
 
}


