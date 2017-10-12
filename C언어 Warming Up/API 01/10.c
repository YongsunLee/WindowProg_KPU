#include <stdio.h>

int weekday(int y, int m, int d);

long t = 0;

int main()
{
	int year = 0;
	int month = 0;
	int day = 0;
	int date;
	char *day[] = {"Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri", "Satur"};

	while(1){
		printf("알고 싶은 년,일,월을 입력하세요.\n");
		scanf("%4d/%2d/%d", &year, &month, &date);
		fflush(stdin);

		t += weekday(year,month,date);

		if(year > 2020 || year < 2000){
			printf("년도의 범위는 2000~2020년 입니다.\n");
		}
		if(month > 12 || month < 0){
			printf("잘못된 달을 입력하셨습니다.\n");
		}
		if(date > 31 || date < 0){
			printf("잘못된 날을 입력하셨습니다.\n");
		}

		switch (month) {
			 case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				day = 31;
				break;
			 case 4: case 6: case 9: case 11:
				day = 30;
				break;
			 case 2:
				if (year % 4==0 && year % 100 !=0 || year % 400 == 0)
					day = 29;
				else
					day = 28;
				break;
			 default:
				day = 0;
			 break;
		}

		printf("%4d/%2d/%d : %s day ", year, month, date, day[t]);
	}
	return 0;
}

int weekday( int y, int m, int d)
{
	if( m < 3 ){
		--y;
		m+=12;
	}
	return (21*(y/100)/4 + 5*(y%100)/4 + 26*(m+1)/10+d-1) %7;
}