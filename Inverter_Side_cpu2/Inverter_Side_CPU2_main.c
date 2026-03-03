/*
 * Inverter_Side_CPU2_main.c
 *
 *  Created on: 18-Apr-2025
 *      Author: Anamitra Sarkar
 */


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "device.h"
#include "driverlib.h"
#include "Device/driverlib/sci.h"
#include "Device/driverlib/inc/hw_memmap.h"

#include "uart.h"
#include "Control_Variables.h"
#include "Common_Memmap.h"
#include "Config.h"
#include "Ref_Gen.h"


volatile SharedFlags cpu2Write_Flags;
volatile int cpu2Read_Flag;
volatile uint16_t cpu2Read[MAX_LENGTH];
volatile uint16_t cpu2Write[MAX_LENGTH];


volatile signed int BaseLUT_A[LUT_SIZE];
volatile signed int BaseLUT_B[LUT_SIZE];
volatile signed int BaseLUT_C[LUT_SIZE];
volatile float32_t HarmonicA[MAX_HARMONIC_NO][3];
volatile float32_t HarmonicB[MAX_HARMONIC_NO][3];
volatile float32_t HarmonicC[MAX_HARMONIC_NO][3];


#pragma DATA_SECTION(cpu2Write, "SHARERAMGS6");
#pragma DATA_SECTION(cpu2Read, "SHARERAMGS7");
#pragma DATA_SECTION(cpu2Write_Flags,"SHARERAMGS8");
#pragma DATA_SECTION(cpu2Read_Flag,"SHARERAMGS9");

#pragma DATA_SECTION(BaseLUT_A, "SHARERAMGS10");
#pragma DATA_SECTION(BaseLUT_B, "SHARERAMGS10");
#pragma DATA_SECTION(BaseLUT_C, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicA, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicB, "SHARERAMGS10");
#pragma DATA_SECTION(HarmonicC, "SHARERAMGS10");

volatile float32_t Angle_Step;


uint16_t Receive_Buf_Primary[MAX_LENGTH];  // to handle incoming data from IOD, connected to SCIB
uint16_t Receive_Buf_Primary1[MAX_LENGTH];  // to handle incoming data from PFC, connected to SCIC
uint16_t Receive_Buf_Secondary[MAX_LENGTH];
uint16_t tempArr[MAX_LENGTH] = {0x0000};


uint16_t  receivedChar; // to handle incoming data from IOD, connected to SCIB
uint16_t  receivedChar1; // to handle incoming data from PFC, connected to SCIC
uint16_t index = 0; // to handle incoming data from IOD, connected to SCIB
uint16_t index1 = 0; // to handle incoming data from PFC, connected to SCIC
int check = 0;
int check1 = 0;
int check1_prev = 0;
uint16_t count = 0;
int Delay_counter;

//uint16_t SendBuffer[256] = {0x0000};
uint16_t transmitBuff[MAX_LENGTH] = {0x0000};

//********CRC variables***********//
bool CRCvar = false;
int CRCindex = 0;
uint16_t CRCbuff[2] = {0x0000};


//*******Function Prototypes**********//
void config_SCI_interrupt(uint32_t base);
__interrupt void scib_isr(void);  // for SCI B
__interrupt void scic_isr(void);  // for SCI C
void Read_Data_from_Shared_Memory(void);


void main(void)
{

    Device_init();
    Interrupt_initModule();
    Interrupt_initVectorTable();

//    //********Booting CPU2 after CPU1************//

    IPC_sync(IPC_CPU2_L_CPU1_R, IPC_FLAG31);

//    while((HWREGH(MEMCFG_BASE + MEMCFG_O_GSXMSEL) &
//            (MEMCFG_GSXMSEL_MSEL_GS6 |MEMCFG_GSXMSEL_MSEL_GS8)) == 0)
//    {
//    }
//
//    IPC_clearFlagLtoR(IPC_CPU2_L_CPU1_R, IPC_FLAG_ALL);
//
//    while(!(HWREG(IPC_CPUXTOCPUX_BASE + IPC_O_CPU1TOCPU2IPCSET) & (1UL << ipcFlag17)))
//    {
//    }
//    HWREG(IPC_CPUXTOCPUX_BASE + IPC_O_CPU2TOCPU1IPCACK) = 1UL << ipcFlag17;


     //********For SCI B **********//

     Interrupt_register(INT_SCIB_RX, scib_isr);
     config_SCI_interrupt(SCIB_BASE);
     Interrupt_enable(INT_SCIB_RX);

     Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

//    //********For SCI C **********//

    Interrupt_register(INT_SCIC_RX, scic_isr);
    config_SCI_interrupt(SCIC_BASE);
    Interrupt_enable(INT_SCIC_RX);

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP8);


    EINT;
    ERTM;

    int j = 0;
    for(j = 0; j < 256; j++)
    {
        Receive_Buf_Primary[j] = '\0';
    }

    cpu2Write_Flags.VarAddr[0] = 0x0000;
    cpu2Write_Flags.VarAddr[1] = 0x0000;
    cpu2Write_Flags.dataLength[0] = 0x0000;
    cpu2Write_Flags.dataLength[1] = 0x0000;

    Angle_Step = TWO_PI / (float)LUT_SIZE;

    Harmonic_Array_clear();
    Harmonic_Array_Init();
    updateBaseLookUpTable();


    while(1)
    {
        ReadMeasureDataFromSharedMemory();
//        updateBaseLookUpTable();

        if(Delay_counter==5)
        {
            send_Data_to_STM();
            Delay_counter = 0;
        }
        Delay_counter++;
//        if(ProcessDataFlag == 1)
//        {
//            send_Data_to_Display();
//            ProcessDataFlag = 0;
//        }



//        if(TransmitData)
//        {

//            DEVICE_DELAY_US(500000);

//        }
//        else{}
    }
}


void config_SCI_interrupt(uint32_t base)
{
    SCI_performSoftwareReset(base);
    SCI_setConfig(base, 50000000, 9600, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(base);
    SCI_resetRxFIFO(base);
    SCI_resetTxFIFO(base);

    SCI_clearOverflowStatus(base);
    SCI_clearInterruptStatus(base, SCI_INT_TXFF |
                                   SCI_INT_RXERR |
                                   SCI_INT_RXFF);
//    SCI_clearInterruptStatus(base, SCI_INT_RXERR);
//    SCI_clearInterruptStatus(base, SCI_INT_RXFF);

    SCI_enableFIFO(base);

    SCI_enableInterrupt(base, SCI_INT_RXFF);
    SCI_setFIFOInterruptLevel(base, SCI_FIFO_TX1, SCI_FIFO_RX1);

    SCI_enableModule(base);
    SCI_performSoftwareReset(base);
}


//********For SCI B  connected to IOD**********//

__interrupt void scib_isr(void)
{


    uint32_t istat = SCI_getInterruptStatus(SCIB_BASE);

    // Handle RX errors first (framing/parity/break). These suppress further RX interrupts until cleared.
    if (istat & SCI_INT_RXERR)
    {
        volatile uint16_t dump = HWREGH(SCIB_BASE + SCI_O_RXBUF); // read to pop error char
        (void)dump;
        SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_RXERR);
        SCI_resetRxFIFO(SCIB_BASE);          // flush garbage
        SCI_clearOverflowStatus(SCIB_BASE);  // clear RXFFOVF if it happened
    }

    // Drain the FIFO completely so the next interrupt can re-arm
    while (SCI_getRxFIFOStatus(SCIB_BASE) != SCI_FIFO_RX0)
    {
        receivedChar = (uint16_t)(SCI_readCharNonBlocking(SCIB_BASE) & 0xFF);
        SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_RXFF);
        SCI_clearOverflowStatus(SCIB_BASE);

        if(!CRCvar)
        {
            Receive_Buf_Primary[index] = (uint8_t)receivedChar;
            index++;
            if (receivedChar == 0x00EF)
            {
                Receive_Buf_Primary[index] = 0x0000;
                CRCvar = true;
            }
        }

        else
        {
            if(CRCindex < 2)
            {
                Receive_Buf_Primary[index] = (uint8_t)receivedChar;
                index++;
                Receive_Buf_Primary[index] = 0x0000;
                CRCindex++;
            }
            if(CRCindex == 2)
            {
                handle_Display_uart(Receive_Buf_Primary, index);
                CRCvar = false;
                CRCindex = 0;
                index = 0;
            }
        }
    }
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}


//********For SCI C  connected to PFC**********//
__interrupt void scic_isr(void)
{
    uint32_t istat = SCI_getInterruptStatus(SCIC_BASE);

    // Handle RX errors first (framing/parity/break). These suppress further RX interrupts until cleared.
    if (istat & SCI_INT_RXERR)
    {
        volatile uint16_t dump = HWREGH(SCIC_BASE + SCI_O_RXBUF); // read to pop error char
        (void)dump;
        SCI_clearInterruptStatus(SCIC_BASE, SCI_INT_RXERR);
        SCI_resetRxFIFO(SCIC_BASE);          // flush garbage
        SCI_clearOverflowStatus(SCIC_BASE);  // clear RXFFOVF if it happened
    }

    // Drain the FIFO completely so the next interrupt can re-arm
    while (SCI_getRxFIFOStatus(SCIC_BASE) != SCI_FIFO_RX0)
    {
        receivedChar1 = (uint16_t)(SCI_readCharNonBlocking(SCIC_BASE) & 0xFF);
        SCI_clearInterruptStatus(SCIC_BASE, SCI_INT_RXFF);
        SCI_clearOverflowStatus(SCIC_BASE);
        Receive_Buf_Primary1[index1] = (uint8_t)receivedChar1;
        index1++;

        if (receivedChar1 == 0x00EF)
        {
            Receive_Buf_Primary1[index1] = 0x0000;
            handle_PFC_uart(Receive_Buf_Primary1, index1);
            index1 = 0;
        }
    }

    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP8);
}



void Read_Data_from_Shared_Memory(void)
{
    memset(transmitBuff, 0, sizeof(transmitBuff));
    int i = 0;
    for(i = 0; i < 256; i++)
    {
        transmitBuff[i] = cpu2Read[i];
    }
}






