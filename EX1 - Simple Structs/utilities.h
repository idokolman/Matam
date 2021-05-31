#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdbool.h>
#include <stdio.h>

/**
* intToStr: Allocates a new string which is a copy of the number received
* @param num -
* The number we need to copy into the new string
* @return
* NULL if the memory allocation failed
* A new char* which is a copy of num in case of success
*/
char* intToStr (int num);

/**
* strToInt: Creates a new int which value's is equal to the string received
* @param str -
* The string we need to copy into the new number
* @return
* EOF if the memory allocation failed or if NULL was received
* A new int which is a copy of str in case of success
*/
int strToInt (char* str);

/**
* duplicateData: Allocates a new string and copies str content to it
*
* @param str - string to copy
* @return
* 	NULL - if the allocation of string failed.
* 	A new coppied string in case of success.
*/
char* duplicateData(const char* str);

/**
 * VALIDATE_ARGUMENT: Macro to check if input is NULL
 * If input is NULL, returns the return_value.
 */
#define VALIDATE_ARGUMENT(input,return_value)\
if (!(input))\
{\
	return (return_value);\
}

/**
 * FREE_TWO_STRINGS: Macro to free two elements, strings in this program
 */
#define FREE_TWO_STRINGS(str1, str2)\
{\
    free (str1);\
    free (str2);\
}

#endif /* UTILITIES_H_ */
