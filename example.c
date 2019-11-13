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

int main(int argc, char *argv[])
{
    // char *str[1] = {(char *)NULL};
    // *str = (char *)malloc(256*sizeof(char));
    int *ptr = (int *)malloc(16*sizeof(int));
    for (int i = 0; i < 10; i++){
        ptr[i] = i;
    }
    int x;
    for (int j = 0; j < 10; j++) {
        x = ptr[j];
    }
    free(ptr);
    // free(str);
	// char *str[1] = {(char *)NULL};
	// if ((*str = (char *)malloc(256*sizeof(char))) != NULL)
	// {
	// 	strcpy(*str, "Falut!");
	// 	**str = 'S';
	// 	printf("%s\n", *str);
	// 	free(*str);	
	// 	char *str1 = malloc(65536); free(str1);
	// 	*str = 'S';
	// }
	return 0;
}