const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3, 4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)
volatile int clkState = LOW;
int digState[2] = {LOW, LOW};        // start with both set low
unsigned long digMilli[2] = {0, 0};  // a place to store millis()

// for the on/off switch on clock in
int inputIsHigh = 0;

int probabilitySwitch = 0;

// for the calculation of distances between "clock highs"
long time;
long halfTime;
long oldTime = 0;

int currentchordValue = 0;
int previousNote = 98;

int place;
const int noOfplaces = 9;


//////////////////////////////

const int noOfTranspositions = 3;
int transpositions[noOfTranspositions] = {0, 2, 3};
int transposeValue;
int transpose = 0;

//////////////////////////////

int pauseCounterNoOfSteps = 16;
int pauseCounterEndValue = 0; // the value user sets for when pause is over.
int pauseCounter = 0;

//////////////////////////////

int patternPlayLength = 16;

//////////////////////////////

