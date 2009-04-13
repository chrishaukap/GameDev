#if !defined MAIN_H
#define MAIN_H
//------------------------------------
//  main.h
//  Generic Windows Program
//  (c) Reliable Software, 1997, 98
//------------------------------------

#include <windows.h>

LRESULT CALLBACK MainWndProc
   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK AboutDlgProc
   (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif

