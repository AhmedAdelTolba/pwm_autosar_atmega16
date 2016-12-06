/*
 * PWM_cfg.c
 *
 *  Created on: May 24, 2016
 *      Author: Ahmed
 */

#include"PWM_cfg.h"

Pwm_ConfigType pwmconfig[2] =
    {
	{
		.HardwareChannelId = HardwarePwmChannel1,
		.polarity = PWM_HIGH,
		.IdleState = PWM_HIGH,
		.ChannelClass = PWM_VARIABLE_PERIOD,
		.DutyCycle = 0x2200
	},
	{
		.HardwareChannelId = HardwarePwmChannel2,
		.polarity = PWM_HIGH,
		.IdleState = PWM_LOW,
		.ChannelClass = PWM_VARIABLE_PERIOD,
		.DutyCycle = 0x4801
	}
    };

