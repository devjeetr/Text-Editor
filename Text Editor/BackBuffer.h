#pragma once
#include <Windows.h>

class BackBuffer
{
public:
	BackBuffer(HWND hwnd, int height, int width);
	~BackBuffer(void);

	HDC getDC();

	int getWidth();
	int getHeight();

	void present();

private:
	HWND mhWnd;		//Handle to main window 
	HDC mhDC;
	HBITMAP mhSurface;
	HBITMAP mhOldObject;
	int mHeight;
	int mWidth;
};

