#include <stdio.h>
#include <Windows.h>

#define SIZE 100

void reverse( char );

int main()
{
	char arr[SIZE];
	
	printf("입력: ");
	gets(arr);

	reverse( arr );

	printf("변환: %s\n", arr);

	return 0;
}

void reverse( char arr[] )
{
	int i;

	for(i = 0; i < SIZE; ++i){
		//대문자를 소문자로
		if( arr[i] >= 'A' && arr[i] <= 'Z'){
			arr[i] += 32;
		}
		//소문자를 대문자로
		else if( arr[i] >= 'a' && arr[i] <= 'z'){
			arr[i] -= 32;
		}
	}
}