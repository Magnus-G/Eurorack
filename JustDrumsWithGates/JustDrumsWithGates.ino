
// Clock and drum pattern sketches for the Ardcore Expander
// Feed a clock in "Clock in" for external beat. Set A1 to more than 0 for internal beat.
// The clock in toggles between D0 and D1 to open and close.
// Programs are set with A0. 1, 2, 3... etc from CCW to CW
// 1. Cycle Program 
// 2. Halves Program
// 3. Divider Program
// 4. Drum Program      Use A3 to set different drum patterns.
// A2 controls probablility of output for triggers. Set A2 full CCW for no randomization and turn it CW for increased "drop out" of trig outs. 
// There is a 1 second signal as the program is changed.
// Written by: Magnus Gyllensward

// the drumPatterns.h file must be in the same folder as this file when compiling.
#include "drumPatterns.h";

const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3,4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)
const int trigTime = 10;       // triggers are 10 ms.

//  variables for interrupt handling of the clock input
volatile int clkState = LOW;
int loops = 0;

// randomization/probability
int randValue = 1023;

// for the on/off switch on clock in
int inputIsHigh = 0;
int digState[2] = {LOW, LOW};  // start with both set low

// counting drumloops. see drums.h for all drum patterns
int drumLoops = 0;
int drumDivision[6] = {1, 2, 4, 8, 16, 32};    

int anslag[noOfDrumSteps] = {1,1,1,1,1,1,1};
int anslagEveryOther[noOfDrumSteps] = {1,1,1,1,1,1,1};

int sixth = 0;
int seventh = 0;
int eighth = 0;

//  ==================== start of setup() ======================

void setup() {
  Serial.begin(9600);
  
  // set up the digital (clock) input
  pinMode(clkIn, INPUT);
  
  // set up the digital outputs
  for (int i=0; i<2; i++) {
    pinMode(digPin[i], OUTPUT);
    digitalWrite(digPin[i], LOW);
  }
  
  // set up the 8-bit DAC output pins
  for (int i=0; i<8; i++) {
    pinMode(pinOffset+i, OUTPUT);
    digitalWrite(pinOffset+i, LOW);
  }
  
  attachInterrupt(0, isr, RISING);
}

//  ==================== start of loop() =======================
void loop() {  
            
      // go through drum matrix
      for (int x=1; x<17; x++) { // temporal. start with first beat point...
        
        // set melodyPatternType
       // int melodyPatternType = (analogRead(0) / (1023/noOfMelodyPatterns)); if (melodyPatternType > 0) {  melodyPatternType--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.
        
        ////////////////////////////////////////////
        // MELODY PATTERN out on 8. Controlled by A0
        // write 0 or 1 on output to trigger melody pattern, set independently of the current drumpattern.
//        digitalWrite(pinOffset+(7), melodyPattern[melodyPatternType][x-1]);
//        delay(1);
//        digitalWrite(pinOffset+(7), LOW);  
      
        for (int y=1; y<noOfDrumSteps; y++) { // vertical, outputs. start with output 0...
          randValue = random(0, 1023);
          if (randValue > analogRead(2)) {     
            
            // set drumProgram
            int drumProgram = (analogRead(3) / (1023/noOfDrumPrograms)); if (drumProgram > 0) {  drumProgram--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.
            
            // the hit
            if (drums[drumProgram][y-1][x-1] == 1) {
              digitalWrite(pinOffset+(y-1), drums[drumProgram][y-1][x-1]);
            }
            delay(1);
            
            anslag[y-1] = drums[drumProgram][y-1][x-1]; // give anslag a 1 or 0 depending on hit or not
            
            if (anslag[y-1] == 1) { // if this is a hit
              if (anslagEveryOther[y-1] == 1) { // if the indicator variable shows 1
                digitalWrite(pinOffset+(y-1), LOW);  // turn gate off
                digitalWrite(pinOffset+(y-1), HIGH);  // turn gate on 
                digitalWrite(pinOffset+(y-1), LOW);  // turn gate off
                anslagEveryOther[y-1] = 0; // indicator value set to 0 to indicate that the last hit turned gate off... so next one should keep it on and not go through this loop
              }
              else {
                anslagEveryOther[y-1] = 1; // so that next time there will be a turning off 
              }
            }
            
            
            

//            Serial.print("anslag[y-1]: ");
//            Serial.println(anslag[y-1]);            
          }          
        }
        delay(1023 - analogRead(1)); // delay after each beat == bpm
      }
      
  // extra hits, outside of the drum program
  drumLoops++;
    


  // controls the on/off for the sixth output

  if (drumLoops % 2 == 0) {
    digitalWrite(pinOffset+(5), HIGH);
    delay(1);
    
    if (sixth == 1) {
      digitalWrite(pinOffset+(5), LOW);
      sixth = 0;
    }
    else {
      sixth = 1;
    }
  }
  
  // controls the on/off for the seventh output

  if (drumLoops % 4 == 0) {
    digitalWrite(pinOffset+(6), HIGH);
    delay(1);
    
    if (seventh == 1) {
      digitalWrite(pinOffset+(6), LOW);
      seventh = 0;
    }
    else {
      seventh = 1;
    }
  }  
  
  // controls the on/off for the eighth output

  if (drumLoops % 5 == 0) {
    digitalWrite(pinOffset+(7), HIGH);
    delay(1);
    
    if (eighth == 1) {
      digitalWrite(pinOffset+(7), LOW);
      eighth = 0;
    }
    else {
      eighth = 1;
    }
  }  

  
  
  
  
  
  
  
  
} // end loop

//  =================== convenience routines ===================
//  isr() - quickly handle interrupts from the clock input
//  ------------------------------------------------------
void isr()
{
  // Note: you don't want to spend a lot of time here, because
  // it interrupts the activity of the rest of your program.
  // In most cases, you just want to set a variable and get
  // out.
  clkState = HIGH;
}

//  dacOutput(byte) - deal with the DAC output
//  -----------------------------------------
void dacOutput(byte v)
{
  PORTB = (PORTB & B11100000) | (v >> 3);
	PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);
}

//  ===================== end of program =======================
