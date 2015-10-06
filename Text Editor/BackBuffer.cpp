#include "BackBuffer.h"


BackBuffer::BackBuffer(HWND hwnd, int height, int width): mhWnd(hwnd),mHeight(height),mWidth(width)
{
	HDC hWndDC;
	hWndDC = GetDC(hwnd);

	mhDC = CreateCompatibleDC(hWndDC);
	mhSurface = CreateCompatibleBitmap(hWndDC,width, height);

	ReleaseDC(hwnd, hWndDC);

	mhOldObject = (HBITMAP) SelectObject(mhDC,mhSurface);

	HBRUSH white = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH oldBrush = (HBRUSH) SelectObject(mhDC, white);

	Rectangle(mhDC, 0 ,0 ,width, height);

	HFONT hEditorFont = CreateFont(16, 0, 0, 0, 0, false, 0, 0, 0, 0, 0, 0, FIXED_PITCH, "Consolas");
	DeleteObject(SelectObject(mhDC, hEditorFont));

	DeleteObject(SelectObject(mhDC, oldBrush));
	DeleteObject(white);
}


BackBuffer::~BackBuffer(void)
{
	DeleteObject(SelectObject(mhDC, mhOldObject));
	DeleteObject(mhSurface);
	DeleteDC(mhDC);
}

HDC BackBuffer::getDC()
{
	return mhDC;
}

int BackBuffer::getWidth()
{
	return mWidth;
}

int BackBuffer::getHeight()
{
	return mHeight;
}

void BackBuffer::present()
{
	HDC hWndDC = GetDC(mhWnd);

	BitBlt(hWndDC, 0, 0,mWidth, mHeight, mhDC, 0, 0,SRCCOPY);

	ReleaseDC(mhWnd,hWndDC);
}