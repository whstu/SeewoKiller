#ifndef CMDCTRL_H
#define CMDCTRL_H

#include <windows.h>
void SetColorAndBackground(int ForgC, int BackC);
void gotoxy(long long x, long long y);
void setfont(int size);
extern HWND hwnd;

#endif
