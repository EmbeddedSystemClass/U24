
#include "stdafx.h"

#include "Util.h"
int PreDefMacroToDigitString(char* InString,char* OutString,int PreDefMacroType)
{
    int InLen = 0;
    int SpaceIndex[2] ;
	int SpaceCount = 0;
    int i=0;
	int j=0;
	char InBuffer[64];
    memset(InBuffer,0,sizeof(InBuffer));
	InLen =  strlen(InString);
    strncpy(InBuffer,InString,InLen);

    if (PreDefMacroType == PREDEF_DATE )
    {
        
		for( i=0;i<InLen;i++)
		{
            if(InBuffer[i] == ' ')
			{
				if(InBuffer[i-1] != 0)//日只有一位時會有2個空格
				{
                  // char bb =  InBuffer[i];
					SpaceIndex[SpaceCount] = i;
                   SpaceCount++;
				   InBuffer[i] ='\0';
                }else{
                   InBuffer[i] ='0';//填0
				}


			}
		}

        //利用index 取出 year
        for(i=SpaceIndex[1]+1;i<InLen;i++)
		{
            OutString[j] = InBuffer[i]; 
			j++;
		}

		//
		if(strcmp("Jan" ,InBuffer ) ==0)
		{
            strcpy(OutString+j,"01");
			
			 
		}else if(strcmp("Feb" ,InBuffer ) ==0)
		{
            strcpy(OutString+j,"02");
		}else if(strcmp("Mar" ,InBuffer ) ==0)
		{
             strcpy(OutString+j,"03");
		}else if(strcmp("Apr" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"04");
		}else if(strcmp("May" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"05");
		}else if(strcmp("Jan" ,InBuffer ) ==0)
		{ 
			 strcpy(OutString+j,"06");
		}else if(strcmp("Jul" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"07");
		}else if(strcmp("Aug" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"08");
		}else if(strcmp("Sep" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"09");
		}else if(strcmp("Oct" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"10");
		}else if(strcmp("Nov" ,InBuffer ) ==0)
		{
			 strcpy(OutString+j,"11");
		}else if(strcmp("Dec" ,InBuffer ) ==0)
		{
              strcpy(OutString+j,"12");
		}
        j+=2;

		//day
		for(i=SpaceIndex[0]+1;i<SpaceIndex[1];i++)
		{
            OutString[j] = InBuffer[i]; 
			j++;
		} 
	}

    if (PreDefMacroType == PREDEF_TIME )
    {
       
		for(i=0;i<InLen;i++)
		{
            if(InBuffer[i] != ':')
			{
               OutString[j] = InBuffer[i]; 
			   j++;
			}
            
		}		
	
    }
    return 1;
}