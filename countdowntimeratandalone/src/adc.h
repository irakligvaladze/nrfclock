#pragma once


#include <stdint.h>
#include "ch554.h"    

#define ADC_INTERRUPT  0
#define ADC_CHANNEL    1
#define ADC_SPEED      1

/*******************************************************************************
* Function Name  : ADCClkSet(uint8_t div)
* Description    :ADC sampling clock setting, module is turned on, interrupt is turned on
* Input          : uint8_t speed clock setting 
                   0 Slow 384 Fosc           								
                   1 fast 96 Fosc									 
* Output         : None
* Return         : None
*******************************************************************************/
void ADCInit(uint8_t speed);

/*******************************************************************************
* Function Name  : ADC_ChannelSelect(uint8_t ch)
* Description    : ADC sampling channel settings
* Input          : uint8_t ch uses channels 0-3
* Output         : None
* Return         : success SUCCESS
                   failure FAIL Channel setting out of range
*******************************************************************************/
uint8_t ADC_ChannelSelect(uint8_t ch);

/*******************************************************************************
* Function Name  : VoltageCMPModeInit()
* Description    : Voltage comparator mode initialization
* Input          : uint8_t fo Forward port 0\1\2\3
                   uint8_t re Reverse port 1\3
* Output         : None
* Return         : success SUCCESS
                   failure FAIL
*******************************************************************************/
uint8_t VoltageCMPModeInit(uint8_t fo,uint8_t re);
void ADCInit(uint8_t speed);

/*******************************************************************************
* Function Name  : ADC_ChannelSelect(uint8_t ch)
* Description    : ADC sampling enabled
* Input          : uint8_t ch Use channel
* Output         : None
* Return         : SUCCESS
                   FAIL
*******************************************************************************/
uint8_t ADC_ChannelSelect(uint8_t ch);

/*******************************************************************************
* Function Name  : VoltageCMPModeInit()
* Description    : Voltage comparator mode initialization
* Input          : uint8_t fo forward port 0 \ 1 \ 2 \ 3
                   uint8_t re Reverse port 1 \ 3
* Output         : None
* Return         : success SUCCESS
                   failure FAIL
*******************************************************************************/
uint8_t VoltageCMPModeInit(uint8_t fo,uint8_t re);

void ADC_BUTTONSInit(uint8_t speed, uint8_t channel);

uint8_t ADC_BUTTONSGetButton(uint8_t channel);
