/*
 * uart.c
 *
 *  Created on: Jul 22, 2025
 *      Author: Anamitra Sarkar
 */

#include "uart.h"

    uint16_t Start_Address;
    uint16_t DataSize;

    uint16_t crcACKglobal[4] = {0x0000};

    int harmonicChunk = 0;


//    bool MemRead = FALSE;
    bool MemRead = TRUE;


    bool TransmitData = FALSE;

    int MemReadCount = 0;
    uint16_t r_Buff2[1024] = {0x0000};

void handle_Display_uart(uint16_t r_Buff[], uint16_t size)
{

    int k = 0;
    int n = 0;

    if(r_Buff[0] == 0x003A || r_Buff[0] == 0x002A)
    {
        for(n = 0; n < size; n++)
        {
            r_Buff2[n] = r_Buff[n];
        }
    }
    else{
        for(k = 0; k < size; k++)
        {
            if(r_Buff[k] == 0x003A) break;
        }
        for(n = 0; n < size; n++)
        {
            r_Buff2[n] = r_Buff[n + k];
        }
    }

    if(r_Buff2[0] == 0x003A && r_Buff2[(size - k) - 3] == 0x00EF)
    {

        uint16_t crcCheck = crc16(r_Buff2, size - k);

        if(crcCheck == 0x0000)
        {
            Start_Address = r_Buff2[1]*256 + r_Buff2[2];
            DataSize = r_Buff2[3]*256 + r_Buff2[4];

            if(Start_Address == (uint16_t)BUFF_ADR_SET_VAC)
            {
                uint16_t p = 0;
                for(p = 0; p < DataSize; p++)
                {
                    Receive_Buf_Secondary[Start_Address + p] = r_Buff2[5 + p];
                    cpu2Write[Start_Address + p] = r_Buff2[5 + p];
                }
                MemDataUpdate();
            }
            else
            {
                uint16_t p = 0;
                for(p = 0; p < DataSize; p++)
                {
                    Receive_Buf_Secondary[Start_Address + p] = r_Buff2[5 + p];
                }
                HarmonicTableUpdate(Start_Address, DataSize);

                uint16_t q = 0;
                for(q = 0; q < MAX_LENGTH; q++)
                {
                    Receive_Buf_Primary[q] = 0x0000;
                }

//                harmonicChunk++;
//
//                if(harmonicChunk == 5)
//                {
//                    ackDataReceive();
//                    harmonicChunk = 0;
//                }
//                else{}
            }

        }

    }


    else if(r_Buff[0] == 0x002A && r_Buff[size - 3] == 0x00EF)
    {
        uint16_t crcCheck = crc16(r_Buff, size);

        if(crcCheck == 0x0000)
        {

            Start_Address = r_Buff[1]*256 + r_Buff[2];
            DataSize = r_Buff[3]*256 + r_Buff[4];

            transmitBuff[0] = 0x002A;
            transmitBuff[1] = r_Buff[1];
            transmitBuff[2] = r_Buff[2];
            transmitBuff[3] = r_Buff[3];
            transmitBuff[4] = r_Buff[4];

            uint16_t i = 0;
            for(i = 0; i < DataSize; i++)
            {
//                transmitBuff[5 + i]  =  Receive_Buf_Secondary[Start_Address + i] ;
                transmitBuff[5+i]  = 0x000A;
            }

            transmitBuff[5 + DataSize] = 0x00EF;

            crcCheck = crc16(transmitBuff, (uint16_t)(6U + DataSize));
            transmitBuff[6 + DataSize] = (uint16_t)(crcCheck & 0x00FF); // CRC Low
            transmitBuff[7 + DataSize] = (uint16_t)((crcCheck >> 8) & 0x00FF); //CRC High


            int j = 0;
//                for(j = 0; j < (sizeof(transmitBuff)/sizeof(transmitBuff[0])); j++)
            for(j = 0; j < (int)(8U + DataSize); j++)
            {
                SCI_writeCharBlockingFIFO(SCIB_BASE, transmitBuff[j]);
                while(SCI_getTxFIFOStatus(SCIB_BASE) != SCI_FIFO_TX0){}
            }

//            MemRead = true;
//            TransmitData = TRUE;
        }

    }


}

void send_Data_to_STM(void)
{
//    const DataMapEntry *Start_entry = &Measure_Input_dataMap[0];
//    const DataMapEntry *End_entry = &Measure_Input_dataMap[NUM_ENTRIES_MEAS_IN];

#if CONVERTER_TYPE == SINGLE_PHASE

    uint16_t StartIdx = (uint16_t)BUFF_ADR_MEAS_VOLT;
    uint16_t EndIdx = (uint16_t)BUFF_END_ADDR;

#elif CONVERTER_TYPE == THREE_PHASE

    uint16_t StartIdx = (uint16_t)BUFF_ADR_MEAS_VA_RMS;
    uint16_t EndIdx = (uint16_t)BUFF_END_ADDR;
#endif


//    uint16_t StartIdx = Start_entry->BUF_ADR;


//    uint16_t Length = (uint16_t)(End_entry->BUF_ADR - Start_entry->BUF_ADR) ;
    uint16_t Length = EndIdx - StartIdx;

    transmitBuff[0] = 0x002A;
    transmitBuff[1] = ((StartIdx >> 8) & 0x00FF);
    transmitBuff[2] = (StartIdx & 0x00FF);
    transmitBuff[3] = ((Length >> 8) & 0x00FF);
    transmitBuff[4] = (Length & 0x00FF);

    int i;
    for(i = 0; i < Length; i++)
    {
        transmitBuff[5 + i] = cpu2Read[StartIdx + i];
    }

    transmitBuff[5 + Length] = 0x00EF;

    // compute CRC
    uint16_t crc = crc16(transmitBuff, (uint16_t)(6U + Length));

    // append CRC (Modbus order: Low, then High)
    transmitBuff[6 + Length] = (uint8_t)(crc & 0xFF);          // CRC Low
    transmitBuff[7 + Length] = (uint8_t)((crc >> 8) & 0xFF);   // CRC High

    uint16_t frame_len = 8U + Length;

//    uint16_t frame_len ;
    uint16_t j = 0;

    for(j = 0; j < frame_len; j++)
    {
        SCI_writeCharBlockingFIFO(SCIB_BASE, transmitBuff[j]);
//        while(SCI_getTxFIFOStatus(SCIB_BASE) != SCI_FIFO_TX0){}
    }

}

static inline void ackDataReceive(void)
{
    uint16_t ackBuf[4] = {0x004F, 0x004B, 0x0000, 0x0000};
    uint16_t crcCheck = crc16(ackBuf, (uint16_t)2u);
    ackBuf[2] = (uint16_t)(crcCheck & 0x00FF); // CRC Low
    ackBuf[3] = (uint16_t)((crcCheck >> 8) & 0x00FF); //CRC High

    int j = 0;
    for(j = 0; j < 4; j++)
    {
        crcACKglobal[j] = ackBuf[j];
        SCI_writeCharBlockingFIFO(SCIB_BASE, ackBuf[j]);
        while(SCI_getTxFIFOStatus(SCIB_BASE) != SCI_FIFO_TX0){}
//        SCI_writeCharBlockingFIFO(SCIC_BASE, ackBuf[j]);
//        while(SCI_getTxFIFOStatus(SCIC_BASE) != SCI_FIFO_TX0){}
        ackBuf[j] = 0x0000;
    }
}


void ReadDatafromEEPROM(void)
{
#if CONVERTER_TYPE == SINGLE_PHASE
    uint16_t StartIdx = (uint16_t)BUFF_ADR_SET_VAC;
    uint16_t EndIdx = (uint16_t)BUFF_ADR_MEAS_VOLT;
#elif CONVERTER_TYPE == THREE_PHASE
    uint16_t StartIdx = (uint16_t)BUFF_ADR_SRC_VA;
    uint16_t EndIdx = (uint16_t)BUFF_ADR_MEAS_VA_RMS;
#endif
    uint16_t Length = EndIdx - StartIdx;

    transmitBuff[0] = 0x002A;
    transmitBuff[1] = ((StartIdx >> 8) & 0x00FF);
    transmitBuff[2] = (StartIdx & 0x00FF);
    transmitBuff[3] = ((Length >> 8) & 0x00FF);
    transmitBuff[4] = (Length & 0x00FF);
    int i;
    for(i = 0; i < Length; i++)
    {
        transmitBuff[5 + i] = 0x0000;
    }
    transmitBuff[5 + Length] = 0x00EF;

    // compute CRC
    uint16_t crc = crc16(transmitBuff, (uint16_t)(6U + Length));

    // append CRC (Modbus order: Low, then High)
    transmitBuff[6 + Length] = (uint8_t)(crc & 0xFF);          // CRC Low
    transmitBuff[7 + Length] = (uint8_t)((crc >> 8) & 0xFF);   // CRC High

    uint16_t frame_len = 8U + Length;
    uint16_t j = 0;

    for(j = 0; j < frame_len; j++)
    {
        SCI_writeCharBlockingFIFO(SCIB_BASE, transmitBuff[j]);
    }
}








