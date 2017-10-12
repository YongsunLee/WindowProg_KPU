#include <stdio.h>

void SearchDay();
void DrawCalendar();
int UseZera( int year, int month);
int CheckLastDay( int year, int month, int day);

int main()
{	
	DrawCalendar();
	SearchDay();
	return 0;
}

void SearchDay( )
{
	int allday;
	int remain;
	int i, j;
	char *week[] = { "Sun" , "Mon", "Tue", "Wed" , "Thu", "Fri", "Sat" };
	int month[12] = { 31,28,31,30,31,30,31,31,30,31,30,31};
	int year, mon, day;
	
	while(1){
		printf("년 월 일 순으로 입력해주세요: ");
		scanf("%d %d %d", &year, &mon, &day);

		if ( year % 4 !=0 && (year % 100 ==0 || year % 400 != 0) && mon == 2 && day == 29){
			printf("윤년이 아니다. \n");
			continue;
		}

		if( 2000 < year && year < 2020 || 0 < mon && mon < 13){
			allday = ((year-1)+(year-1)/4-(year-1)/100+(year-1)/400);
		for(i = 0; i < mon; ++i){
			if(i == 1 && (year % 4==0 && year % 100 !=0 || year % 400 == 0)){
				month[1] += 1;
			}
			allday += month[i];
		}
		allday -= month[i-1];
		remain = (allday + 1) % 7;
		i = (remain+day-1)%7;

		printf("%d/%d/%d: %sday\n", year, mon, day, week[i]);
		} else{
			printf("년, 월 중 범위에 맞지 않는 수가 입력되었습니다.");
			printf("년도는 2000~2020년 사이의 달력입니다.");
			printf("월은 1 ~ 12월 입니다.");
		}
		
	}
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

int UseZera( int year, int month)
{
	if (month == 1 || month == 2) {
			year--;
			month +=12;
	}
		 
	return (year + year/4 - year/100 + year/400 + (13 * month + 8) / 5 + 1) % 7;
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