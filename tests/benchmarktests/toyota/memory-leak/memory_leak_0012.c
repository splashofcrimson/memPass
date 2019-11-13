#include "HeaderFile.h"
void free(void* n){
return;
}


/*
 * Types of defects: Memory Leakage - Allocate Memory and not freeing it
 * Complexity: Union of pointer	Constant	 expressions	Write
 */
typedef struct {
	int a;
	int b;
} memory_leak_0012_s_001;

typedef struct {
	int a;
	int b;
} memory_leak_0012_s_002;

typedef struct {
	int a;
	int b;
} memory_leak_0012_s_003;

typedef union {
	memory_leak_0012_s_001 s1;
	memory_leak_0012_s_002 s2;
	memory_leak_0012_s_003 s3;
} memory_leak_0012_uni_001;

void memory_leak_0012 ()
{
	memory_leak_0012_uni_001 *u = (memory_leak_0012_uni_001 * )malloc(5*sizeof( memory_leak_0012_uni_001 ));
	memory_leak_0012_uni_001 *p  = (memory_leak_0012_uni_001 * )malloc(5*sizeof( memory_leak_0012_uni_001 ));/*Tool should detect this line as error*/ /*ERROR:Memory Leakage */
	p = u;

	p->s1.a = 1;

	free(u);
}


int main () {
	
	
	memory_leak_0012();

	
};
