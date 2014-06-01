#include "variables.h";
#include "notes_and_patterns.h";

////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////

void loop() {

if (clkState == HIGH) {
  clkState = LOW;
        
                                      // // UNCOMMENT THIS FOR USE OF PAUSE SETTING   
                                      // // Set the length of pause between end of pattern to
                                      // // beginning of next pattern.
                                      // // pauseCounterEndValue = (analogRead(1) / (1023/pauseCounterNoOfSteps)); if (pauseCounterEndValue > 0) {  pauseCounterEndValue--; } 
                                      // if (pauseCounter > pauseCounterEndValue) {
                                      //   pauseCounter = pauseCounterEndValue;
                                      // }
                                      // if (pauseCounter == pauseCounterEndValue) {
    
      // patternType sets the type of pattern to be played in 
      // the chord matrix. Patterns "jump" up and down through 
      // the current chord matrix column. Number shows how far 
      // from the top it is.

      patternType = (analogRead(0) / (1023/noOfPatterns)); if (patternType > 0) {  chord--; } 
      // between 0 and 16 steps that the sequence will be able 
      //to play through. A1 sets the allowed lenght of the played pattern
      patternPlayLength = (analogRead(3) / (1023/patternLength)); if (patternPlayLength > 0) {  patternPlayLength--; } 

      // // A2 is used to set a transposition number for the output note. 
      // // It selects its transposition value in an array of 
      // // "good sounding" steps.
      //  transposeValue =       (analogRead(2) / (1023/noOfTranspositions)); if (transposeValue > 0) {  transposeValue--; }  
 
      // 9 steps x-axis. place sets the column to be played in 
      // the chord matrix. different versions of the same chord.
      place = (analogRead(2) / (1023/noOfplaces)); if (place > 0) {  place--; } 

      int randValuePatternJumpYesOrNo = random(0, 1023);
      if (randValuePatternJumpYesOrNo < analogRead(1)) {   
        int randValuePatternJumpTarget = random(0, patternLength);
        patternPlace = randValuePatternJumpTarget;
      }

      // Gate high for D0 on each output signal
      digitalWrite(digPin[0], HIGH); digitalWrite(digPin[0], LOW);
    
      // patternValue is the current Y-axis place in the chord matrix. 
      // patternPlace advances by 1 for each loop.
      patternValue = pattern[patternType][patternPlace];    
      transpose = transpositions[transposeValue];
  
      //  Serial.print("patternType"); Serial.println(patternType);
    
      // play
      int note = (((chords[patternValue + transpose][place]) * 4) + (12)); //    int note = (((chords[patternValue][place]) * 4) + (12 * 4));
      dacOutput(note);

      // gate high for D1 only if the note changes from last time
      if (previousNote != note) {
        digitalWrite(digPin[1], HIGH);   
        digitalWrite(digPin[1], LOW);
      }
    
      previousNote = note;

      // advance one step in pattern
      patternPlace++;

      // when the pattern has reached its final note in the 
      //pattern series, determined by A1 (setting number of 
      // steps in the sequence)
      if (patternPlace >= patternPlayLength+1 ) {
        patternPlace = 0;

                                        // // UNCOMMENT THIS FOR USE OF PAUSE SETTING   
                                        // pauseCounter = 0;

      } // pattern has reached its final note

                                        // // UNCOMMENT THIS FOR USE OF PAUSE SETTING   
                                        // } // pauseCounter if statement ends
                                        // else {
                                        //   pauseCounter++;
                                        // }

  } // clock high ends  
} // loop








void isr()
{
  clkState = HIGH;
}

void writeStep(boolean on){
    if(on){
        digitalWrite(digPin[0], HIGH);
        digitalWrite(digPin[1], LOW);
    }else{
        digitalWrite(digPin[0], LOW);
        digitalWrite(digPin[1], HIGH);
    }  
}

//  dacOutput(long) - deal with the DAC output
//  ------------------------------------------
void dacOutput(long v)
{
  // feed this routine a value between 0 and 255 and teh DAC
  // output will send it out.
  int tmpVal = v;
  for (int i=0; i<8; i++) {
    digitalWrite(pinOffset + i, tmpVal & 1);
    tmpVal = tmpVal >> 1;
  }
}

