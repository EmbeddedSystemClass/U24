#define PREDEF_DATE 1
#define PREDEF_FILE 2
#define PREDEF_LINE 3
#define PREDEF_STDC 4
#define PREDEF_TIME 5
#define PREDEF_TIMESTAMP 6 //Mmm dd yyyy
int PreDefMacroToDigitString(char* InString,char* OutString,int PreDefMacroType);