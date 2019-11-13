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
/*
* Types of defects: Double free
* Complexity: Basic type where pointer is "freed"  twice
*/

void free(void* n){
return;
}

void double_free_001()
{
	char* ptr= (char*) malloc(sizeof(char));
	free(ptr);

	free(ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
}

int main(){
  double_free_001();
}