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

      // Set random numbers for potential additions/subtractions
      int randValueSubtractions = random(0, 1023);
      int randValueAdditions = random(0, 1023);

      // Assign a trigger anyway although there is no trigger 
      // in the drumPattern at this place
      if (drumPatternListBuffer[pattern][row][column] == 0) { 
        if (randValueAdditions < analogRead(2)) {   
          drumPatternListBuffer[pattern][row][column] = 1;        
        }
      }
      
      trigOrNot[row] = drumPatternListBuffer[pattern][row][column]; // give trigOrNot a 1 or 0 depending on trigger strike or not
      
      // if (trigOrNot[row] == 1) { // if this is a trigger
      //   if (nextStateForTrig[row] == 1) { // if the indicator variable shows 1
      //     if (randValueSubtractions > analogRead(3)) {  
      //       digitalWrite(pinOffset + row, LOW);  // turn gate off
      //       digitalWrite(pinOffset + row, HIGH);  // turn gate on 
      //       digitalWrite(pinOffset + row, LOW);  // turn gate off
      //     }
      //     nextStateForTrig[row] = 0; // indicator value set to 0 to indicate that the last trig turned gate off... so next one should keep it on and not go through this loop
      //   }
      //   else {
      //     nextStateForTrig[row] = 1; // so that next time there will be a turning off 
      //   }
      // }
    } // gone through a whole column

    previousClockInTime = clockInTime;



    column++;

    if (column >= noOfColumns) {
      column = 0;
    }
  } // end clock high


  // is this needed? set pattern again?
  int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.s

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
      digitalWrite(pinOffset + row, 1);  
      digitalWrite(pinOffset + row, 0);  
      drumPatternListBuffer[pattern][row][column] = 0;
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
