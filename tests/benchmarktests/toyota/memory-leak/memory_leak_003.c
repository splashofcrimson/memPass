
void free(void* n){
return;
}

/*
 * Types of defects: Memory Leakage - Allocate Memory and not freeing it
 *  Memory allocated in a function and Memory used in another function
 */
void memory_leak_003_func_001 (int len,char **stringPtr)
{
	char * p = malloc(sizeof(char) * (len+1));
	*stringPtr = p;
}

void memory_leak_003 ()
{
	char *str = "This is a string";
	char *str1;
	memory_leak_003_func_001(strlen(str),&str1);/*Tool should detect this line as error*/ /*ERROR:Memory Leakage */
	strcpy(str1,str);
}


/*
 	* Types of defects: Memory Leakage - Allocate Memory and not freeing it
 	* When using double pointers to the type INT
*/
int main () {
	
	
	memory_leak_003();

	
};
