#include "HeaderFile.h"
void free(void* n){
return;
}

/*
* Types of defects: Memory Leakage - Allocate Memory and not freeing it
* Complexity: When using a double pointer to long , memory allocated in another function inside goto label and if condition
*/
long ** memory_leak_0017_gbl_doubleptr;
int memory_leak_0017_func_001(int flag)
{
   int ret ;
   if (flag ==0)
	   ret = 0;
   else
	   ret=1;
   return ret;
}

void memory_leak_0017_func_002()
{
	int i,j;
	memory_leak_0017_gbl_doubleptr=(long**) malloc(10*sizeof(long*));/*Tool should detect this line as error*/ /*ERROR:Memory Leakage */

	for(i=0;i<10;i++)
	{
		memory_leak_0017_gbl_doubleptr[i]=(long*) malloc(10*sizeof(long));
	}
	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			memory_leak_0017_gbl_doubleptr[i][j]=i;
		}
	}
}

#define ZERO 0
void memory_leak_0017()
{
	int flag=0,i,j;
	memory_leak_0017_gbl_doubleptr=NULL;
goto label;

    if(memory_leak_0017_func_001(flag)==ZERO)
	{
		for(i=0;i<10;i++)
		{
			for(j=0;j<10;j++)
			{
				memory_leak_0017_gbl_doubleptr[i][j] += 1;
			}
			free (memory_leak_0017_gbl_doubleptr[i]);
			memory_leak_0017_gbl_doubleptr[i] = NULL;
	   }

	}

label:
    	if(memory_leak_0017_func_001(flag)==ZERO)
    	{
    		memory_leak_0017_func_002();
    	}
}



int main () {
	
	
	memory_leak_0017();

	
};
