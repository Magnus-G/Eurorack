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
  if (clkState == HIGH) {
    for (int row=0; row<noOfRows; row++) {
      clkState = LOW;  // reset for the next clock

      int randValueSubtractions = random(0, 1023);
      int randValueAdditions = random(0, 1023);

      int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.

      if (drumPatternList[pattern][row][column] == 1) {
        if (randValueSubtractions > analogRead(3)) {   
          digitalWrite(pinOffset + row, drumPatternList[pattern][row][column]);  
        }
      }

      if (drumPatternList[pattern][row][column] == 0) {
        if (randValueAdditions < analogRead(2)) {   
          digitalWrite(pinOffset + row, LOW);  
          digitalWrite(pinOffset + row, HIGH);  
          digitalWrite(pinOffset + row, LOW);
        }
      }
      
      // delay(1);

      trigOrNot[row] = drumPatternList[pattern][row][column]; // give trigOrNot a 1 or 0 depending on trigger strike or not
      
      if (trigOrNot[row] == 1) { // if this is a trigger
        if (nextStateForTrig[row] == 1) { // if the indicator variable shows 1
          if (randValueSubtractions > analogRead(3)) {  
            digitalWrite(pinOffset + row, LOW);  // turn gate off
            digitalWrite(pinOffset + row, HIGH);  // turn gate on 
            digitalWrite(pinOffset + row, LOW);  // turn gate off
          }
          nextStateForTrig[row] = 0; // indicator value set to 0 to indicate that the last trig turned gate off... so next one should keep it on and not go through this loop
        }
        else {
          nextStateForTrig[row] = 1; // so that next time there will be a turning off 
        }
      }
    } // gone through a whole column
    
    column++;

    if (column >= noOfColumns) {
      column = 0;
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
//test