#include "iMAXb6-private.h"

#ifdef _DEBUG
void PrindBinaryData(uint16_t len, uint8_t *data)
{
    uint16_t offset = 0;
    uint8_t toPrint = 0;
    uint16_t i;

    while (len > offset)
    {
        printf("0x%.4X: ", offset);
        if (len - offset < 16)
        {
            toPrint = len - offset;
        }
        else
        {
            toPrint = 16;
        }

        for (i = 0; i < toPrint; i++)
        {
            printf("%.2X ", data[i + offset]);
        }

        if (toPrint < 16)
        {
            for (i = 0; i < 16-toPrint; i++)
            {
                printf("   ");
            }
        }

        printf("| ");

        for (i = 0; i < toPrint; i++)
        {
            printf("%c", isprint(data[i + offset]) ? data[i + offset] : '.');
        }

        offset += toPrint;

        printf("\n");
    }
}
#endif

int iMAXb6Init()
{
    if (hid_init())
    {
        return -1;
    }

    iMAXb6Device = hid_open(IMAXB6_VID, IMAXB6_PID, NULL);

    if (iMAXb6Device == NULL)
    {
        return -1;
    }

    return 0;
}

int iMAXb6Cleanup()
{
    if (iMAXb6Device)
    {
        hid_close(iMAXb6Device);
    }

    return hid_exit();
}

int iMAXb6SendPacket(uint8_t code, uint8_t *payload, uint8_t payloadLen)
{
    uint8_t i;
    uint8_t checksum = code;
    uint8_t packet[65] = {
        0x00, // sub-addr?
        0x0F, // ??
        0x03, // bytes count?
        code, // code
        0x00, // ??
        checksum, // checksum (packet[3]+packet[4])
        0xFF, // EOF?
        0xFF  // EOF?
    };
    
    if (payload)
    {
        packet[2] = 3 + payloadLen;
        for (i = 0; i < payloadLen; ++i)
        {
            packet[5 + i] = payload[i];
            checksum += payload[i];
        }

        packet[5 + i] = checksum;
        packet[5 + i + 1] = 0xFF;
        packet[5 + i + 2] = 0xFF;
    }

    return hid_write(iMAXb6Device, packet, 65);
}

int iMAXb6GetState(struct ChargeInfo *chargeState)
{
    uint8_t buffer[128];
    uint8_t *dataPntr = buffer + 4;
    int count;

    if (iMAXb6SendPacket(CMD_GET_PROCESS_DATA, NULL, 0) == -1)
    {
#ifdef _DEBUG
        printf("iMAXb6SendPacket error\n");
#endif
        return -1;
    }

    count = hid_read_timeout(iMAXb6Device, buffer, sizeof(buffer), 100);
    if ((count == -1) || (count == 0))
    {
#ifdef _DEBUG
        printf("hid_read_timeout error\n");
#endif
        return -1;
    }

#ifdef _DEBUG
#if 0
    PrindBinaryData(count, buffer);
    FILE *testfile = fopen("D:\\tempdump.bin", "wb");
    if (testfile)
    {
        fwrite(buffer, 1, count, testfile);
        fclose(testfile);
    }
#endif
#endif

    if (!iMAXb6CheckSum(buffer[1], buffer, 2))
    {
#ifdef _DEBUG
        printf("iMAXb6CheckSum error\n");
#endif
        return -1;
    }

    chargeState->workState = dataPntr[0];
    chargeState->ChargeMah = dataPntr[1] << 8 | dataPntr[2];
    chargeState->ChargeTimer = dataPntr[3] << 8 | dataPntr[4];
    chargeState->OutVoltage = dataPntr[5] << 8 | dataPntr[6];
    chargeState->Current = dataPntr[7] << 8 | dataPntr[8];
    chargeState->ExtTemp = dataPntr[9];
    chargeState->IntTemp = dataPntr[10];
    chargeState->Intimpedance = dataPntr[11] << 8 | dataPntr[12];
    chargeState->CELL1 = dataPntr[13] << 8 | dataPntr[14];
    chargeState->CELL2 = dataPntr[15] << 8 | dataPntr[16];
    chargeState->CELL3 = dataPntr[17] << 8 | dataPntr[18];
    chargeState->CELL4 = dataPntr[19] << 8 | dataPntr[20];
    chargeState->CELL5 = dataPntr[21] << 8 | dataPntr[22];
    chargeState->CELL6 = dataPntr[23] << 8 | dataPntr[24];
    chargeState->CELL7 = dataPntr[25] << 8 | dataPntr[26];
    chargeState->CELL8 = dataPntr[27] << 8 | dataPntr[28];

    return count;
}

int iMAXb6GetDeviceData(struct DeviceInfo *devInfo)
{
    uint8_t buffer[128];
    uint8_t *dataPntr = buffer + 5;
    int count;
    uint8_t i;
    uint8_t sum = 0;

    if (iMAXb6SendPacket(CMD_GET_DEVICE_DATA, NULL, 0) == -1)
    {
#ifdef _DEBUG
        printf("iMAXb6SendPacket error\n");
#endif
        return -1;
    }

    count = hid_read_timeout(iMAXb6Device, buffer, sizeof(buffer), 100);
    if ((count == -1) || (count == 0))
    {
#ifdef _DEBUG
        printf("hid_read_timeout error\n");
#endif
        return -1;
    }

#ifdef _DEBUG
#if 0
    PrindBinaryData(count, buffer);
    FILE *testfile = fopen("D:\\tempdump.bin", "wb");
    if (testfile)
    {
        fwrite(buffer, 1, count, testfile);
        fclose(testfile);
    }
#endif
#endif

    if (!iMAXb6CheckSum(buffer[1], buffer, 2))
    {
#ifdef _DEBUG
        printf("iMAXb6CheckSum error\n");
#endif
        return -1;
    }

    devInfo->machine_id[0] = 0;
    devInfo->core_type[0] = 0;

    for (i = 0; i < 16; i++)
    {
        if (i < 15)
        {
            sum += dataPntr[i];
        }

        sprintf(devInfo->machine_id + strlen(devInfo->machine_id), "%.2X", dataPntr[i]);

        if (i < 11)
        {
            sprintf(devInfo->core_type + strlen(devInfo->core_type), "%.2X", dataPntr[i]);
        }
    }

    if ((0x100 - sum) == dataPntr[15])
    {
        devInfo->upgrade_type = dataPntr[6];
        devInfo->is_encrypt = dataPntr[7] == 1;
        devInfo->customer_id = dataPntr[8] << 8 | dataPntr[9];
        devInfo->language_id = dataPntr[10];
        devInfo->software_version = (float)dataPntr[11] + ((float)dataPntr[12] / 100.0f);
        devInfo->hardware_version = (float)dataPntr[13];
        devInfo->reserved = dataPntr[14];
        devInfo->mctype = dataPntr[14];
        devInfo->checksum = dataPntr[15];
    }
    else
    {
#ifdef _DEBUG
        printf("sub-checksum error\n");
#endif
        return -1;
    }

    return count;
}

int iMAXb6GetChargeData(struct Chargedata *chargeData)
{
    uint8_t buffer[128];
    uint8_t *dataPntr = buffer + 4;
    int count;

    if (iMAXb6SendPacket(CMD_GET_CHARGE_DATA, NULL, 0) == -1)
    {
#ifdef _DEBUG
        printf("iMAXb6SendPacket error\n");
#endif
        return -1;
    }

    count = hid_read_timeout(iMAXb6Device, buffer, sizeof(buffer), 100);
    if ((count == -1) || (count == 0))
    {
#ifdef _DEBUG
        printf("hid_read_timeout error\n");
#endif
        return -1;
    }

#ifdef _DEBUG
#if 0
    PrindBinaryData(count, buffer);
    FILE *testfile = fopen("D:\\tempdump.bin", "wb");
    if (testfile)
    {
        fwrite(buffer, 1, count, testfile);
        fclose(testfile);
    }
#endif
#endif

    if (!iMAXb6CheckSum(buffer[1], buffer, 2))
    {
#ifdef _DEBUG
        printf("iMAXb6CheckSum error\n");
#endif
        return -1;
    }


    chargeData->CycleTime = dataPntr[0];
    chargeData->TimeLimitEnable = !!dataPntr[1];
    chargeData->TimeLimit = dataPntr[2] << 8 | dataPntr[3];
    chargeData->CapLimitEnable = !!dataPntr[4];
    chargeData->CapLimit = dataPntr[5] << 8 | dataPntr[6];
    chargeData->KeyBuzz = !!dataPntr[7];
    chargeData->SysBuzz = !!dataPntr[8];
    chargeData->InDClow = dataPntr[9] << 8 | dataPntr[10];
    //dataPntr[11];
    //dataPntr[12];
    chargeData->TempLimit = dataPntr[13];
    chargeData->Voltage = dataPntr[14] << 8 | dataPntr[15];
    chargeData->CELL1 = dataPntr[16] << 8 | dataPntr[17];
    chargeData->CELL2 = dataPntr[18] << 8 | dataPntr[19];
    chargeData->CELL3 = dataPntr[20] << 8 | dataPntr[21];
    chargeData->CELL4 = dataPntr[22] << 8 | dataPntr[23];
    chargeData->CELL5 = dataPntr[24] << 8 | dataPntr[25];
    chargeData->CELL6 = dataPntr[26] << 8 | dataPntr[27];

    return count;
}

int iMAXb6GetSomeChargeData(struct SomeChargeData *someChargeData)
{
    uint8_t buffer[128];
    uint8_t *dataPntr = buffer + 4;
    int count;

    if (iMAXb6SendPacket(CMD_UND_CHARGE_DATA, NULL, 0) == -1)
    {
#ifdef _DEBUG
        printf("iMAXb6SendPacket error\n");
#endif
        return -1;
    }

    count = hid_read_timeout(iMAXb6Device, buffer, sizeof(buffer), 100);
    if ((count == -1) || (count == 0))
    {
#ifdef _DEBUG
        printf("hid_read_timeout error\n");
#endif
        return -1;
    }

#ifdef _DEBUG
#if 0
    PrindBinaryData(count, buffer);
    FILE *testfile = fopen("D:\\tempdump.bin", "wb");
    if (testfile)
    {
        fwrite(buffer, 1, count, testfile);
        fclose(testfile);
    }
#endif
#endif

    if (!iMAXb6CheckSum(buffer[1], buffer, 2))
    {
#ifdef _DEBUG
        printf("iMAXb6CheckSum error\n");
#endif
        return -1;
    }

    someChargeData->a = (float)dataPntr[4];
    someChargeData->b = (float)dataPntr[5];
    someChargeData->c = (uint8_t)dataPntr[0];

    return count;
}

int iMAXbStartProcess(struct ProcessParams *processParams)
{
    uint8_t buffer[128];
    uint8_t *dataPntr = buffer + 4;
    int count;

    buffer[0] = processParams->BattType;
    buffer[1] = processParams->Cell;

    if (processParams->BattType < 4)
    {
        buffer[2] = processParams->LiPwmMode;
    } 
    else if (processParams->BattType < 6)
    {
        buffer[2] = processParams->NiPwmMode;
    }
    else
    {
        buffer[2] = processParams->PbPwmMode;
    }

    buffer[3] = ((processParams->CCurrent) >> 8) & 0xFF;
    buffer[4] = (processParams->CCurrent) & 0xFF;
    buffer[5] = ((processParams->DCurrent) >> 8) & 0xFF;
    buffer[6] = (processParams->DCurrent) & 0xFF;
    buffer[7] = ((processParams->CellVoltage) >> 8) & 0xFF;
    buffer[8] = (processParams->CellVoltage) & 0xFF;
    buffer[9] = ((processParams->EndVoltage) >> 8) & 0xFF;
    buffer[10] = (processParams->EndVoltage) & 0xFF;

    if (processParams->NiPwmMode > 2 && processParams->BattType > 3 && processParams->BattType < 6)
    {
        if (processParams->NiPwmMode == 3)
        {
            buffer[11] = processParams->R_PeakCount;
            buffer[12] = 0;
        }
        else if (processParams->NiPwmMode == 4)
        {
            buffer[11] = processParams->CycleType;
            buffer[12] = processParams->Cyc_count;
        }
    }
    else
    {
        buffer[11] = 0;
        buffer[12] = 0;
    }

    buffer[13] = ((processParams->Trickle) >> 8) & 0xFF;
    buffer[14] = (processParams->Trickle) & 0xFF;
    buffer[15] = 0;
    buffer[16] = 0;
    buffer[17] = 0;
    buffer[18] = 0;

    if (iMAXb6SendPacket(CMD_START_WITH_PARAMS, buffer, 19) == -1)
    {
#ifdef _DEBUG
        printf("iMAXb6SendPacket error\n");
#endif
        return -1;
    }

    count = hid_read_timeout(iMAXb6Device, buffer, sizeof(buffer), 100);
    if ((count == -1) || (count == 0))
    {
#ifdef _DEBUG
        printf("hid_read_timeout error\n");
#endif
        return -1;
    }

#ifdef _DEBUG
#if 0
    PrindBinaryData(count, buffer);
    FILE *testfile = fopen("D:\\tempdump.bin", "wb");
    if (testfile)
    {
        fwrite(buffer, 1, count, testfile);
        fclose(testfile);
    }
#endif
#endif

    if (!iMAXb6CheckAck(buffer))
    {
#ifdef _DEBUG
        printf("iMAXb6CheckAck error\n");
#endif
        return -1;
    }
    
    return count;
}

int iMAXb6CheckAck(uint8_t *data)
{
    if ((data[0] == 0xF0) && (data[1] == 0xFF) && (data[2] == 0xFF))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int iMAXb6CheckSum(uint8_t bytesCount, uint8_t *data, uint8_t startFrom)
{
    uint8_t i;
    uint8_t sum = 0;

    for (i = startFrom; i < (bytesCount + 1); ++i)
    {
        sum += data[i];
    }

    if (sum == data[i])
    {
        return 1;
    }

    return 0;
}
