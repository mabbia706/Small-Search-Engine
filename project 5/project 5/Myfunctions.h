#pragma once
#include <Windows.h>
void gotoxy(int x, int y)
{
	static HANDLE hStdout = NULL;
	COORD coord;

	coord.X = x;
	coord.Y = y;

	if(!hStdout)
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	SetConsoleCursorPosition(hStdout,coord);
}

void clrscr(void)
{
	static HANDLE hStdout = NULL;      
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = {0,0};   
	DWORD dummy;

	if(!hStdout)               
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hStdout,&csbi);
	}

	FillConsoleOutputCharacter(hStdout,
		' ',
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);    
	gotoxy(0,0);
}
int textcolor(int c)
{
	static HANDLE hStdout = NULL;
	if(!hStdout)
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	return
		SetConsoleTextAttribute(hStdout,c);
}

void showcursor(bool b)
{
	static HANDLE hStdout = NULL;
	if(!hStdout)
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	CONSOLE_CURSOR_INFO   cursorInfo;
	cursorInfo.dwSize   = 1;
	cursorInfo.bVisible = b;

	SetConsoleCursorInfo( hStdout, &cursorInfo );
}

bool SetWindow(int Width, int Height) 
{ 
	_COORD coord; 
	coord.X = Width; 
	coord.Y = Height; 

	_SMALL_RECT Rect; 
	Rect.Top = 0; 
	Rect.Left = 0; 
	Rect.Bottom = Height - 1; 
	Rect.Right = Width - 1; 

	// Get handle of the standard output 
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE); 
	if (Handle == NULL) 
	{ 
		cout<<"Failure in getting the handle\n"<<GetLastError(); 
		return FALSE; 
	} 

	// Set screen buffer size to that specified in coord 
	if(!SetConsoleScreenBufferSize(Handle, coord)) 
	{ 
		cout<<"Failure in setting buffer size\n"<<GetLastError(); 
		return FALSE; 
	} 

	// Set the window size to that specified in Rect 
	if(!SetConsoleWindowInfo(Handle, TRUE, &Rect)) 
	{ 
		cout<<"Failure in setting window size\n"<<GetLastError(); 
		return FALSE; 
	} 

	return TRUE; 
} 

