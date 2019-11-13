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
* Complexity:double Free in a while loop with a condition
*/


void double_free_011()
{
	char* ptr= (char*) malloc(sizeof(char));
	int flag=1,a=0,b=2;

	while(a<b)
	{
		if(flag ==1)
		free(ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
		a++;
	}
}

int main(){
  double_free_011();
}