#include <AltSoftSerial.h>

AltSoftSerial altSerial; //9 tx 8 rx 

#define MIDI_CONTROL       0xB0
#define MIDI_NOTE_ON       0x80
#define MIDI_NOTE_OFF      0x90
#define MIDI_CHANNEL       0x01

#define KORG_TOUCH_PAD_X_AXIS   12
#define KORG_TOUCH_PAD_Y_AXIS   13
#define KORG_TOUCH_PAD_ON_OFF   92


void setup()
{
   // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {}
  
  altSerial.begin(31250);    
}

void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY,byte midichannel) 
{
  static byte status1;

  status1 = MESSAGE + midichannel;
  altSerial.write(status1);
  altSerial.write(PITCH);
  altSerial.write(VELOCITY);

}


unsigned char buffer[3];
unsigned char bufferI = 0;

void loop() // run over and over
{
  //*
  if (altSerial.available()){
    unsigned char readValue = altSerial.read();
    Serial.print("got some data");
    /*
    if((readValue & 0b10000000) == true)
    {
      bufferI = 0;
    }
    */
    buffer[bufferI] = readValue;
    bufferI++;
    if(bufferI == 3)
    {
      bufferI = 0;
      /*
      if(readValue & MIDI_NOTE_ON  == true ||
         readValue & MIDI_NOTE_OFF == true )
      {
        MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_X_AXIS,buffer[1],MIDI_CHANNEL);
        MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_Y_AXIS,0,MIDI_CHANNEL);
        if(readValue & MIDI_NOTE_ON == true){
          MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_ON_OFF,127,MIDI_CHANNEL);
        }
        else if(readValue & MIDI_NOTE_ON == true){
          MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_ON_OFF,0,MIDI_CHANNEL);
        }
      }
      */
      Serial.print("got something - ");
      for(int i =0; i < 3; i++)
      {
        Serial.print(buffer[i],HEX);
        Serial.print(",");
      }
      Serial.print("\n");
    }
  }
  //*/
  /*
  while(true){
    int i =1;
    for(int j =0; j < 127; j++)
    {
      MIDI_TX(0xB0,12,j,i);
      MIDI_TX(0xB0,13,j,i);
      MIDI_TX(0xB0,92,127,i);
      delay(100);
      MIDI_TX(0xB0,92,0,i);
      delay(100);
    }
  }
  //*/
}

