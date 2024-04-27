
/* Debug */
/* Provide printf subroutine and delay function */

#pragma once

#include <stdint.h>
#include "ch554.h"



/*******************************************************************************
* Function Name  : CH554UART0Alter()
* Description    : CH554 serial port 0 pin mapping, serial port mapping to P0.2 and P0.3

*******************************************************************************/
inline void CH554UART0Alter()
{
    PIN_FUNC |= bUART0_PIN_X;           //串口映射到P1.2和P1.3
}

/*******************************************************************************
* Function Name  : mInitSTDIO()
* Description    : CH554 serial port 0 is initialized, T1 is used as the baud rate generator of UART0 by default, T2 can also be used
                   As a baud rate generator
*******************************************************************************/
inline void	UART_Init(uint32_t baudrate)
{
    uint32_t x;
    uint8_t x2;

    SM0 = 0;
    SM1 = 1;
    SM2 = 0;                                                                   //Serial port 0 usage mode 1
                                                                               //Use Timer1 as a baud rate generator
    RCLK = 0;                                                                  //UART0 receive clock
    TCLK = 0;                                                                  //UART0 transmit clock
    PCON |= SMOD;
    x = 10 * F_CPU / baudrate / 16;                                       //If you change the main frequency, be careful not to overflow the value of x
    x2 = x % 10;
    x /= 10;
    if ( x2 >= 5 ) x ++;                                                       //rounding

    TMOD = TMOD & ~ bT1_GATE & ~ bT1_CT & ~ MASK_T1_MOD | bT1_M1;              //0X20, Timer1 as 8-bit auto-reload timer
    T2MOD = T2MOD | bTMR_CLK | bT1_CLK;                                        //Timer1 clock selection
    TH1 = 0-x;                                                                 //12MHz crystal oscillator, buad / 12 is the actual need to set the baud rate
    TR1 = 1;                                                                   //Start timer 1
    TI = 1;
    REN = 1;                                                                   //Serial 0 receive enable
}

/*******************************************************************************
* Function Name  : CH554UART0RcvByte()
* Description    : CH554UART0 receives a byte
* Return         : SBUF
*******************************************************************************/
inline uint8_t  UART0_RcvByte( )
{
    while(RI == 0);                     // wait for uart rx interrupt flag
    RI = 0;
    return SBUF;
}

/*******************************************************************************
* Function Name  : CH554UART0SendByte(uint8_t SendDat)
* Description    : CH554UART0 sends a byte
* Input          : uint8_t SendDat; the data to be sent
*******************************************************************************/
inline void UART0_SendByte(uint8_t SendDat)
{

        SBUF = SendDat;
        while(TI ==0);                  // wait for transmit to finish (TI == 1)
        TI = 0;
}

/*******************************************************************************
* Function Name  : CH554UART1Alter()
* Description    : Set the alternate pin mappings for UART1 (TX on P3.2, RX on P3.4)
*******************************************************************************/
inline void UART1_Alter()
{
    PIN_FUNC |= bUART1_PIN_X;
}

/*******************************************************************************
* Function Name  : UART1Setup()
* Description    : CH554串口1初始化
*******************************************************************************/
inline void	UART1_Setup(uint8_t baudrate)
{
    U1SM0 = 0;                          //UART1选择8位数据位
    U1SMOD = 1;                         //快速模式
    U1REN = 1;                          //使能接收
    // should correct for rounding in SBAUD1 calculation 
    SBAUD1 = 256 - F_CPU/16/baudrate;
}

/*******************************************************************************
* Function Name  : CH554UART1RcvByte()
* Description    : CH554UART1接收一个字节
* Return         : SBUF
*******************************************************************************/
inline uint8_t  UART1_RcvByte( )
{
    while(U1RI == 0);                   //查询接收，中断方式可不用
    U1RI = 0;
    return SBUF1;
}

/*******************************************************************************
* Function Name  : CH554UART1SendByte(uint8_t SendDat)
* Description    : CH554UART1发送一个字节
* Input          : uint8_t SendDat；要发送的数据
*******************************************************************************/
inline void UART1_SendByte(uint8_t SendDat)
{
        SBUF1 = SendDat;                //查询发送，中断方式可不用下面2条语句,但发送前需TI=0
        while(U1TI ==0);
        U1TI = 0;
}


