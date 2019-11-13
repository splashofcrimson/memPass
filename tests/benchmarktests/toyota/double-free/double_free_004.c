/********Software Analysis - FY2013*************/
/*
* File Name: double_free.c
* Defect Classification
* ---------------------
* Defect Type: Resource management defects
* Defect Sub-type: Double free
*
*/

#include "HeaderFile.h"

void free(void* n){
return;
}

/*
* Types of defects: Double free
* Complexity: Memory is free in a if statement
*/

void double_free_004()
{
	char* ptr= (char*) malloc(10*sizeof(char));
	int i;
	for(i=0;i<10;i++)
	{
		*(ptr+i)='a';
	}

	if (rand() % 2==0)
	{
		free(ptr);
	}

	if(rand() % 3==0)
	free(ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
}

int main(){
  double_free_004();
}