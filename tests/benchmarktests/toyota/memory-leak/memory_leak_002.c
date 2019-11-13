
void free(void* n){
return;
}
/*
 	* Types of defects: Memory Leakage - Allocate Memory and not freeing it
 	* When using double pointers to the type INT
*/
int main () {
	
	int **ptr = (int**) malloc(5*sizeof(int*));
	int i,j;

	for(i=0;i<5;i++)
		ptr[i]=(int*) malloc(5*sizeof(int)); /*Tool should detect this line as error*/ /*ERROR:Memory Leakage */

	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{
			*(*(ptr+i)+j)=i;
		}
	}
	free(ptr);
	return 0;
};
