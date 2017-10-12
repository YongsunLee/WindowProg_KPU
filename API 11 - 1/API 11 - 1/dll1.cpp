#include <Windows.h>

extern "C"
__declspec(dllexport)POINT FindCenter(RECT rect)
{
	POINT center;

	center.x = (rect.right + rect.left) / 2;
	center.y = (rect.bottom + rect.top) / 2;

	return center;
}

extern "C"
__declspec(dllexport)int Rect_area(RECT rect)
{
	int x, y;

	x = rect.right - rect.left;
	y = rect.bottom - rect.top;

	return x * y;
}

extern "C"
__declspec(dllexport)RECT Size_Up(RECT rect)
{
	rect.left *= 2;
	rect.top *= 2;
	rect.right *= 2;
	rect.bottom *= 2;
	
	return rect;
}

extern "C"
__declspec(dllexport)RECT Size_Dwon(RECT rect)
{
	rect.left /= 2;
	rect.top /= 2;
	rect.right /= 2;
	rect.bottom /= 2;

	return rect;
}
