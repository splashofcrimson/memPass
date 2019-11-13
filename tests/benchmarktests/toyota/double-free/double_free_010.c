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
* Complexity:Free in a while loop with a constant
*/


void double_free_010()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=1;

	while(flag)
	{
		free(ptr);
		flag--;
	}
	free(ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
}

int main(){
  double_free_010();
}