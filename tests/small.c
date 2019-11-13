#include<stdio.h>
#include<stdlib.h>

	int main()
	{
		int *ptr = malloc(sizeof (int));
		printf("address of pointer is: 0x%0X\n", (unsigned)&ptr);
		*ptr = 10;

		printf("vallue of pointer is: %d\n",*ptr);
		free(ptr);
		printf("address of free is: 0x%0X\n", (unsigned)&ptr);
		*ptr = 7; /* Undefined behavior */
		printf("%d",*ptr);
		return 0;

	}
