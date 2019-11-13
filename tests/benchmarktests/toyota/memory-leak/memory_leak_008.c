#include "HeaderFile.h"
void free(void* n){
return;
}


/*
 * Types of defects: Memory Leakage - Allocate Memory and not freeing it
*  When using 1 single pointer alias
*/
void memory_leak_008 ()
{
	int *ptr=(int*) malloc(5 * sizeof(int));
	int *p = (int*)malloc(5 * sizeof(int));/*Tool should detect this line as error*/ /*ERROR:Memory Leakage */
	if(ptr !=NULL)
	{
	p = ptr;
	*(p+1) = 1;
	free(ptr);
	}
}



int main () {
	
	
	memory_leak_008();

	
};
