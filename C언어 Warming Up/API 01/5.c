#include <stdio.h>
#include <Windows.h>

#define SIZE 100

void reverse( char );

int main()
{
	char arr[SIZE];
	
	printf("�Է�: ");
	gets(arr);

	reverse( arr );

	printf("��ȯ: %s\n", arr);

	return 0;
}

void reverse( char arr[] )
{
	int i;

	for(i = 0; i < SIZE; ++i){
		//�빮�ڸ� �ҹ��ڷ�
		if( arr[i] >= 'A' && arr[i] <= 'Z'){
			arr[i] += 32;
		}
		//�ҹ��ڸ� �빮�ڷ�
		else if( arr[i] >= 'a' && arr[i] <= 'z'){
			arr[i] -= 32;
		}
	}
}