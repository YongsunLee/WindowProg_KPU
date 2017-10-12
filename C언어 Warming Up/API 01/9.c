#include <stdio.h>

int main(void)
{
	int i;
	int check = 0 ; 
	int temp ;     
	int num ;  
	
	for( i = 1; i < 1000; ++i){
		check = 0;            // 체크값을기본적으로0으로준다
		temp = i;             // i의값을temp에저장
		while( temp > 0 ){      
			num = temp%10 ;
			if( num == 3 || num == 6 || num == 9 ){     
				printf("  ! ");
				check = 1;
				break;
			}     
			temp/=10;
		}
		if( check == 0 )
			printf("%3d ", i );
		if( i % 10 == 0 )
			printf("\n");
	}

	return 0 ;
}

