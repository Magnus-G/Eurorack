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

      clockInTime = millis();
      timeBetweenClockIns = clockInTime - previousClockInTime;
      halfwayThroughBetweenClockIns = timeBetweenClockIns / 2;
      extraTriggerHasBeenUsed = 0;

      int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.s

      if (drumPatternList[pattern][row][column] == 1) {
        drumPatternListBuffer[pattern][row][column] = drumPatternList[pattern][row][column];
      }

      int randValueSubtractions = random(0, 1023);
      int randValueAdditions = random(0, 1023);

      if (drumPatternListBuffer[pattern][row][column] == 0) { // send a trigger anyway although there is no trigger in the drumPattern at this place
        if (randValueAdditions < analogRead(2)) {   
          digitalWrite(pinOffset + row, LOW);  
          digitalWrite(pinOffset + row, HIGH);  
          digitalWrite(pinOffset + row, LOW);
          drumPatternListBuffer[pattern][row][column] = 0;
        }
      }
      
      delay(1);

      // trigOrNot[row] = drumPatternList[pattern][row][column]; // give trigOrNot a 1 or 0 depending on trigger strike or not
      
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



  int pattern = (analogRead(0) / (1023/noOfPatterns)); if (pattern > 0) {  pattern--; } // deal with zero indexing on addressing the array vs the integer declared to set the number.s

  for (int row=0; row<noOfRows; row++) {

    if (drumPatternListBuffer[pattern][row][column] == 1) {
      digitalWrite(pinOffset + row, drumPatternListBuffer[pattern][row][column]);  
      digitalWrite(pinOffset + row, 0);  
      drumPatternListBuffer[pattern][row][column] = 0;
    }

    int randValueInbetweenTrigger = random(0, 1023);
    if (randValueInbetweenTrigger < (analogRead(1) / 2)) {

      // Serial.println(millis());    
      // Serial.println(clockInTime + halfwayThroughBetweenClockIns);    
      // Serial.println();    

      if ((millis() > (clockInTime + halfwayThroughBetweenClockIns)) && (extraTriggerHasBeenUsed == 0)) {
        // Serial.println("inne");
        // Serial.println();    

        digitalWrite(pinOffset + row, 1);  
        digitalWrite(pinOffset + row, 0);  
        drumPatternListBuffer[pattern][row][column] = 0;

        extraTriggerHasBeenUsed = 1;
      }
    }
  }


  
    
  //     digitalWrite(pinOffset + row, 0);  
  //     drumPatternListBuffer[pattern][row][column] = 0;




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
