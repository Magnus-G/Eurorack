int previousPatternPlace;

const int noOfPatterns = 9; // plus one here if a pattern is added to "pattern"
const int patternLength = 16;
int patternValue;
int patternType;
int patternTypeUtility = 1023 / noOfPatterns;
int patternPlace = 0;
int pattern[noOfPatterns][patternLength] = {
  
  // | 5 A     || 6 A#     | 7 B      | 8 C     | 9 C#    |10 D    |11 D#    | 12    |
  // | 0(13) E || 1(14) F  | 2(15) F# | 3 G     | 4 G#    | 5 A    | 6 A#    | 7     |
  
  // 1,  2,  3,  4,  5,  6,  7,  8,    1,  2,  3,  4,  5,  6,  7,  8
    {0,  1,  2,  3,  4,  5,  6,  7,    8,  9,  10, 11, 12, 13, 14, 15},  // 0
    {0,  7,  10, 7,  9,  5,  7,  3,    2,  9,  12, 9,  11,  7,  9,  5},  // 1
    {0,  1,  0,  2,  0,  3,  0,  4,    0,  1,  0,  2,  0,  3,  0,  4},   // 2
    {0,  2,  3,  3,  7,  3,  5,  5,    2,  4,  5,  5,  9,  5,  7,  7},   // 3
    {0,  8,  7,  5,  3,  5,  7, 10,    0,  8,  7,  5,  3,  5,  7, 10},   // 4
    {0,  10, 0,  8,  0,  7,  3,  5,    2,  12, 2,  10,  2,  9,  5,  7},   // 5
    {0,  7,  3,  0,  9,  5,  9,  10,   0,  7,  3,  0,  9,  5,  9, 10},   // 6
    
    {0,  0,  1,  0,  0,  2,  0,  3,    0,  0,  3,  0,  0,  2,  0,  1},   // 7 
    {0,  0,  1,  0,  0,  6,  0,  3,    0,  0,  6,  0,  0,  3,  0,  1},   // 8 
};

int c     = 1;      int c2     = 13;     int c3     = 25;     int c4     = 37;    int c5     = 49;
int ciss  = 2;      int ciss2  = 14;     int ciss3  = 26;     int ciss4  = 38;    int ciss5  = 50;
int d     = 3;      int d2     = 15;     int d3     = 27;     int d4     = 39;    int d5     = 51;
int diss  = 4;      int diss2  = 16;     int diss3  = 28;     int diss4  = 40;    int diss5  = 52;
int e     = 5;      int e2     = 17;     int e3     = 29;     int e4     = 41;    int e5     = 53;
int f     = 6;      int f2     = 18;     int f3     = 30;     int f4     = 42;    int f5     = 54;
int fiss  = 7;      int fiss2  = 19;     int fiss3  = 31;     int fiss4  = 43;    int fiss5  = 55;
int g     = 8;      int g2     = 20;     int g3     = 32;     int g4     = 44;    int g5     = 56;
int giss  = 9;      int giss2  = 21;     int giss3  = 33;     int giss4  = 45;    int giss5  = 57;
int a     = 10;     int a2     = 22;     int a3     = 34;     int a4     = 46;    int a5     = 58;
int aiss  = 11;     int aiss2  = 23;     int aiss3  = 35;     int aiss4  = 47;    int aiss5  = 59;
int b     = 12;     int b2     = 24;     int b3     = 36;     int b4     = 48;    int b5     = 60;

int chord;
int oldChord;
const int noOfchords = 16;
const int chordLength = 15;
int chordUtility = 1023 / noOfchords;
int chords[noOfchords][noOfplaces] = {
  
  // patternType steps through from top to bottom
  
  // 0,    1,     2,      3,     4,     5,      6,     7,     8,   
  
    {e,    giss,  b,      e2,    giss2, b2,     e3,    giss3, b3    },      // E 
    {f,    a,     c2,     aiss2, ciss3, f3,     aiss3, ciss3, f4    },      // F    
    {fiss, aiss,  ciss2,  fiss2, aiss2, ciss3,  fiss3, aiss3, ciss4 },      // Fiss 
    {g,    b,     d2,     g2,    b2,    d3,     g3,    b3,    d4    },      // G
    {giss, c2,    d2,     giss2, c3,    d3,     giss3, c4,    d4    },      // Giss
    {a,    ciss2, e2,     a2,    ciss3, e3,     a3,    ciss4, e4    },      // A
    {aiss, d2,    f2,     aiss2, d3,    f3,     aiss3, d4,    f4    },      // Aiss
    {b,    diss2, fiss2,  b2,    diss3, fiss3,  b3,    diss4, fiss4 },      // B
    {c2,    e,     g,      c2,    e2,    g2,     c3,    e3,    g3    },     // C
    {ciss2, f,     giss,   ciss2, f2,    giss2,  ciss3, f3,    giss3 },     // Ciss
    {d2,    fiss,  a,      d2,    fiss2, a2,     d3,    fiss3, a3    },     // D
    {diss2, g,     aiss,   diss2, g2,    aiss2,  diss3, g3,    aiss3 },     // Diss
    
    {e2,    giss2, b2,     e3,    giss3, b3,     e4,    giss4, b4    },     // E 
    {f2,    a2,     c3,     aiss3, ciss4, f4,     aiss4, ciss4, f5    },    // F    
    {fiss2, aiss2,  ciss3,  fiss3, aiss3, ciss4,  fiss4, aiss4, ciss5 },    // Fiss 
    {g2,    b2,     d3,     g3,    b3,    d4,     g4,    b4,    d5    },    // G
};