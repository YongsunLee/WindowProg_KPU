#include <stdio.h>
#include <Windows.h>

void swap( int *a, int *b);

int main()
{
	int num[2];
	int Num;
	int sum;
	int minus;
	int multiple;
	float division;
	
	printf("�Է�: ");
	scanf("%d %d", &num[0], &num[1]);
	
	while(1){
		printf("��ɾ� �Է�: ");
		scanf("%d", &Num);

		switch (Num)	
		{
		case 1:
			sum = num[0] + num[1];
			printf("��ɾ�1 (����): %d\n", sum);
			break;
		case 2:
			minus = num[0] - num[1];
			printf("��ɾ�2 (����): %d\n", minus);
			break;
		case 3:
			multiple = num[0] * num[1];
			printf("��ɾ�3 (����): %d\n", multiple);
			break;
		case 4:
			division = (float)num[0] / num[1];
			printf("��ɾ�4 (������): %.2f\n", division);
			break;
		case 5:
			swap( &num[0] , &num[1]);
			printf("��ɾ�5 (����): %d %d\n" ,num[0], num[1]);
			break;
		}
	}
	return 0;
}

void swap( int *a, int *b )
{
	int buf;

	buf = *b;
	*b = *a;
	*a = buf;
}