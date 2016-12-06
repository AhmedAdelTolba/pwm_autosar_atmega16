/*
 * Pwm_Prototypes.h
 *
 *  Created on: May 28, 2016
 *      Author: Ahmed
 */

#ifndef PWM_PROTOTYPES_H_
#define PWM_PROTOTYPES_H_

void Pwm_Init(const Pwm_ConfigType* ConfigPtr);

#if PWM_DE_INIT_API==STD_ON
void Pwm_Deinit(void);
#endif

#if PWM_SET_DUTY_CYCLE_API==STD_ON
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle);
#endif

#if PWM_SET_PERIOD_AND_DUTY_API==STD_ON
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle);
#endif

#if PWM_SET_OUTPUT_TO_IDLE_API==STD_ON
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber);
#endif

#if PWM_NOTIFICATION_SUPPORTED==STD_ON
void Pwm_DisableNotification(Pwm_ChannelType ChannelNumber);
void Pwm_EnableNotification(Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification);
#endif

#endif /* PWM_PROTOTYPES_H_ */
