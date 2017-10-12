#include <stdio.h>
#include <Windows.h>
#include <time.h>

int Max( int arr[][5] );

int main()
{
	int arr[4][5];
	int i, j;
	int max;
	int com;

	srand((unsigned)time(NULL));
	for( i = 0; i < 4; ++i){
		for( j = 0; j < 5; ++j){
			arr[i][j] = rand()%20 + 1;
		}
	}

	for( i = 0; i < 4; ++i){
		for( j = 0; j < 5; ++j){
			printf("%3d", arr[i][j]);
		}
		printf("\n");
	}

	for( ; ; ){

		printf("명령어 행 열\n");
		scanf("%d %d %d", &com, &i, &j);
		
		i = i - 1;
		j = j - 1;

		switch( com ){
		case 1:
			arr[i][j] = arr[i][j] * 5;
			break;
		case 2:
		if( i > 4 && 5 < j )
			printf("범위를 벗어났습니다.\n");
		else	
			arr[i][j] = arr[i][j] + arr[j][i];
		break;
		case 3:
			arr[i][j] = -1 * arr[i][j];
			break;
		case 4:
			max = Max(arr);
			arr[i][j] = max;
			break;
		default:
			break;
		}

		printf("결과\n");
		for( i = 0; i < 4; ++i){
			for( j = 0; j < 5; ++j){
				printf("%3d", arr[i][j]);
			}
			printf("\n");
		}

	}

	return 0;
}

int Max(int arr[4][5]){

	int i, j;
	int start;

	start = arr[0][0];
	for( i = 0; i < 4; ++i){
		for( j = 0; j < 5; ++j){
			if( start < arr[i][j] ){
				start = arr[i][j];
			}
		}
	}

	return start;
}