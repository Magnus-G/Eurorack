const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3,4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)

//  variables for interrupt handling of the clock input
volatile int clkState = LOW;

// for the on/off switch on clock in
int inputIsHigh = 0;
int digState[2] = {LOW, LOW};  // start with both set low

//////////////////////////////////////////// drumPatterns

int column = 0;

int const noOfPatterns = 2;
int const noOfRows = 5;
int const noOfColumns = 16;
int drumPatternList[noOfPatterns][noOfRows][noOfColumns] = 
{
{
	{1, 0, 0, 0, 0, 0, 0, 0,  1, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 0, 0, 0,  0, 0, 0, 0, 1, 0, 0, 0},
	{1, 0, 1, 0, 1, 0, 1, 0,  1, 0, 1, 0, 1, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
},
{
	{1, 0, 0, 0, 1, 0, 0, 1,  0, 1, 0, 0, 1, 0, 0, 0},
	{0, 0, 1, 0, 0, 1, 0, 0,  0, 0, 1, 0, 0, 0, 1, 0},
	{0, 0, 0, 1, 0, 0, 0, 0,  0, 0, 0, 1, 0, 1, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
}
};

//////////////////////////////////////////// strikes

int trigOrNot[noOfRows] = {1,1,1,1};
int nextStateForTrig[noOfRows] = {1,1,1,1}; 


	// {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},
	// {0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0},