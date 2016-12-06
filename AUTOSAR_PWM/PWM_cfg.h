/*
 * PWM_cfg.h
 *
 *  Created on: May 24, 2016
 *      Author: Ahmed
 */
#include"Std_Types.h"

#ifndef PWM_CFG_H_
#define PWM_CFG_H_

#define STD_ON                               1

#define STD_OFF                              0

#define HardwarePwmChannel1                  0

#define HardwarePwmChannel2                  1

#define LogicalPwmChannel1                   0

#define LogicalPwmChannel2                   1

#define Period1                              0

#define Period2                              1

#define Period3                              2

#define Period4                              3

#define Period5                              4

#define MaximumDutyCycle                   0x8000

#define PwmIndex                             2

#define InitPeriod                         Period1

#define PWM_DEV_ERROR_DETECT               STD_ON

#define PWM_DUTYCYCLE_UPDATED_ENDPERIOD    STD_ON

#define PWM_NOTIFICATION_SUPPORTED         STD_ON

#define PWM_DUTY_PERIOD_UPDATED_ENDPERIOD  STD_ON

#define PWM_DE_INIT_API                    STD_ON

#define PWM_GET_OUTPUT_STATE_API           STD_ON

#define PWM_SET_DUTY_CYCLE_API             STD_ON

#define PWM_SET_OUTPUT_TO_IDLE_API         STD_ON

#define PWM_SET_PERIOD_AND_DUTY_API        STD_ON

#define PWM_VERSION_INFO_API               STD_ON

typedef uint8 Pwm_ChannelType;

typedef uint8 Pwm_PeriodType;

typedef enum
    {
    PWM_LOW,
    PWM_HIGH
    } Pwm_OutputStateType;

typedef enum
    {
    PWM_RISING_EDGE,
    PWM_FALLING_EDGE,
    PWM_BOTH_EDGES
    } Pwm_EdgeNotificationType;

typedef enum
    {
    PWM_VARIABLE_PERIOD,
    PWM_FIXED_PERIOD
    } Pwm_ChannelClassType;

typedef struct
    {
	boolean HardwareChannelId :1;
	//Pwm_PeriodType ChannelPeriod :3;
	Pwm_OutputStateType polarity :1;
	Pwm_OutputStateType IdleState :1;
	Pwm_ChannelClassType ChannelClass :1;
	uint16 DutyCycle;
    } Pwm_ConfigType;

#endif /* PWM_CFG_H_ */
