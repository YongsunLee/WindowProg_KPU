#include <stdio.h>

int main()
{
	int a[100];
	int x = 0;
	int n = 0;

	for( x = 0; x < 100; ++x){
		a[x] = x+1;
	}

	// 2�� ��� 3�� ����� �ƴѰ�
	for( x = 0; x < 100; ++x){
		if( a[x]%2 != 0 && a[x]%3 != 0){
			++n;
			printf("%5d" ,a[x]);
		}
	}
	printf("\n2�� ����� 3�� ����� �ƴ� ��: %d��\n", n);
	n = 0;

	// 2�� ����� �ƴϰ� 3�� ����� �ƴ� �� �߿� �Ҽ��ΰ�
	for( x = 0; x < 100; ++x){
		if( a[x] == 1 )
			a[x] = 0;
		if( a[x]%2 == 0 )
			a[x] = 0;
		if( a[x]%3 == 0 )
			a[x] = 0;
		if( a[x]%4 == 0 )
			a[x] = 0;
		if( a[x] == 5 )
			a[x] = 5;
		else if( a[x]%5 == 0 )
			a[x] = 0;
		if( a[x]%6 == 0 )
			a[x] = 0;
		if( a[x] == 7 )
			a[x] = 7;
		else if( a[x]%7 == 0 )
			a[x] = 0;
		if( a[x]%8 == 0 )
			a[x] = 0;
		if( a[x]%9 == 0 )
			a[x] = 0;
		if( a[x] != 0 ){
			n++;
			printf("%5d", a[x]);
		}
	}
	printf("\n�� �� �Ҽ�: %d\n", n);

	return 0;
}