#include "set_coordinates.h"
extern DAC_HandleTypeDef hdac;

void set_coordinate(uint8_t x,uint8_t y)
{
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, x);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, y);
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}
