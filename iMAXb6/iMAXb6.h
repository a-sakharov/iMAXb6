#ifndef IMAXB6_H
#define IMAXB6_H

#include <stdint.h>

#ifdef IMAXB6_INNER_DLL
#define IMAXB6_FUNC __declspec(dllexport)
#else
#define IMAXB6_FUNC __declspec(dllimport)
#endif

IMAXB6_FUNC int iMAXb6Init();
IMAXB6_FUNC int iMAXb6Cleanup();
IMAXB6_FUNC int iMAXb6GetState(struct ChargeInfo *chargeState);
IMAXB6_FUNC int iMAXb6GetDeviceData(struct DeviceInfo *devInfo);
IMAXB6_FUNC int iMAXb6GetChargeData(struct Chargedata *chargeData);
IMAXB6_FUNC int iMAXb6GetSomeChargeData(struct SomeChargeData *someChargeData);
IMAXB6_FUNC int iMAXbStartProcess(struct ProcessParams *processParams);

struct ChargeInfo
{
    uint8_t workState;      // some state. 1 - charging/discharging, 2 - no process, 3 - end of process caused by discharge cutoff/by charge finish, 4 - error, check main port
    uint16_t ChargeMah;     // current, mAh
    uint16_t ChargeTimer;   // seconds from start
    uint16_t OutVoltage;    // voltage on battery, mV
    int16_t Current;        // current, mA
    int8_t ExtTemp;         // temperature on battery, C
    int8_t IntTemp;         // temperature on charger, C
    uint16_t Intimpedance;  // battery impedance?
    uint16_t CELL1;         // ??
    uint16_t CELL2;         // ??
    uint16_t CELL3;         // ??
    uint16_t CELL4;         // ??
    uint16_t CELL5;         // ??
    uint16_t CELL6;         // ??
    uint16_t CELL7;         // ??
    uint16_t CELL8;         // ??
};

struct DeviceInfo
{
    char core_type[23];     // ??
    uint8_t upgrade_type;   // ?? booltoader type??
    uint8_t is_encrypt;     // ?? bool
    uint16_t customer_id;   // ??
    uint8_t language_id;    // ??
    float software_version; // FW version
    float hardware_version; // HW ver?
    uint8_t reserved;       // ??
    uint8_t mctype;         // ??
    uint8_t checksum;       // ??
    char machine_id[33];    // ??
};

struct Chargedata
{
    uint8_t CycleTime;       //charge->discharge rest time, min
    uint8_t TimeLimitEnable; //bool
    uint16_t TimeLimit;      //safety timer value, min
    uint8_t CapLimitEnable;  //bool
    uint16_t CapLimit;       //capacity cut-off, mAh
    uint8_t KeyBuzz;         //bool, key beep value
    uint8_t SysBuzz;         //bool, buzzer value
    uint16_t InDClow;        //input power low value, mV
    uint8_t TempLimit;       //temp cut-off value, C
    uint16_t Voltage;        //?
    uint16_t CELL1;          //?
    uint16_t CELL2;          //?
    uint16_t CELL3;          //?
    uint16_t CELL4;          //?
    uint16_t CELL5;          //?
    uint16_t CELL6;          //?
};

struct SomeChargeData
{
    float a;
    float b;
    uint8_t c;
};

struct ProcessParams
{
    uint8_t  BattType;       //hi-group of battery types?
    uint8_t  Cell;           //Cell count?
    uint8_t  LiPwmMode;      //low group of bat types?
    uint8_t  NiPwmMode;      //low group of bat types?
    uint8_t  PbPwmMode;      //low group of bat types?
    uint16_t CCurrent;       //charge current in mA?
    uint16_t DCurrent;       //discharge current in mA?
    uint16_t CellVoltage;    //min cell voltage?
    uint16_t EndVoltage;     //max cell voltage?
    uint8_t  R_PeakCount;    //?
    uint8_t  CycleType;      //?
    uint8_t  Cyc_count;      //?
    uint16_t Trickle;        //?
};
#endif
