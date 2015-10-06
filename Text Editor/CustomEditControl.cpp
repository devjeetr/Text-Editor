#include "CustomEditControl.h"

#define DEFAULT_BUFFER_SIZE 500
#define TAB_SPACE 4

//hello my name is devjeet


CustomEditControl::CustomEditControl():szClassName("Custom Class"),currentLine(0),counter(0),bufferSize(DEFAULT_BUFFER_SIZE),mHeight(0),mWidth(0),fileReadBuffer(0)
{
	/*
	*		Change Required Here
	*	No text buffer is required in this class using the piece table method
	*/
	//Initialize textBuffer with default size and clear out the contents using ZeroMemory
	textBuffer = new char[DEFAULT_BUFFER_SIZE];
	ZeroMemory(textBuffer,sizeof(textBuffer));

	/*
	*		Change Required Here
	*	The project has be migrated from lineStruct to lineStruct2 data structure.
	*	Best solution in mind is to create a wrapper around piece table
	*/
	//Add a new lineStruct, for the first line of the editor.
	//We will always have a first line, no matter what, so it fits that we use the constructor to initialize the line structure
	lineStruct tmp;
	tmp.offset = 0;
	tmp.length = NULL;
	lines.push_back(tmp);

	//Initialize the caret position to (0,0), i.e. the start of the text field
	caretPos.x = 0;
	caretPos.y = 0;
}

CustomEditControl::~CustomEditControl(void)
{

	//release all the resources here
	DeleteDC(memDC);
	DeleteObject(mSurface);
	
	DeleteObject(hEditorFont);
	
	/*
	*		Change Required Here
	*	No text buffer if piece table method is used
	*/
	delete[] textBuffer;
	textBuffer = 0;

	if(fileReadBuffer != 0)
	{
		delete[] fileReadBuffer;							//be careful to use delete[] with fileReadBuffer.
		fileReadBuffer = 0;
	}
}

//don't worry about this function
void CustomEditControl::InitCustomControl(HWND hWndParent,WNDPROC wndProc)
{
	WNDCLASSEX wndClass;
	RECT rc;
	ZeroMemory(&rc,sizeof(rc));
	ZeroMemory(&wndClass,sizeof(wndClass));
	
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hInstance = GetModuleHandle(0);
	wndClass.lpfnWndProc = wndProc;
	wndClass.lpszClassName = szClassName.c_str();
	wndClass.cbWndExtra = NULL;
	
	if(!RegisterClassEx(&wndClass))
		MessageBoxA(NULL,"Custom Control Registration Failed", "Fatal Error", MB_ICONERROR);
	
	GetClientRect(hWndParent,&rc);

	mWidth = rc.right-rc.left;
	mHeight = rc.bottom-rc.top;

	cx = rc.left;
	cy = rc.top;

	if((hWnd=CreateWindowEx(NULL, szClassName.c_str(),"Edit Control", WS_VISIBLE | WS_CHILD,rc.left,rc.top,mWidth,mHeight,hWndParent,NULL, GetModuleHandle(0),NULL))==0)
		MessageBoxA(NULL,"Window Creation Failed","Fatal Application Error",MB_ICONERROR);

	InitBackBuffer(hWnd);
	
	SetFocus(hWnd);
}

LRESULT CustomEditControl::messageCharWM (HWND hwnd,WPARAM wParam,LPARAM lParam)
{
	switch(wParam)
	{
	/*case VK_LEFT:
		MessageBox(NULL,"Left","sa",MB_OK);
		break;
	case VK_RIGHT:
		break;
	case VK_UP:
		break;
	case VK_DOWN:
		break;*/
	case '\n':
	case VK_RETURN:
		addLine(hwnd);
		break;
	
	case VK_TAB:
		for(int i = 0; i<TAB_SPACE; i++)
			SendMessage(hwnd,WM_CHAR,VK_SPACE,NULL);
		break;
	case VK_BACK:
		deleteChar(hwnd);
		break;
	default:
		addCharacter((TCHAR)wParam);
	}
	InvalidateRect(hwnd,NULL,false);
	return 0;
}
LRESULT CustomEditControl::messagePaintWM (HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	PAINTSTRUCT ps;
	ZeroMemory(&ps,sizeof(ps));
	
	//HRGN oldRegion;
	HideCaret(hwnd);
		
	hDC = BeginPaint(hwnd, &ps);
	
	//prepare memory dc
	DeleteObject(SelectObject(memDC, hEditorFont));
	
	//Highlight current line
	
	
	for(unsigned int i = 0; i<=currentLine;i++)
	{
		AnalyzeAndPrint(memDC,&textBuffer[lines[i].offset],lines[i].length,i*(textProperties.tmHeight + textProperties.tmExternalLeading));
	}
	//present the backbuffer
	PresentBuffer(ps.hdc);


	//DeleteObject(SelectObject(hDC, oldRegion));
	EndPaint(hwnd,&ps);
	//DeleteObject(oldBrush);
	
	SetCaretPos(caretPos.x, caretPos.y);
	ShowCaret(hwnd);
	
	return 0;
}
LRESULT CustomEditControl::messageSetFocusWM(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	SetFocus(hwnd);
	CreateCaret(hwnd,NULL,textProperties.tmAveCharWidth,textProperties.tmHeight + textProperties.tmExternalLeading);
	SetCaretPos(caretPos.x,caretPos.y);
	ShowCaret(hwnd);
	return 0;
}
LRESULT CustomEditControl::messageCreateWM(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
	
	HDC hDC;
	
	hDC = GetDC(hwnd);
	hEditorFont = CreateFont(16, 0, 0, 0, 0, false, 0, 0, 0, 0, 0, 0, FIXED_PITCH, "Consolas");
	DeleteObject(SelectObject(hDC, hEditorFont));
	GetTextMetrics(hDC,&textProperties);
	ReleaseDC(hwnd, hDC);
	
	fontHeight = textProperties.tmExternalLeading + textProperties.tmHeight;

	ZeroMemory(&updateRegion, sizeof(updateRegion));

	return 0;
}

LRESULT CustomEditControl::messageSizeWM(LPRECT rc)
{
	MoveWindow(hWnd,0,0,rc->right - rc->left ,rc->bottom - rc->top,true);
	mWidth = rc->right - rc->left;
	mHeight = rc->bottom - rc->top;

	resizeBackbuffer();
	
	return 0;
}

void CustomEditControl::resizeBackbuffer()
{
	mSurface = CreateCompatibleBitmap(GetDC(hWnd),mWidth, mHeight);
	prepareBackBuffer();
}

void CustomEditControl::prepareBackBuffer()
{
	DeleteObject(SelectObject(memDC,mSurface));
	DeleteObject(SelectObject(memDC,hEditorFont));
	DeleteObject(SelectObject(memDC,GetStockObject(WHITE_BRUSH)));
	DeleteObject(SelectObject(memDC,GetStockObject(WHITE_PEN)));
	Rectangle(memDC,cx,cy,mWidth,mHeight);
}
void CustomEditControl::InitBackBuffer(HWND hwnd)
{
	memDC = CreateCompatibleDC(GetDC(hwnd));
	mSurface = CreateCompatibleBitmap(GetDC(hwnd),mWidth,mHeight);
	prepareBackBuffer();
}

void CustomEditControl::addCharacter(char wParam)
{
	/*
	*		Change Required Here
	*	As stated previously, there will be no text buffer
	*	In case of character addition, we simply need to call
	*	the addText method from our wrapper class
	*/
	if(wParam>31 && wParam<127)
	{
		if((counter+5)>bufferSize)
			{
				//resize buffer to prevent overflow
				char* tmp = textBuffer;
				textBuffer = new char[2*bufferSize];
				ZeroMemory(textBuffer,sizeof(textBuffer));
				memcpy(textBuffer,tmp,bufferSize);
				bufferSize *= 2 ;
			}
			textBuffer[counter++] = wParam;
			textBuffer[counter] = '\0';
			//caretPos.x += getWidth(hwnd,wParam);
			caretPos.x += textProperties.tmAveCharWidth ;
			lines[currentLine].length++;
	}
}

void CustomEditControl::PresentBuffer(HDC hDC)
{
	BitBlt(hDC, cx, cy,mWidth, mHeight, memDC, 0, 0,SRCCOPY);
	Rectangle(memDC,cx,cy,mWidth,mHeight);
}

int CustomEditControl::getWidth(HWND hwnd, char character)
{
	return textProperties.tmAveCharWidth;
}

void CustomEditControl::deleteChar(HWND hwnd)
{
	/*
	*		Change Required Here
	*	Change pertaining to loss of text buffer when using piece tables.
	*	We still have to be careful with how character deletes are handled.
	*	Since it requires an update of caret position, it would be simpler
	*	to do the major part of the computations here
	*/
	if(counter>0)
	{
		if(lines[currentLine].length == 0)
		{
			deleteLine(hwnd);
		}
		else
		{
			caretPos.x -= getWidth(hwnd,textBuffer[--counter]);
			textBuffer[counter] = '\0';
			lines[currentLine].length--;
		}
	}
}


void CustomEditControl::deleteLine(HWND hwnd)
{
	/*
	*		Change Required Here
	*	this deals with migration to lineStruct2
	*/
	lines.pop_back();

	moveCaretUpALine(hwnd);
}

void CustomEditControl::addLine(HWND hwnd)
{
	lineStruct tmp;
	tmp.length = NULL;
	tmp.offset = counter;

	lines.push_back(tmp);
	currentLine++;
	caretPos.y +=fontHeight;
	caretPos.x = 0;
}

int CustomEditControl::getLineWidth(HWND hwnd, int lineNumber)
{
	int sum;
	sum = 0;
	for(int i = lines[lineNumber].offset; i< lines[lineNumber].offset + lines[lineNumber].length; i++)
		sum += getWidth(hwnd,textBuffer[i]);
	return sum;
}

void CustomEditControl::moveLeft(HWND hwnd)
{
	if(caretPos.x == 0)
	{
		moveCaretUpALine(hwnd);			//code to move up a line
	}
	else
		caretPos.x -= textProperties.tmAveCharWidth;
}

void CustomEditControl::moveCaretUpALine(HWND hwnd)
{
	currentLine--;

	caretPos.x = lines[currentLine].length * textProperties.tmAveCharWidth;
	caretPos.y -= fontHeight;
}

void CustomEditControl::AnalyzeAndPrint(HDC hDC,char stream[],int length,int y)
{
	string temp;
	temp = "";
	int currentPos;
	currentPos = 0;
	int i;
	for(i = 0; i<length; i++)
	{
		if(stream[i] ==' ' || stream[i] =='\0')
		{
			
			if((strcmp(temp.c_str(),"class"))==0)
				SetTextColor(hDC,RGB(34,240,150));
			if((strcmp(temp.c_str(),"void"))==0)
				SetTextColor(hDC,RGB(255,0,0));
			if((strcmp(temp.c_str(),"int"))==0)
				SetTextColor(hDC,RGB(0,100,0));
			if((strcmp(temp.c_str(),"char"))==0)
				SetTextColor(hDC,RGB(255,215,0));
			temp += stream[i];
			TextOut(hDC,currentPos*textProperties.tmAveCharWidth,y,temp.c_str(),temp.size());
			currentPos +=temp.size();
			SetTextColor(hDC,RGB(0,0,0));
			temp = "";
		}
		else
		temp += stream[i];
	}
	if((strcmp(temp.c_str(),"void"))==0)
		SetTextColor(hDC,RGB(255,0,0));
	if((strcmp(temp.c_str(),"int"))==0)
		SetTextColor(hDC,RGB(0,100,0));
	if((strcmp(temp.c_str(),"char"))==0)
		SetTextColor(hDC,RGB(255,215,0));
	if((strcmp(temp.c_str(),"class"))==0)
		SetTextColor(hDC,RGB(34,240,150));
	TextOut(hDC,currentPos*textProperties.tmAveCharWidth,y,temp.c_str(),temp.size());
	SetTextColor(hDC,RGB(0,0,0));
	temp = "";
}

void CustomEditControl::insertText(char character)
{
	char* tmp = textBuffer;
	/*
	*		Change Required Here
	*	related to using piece tables as backend
	*/
	if((strlen(textBuffer)+5)>bufferSize)
	{
		textBuffer = new char[bufferSize*2];
		ZeroMemory(textBuffer,sizeof(textBuffer));
		memcpy(textBuffer,tmp,bufferSize);
		bufferSize *=2;
	}
	tmp = textBuffer;

	ZeroMemory(textBuffer,sizeof(textBuffer));
	memcpy(textBuffer,tmp,counter);
	textBuffer[counter] = character;
}

void CustomEditControl::leftClick(LPARAM lParam)
{
	//we need to convert the coordinates to our logical coordinates so as to represent the caret on the logical lines
	//In order to do so, we first extract the coordinate from lParam, then we subtract from this value the modulus of the value by  one logical unit
	//this ensures that there is an illusion of lines in the editor
	//for example:
	//if the x coordinate is 7, and the logical unit for the x coordinate, ie the character width is 4, then
	//we extract 7 and subtract from it 7%4, ie:
	//caretPos.x = 7 - ( 7 % 4 )
	//			 = 7 - 3
    //			 = 4
	// as for the y coordinate, we do not calculate the exact coordinate, instead we opt for calculating the hypothetical line number.
	//this is not the real number, but it is a line number that is pointed ot by the y coordinate of the mouse clikc by the user
	//We will later check if the line number is greater than the total number of lines and then take appropriate action\
	//not calculating the actual y coordinate saves use some processing power
	caretPos.x = LOWORD(lParam) - (LOWORD(lParam))%textProperties.tmAveCharWidth;									//the x coordinate passed 
	int intendedLine= (HIWORD(lParam) - HIWORD(lParam)%(textProperties.tmHeight+textProperties.tmExternalLeading))/(textProperties.tmHeight+textProperties.tmExternalLeading);	//line number

	//Now we check for a special case when the user clicks somewhere after the last character in the buffer area
	if(intendedLine > lines.size()-1)//check if user has clicked after the last line
	{
		//place caret at the end of the last line
		caretPos.x = lines[lines.size()-1].length * textProperties.tmAveCharWidth;	//end of the last line
		caretPos.y = (lines.size()-1) * fontHeight; 
	}
	//now we look for another case where the user has clicked at a place above the y coordinate of the last line, but at an x coordinate that exists after the last character of the line
	else 
	{
		if(caretPos.x > lines[intendedLine].length * textProperties.tmAveCharWidth)
			caretPos.x = lines[intendedLine].length * textProperties.tmAveCharWidth;
		caretPos.y = intendedLine * fontHeight;
	}
}