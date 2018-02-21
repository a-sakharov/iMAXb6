#ifndef IMAXB6_PRIVATE_H
#define IMAXB6_PRIVATE_H

#define IMAXB6_INNER_DLL

#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#include <ctype.h>
#endif


#include "iMAXb6.h"
#include <string.h>
#include <stdio.h>
#include <hidapi.h>


#define IMAXB6_VID (0x0000)           
#define IMAXB6_PID (0x0001)           //yes, this is iMAX b6 vid&pid


#define CMD_START_WITH_PARAMS (0x05) // 5
#define CMD_SEND_SETTINGS     (0x11) // 17
#define CMD_GET_PROCESS_DATA  (0x55) // 85
#define CMD_GET_DEVICE_DATA   (0x57) // 87
#define CMD_GET_CHARGE_DATA   (0x5A) // 90
#define CMD_GET_MAX_CURRENT   (0x5F) // 95
#define CMD_UPDATE_FIRMWARE   (0x88) // 136
#define CMD_STOP              (0xFE) // 254

#define SET_CYCLE_TIME (0)
#define SET_TIME_LIMIT (1)
#define SET_CAP_LIMIT  (2)
#define SET_BUZZ       (3)
//4?
#define SET_TEMP_LIMIT (5)

int iMAXb6CheckSum(uint8_t bytesCount, uint8_t *data, uint8_t startFrom);
int iMAXb6SendPacket(uint8_t code, uint8_t subCode, uint8_t *payload, uint8_t payloadLen);
int iMAXb6CheckAck(uint8_t *data);

hid_device *iMAXb6Device;

#endif
