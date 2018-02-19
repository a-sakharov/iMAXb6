#ifndef CHARGEMASTERNATIVE_H
#define CHARGEMASTERNATIVE_H

#include <Windows.h>
#include <stdint.h>
#include <process.h>
#include <wchar.h>
#include "iMAXb6.h"
#include "resource.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void PrintError();
void WorkerThread(void *lpThreadParameter);

uint16_t QuenuedActions;
HWND Dialog;

enum QuenuedActionsList 
{
    READ_DEVICEINFO = 1 << 0,
    READ_SOMECHARGEDATA = 1 << 1,
    READ_CHARGEDATA = 1 << 2,
    STOP_PROCESS = 1 << 3,
    START_PROCESS = 1 << 4
};

#endif
