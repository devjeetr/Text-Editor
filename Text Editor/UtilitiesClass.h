#pragma once
#include <Windows.h>

class UtilitiesClass
{
public:
	UtilitiesClass();

	void init(HWND hwnd);
	
	POINT getCaretPos();
	void setCaretPos(const int& x, const int& y);

	unsigned int getAveWidth();
	unsigned int getFontHeight();
	unsigned int changeFont(char& szFontName);

	void updateCaret(HWND hwnd);
	
	POINT getAbsFromRel(const int& lineNum,const int& charIndex); 

	~UtilitiesClass();
private:
	TEXTMETRIC textProperties;
	
	POINT caretPos;
	
	HFONT currentFont;
	POINT fontProperty;

};

