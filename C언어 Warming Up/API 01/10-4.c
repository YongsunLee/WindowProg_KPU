#include <stdio.h>
#include <stdlib.h>

int CheckLastDay( int , int , int );
int UseZera( int , int );
void DrawCalendar();
void InputDay();
int StartDay( int , int );
int Calcweek( int );
int Weekday(int, int, int);

int main(void)
{
	printf("2014년의 달력을 출력합니다.\n");
	DrawCalendar();
	InputDay();
	return 0;
}

int CheckLastDay( int year, int month, int day)
{
	switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			return day = 31;
			break;
    
		case 4: case 6: case 9: case 11:
			return day = 30;
			break;
    
		case 2:
			if (year % 4==0 && year % 100 !=0 || year % 400 == 0)
				return day = 29;
			else
				return day = 28;
			break;
	}
}

int UseZera( int year, int month)
{
	if (month == 1 || month == 2) {
			year--;
			month +=12;
	}
		 
	return (year + year/4 - year/100 + year/400 + (13 * month + 8) / 5 + 1) % 7;
}

int Weekday( int y, int m, int d)
{
	if( m < 3 ){
		--y;
		m+=12;
	}
	return (21*(y/100)/4 + 5*(y%100)/4 + 26*(m+1)/10+d-1) %7;
}

void DrawCalendar()
{
	int year = 2014;
	int month = 1;
	int day = 0, weekday;
	int n, d;

	for (month = 1; month <= 12; ++month){
		printf( " %d월\n", month);

		day = CheckLastDay( year, month, day);
		weekday = UseZera( year, month );
    
		printf("----------------------------\n");
		printf(" Sun Mon Tue Wed Thu Fri Sat\n");
		printf("----------------------------\n");
    	for (n = 0; n < weekday; ++n) 
			printf("    ");
			for (d = 1; d <= day; d++) {
				printf(" %2d ", d);
				if (n % 7 == 6 && d != day) 
					printf("\n");
				n++;
			}
			printf("\n--------------------------\n");
		}
}

void InputDay()
{
	int year = 0, month = 0, day = 0;
	int weekday; int lastday;
	int totaldate = 0; int i; int sd;
	char *week[] = { "Sun" , "Mon", "Tue", "Wed" , "Thu", "Fri", "Sat" };
	
	do{
		printf("년도 월 날짜를 입력해 주세요.");
		scanf("%d %d %d", &year, &month, &day);

		if( year > 2020 && year < 2000){
			printf("이 달력의 년도는 2000~2020년 입니다.");
		}
		if( month > 12 && month < 0){
			printf("월을 잘못 입력하셨습니다.");
		}

		sd = Weekday( year, month, day );
		weekday = Calcweek(sd);

		printf("%d/%d/%d: %sday\n", year, month, day, week[weekday]);
	}while(1);
}

int StartDay( int year, int month )
{
	int temp = 0;
	int i;

	for( i = 1; i < year; ++i){
		if(year % 4==0 && year % 100 !=0 || year % 400 == 0){
			temp += 366;
		} else {
			temp += 365;
		}
	}

	for(i = 1; i < month; ++i){
		if( i == 2 ){
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

	temp = temp +1;
	return temp %7;
}

int Calcweek( int totaldate )
{
	if ( totaldate == 6)
		return 0;
	return totaldate + 1;
}