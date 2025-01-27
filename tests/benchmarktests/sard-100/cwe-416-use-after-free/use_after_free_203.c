

/* This software was developed at the National Institute of Standards and
 * Technology by employees of the Federal Government in the course of their
 * official duties. Pursuant to title 17 Section 105 of the United States
 * Code this software is not subject to copyright protection and is in the
 * public domain. NIST assumes no responsibility whatsoever for its use by
 * other parties, and makes no guarantees, expressed or implied, about its
 * quality, reliability, or any other characteristic.

 * We would appreciate acknowledgement if the software is used.
 * The SAMATE project website is: http://samate.nist.gov
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct str_t str;
struct str_t {
	union {
		int   a;
		char *b;
	} foo;
};



void free(void* n){
return;
}


int main()
{
	str container;
	char *str1;
	container.foo.a = 0;
	
	if ((container.foo.b = (char *)malloc(256*sizeof(char))) != NULL)
	{
		strcpy(container.foo.b, "Falut!");
		container.foo.b[0] = 'S';
		printf("%s\n", container.foo.b);
		free(container.foo.b);
		str1 = malloc(65536); 
 		if (str1 == NULL)
        		{printf("Memory allocation problem"); return 1;}

		free(str1);
		
		if (container.foo.b)
			{container.foo.b[0] = 'S'; printf("%s\n", container.foo.b);}	/* FLAW */
	}
	return 0;
}

