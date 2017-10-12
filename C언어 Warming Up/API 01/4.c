#include <stdio.h>
#include <string.h>

#define SIZE 100

void reverse( char arr );

int main()
{
	int i, j = 0;
	char arr[SIZE];

	printf("Enter sentence: ");
	gets(arr);

	reverse( arr );

	return 0;
}

void reverse( char arr[] )
{
	int i, j = 0;
	i = strlen(arr);
	
	for( i - 1; i >= 0; --i ){
		printf("%c", arr[i]);
	}
	printf("\n");
}