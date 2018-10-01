#include "nRF24L01.h"
#include "printf.h"
#include "RF24.h"
#include "RF24_config.h"
#include <SPI.h>

#define LeftLeg 5
#define LeftBody 6
#define LeftArm 7

#define RightLeg 2
#define RightBody 1
#define RightArm 0

#define Head 3
#define Eyes 4

#define On HIGH
#define Off LOW

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)
RF24 radio(15,10);

// EL Enable Pins on the EL Sequencer
const uint8_t ElWire[] = { 2,3,4,5,6,7,8,9 };//todo get pins for sequencer

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

uint8_t state = 0;
uint8_t OldState = 0;

void AllOff(void);
void Sequence1(void);
void AllOn(void);
void PowerUp(void);
void PowerDown(void);
void MotorCycle(void);
void KeithOn(void);
void GraceOn(void);
void Wave(void);

typedef void (* GenericFP)(void);
GenericFP StateFuncs[] = {AllOff, //Grace
                          PowerUp, // 1 both power on
                          AllOn, // 2 keith off ships & motorcycles
                          AllOn, // 3 keith on
                          AllOn, // 4 Keith off, carry and drop
                          AllOn, // 5 Keith On
                          AllOff,// 6 grace off, keith on solo
                          AllOn, // 7 keith off, grace on solo
                          AllOn, // 8 keith on
                          AllOn, // 9 keith off, grace back bend
                          AllOn, // 10 keith on
                          PowerDown}; // 11 power down

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
  
  int i;
  for(i=0;i<sizeof(ElWire); i++)
  {
    pinMode(ElWire[i],OUTPUT);
    digitalWrite(ElWire[i],HIGH);
  }

  AllOff();
}

void loop(void)
{
    delay(4);

    // if there is data ready
    if ( radio.available() )
    {
     // printf("Radio Avail\r\n");
      
      // Read Data from the radio
      while (radio.available())
      {
        //printf("Radio read\r\n");
        radio.read( &state, 1 );

        //printf("Radio read 1\r\n");
        
        if(state != OldState)
        {
            if(state < sizeof(StateFuncs))
            {
                OldState = state;
                //printf("State Change: %d\r\n", state);
                StateFuncs[state](); //call state function 
            }
            else
            {
                printf("Unknown State: %d\r\n", state);
            }
                
        }
      }
    }
}

void AllOff(void)
{
    printf("All Off\r\n");
  
    // Turn EL Off
    int i;
    for(i=0;i<sizeof(ElWire); i++)
    {
      digitalWrite(ElWire[i],LOW);
    }
}

void AllOn(void)
{
    printf("All On\r\n");
    // Turn EL Off
    int i;
    for(i=0;i<sizeof(ElWire); i++)
    {
      digitalWrite(ElWire[i],HIGH);
    }
}

void Sequence(void)
{
   printf("Sequence\r\n");
    // Turn EL Off
    int i;
    for(i=0;i<sizeof(ElWire); i++)
    {
      digitalWrite(ElWire[i],HIGH);
      delay(500);
    }
}

void Sequence1(void)
{
    AllOff();delay(500);
  
    digitalWrite(ElWire[RightLeg],On);delay(500);
    digitalWrite(ElWire[RightBody],On);delay(500);
    digitalWrite(ElWire[RightArm],On);delay(500);
    
    digitalWrite(ElWire[LeftLeg],On);delay(500);
    digitalWrite(ElWire[LeftBody],On);delay(500);
    digitalWrite(ElWire[LeftArm],On);delay(500);

    digitalWrite(ElWire[Head],On);delay(500);
    digitalWrite(ElWire[Eyes],On);
}

//lright leg on, right bodyon
// left leg on, right leg on
//arms on, head on, eyes on
//
// the power up sequence is 3 seconds long
void PowerUp(void)
{
  AllOff();
  
    printf("PowerUp\r\n");
    digitalWrite(ElWire[LeftLeg],On);
    delay(200);
    digitalWrite(ElWire[RightLeg],On);
    delay(300);
    
    digitalWrite(ElWire[LeftBody],On);
    delay(200);
    digitalWrite(ElWire[RightBody],On);
    delay(300);
    
    //arms
    digitalWrite(ElWire[LeftArm],On);
    delay(200);
    digitalWrite(ElWire[RightArm],On);
    delay(200);
    digitalWrite(ElWire[LeftArm],Off);
    delay(200);
    digitalWrite(ElWire[RightArm],Off);
    delay(200);
    digitalWrite(ElWire[LeftArm],On);
    digitalWrite(ElWire[RightArm],On);
    delay(300);
    
    digitalWrite(ElWire[Head],On);
    delay(200);
    digitalWrite(ElWire[Eyes],On);
    delay(100);
    digitalWrite(ElWire[Eyes],Off);
    delay(200);
    digitalWrite(ElWire[Eyes],On);
}

void BodyPwrDwn(void)
{
   printf("BodyPwrDwn\r\n");
  
    int i;
    for(i=0;i<10;i++)
    {
        digitalWrite(ElWire[LeftLeg],On);
        digitalWrite(ElWire[RightLeg],On);
       
        digitalWrite(ElWire[LeftBody],On);
        digitalWrite(ElWire[RightBody],On);
        digitalWrite(ElWire[LeftArm],On);
        digitalWrite(ElWire[RightArm],On);
        delay(i*20); //on more and more each loop
        
        digitalWrite(ElWire[LeftLeg],Off);
        digitalWrite(ElWire[RightLeg],Off);
       
        digitalWrite(ElWire[LeftBody],Off);
        digitalWrite(ElWire[RightBody],Off);
        digitalWrite(ElWire[LeftArm],Off);
        digitalWrite(ElWire[RightArm],Off);
        delay(100-(i*10));//off more and more each loop
    }
}

//flash everything except heard and eyes
//Start w high duty cycle and decrease
//At end turn hair / hat off
//then turn eyes off, delay, then back on briefly
void PowerDown(void)
{
  printf("PowerDown\r\n");
  
  BodyPwrDwn();
  
  digitalWrite(ElWire[Head],Off);
  delay(100);
  digitalWrite(ElWire[Head],On);
  delay(200);
  digitalWrite(ElWire[Eyes],Off);
  delay(100);
  digitalWrite(ElWire[Eyes],On);
  delay(200);
  digitalWrite(ElWire[Head],Off);
  delay(200);
  digitalWrite(ElWire[Eyes],Off);
  delay(200);
  digitalWrite(ElWire[Eyes],On);
  delay(200);
  digitalWrite(ElWire[Eyes],Off);
  delay(1000);
  digitalWrite(ElWire[Eyes],On);
  delay(100);
  digitalWrite(ElWire[Eyes],Off);
}

#define WaveDelay 100

void WaveRight(bool show)
{
  printf("WaveRight\r\n");
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
  printf("WaveLeft\r\n");
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
  printf("Wave\r\n");
    //starts on Graces Right
//    WaveRight(me == Grace);//Graces version
//    WaveRight(me == Keith);//Keiths version
    
    //and back again
//    WaveLeft(me == Grace);
//   WaveLeft(me == Keith);
 
}


