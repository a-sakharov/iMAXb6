#ifndef IMAXB6_PRIVATE_H
#define IMAXB6_PRIVATE_H

#define IMAXB6_INNER_DLL

#ifdef _DEBUG
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#endif

#include "iMAXb6.h"
#include <hidapi.h>


#define IMAXB6_VID (0x0000)           
#define IMAXB6_PID (0x0001)           //yes, this is iMAX b6 vid&pid


#define CMD_GET_PROCESS_DATA (0x55) // 85
#define CMD_GET_DEVICE_DATA  (0x57) // 87
#define CMD_GET_CHARGE_DATA  (0x5A) // 90
#define CMD_UPDATE_FIRMWARE  (0x88) // 136


int iMAXb6CheckSum(uint8_t bytesCount, uint8_t *data, uint8_t startFrom);
int iMAXb6SendPacket(uint8_t code);

hid_device *iMAXb6Device;
#endif
