#include <stdio.h>
#include <stdlib.h>

int main(void)
{
int year, month, day, weekday;
int year2, month2;
int n, d;

printf("�޷��� ǥ���մϴ�\n");
printf("������ �Է����ּ���\n");

/* �⵵�� �Է��� �޴´�. */ 
    scanf("%d", &year);

printf( " ���� %d��\n", year);
    for (month = 1; month <= 12; month++)
    {
        year2 = year;
        month2 = month;
     printf( " %d��\n", month);

/* �ش��ϴ� ���� ��¥���� ����Ѵ�. */ 
     
    
     if (month == 1 || month == 2) {
     year2--;
     month2 +=12;
     }
	 //���۳�¥
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
