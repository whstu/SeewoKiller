#ifndef CMDCTRL_H
#define CMDCTRL_H

#include "./main.h"
using namespace std;
void SetColorAndBackground(int ForgC, int BackC);
void gotoxy(long long x, long long y);
void setfont(int size);
void prints(const string& taskname,int result,int taskColorForg=7,int taskColorBack=0, const string& connectString = " - ");
extern HWND hwnd;

#endif
