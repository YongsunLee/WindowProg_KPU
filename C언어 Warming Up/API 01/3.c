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
	
	printf("ÀÔ·Â: ");
	scanf("%d %d", &num[0], &num[1]);
	
	while(1){
		printf("¸í·É¾î ÀÔ·Â: ");
		scanf("%d", &Num);

		switch (Num)	
		{
		case 1:
			sum = num[0] + num[1];
			printf("¸í·É¾î1 (µ¡¼À): %d\n", sum);
			break;
		case 2:
			minus = num[0] - num[1];
			printf("¸í·É¾î2 (»¬»ù): %d\n", minus);
			break;
		case 3:
			multiple = num[0] * num[1];
			printf("¸í·É¾î3 (°ö¼À): %d\n", multiple);
			break;
		case 4:
			division = (float)num[0] / num[1];
			printf("¸í·É¾î4 (³ª´°¼À): %.2f\n", division);
			break;
		case 5:
			swap( &num[0] , &num[1]);
			printf("¸í·É¾î5 (½º¿Ò): %d %d\n" ,num[0], num[1]);
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