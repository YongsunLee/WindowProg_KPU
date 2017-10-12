#include <stdio.h>
#include <stdlib.h>

int main(void)
{
int year, month, day, weekday;
int year2, month2;
int n, d;

printf("달력을 표시합니다\n");
printf("연도를 입력해주세요\n");

/* 년도를 입력을 받는다. */ 
    scanf("%d", &year);

printf( " 서기 %d년\n", year);
    for (month = 1; month <= 12; month++)
    {
        year2 = year;
        month2 = month;
     printf( " %d월\n", month);

/* 해당하는 달의 날짜수를 계산한다. */ 
     
    
     if (month == 1 || month == 2) {
     year2--;
     month2 +=12;
     }
	 //시작날짜
     weekday = (year2 + year2/4 - year2/100 + year2/400 + (13 * month2 + 8) / 5 + 1) % 7;
    
     printf("----------------------------\n");
     printf(" Sun Mon Tue Wed Thu Fri Sat\n");
     printf("----------------------------\n");
    
     for (n = 0; n < weekday; n++) printf(" ");
    
     for (d = 1; d <= day; d++) {
		printf(" %2d ", d);
		if (n % 7 == 6 && d != day) printf("\n");
			n++;
     }
    
     printf("\n--------------------------\n");
    }

    system("PAUSE");    
return 0;
}
