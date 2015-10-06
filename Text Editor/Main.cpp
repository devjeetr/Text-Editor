#include <Windows.h>
#include "resource.h"
#include "CustomEditControl.h"
#include <sstream>

using std::string;
using std::ostringstream;
//define menu constants here


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CustomEditControlProc(HWND hwnd,  UINT uMsg, WPARAM wParam, LPARAM lParam);

CustomEditControl gpWnd;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wndClass;
	MSG msg;
	RECT rc;
	HWND mhWnd;
	

	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hPrevInstance);
	
	ZeroMemory(&wndClass,sizeof(WNDCLASSEX));

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = "Comment Generator";
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszMenuName = "IDR_MENU1";
	
	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	rc.left = 0;
	rc.bottom = 400;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, true);

	if(!RegisterClassEx(&wndClass))
	{
		MessageBoxA(NULL,"Window Registration Failed","Fatal Application Error",MB_ICONERROR);
		return -1;
	}
	
	if((mhWnd=CreateWindowEx(NULL,"Comment Generator", "Text Editor", WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,hMenu,hInstance,NULL))==0) 
	{
		MessageBoxA(NULL,"Window Creation Failed","Fatal Application Error",MB_ICONERROR);
		return -1;
	}
	
	gpWnd.InitCustomControl(mhWnd,CustomEditControlProc);

	ShowWindow(mhWnd,nCmdShow);
	UpdateWindow(mhWnd);

	ZeroMemory(&msg, sizeof(msg));

	while(msg.message!=WM_QUIT)
	{
		if(GetMessage(&msg,0,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ostringstream os;
	string str;
	
	switch(uMsg)
	{

	case WM_COMMAND:
		switch(wParam)
		{
		case ID_ABOUT_ABOUTDEDITOR:
			MessageBox(NULL, "Copyright@ Devjeet \n\nThis program is free software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program.  If not, see <http://www.gnu.org/licenses/>.","About dEditor",MB_OK);
			break;
		case ID_FILE_EXIT:
			SendMessage(hwnd,WM_DESTROY,NULL,NULL);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	case WM_SIZE:
		RECT rc;
		ZeroMemory(&rc,sizeof(rc));
		GetClientRect(hwnd, &rc);
		gpWnd.messageSizeWM(&rc);
		DefWindowProc(hwnd,uMsg,wParam,lParam);
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}

LRESULT CALLBACK CustomEditControlProc(HWND hwnd,  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch(uMsg)
	{
	case WM_CREATE:
		return gpWnd.messageCreateWM(hwnd,wParam,lParam);
	case WM_CHAR:
		return gpWnd.messageCharWM(hwnd,wParam,lParam);
	case WM_PAINT:
		return gpWnd.messagePaintWM(hwnd,wParam,lParam);
	case WM_SETFOCUS:
		return gpWnd.messageSetFocusWM(hwnd,wParam,lParam);
	case WM_LBUTTONDOWN:
		gpWnd.leftClick(lParam);
		SendMessage(hwnd,WM_SETFOCUS,(WPARAM)GetFocus(),NULL);
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}





