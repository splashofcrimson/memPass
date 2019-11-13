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

/*
* Types of defects: Double free
* Complexity:Free in a function
*/
char *double_free_function_008_gbl_ptr;
void double_free_function_008()
{
	free (double_free_function_008_gbl_ptr);
}

void double_free_008()
{
	double_free_function_008_gbl_ptr= (char*) malloc(sizeof(char));

	double_free_function_008();
	free(double_free_function_008_gbl_ptr); /*Tool should detect this line as error*/ /*ERROR:Double free*/
}



int main(){
  double_free_008();
}