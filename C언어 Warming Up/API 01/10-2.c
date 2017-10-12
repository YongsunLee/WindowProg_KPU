#include <stdio.h>

int Weekday( int , int , int );

int main()
{
	int year; int month;
	int date; int lastday; int i, j;
	int weekday;
	printf("년도를 입력하세요: ");
	scanf("%d", &year);
	printf("월을 입력하세요: ");
	scanf("%d", &month);

	if( year > 2020 && year < 2000 ){
		printf("이 달력의 범위는 2000~2020년 입니다.");
	}
	if( month >= 1 && month <= 12 ){
		switch (month) {
			 case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				lastday = 31;
				break;
			 case 4: case 6: case 9: case 11:
				lastday = 30;
				break;
			 case 2:
				if (year % 4==0 && year % 100 !=0 || year % 400 == 0)
					lastday = 29;
				else
					lastday = 28;
				break;
			 default:
				 lastday = 0;
			 break;
		}
	} else {
		printf("월의 입력 범위가 잘못되었습니다.");
	}

	printf(" 일 월 화 수 목 금 토\n");
	printf(" --------------------\n");
	date = 1;
	for(i = 0; i < 7; ++i){
		weekday = Weekday( year, month, date);
		for(j = 0; j < 7; ++j){
			if( i = 1 && j < weekday){
				printf("  ");
			} else {
				if( j >= weekday && date < 10){
					printf(" %2d", date);
				} else if( j >= weekday && date >= 10){
					printf(" %2d", date);
				}
				if( date < lastday){
					date = date + 1;
				} else if( date >= lastday){
					printf("\n");
					return ;
				}
			}
		}
		printf("\n");
	}
}

int Weekday( int y, int m, int d)
{
	if( m < 3 ){
		--y;
		m+=12;
	}
	return (21*(y/100)/4 + 5*(y%100)/4 + 26*(m+1)/10+d-1) %7;
}