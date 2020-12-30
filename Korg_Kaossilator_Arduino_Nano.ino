#include <AltSoftSerial.h>

AltSoftSerial altSerial; //9 tx 8 rx 

// midi protocol status
#define MIDI_CONTROL           0xB0
#define MIDI_NOTE_ON           0x90
#define MIDI_NOTE_OFF          0x80

// midi protocol data
#define MIDI_MODULATION_WHEEl  0x01


//korg midi protocol status
#define KORG_MIDI_CHANNEL           0x01 //so this is ch 2 on the korg

// korg midi protocol data
#define KORG_TOUCH_PAD_X_AXIS   12
#define KORG_TOUCH_PAD_Y_AXIS   13
#define KORG_TOUCH_PAD_ON_OFF   92


void setup()
{
   //standard serial port 
  Serial.begin(115200);
  while (!Serial) {}

  //alternative serial port for midi 
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


unsigned char midiBuffer[3];
unsigned char midiBufferI = 0;
unsigned char volume = 0x7F;

void loop() // run over and over
{
  //*
  if (altSerial.available()){
    unsigned char readValue = altSerial.read();
    Serial.print(readValue,HEX);
    Serial.print(",");

    //if it's a status byte reset the midi buffer
    if((readValue & 0b10000000) != 0)
    {
      midiBufferI = 0;
    }
    midiBuffer[midiBufferI] = readValue;
    midiBufferI++;
    
    //when you've got a full buffer
    if(midiBufferI == 3)
    {
      midiBufferI = 0;
      //*
      if((midiBuffer[0] & 0xf0)  == MIDI_NOTE_ON  ||
         (midiBuffer[0] & 0xf0) == MIDI_NOTE_OFF)
      {
        MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_X_AXIS,midiBuffer[1],KORG_MIDI_CHANNEL);
        MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_Y_AXIS,volume,KORG_MIDI_CHANNEL);
        if((midiBuffer[0] & 0xf0)  == MIDI_NOTE_ON ){
          MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_ON_OFF,127,KORG_MIDI_CHANNEL);
        }
        else if((midiBuffer[0] & 0xf0) == MIDI_NOTE_OFF){
          MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_ON_OFF,0,KORG_MIDI_CHANNEL);
        }
      }
      else if((midiBuffer[0] & 0xf0)  == MIDI_CONTROL)
      {
        if(midiBuffer[1] == MIDI_MODULATION_WHEEl)
        {
          volume = midiBuffer[2];
          MIDI_TX(MIDI_CONTROL,KORG_TOUCH_PAD_Y_AXIS,volume,KORG_MIDI_CHANNEL);
        }
      }
      //*
      Serial.print("RX - ");
      for(int i =0; i < 3; i++)
      {
        Serial.print(midiBuffer[i],HEX);
        Serial.print(",");
      }
      Serial.print("\n");
      //*
    }
  }
  //*/
}
