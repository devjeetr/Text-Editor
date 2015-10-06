#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "LinkedList.h"

using std::vector;
using std::string;

struct lineStruct
{
	int length;
	int offset;
};






/*

				This is the main custom edit control part of the program. This is the backbone of the program. It handles
				all of the GUI elements and more. This is the most important but at the some time largest and unfortunately disorganised
				part of this program. It needs some work to hide the implementation details.

				Tread with care. Memory leaks are unacceptable. Work is needed to minimize the number of class variables in order 
				to prevent the risk of memory leaks.

				Track all variables through their lifecycle. Be careful with Dynamic Memory allocation. 
				
				No changes can be made to the orginal source code without my permission(Devjeet).
				All changes and revisions have to go through me.


				Functions : 22
				Variables : 24 
*/

class CustomEditControl
{
public:
	CustomEditControl();

	//init functions

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialises the custom control. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hWndParent">	Handle of the window parent. </param>
	/// <param name="wndProc">   	The window proc with which the control is to be created </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void InitCustomControl(HWND hWndParent,WNDPROC wndProc);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialises the back buffer. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle of the window with which the backbuffer is to be made compatible(handle to the main edit control) </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void InitBackBuffer(HWND hwnd);

	//Backbuffer

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Presents the buffer. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hDC">	The device-context to which the backbuffer is to be painted </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void PresentBuffer(HDC hDC);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Resizes the backbuffer(to same size as mWidth, mHeight). </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void resizeBackbuffer();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Prepares back buffer. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void prepareBackBuffer();

	//Message handler functions

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles WM_CHAR message </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">  	Handle of the window which received the message </param>
	/// <param name="wParam">	The wParam field of the message. </param>
	/// <param name="lParam">	The lParam field of the message. </param>
	///
	/// <returns>	true if successful </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LRESULT messageCharWM(HWND hwnd,WPARAM wParam,LPARAM lParam);						//Handles WM_CHAR

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles WM_PAINT message </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">  	Handle of the window that receives the message </param>
	/// <param name="wParam">	The wParam field of the message. </param>
	/// <param name="lParam">	The lParam field of the message. </param>
	///
	/// <returns>	true if successful. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LRESULT messagePaintWM(HWND hwnd, WPARAM wParam, LPARAM lParam);					//Handles WM_PAINT

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles WM_FOCUS </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">  	Handle of the window that receives the message </param>
	/// <param name="wParam">	The wParam field of the message. </param>
	/// <param name="lParam">	The lParam field of the message. </param>
	///
	/// <returns>	. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LRESULT messageSetFocusWM(HWND hwnd, WPARAM wParam, LPARAM lParam);					//handles WM_SETFOCUS

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles WM_CREATE message </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">  	Handle of the window that receives the message </param>
	/// <param name="wParam">	The wParam field of the message. </param>
	/// <param name="lParam">	The lParam field of the message. </param>
	///
	/// <returns>	. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LRESULT messageCreateWM(HWND hwnd,WPARAM wParam,LPARAM lParam);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles MW_SIZE message </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="rc">	The rectangle that represents the client area of the parent window </param>
	///
	/// <returns>	. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	LRESULT messageSizeWM(LPRECT rc);



	//utility functions

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a width up to a character in a line </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">			Handle of the window </param>
	/// <param name="character">	The character of which the width is to be found </param>
	///
	/// <returns>	The width up till which the width is to be found </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int getWidth(HWND hwnd, char character);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a line width. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">		 	Handle of the window </param>
	/// <param name="lineNumber">	The line number. </param>
	///
	/// <returns>	The line width. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int getLineWidth(HWND hwnd,int lineNumber);



	//word analyzer functions

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Analyze and print. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hDC">   	The device-context. </param>
	/// <param name="stream">	The character stream to be printed </param>
	/// <param name="length">	The length of the stream </param>
	/// <param name="y">	 	The y coordinate of the line </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void AnalyzeAndPrint(HDC hDC,char stream[],int length, int y);
	


	//input handler functions

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Deletes the character a character </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle to the window </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void deleteChar(HWND hwnd);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a line. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle to the window </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void addLine(HWND hwnd);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Deletes the line  </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle to the window </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void deleteLine(HWND hwnd);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Moves the caret left </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle of the. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void moveLeft(HWND hwnd);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Moves the caret up a line. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="hwnd">	Handle of the window </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void moveCaretUpALine(HWND hwnd);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a character to the end </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="wParam">	The character </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void addCharacter(char wParam);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inserts a character at the given caret position </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="character">	The character. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void insertText(char character);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles	Left click. </summary>
	///
	/// <remarks>	Devjeet, 11/27/2011. </remarks>
	///
	/// <param name="lParam">	The lParam field of the message WM_LBUTTONDOWN </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void leftClick(LPARAM lParam);

	~CustomEditControl(void);	
private:
	int mHeight;
	int mWidth;

	int cx;
	int cy;
	
	string szClassName;						
	
	HFONT hEditorFont;
	int fontHeight;

	HWND hWnd;
	
	COLORREF foreGroundColor;
	COLORREF backgroundColor; 
	
	POINT fontProperty;
	POINT caretPos;
	
	unsigned int counter;					//current position of the buffer
	int currentWord;						//total current word under operation
	unsigned int currentLine;	
	vector<lineStruct> lines;
	
	RECT updateRegion;

	unsigned int bufferSize;				//total size of the buffer
	char* textBuffer;
	
	TEXTMETRIC textProperties;
	
	LinkedList tBuffer;

	HDC memDC;
	HBITMAP mSurface;

	//memory leak expected. Remember to delete
	char* fileReadBuffer;					//remember to delete this buffer. Always remember to read this buffers
};

