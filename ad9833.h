/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Arbaaz Khan
 Software:  MDK-ARM Keil v5
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef _AD_9833_H
#define _AD_9833_H
#include <math.h>
#include "stm32f4xx_hal.h"

// ------------------------- Defines -------------------------
#define FMCLK 25000000        // Master Clock On AD9833
enum WaveType{SIN, SQR, TRI}; // Wave Selection Enum

// ------------------ Functions  ---------------------
void AD9833_SetWave(uint16_t Wave);                      // Sets Output Wave Type
void AD9833_SetWaveData(float Frequency,float Phase);    // Sets Wave Frequency & Phase
void AD9833_Init(uint16_t Wave,float FRQ,float Phase);   // Initializing AD9833

#endif
