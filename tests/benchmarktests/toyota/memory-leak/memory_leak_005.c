#include "HeaderFile.h"
void free(void* n){
return;
}

/*
 * Types of defects: Memory Leakage - Allocate Memory and not freeing it
 *  When using pointer to float Memory is allocated and freed in conditional statement(if)
 */
void memory_leak_005()
{
	float *ptr;
	int flag=10;

	if(flag > 0)
	{
		ptr= (float*) malloc(5*sizeof(float));/*Tool should detect this line as error*/ /*ERROR:Memory Leakage */
		if(ptr!=NULL)
		{
        *(ptr+1) = 10.5;
		}
	}
    if(flag < 0)
	free(ptr);
}



int main () {
	
	
	memory_leak_005();

	
};
