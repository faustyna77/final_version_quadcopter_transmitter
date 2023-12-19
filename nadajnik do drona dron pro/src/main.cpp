#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


  
const uint64_t pipeOut = 0xE8E8F0F0E1LL;

RF24 radio(7, 8); 


struct MyData {
  byte throttle;
  byte yaw;
  byte pitch;
  byte roll;
  byte AUX1;
  byte AUX2;
};

MyData data;

void resetData() 
{
  
    
  data.throttle = 0;
  data.yaw = 0;
  data.pitch = 0;
  data.roll = 0;
  data.AUX1 = 0;
  data.AUX2 = 0;
}

void setup()
{
  
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  resetData();
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
}


int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}

void loop()
{
  
  if(digitalRead(4)==LOW)
  {

  
  
  data.throttle = mapJoystickValues( analogRead(A4), 13, 524, 1015, true );
  data.yaw      = mapJoystickValues( analogRead(A5),  1, 505, 1020, true );
  data.pitch    = mapJoystickValues( analogRead(A4), 12, 544, 1021, true );
  data.roll     = mapJoystickValues( analogRead(A5), 34, 522, 1020, true );
  
  
  }
 
  
   else if(digitalRead(5)==LOW)
  {
      data.throttle = mapJoystickValues( analogRead(A0), 13, 524, 1015, true );
  data.yaw      = mapJoystickValues( analogRead(A1),  1, 505, 1020, true );
  data.pitch    = mapJoystickValues( analogRead(A2), 12, 544, 1021, true );
  data.roll     = mapJoystickValues( analogRead(A3), 34, 522, 1020, true );

  }
 else if(digitalRead(4)==HIGH){
  resetData();
 }

  radio.write(&data, sizeof(MyData));
}