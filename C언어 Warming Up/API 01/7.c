#include <stdio.h>

struct Student
{
	char name[20];
	int grade;
	char API;
	char Data;
	char Graphic;
	float GPA;
};

int main()
{
	int i = 0;

	struct Student students[3] = {{"Steve", 1, 'A', 'B', 'A', }, 
								  {"Mary", 2, 'B', 'B', 'A', }, 
								  {"Paul", 3, 'A', 'A', 'B', }};
	
	for( i = 0; i < 3; ++i){
		printf("%s %d ¼ºÀû: %c %c %c", 
			students[i].name, students[i].grade, students[i].API, students[i].Data, students[i].Graphic);
		printf("\n");
	}

	for( i = 0; i < 3; ++i){
		if( students[i].API == 'A' )
			students[i].GPA += 4.5;
		else if( students[i].API == 'B')
			students[i].GPA += 3.5;
		else
			students[i].GPA += 2.5;

		if( students[i].Data == 'A' )
			students[i].GPA += 4.5;
		else if( students[i].Data == 'B')
			students[i].GPA += 3.5;
		else
			students[i].GPA += 2.5;

		if( students[i].Graphic == 'A' )
			students[i].GPA += 4.5;
		else if( students[i].Graphic == 'B')
			students[i].GPA += 3.5;
		else
			students[i].GPA += 2.5;
	}

	for(i = 0; i < 3; ++i){
		students[i].GPA = students[i].GPA / 3;
		printf("%s %d Æò±Õ: %.2f", students[i].name, students[i].grade, students[i].GPA);
		printf("\n");
	}

	return 0;
}