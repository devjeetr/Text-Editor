#include "UtilitiesClass.h"


UtilitiesClass::UtilitiesClass()
{
	caretPos.x = 0;
	caretPos.y = 0;
}

void UtilitiesClass::init(HWND hwnd)
{
	HDC hDC;
	hDC = GetDC(hwnd); 
	GetTextMetrics(hDC, &textProperties);
	ReleaseDC(hwnd,hDC);
}

UtilitiesClass::~UtilitiesClass(void)
{
}


POINT UtilitiesClass::getCaretPos()
{
	return caretPos;
}

void UtilitiesClass::setCaretPos(const int& x, const int& y)
{
	caretPos.x = x;
	caretPos.y = y;
}

POINT UtilitiesClass::getAbsFromRel(const int& lineNum,const int& charIndex)
{
	POINT temp;
	temp.x= 0;
	temp.y = 0;
	return temp;
}

unsigned int UtilitiesClass::getAveWidth()
{
	return textProperties.tmAveCharWidth;
}

unsigned int UtilitiesClass::getFontHeight()
{
	return (textProperties.tmHeight+textProperties.tmExternalLeading);
}

void UtilitiesClass::updateCaret(HWND hwnd)
{
	
	SetCaretPos(caretPos.x, caretPos.y);
}