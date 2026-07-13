
#define CHAR_ZERO   48
#define CHAR_ONE    49
#define CHAR_TWO    50
#define CHAR_THREE  51
#define CHAR_FOUR   52
#define CHAR_FIVE   53
#define CHAR_SIX    54
#define CHAR_SEVEN  55
#define CHAR_EIGHT  56
#define CHAR_NINE   57
#define CHAR_HASH   35
#define CHAR_STAR   42

#define ROW_697   14
#define ROW_770   15
#define ROW_852   17
#define ROW_941   19

#define COL_1209  24
#define COL_1336  27
#define COL_1477  30

#define WAVE_TABLE_SIZE 200

unsigned char waveTable[WAVE_TABLE_SIZE] = 
{ 
   64,  66,  68,  69,  71,  73,  75,  77,  79,  80,  82,  84,  86,  88,  89, 
   91,  93,  94,  96,  97,  99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 
  112, 113, 114, 115, 116, 117, 118, 119, 120, 120, 121, 121, 122, 122, 123, 
  123, 123, 123, 124, 124, 124, 124, 124, 123, 123, 123, 123, 122, 122, 121, 
  121, 120, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 
  106, 105, 103, 102, 101,  99,  97,  96,  94,  93,  91,  89,  88,  86,  84, 
   82,  80,  79,  77,  75,  73,  71,  69,  68,  66,  64,  62,  60,  58,  56, 
   54,  53,  51,  49,  47,  45,  43,  42,  40,  38,  37,  35,  33,  32,  30, 
   28,  27,  26,  24,  23,  21,  20,  19,  18,  16,  15,  14,  13,  12,  11, 
   10,   9,   9,   8,   7,   7,   6,   6,   5,   5,   4,   4,   4,   4,   4, 
   04,   4,   4,   4,   4,   4,   5,   5,   6,   6,   7,   7,   8,   9,   9, 
   10,  11,  12,  13,  14,  15,  16,  18,  19,  20,  21,  23,  24,  26,  27, 
   28,  30,  32,  33,  35,  37,  38,  40,  42,  43,  45,  47,  49,  51,  53, 
   54,  56,  58,  60,  62 };

int toneFromOffset(int offset)
{
    switch (offset)
    {
        case ROW_697:  return 697;
        case ROW_770:  return 770;
        case ROW_852:  return 852;
        case ROW_941:  return 941;
        case COL_1209: return 1209;
        case COL_1336: return 1336;
        case COL_1477: return 1477;
    }
}

void printTable(int rowOffset, int colOffset)
{
    int rowIndex = 0;
    int colIndex = 0;
    int i = 0;

    Serial.print("ROW_");
    Serial.println(toneFromOffset(rowOffset), DEC);
    do
    {
        if (i % 10 == 0)
            Serial.println();
        Serial.print(waveTable[rowIndex], DEC);
        Serial.print(", ");
        
        rowIndex += rowOffset;
        if (rowIndex >= WAVE_TABLE_SIZE)
            rowIndex -= WAVE_TABLE_SIZE;
        i++;
    } while (i < 100); //(waveTable[rowIndex] != 64);
    Serial.println("64");
    Serial.println();
    
    Serial.print("COL_");
    Serial.println(toneFromOffset(colOffset), DEC);
    i = 0;
    do
    {
        if (i % 10 == 0)
            Serial.println();
        Serial.print(waveTable[colIndex], DEC);
        Serial.print(", ");
        
        colIndex += colOffset;
        if (colIndex >= WAVE_TABLE_SIZE)
            colIndex -= WAVE_TABLE_SIZE;
        i++;
    } while (i < 100); //(waveTable[colIndex] != 64);
    Serial.println("64");
    Serial.println();

}

void setup() 
{
    Serial.begin(115200);

    printTable(ROW_697, COL_1209);
    printTable(ROW_697, COL_1336);
    printTable(ROW_697, COL_1477);
    
    printTable(ROW_770, COL_1209);
    printTable(ROW_770, COL_1336);
    printTable(ROW_770, COL_1477);
   
    printTable(ROW_852, COL_1209);
    printTable(ROW_852, COL_1336);
    printTable(ROW_852, COL_1477);
    
    printTable(ROW_941, COL_1209);
    printTable(ROW_941, COL_1336);
    printTable(ROW_941, COL_1477);
  
}

void loop() 
{

}
