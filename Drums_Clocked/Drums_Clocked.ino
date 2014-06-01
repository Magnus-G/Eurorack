#include "variables.h";
#include "drumPatternList.h";

//////////////////////////////////////////// setup

void setup() {
  Serial.begin(9600);
  
  pinMode(clkIn, INPUT);
  
  for (int i=0; i<2; i++) {
    pinMode(digPin[i], OUTPUT);
    digitalWrite(digPin[i], LOW);
  }
  
  for (int i=0; i<8; i++) {
    pinMode(pinOffset+i, OUTPUT);
    digitalWrite(pinOffset+i, LOW);
  }
  
  attachInterrupt(0, isr, RISING);
}

//////////////////////////////////////////// loop

void loop() {    

  // Clock state high encompasses the preparation for the triggers. 
  // Actual triggers happen after the "clock high"
  if (clkState == HIGH) {
    for (int row=0; row<noOfRows; row++) {
      clkState = LOW;  // reset for the next clock

      // For the InbetweenTrigger
      clockInTime = millis();
      timeBetweenClockIns = clockInTime - previousClockInTime;
      halfwayThroughBetweenClockIns = timeBetweenClockIns / 2;
      extraTriggerHasBeenUsed = 0;

      // Set pattern type
      // deal with zero indexing on addressing the array 
      // vs the integer declared to set the number.
      int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } 

      // Assign pattern state to buffer pattern to triger after clock high. 
      if (drumPatternList[pattern][row][column] == 1) {
        drumPatternListBuffer[pattern][row][column] = drumPatternList[pattern][row][column];
      }

      // Set random numbers for potential additions
      int randValueAdditions = random(0, 1023);

      // Assign a trigger anyway although there is no trigger 
      // in the drumPattern at this place
      if (drumPatternListBuffer[pattern][row][column] == 0) { 
        if (randValueAdditions < analogRead(2)) {   
          drumPatternListBuffer[pattern][row][column] = 1;
        }
      }

      // Set random numbers for potential subtractions
      int randValueSubtractions = random(0, 1023);
      if (drumPatternListBuffer[pattern][row][column] == 1) { 
        if (randValueAdditions < analogRead(3)) {   
          drumPatternListBuffer[pattern][row][column] = 0;
        }
      }
    } // gone through a whole column

    previousClockInTime = clockInTime;

    column++;

    if (column >= noOfColumns) {
      column = 0;
    }

    drumLoops++;

    for (int output=5; output<8; output++) {
      if (drumLoops % outputNumber[output] == 0) {
        digitalWrite(pinOffset+(output), HIGH);
        // delay(1);
        
        if (gateOpenOnThisRow[output] == 1) {
          digitalWrite(pinOffset+(output), LOW);
          gateOpenOnThisRow[output] = 0;
        }
        else {
          gateOpenOnThisRow[output] = 1;
        }
      }
    }
  } // end clock high

  ///////////////////////////////////////////////////////////////

  // Set pattern type
    // deal with zero indexing on addressing the array 
    // vs the integer declared to set the number.
    int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } 

  for (int row=0; row<noOfRows; row++) {

    // in that for loop, ask if the current row is ON or OFF. 
    // if ON then close gate. if OFF then open gate
    if (drumPatternListBuffer[pattern][row][column] == 1) {
      trigger = 1;
    }

    // Will there be an extra trigger between the two set triggers?
    int randValueInbetweenTrigger = random(0, 1023);
    if (randValueInbetweenTrigger < (analogRead(1) / 2)) {

      if ((millis() > (clockInTime + halfwayThroughBetweenClockIns)) && (extraTriggerHasBeenUsed == 0)) {
        trigger = 1;
        extraTriggerHasBeenUsed = 1;
      }
    }

    // The actual sending of trigger
    if (trigger == 1) {
      // if gate is open, turn it off quickly to generate a trigger.
      digitalWrite(pinOffset + row, 0);  
      digitalWrite(pinOffset + row, 1);  

      // if gate is open on this row, close gate.
      if (gateOpenOnThisRow[row] == 1) {
        digitalWrite(pinOffset + row, 0);
        gateOpenOnThisRow[row] = 0;
      }

      // if gate is closed on this row, keep gate open from trigger above.
      else {
        gateOpenOnThisRow[row] = 1;
      }

      // reset buffer variable
      drumPatternListBuffer[pattern][row][column] = 0;
      // reset trigger variable
      trigger = 0;
    }
  }
} // end loop






//////////////////////////////////////////// convenience routines

void isr() {
  clkState = HIGH;
}

void dacOutput(byte v) {
  PORTB = (PORTB & B11100000) | (v >> 3);
	PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);
}

      // Serial.print("row: ");
      // Serial.println(row);
      // Serial.println("trigOrNot[row][column]: ");
      // Serial.print(row      ); Serial.println(column);
      // Serial.println(trigOrNot[row][column]);
