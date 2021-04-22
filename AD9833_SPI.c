/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Arbaaz Khan
 Software:  MDK-ARM Keil v5
 Hardware:  Any STM32 device
*************************************************************************************/
#include "AD9833.h"
#include "main.h"
#include "spi.h"
// -------------------------------- Functions --------------------------------

// ------------------------------------------------ Software SPI Function
void WriteControlRegSPI(uint16_t word) {
	uint8_t temp[2];
	temp[0] = ((word >> 8) & 0xFF);		//Higher Byte
	temp[1] = (word  & 0xFF);					//Lower Byte
  HAL_GPIO_WritePin(FSYNC_GPIO_Port, FSYNC_Pin,GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi1, (uint8_t *)&temp, 2, HAL_MAX_DELAY);
	
  HAL_GPIO_WritePin(FSYNC_GPIO_Port, FSYNC_Pin,GPIO_PIN_SET);
	HAL_Delay(10);
}

// ------------------------------------------------ Sets Output Wave Type
void AD9833_SetWave(uint16_t Wave){
  switch(Wave){
  case 0:
    WriteControlRegSPI(0x2000); // Value for Sinusoidal Wave
    break;
  case 1:
    WriteControlRegSPI(0x2028); // Value for Square Wave
    break;
  case 2:
    WriteControlRegSPI(0x2002); // Value for Triangle Wave
    break;
  default:
    break;
  }
}

// ------------------------------------------------ Sets Wave Frequency & Phase (In Degree) In PHASE0 & FREQ0 Registers
void AD9833_SetWaveData(float Frequency,float Phase)
{ 
	uint16_t FRQLW = 0;    // MSB of Frequency Tuning Word
	uint16_t FRQHW = 0;    // LSB of Frequency Tuning Word
	uint32_t phaseVal=0;  // Phase Tuning Value   
	long freq=0;
	
	// ---------- Tuning Word for Phase ( 0 - 360 Degree )
	if(Phase<0)Phase=0; // Changing Phase Value to Positive
	if(Phase>360)Phase=360; // Maximum value For Phase (In Degree)
	phaseVal  = ((int)(Phase*(4096/360)))|0XC000;  // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address

	// ---------- Tuning word for Frequency  
	freq=(int)(((Frequency*pow(2,28))/FMCLK)+1); // Tuning Word
	FRQHW=(int)((freq & 0xFFFC000) >> 14); // FREQ MSB
	FRQLW=(int)(freq & 0x3FFF);  // FREQ LSB 
	FRQLW |= 0x4000;
	FRQHW |= 0x4000; 

	WriteControlRegSPI(0x2100); // enable 16bit words and set reset bit
	WriteControlRegSPI(FRQLW);
	WriteControlRegSPI(FRQHW);
	WriteControlRegSPI(phaseVal);
	WriteControlRegSPI(0x2000); // clear reset bit 
	
	return;
}

// ------------------------------------------------ Initializing AD9833
void AD9833_Init(uint16_t WaveType,float FRQ,float Phase)
{
	HAL_GPIO_WritePin(FSYNC_GPIO_Port, FSYNC_Pin,GPIO_PIN_SET);   // Set All SPI pings to High
	AD9833_SetWaveData(FRQ,Phase);                         // Frequency & Phase Set
	AD9833_SetWave(WaveType);                              // Type Of Wave 
return;
}



