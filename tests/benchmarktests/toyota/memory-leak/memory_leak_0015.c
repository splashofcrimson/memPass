#include "HeaderFile.h"
void free(void* n){
return;
}

/*
 * Types of defects: Memory Leakage - Allocate Memory and not freeing it
 *  Memory allocated and returned from one function and Memory used in another function
 */
char * memory_leak_0015_func_001 (int len)
{
	char *stringPtr = malloc(sizeof(char) * (len+1));
	return stringPtr;
}

void memory_leak_0015 ()
{
	char *str = "This is a string";
	char *str1 = memory_leak_0015_func_001(strlen(str)); /*Tool should detect this line as error*/ /*ERROR:Memory Leakage */
	if(str1!=NULL)
	{
	    strcpy(str1,str);
	}
}



int main () {
	
	
	memory_leak_0015();

	
};
