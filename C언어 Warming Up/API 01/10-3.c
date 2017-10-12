#include <stdio.h>

int StartDay(int ,int ); //함수 선언
void PrintCalendar(int ,int ,int);
void Head(int ,int );

int main()
{
	int year;
	int month;
	int day;

	do {
		printf("년도 월을 입력하세요.");
		scanf("%d %d", &year, &month);
		if( month >= 1 && month <= 12){
			break;
		} else
			printf("잘못된 입력입니다.");
	} while(1);

	day = StartDay( year, month);
	Head( year, month);
	PrintCalendar(day,year,month);
}

int StartDay( int year, int month )
{
	int temp = 0;
	int i;

	for( i = 0; i < year; ++i){
		if(year % 4==0 && year % 100 !=0 || year % 400 == 0){
			temp += 366;
		} else {
			temp += 365;
		}
	}

	for(i = 1; i < month; ++i){
		if( i == 2){
			if(year % 4==0 && year % 100 !=0 || year % 400 == 0)
				temp += 29;
			else
				temp += 28;
		}
		switch(i){
			case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				temp += 31;
				break;
			 case 4: case 6: case 9: case 11:
				temp += 30;
				break;
		}
	}

	temp = temp + 1;
	return temp %7;
}

void PrintCalendar( int sd, int year, int month)
{
	int i, j;
	int temp;

	switch(month){
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			temp = 31;
			break;
		 case 4: case 6: case 9: case 11:
			temp = 30;
			break;
		 case 2:
			 if(year % 4==0 && year % 100 !=0 || year % 400 == 0)
				 temp = 29;
			 else
				 temp = 28;
	}

	printf("\t    ");
	for( i = 0; i <= sd; ++i)
		printf("    ");
	j = sd;
	for( i = 1; i <= temp; ++i){
		if( j == 7){
			printf("\n\t    ");
			j = 0;
		}
		printf("%2d", i);
		++j;
	}
	printf("\n\n");
}

void Head( int year, int month)
{
	
}
