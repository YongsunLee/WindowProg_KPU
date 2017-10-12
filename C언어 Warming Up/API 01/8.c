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

	printf("�簢���� 1, ���� 2�� �����ּ���.");
	scanf("%d", &num);
	
	printf("���� ���� x,y���� ���� ū x,y ���� �Է��ϼ���.");
	scanf("%d %d %d %d", &pos.MinX, &pos.MinY, &pos.MaxX, &pos.MaxY);

	switch(num)
	{
	case 1:
		Rec.MidX = (pos.MaxX + pos.MinX) / 2;
		Rec.MidY = (pos.MaxY + pos.MinY) / 2;
		Rec.Area = (pos.MaxX - pos.MinX) * (pos.MaxY - pos.MinY);
		printf("�簢��: �߽���ǥ (x, y)-(%d, %d) ����-%.0f", Rec.MidX, Rec.MidY, Rec.Area);
		break;
	case 2:
		Cle.MidX = (pos.MaxX + pos.MinX) / 2;
		Cle.MidY = (pos.MaxY + pos.MinY) / 2;
		temp = (pos.MaxX - pos.MinX)*(pos.MaxX - pos.MinX) + (pos.MaxY - pos.MinY)*(pos.MaxY - pos.MinY);
		r = sqrt(temp) / 2;
		Cle.Area = r*r*Pi;
		printf("�簢��: �߽���ǥ (x, y)-(%d, %d) ����-%.2f", Cle.MidX, Cle.MidY, Cle.Area);
		break;
	default:
		printf("1�� 2�� �ƴ� �ٸ����� �����̽��ϴ�.");
		break;
	}

	return 0;
}