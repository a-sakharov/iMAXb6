#ifndef CHARGEMASTERNATIVE_H
#define CHARGEMASTERNATIVE_H

#include <Windows.h>
#include <stdint.h>
#include <process.h>
#include <wchar.h>
#include <Commctrl.h>
#include "iMAXb6.h"
#include "resource.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void PrintError();
void WorkerThread(void *lpThreadParameter);
void SetBatterySettings(uint8_t batteryType);

uint16_t QuenuedActions;
HWND Dialog;

enum QuenuedActionsList 
{
    READ_DEVICEINFO = 1 << 0,
    READ_SOMECHARGEDATA = 1 << 1,
    READ_CHARGEDATA = 1 << 2,
    STOP_PROCESS = 1 << 3,
    START_PROCESS = 1 << 4,
    SET_TEMPLIMIT = 1 << 5,
    SET_BUZZ = 1 << 6,
    SET_CAPLIMIT = 1 << 7,
    SET_TIMELIMIT = 1 << 8,
    SET_CYCLETIME = 1 << 9
};

#endif
