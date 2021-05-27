#include "utilities.h"
#include <stdlib.h>
#include <string.h>

/**
* countDigits: Counts how many digits there are in a number.
* @param num -
* The number whose digits we need to count
* @return
* Integer who is a counter of num's digits
*/
static int countDigits(int num)
{
    int count=0;
    if (num<0) 
    {
        num=-num;
        count++;
    }
    if (num==0)    {
        return 1;
    }
    while (num!=0)
    {
        num/=10;
        count++;
    }
    return count;
}

char* intToStr (int num)
{
    char* new_string=malloc(sizeof(*new_string)*(countDigits(num))+1);
    VALIDATE_ARGUMENT(new_string,NULL);
    if (sprintf(new_string,"%d",num)>=0) //checks the return value of sprintf
    {
        return (new_string);
    }
    else
    {
        free(new_string);
        return NULL;
    }
}

int strToInt (char* str)
{
    VALIDATE_ARGUMENT(str,EOF);
    int new_num;
    if (sscanf(str,"%d",&new_num)==EOF){ //return value of failed sscanf is EOF(=-1)
        return EOF;
    }
    return new_num;
}

char* duplicateData(const char* str)
{
    VALIDATE_ARGUMENT(str,NULL);
	char* str_copy = malloc(strlen(str) + 1);
	return str_copy ? strcpy(str_copy, str) : NULL;
}