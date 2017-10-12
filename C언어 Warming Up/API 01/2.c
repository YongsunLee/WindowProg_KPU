#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Max( int a[]);
int Min( int a[]);

int main()
{
	int num = 0;
	int a[20];
	int i, j = 0;
	int temp = 0;
	int max, min;

	printf("Input number: ");
	scanf("%d", &num);

	srand((unsigned)time(NULL));
		for(i = 0; i < 20; ++i){
			a[i] = rand()%num;
			for( j = 0; j < i; ++j){
				if(a[i]==a[j]){
					--i;
					break;
				}
			}
		}
		
		printf("Number are: ");
		for( i = 0; i < 20; ++i){
			printf("%3d ", a[i]);
			if( i == 5 || i == 10 || i == 15 || i == 20)
				printf("\n");
		}

		for(j = 20-1; j > 0; --j){
			for( i = 0; i < j; ++i){
				if( a[i] > a[i+1]){
					temp = a[i];
					a[i] = a[i+1];
					a[i+1] = temp;
				}
			}
		}
		printf("\n");

		printf("Sorted: ");
		for( i = 0; i < 20; ++i){
			printf("%3d ", a[i]);
			if( i == 5 || i == 10 || i == 15 || i == 20)
				printf("\n");
		}

		printf("\n");

		max = Max(a);
		printf("Maximum number: %d\n", max);
		min = Min(a);
		printf("Minimum number: %d\n", min);

		srand((unsigned)time(NULL));
		for(i = 0; i < 20; ++i){
			a[i] = rand()%max + min;
			for( j = 0; j < i; ++j){
				if(a[i]==a[j]){
					--i;
					break;
				}
			}
		}

		printf("New Number are: ");
		for( i = 0; i < 20; ++i){
			printf("%3d ", a[i]);
			if( i == 5 || i == 10 || i == 15 || i == 20)
				printf("\n");
		}

	return 0;
}

int Max( int a[] )
{
	int max = a[0];
	int i = 0;

	for(i = 0; i < 20; ++i){
		if( max < a[i] ){
			max = a[i];
		}
	}
	
	return max;
}

int Min( int a[] )
{
	int min = a[0];
	int i = 0;

	for( i = 0; i < 20; ++i ){
		if( a[i] < min ){
			min = a[i];
		}
	}

	return min;
}