#include "MIDIUSB.h"

  /* JoyStick Midi Controller
   * 5 way joystick Note On/Off trigger/s
   * Extra fire button trigger
   * 3 Valve buttons to transpose 
      all buttons up/ down by 6.
* 8 x 6 total note range from different button 
  combinations//   \\Base note = C1 (36)
*  For boards with ATmega32u4 chip
*  Written by Jim Foster
*  credits to Gordon Good (note selection implemented from his 
   note selection basics code// breath controller blog
   http://gordophone.blogspot.com/2016/02/note-selection-basics.html
===============================================
____________ JOYSTICK; ________________________
------------|~~~~~~~~~|------------------------
Pins/Push;  | up = 4; |
-----------/.......... \-----------------------
 <left = 5|  push = 2;  |right = 6;>
-----------\.........../-----------------------
____________|down = 3;|________________________
------------|=========|-|(7)|_(Extra button)---
____________|         |________________________
----------{Valve buttons}_|(12)|(11)|(10)|-----
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
|| all pulled to ground with 10kohm resistors 
===============================================
   TODO; up, down, left, right = triggers|| stick button = +1 || extra button = +2 }
 *get new active note from valve changes ( previous note off after new note starts for smooth legato? }
 *add potentiometer to change root note
 *add X/Y analog Joystick for pitchbend/ mod 
 *velocity sensitivity? breath sensor?
 *add kick drum trigger pedals
 */
//int joystick pins
int push = 2;
int up = 4;
int right = 6;
int down = 3;
int left = 5;
int extraButton = 7;

// Array Variables
const int buttonCount = 6;
const int baseCount = 8;
int buttonDown[buttonCount];
int switchCase;

int buttonPins[buttonCount] = {           //Pin order corresponds with Note order
  up, right, down, left, push, extraButton
};
int base[baseCount] = {
  36, 42, 48, 54, 60, 66, 72, 78            //Base note for each case
};                                          //6 Notes per switch case
int isButtonDown(int pin) {
  return digitalRead(pin) == 0;
}
int get_switch_state() {
  // This routine reads the three buttons that are our "trumpet
  // valves", and returns a MIDI note.
  int value = (digitalRead(12) << 2) +   //
              (digitalRead(11) << 1) +   //
              digitalRead(10);           //
  // Full table:
  // v1     v2     v3      value (decimal)
  //Open,   Open,   Open   = 1 | C1 + 6
  //Closed, Open,   Open   = 3 | F1 + 6
  //Closed, Closed, Open   = 0 | C2 + 6
  //Open,   Closed, Open   = 2 | F2 + 6
  //Open,   Closed, Closed = 6 | C3 + 6
  //Open,   Open,   Closed = 4 | F3 + 6
  //Open,   Closed, Closed = 5 | C4 + 6
  //Closed, Closed, Closed = 7 } F4 + 6
  //
#define BASE 7  // The base MIDI note value
  switch (value) {
    case 7:
      return BASE;
      break;
    case 5:
      return BASE - 1;
      break;
    case 3:
      return BASE - 2;
      break;
    case 1:
      return BASE - 3;
      break;
    case 4:
      return BASE - 4;
      break;
    case 2:
      return BASE - 5;
      break;
    case 0:
      return BASE - 6;
      break;
    case 6:
      return BASE - 7;
      break;

    default:
      // If invalid fingering, ignore and return the
      // currently sounding note.
      return switchCase;
  }
}

void setup() {
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);

  for (int i = 0; i < buttonCount; ++i) {
    pinMode(buttonPins[i], INPUT);
    digitalWrite(buttonPins[i], HIGH);
    buttonDown[i] = isButtonDown(buttonPins[i]);

  }
}
void loop() {
int vel = 127;
  int switchCase = get_switch_state();
  switch (switchCase) {
    case 1: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, 36 + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, 42 + i, 0);
            MidiUSB.flush();
            noteOff(0, 48 + i, 0);
            MidiUSB.flush();
            noteOff(0, 54 + i, 0);
            MidiUSB.flush();
            noteOff(0, 60 + i, 0);
            MidiUSB.flush();
            noteOff(0, 66 + i, 0);
            MidiUSB.flush();
            noteOff(0, 72 + i, 0);
            MidiUSB.flush();
            noteOff(0, 78 + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 3: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[1] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 0: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[2] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 2: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[3] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 5: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[4] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 4: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[5] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 6: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            delay(5);
            noteOn(0, base[6] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[7] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
    case 7: {
        for (int i = 0; i < buttonCount; ++i) {
          int down = isButtonDown(buttonPins[i]);

          if (down != buttonDown[i]) {
            noteOn(0, base[7] + i, down ? 0 : vel);
            delay(5);
            MidiUSB.flush();
            noteOff(0, base[0] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[1] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[2] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[3] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[4] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[5] + i, 0);
            MidiUSB.flush();
            noteOff(0, base[6] + i, 0);
            MidiUSB.flush();
            delay(5);
            buttonDown[i] = down;
            MidiUSB.flush();
          }
        }
      }
      break;
  }
}
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
void afterTouch(byte channel, byte pitch, byte value) {
  midiEventPacket_t event = {0x0A , 0xA0 | channel, pitch, value};
  MidiUSB.sendMIDI(event);
}
void channelPressure(byte channel, byte pressure) {
  midiEventPacket_t event = {0x0D, 0xD0 | channel, pressure};
  MidiUSB.sendMIDI(event);
}
void programChange(byte channel, byte instrument) {
  midiEventPacket_t event = {0x0C, 0xC0 | channel, instrument};
  MidiUSB.sendMIDI(event);
}
void pitchBendChange(byte channel, int value) {
  byte lowValue = value & 0x7F;
  byte highValue = value >> 7;
  midiEventPacket_t event = {0x0E, 0xE0 | channel, lowValue, highValue};
  MidiUSB.sendMIDI(event);
  //(0-16383). 0x2000 (8192) is the default / middle value.
}
