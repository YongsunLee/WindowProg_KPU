#include <stdio.h>

#pragma warning (disable : 4996)

int main()
{
	FILE *fp = NULL;

	fp = fopen("sample.txt", "w");
	if ( fp == NULL){
		printf("Fail");
	}
	else 
		printf("Succes");
	fputc('a', fp);
	fputc('b', fp);
	fputc('c', fp);
	fclose(fp);
}