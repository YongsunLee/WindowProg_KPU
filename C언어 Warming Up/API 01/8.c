#include <stdio.h>
#include <math.h>

struct Pos{
	int MaxX, MaxY;
	int MinX, MinY;
};

struct Figure{
	int MidX, MidY;
	float Area;
};

int main()
{
	int num;
	float Pi = 3.14;
	double r;
	float temp;
	struct Pos pos;
	struct Figure Rec;
	struct Figure Cle;

	printf("사각형은 1, 원은 2를 눌러주세요.");
	scanf("%d", &num);
	
	printf("가장 작은 x,y값과 가장 큰 x,y 값을 입력하세요.");
	scanf("%d %d %d %d", &pos.MinX, &pos.MinY, &pos.MaxX, &pos.MaxY);

	switch(num)
	{
	case 1:
		Rec.MidX = (pos.MaxX + pos.MinX) / 2;
		Rec.MidY = (pos.MaxY + pos.MinY) / 2;
		Rec.Area = (pos.MaxX - pos.MinX) * (pos.MaxY - pos.MinY);
		printf("사각형: 중심좌표 (x, y)-(%d, %d) 면적-%.0f", Rec.MidX, Rec.MidY, Rec.Area);
		break;
	case 2:
		Cle.MidX = (pos.MaxX + pos.MinX) / 2;
		Cle.MidY = (pos.MaxY + pos.MinY) / 2;
		temp = (pos.MaxX - pos.MinX)*(pos.MaxX - pos.MinX) + (pos.MaxY - pos.MinY)*(pos.MaxY - pos.MinY);
		r = sqrt(temp) / 2;
		Cle.Area = r*r*Pi;
		printf("사각형: 중심좌표 (x, y)-(%d, %d) 면적-%.2f", Cle.MidX, Cle.MidY, Cle.Area);
		break;
	default:
		printf("1과 2가 아닌 다른수를 누르셨습니다.");
		break;
	}

	return 0;
}