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
* Complexity:double Free in a for loop
*/


void double_free_012()
{
	char* ptr= (char*) malloc(sizeof(char));
	int a=0;

	for(a=0;a<2;a++)
	{
		free(ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
	}
}

int main(){
  double_free_012();
}