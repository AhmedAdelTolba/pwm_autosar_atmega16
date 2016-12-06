/*
 * pwm.c
 *
 *  Created on: May 28, 2016
 *      Author: Ahmed
 */
#include<avr/io.h>
#include "PWM_cfg.h"
#include "util.h"
#include <avr/interrupt.h>

/*---------------------------------------------------------------------*/

typedef struct
    {
	boolean Calling_init_func :1;
	boolean NeedResetPolarity1 :1;
	boolean NeedResetPolarity2 :1;
	boolean Pwm_Init :1;
	boolean Pwm_Deinit :1;
	boolean Pwm_SetDutyCycleChannel1 :1;
	boolean Pwm_SetDutyCycleChannel2 :1;
	boolean Pwm_SetPeriodAndDutyChannel1 :1;
	boolean Pwm_SetPeriodAndDutyChannel2 :1;
	boolean Pwm_SetOutputToIdleChannel1 :1;
	boolean Pwm_SetOutputToIdleChannel2 :1;
	boolean Pwm_DisableNotificationChannel1 :1;
	boolean Pwm_DisableNotificationChannel2 :1;
	boolean Pwm_EnableNotificationChannel1 :1;
	boolean Pwm_EnableNotificationChannel2 :1;
	boolean Channel1Notifications :1;
	boolean Channel2Notifications :1;
    } flags;

static flags Module_Flags =
    {
	    .Calling_init_func = FALSE,
	    .NeedResetPolarity1 = FALSE,
	    .NeedResetPolarity2 = FALSE,
	    .Pwm_Init = TRUE,
	    .Pwm_Deinit = TRUE,
	    .Pwm_SetDutyCycleChannel1 = TRUE,
	    .Pwm_SetDutyCycleChannel2 = TRUE,
	    .Pwm_SetPeriodAndDutyChannel1 = TRUE,
	    .Pwm_SetPeriodAndDutyChannel2 = TRUE,
	    .Pwm_SetOutputToIdleChannel1 = TRUE,
	    .Pwm_SetOutputToIdleChannel2 = TRUE,
	    .Pwm_DisableNotificationChannel1 = TRUE,
	    .Pwm_DisableNotificationChannel2 = TRUE,
	    .Pwm_EnableNotificationChannel1 = TRUE,
	    .Pwm_EnableNotificationChannel2 = TRUE,
	    .Channel1Notifications = FALSE,
	    .Channel2Notifications = FALSE
    };

/*---------------------------------------------------------------------*/

ISR(TIMER1_COMPA_vect)
    {
    PORTB ^= 1; // invert PORTB.0
    }

ISR(TIMER1_COMPB_vect)
    {
    PORTB ^= 1; // invert PORTB.0
    }

ISR(TIMER1_OVF_vect)
    {
    PORTB ^= 1; // invert PORTB.0
    }

/*---------------------------------------------------------------------*/

void Set_Period(uint8 Period)
    {

    switch (Period)
	{

    case Period1:
	SET_BIT(TCCR1B, CS10);
	CLR_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	break;

    case Period2:
	CLR_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	break;

    case Period3:
	SET_BIT(TCCR1B, CS10);
	SET_BIT(TCCR1B, CS11);
	CLR_BIT(TCCR1B, CS12);
	break;

    case Period4:
	CLR_BIT(TCCR1B, CS10);
	CLR_BIT(TCCR1B, CS11);
	SET_BIT(TCCR1B, CS12);
	break;

    case Period5:
	SET_BIT(TCCR1B, CS10);
	CLR_BIT(TCCR1B, CS11);
	SET_BIT(TCCR1B, CS12);
	break;

    default:
	break;
	}

    return;
    }

void Set_Polarity(boolean ChannelId, boolean Polarity)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (pwmconfig[ChannelId].HardwareChannelId)
	{

    case HardwarePwmChannel1:

	if (Polarity == PWM_HIGH)
	    {

	    CLR_BIT(TCCR1A, COM1A0);

	    }
	else if (Polarity == PWM_LOW)
	    {

	    SET_BIT(TCCR1A, COM1A0);

	    }
	else
	    {
	    }

	break;

    case HardwarePwmChannel2:

	if (Polarity == PWM_HIGH)
	    {

	    CLR_BIT(TCCR1A, COM1B0);

	    }
	else if (Polarity == PWM_LOW)
	    {

	    SET_BIT(TCCR1A, COM1B0);

	    }
	else
	    {
	    }
	break;

    default:
	break;
	}

    return;
    }

void Update_DutyCycle(boolean ChannelId, uint16 DutyCycle)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (pwmconfig[ChannelId].HardwareChannelId)
	{

    case HardwarePwmChannel1:

	OCR1A = DutyCycle;
	break;

    case HardwarePwmChannel2:

	OCR1B = DutyCycle;
	break;

    default:
	break;
	}

    return;
    }

void Set_Idle(Pwm_ChannelType ChannelNumber)
    {
    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (pwmconfig[ChannelNumber].IdleState)
	{

    case PWM_HIGH:

	Set_Polarity(pwmconfig[ChannelNumber].HardwareChannelId, PWM_HIGH);

	break;

    case PWM_LOW:

	Set_Polarity(pwmconfig[ChannelNumber].HardwareChannelId, PWM_LOW);

	break;

    default:

	break;
	}

    Update_DutyCycle(pwmconfig[ChannelNumber].HardwareChannelId, MaximumDutyCycle);

    if (ChannelNumber == LogicalPwmChannel1)
	{
	Module_Flags.NeedResetPolarity1 = TRUE;

	}
    else if (ChannelNumber == LogicalPwmChannel2)
	{
	Module_Flags.NeedResetPolarity2 = TRUE;
	}
    else
	{

	}

    }

void SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    if (DutyCycle != 0)
	{
	//check if we should set polarity
	if (Module_Flags.NeedResetPolarity1 == TRUE && ChannelNumber == LogicalPwmChannel1)
	    {

	    Module_Flags.NeedResetPolarity1 = FALSE;

	    Set_Polarity(LogicalPwmChannel1, pwmconfig[LogicalPwmChannel1].polarity);

	    }
	else if (Module_Flags.NeedResetPolarity2 == TRUE && ChannelNumber == LogicalPwmChannel2)
	    {
	    Module_Flags.NeedResetPolarity2 = FALSE;

	    Set_Polarity(LogicalPwmChannel2, pwmconfig[LogicalPwmChannel2].polarity);

	    }

	Update_DutyCycle(ChannelNumber, DutyCycle);
	}
    else
	{

	if (ChannelNumber == LogicalPwmChannel1)
	    {

	    Module_Flags.NeedResetPolarity1 = TRUE;

	    Set_Polarity(LogicalPwmChannel1, !pwmconfig[LogicalPwmChannel1].polarity);

	    Update_DutyCycle(LogicalPwmChannel1, MaximumDutyCycle);

	    }
	else if (ChannelNumber == LogicalPwmChannel2)
	    {
	    Module_Flags.NeedResetPolarity2 = TRUE;

	    Set_Polarity(LogicalPwmChannel2, !pwmconfig[LogicalPwmChannel2].polarity);

	    Update_DutyCycle(LogicalPwmChannel2, MaximumDutyCycle);

	    }

	}

    return;

    }

void DisableNotification(Pwm_ChannelType ChannelNumber)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (pwmconfig[ChannelNumber].HardwareChannelId)
	{

    case HardwarePwmChannel1:

	Module_Flags.Channel1Notifications = FALSE;
	CLR_BIT(TIMSK, OCIE1A);
	break;

    case HardwarePwmChannel2:

	Module_Flags.Channel2Notifications = FALSE;
	CLR_BIT(TIMSK, OCIE1B);
	break;

    default:
	break;
	}

    if ((Module_Flags.Channel2Notifications = FALSE) && (Module_Flags.Channel2Notifications == FALSE))
	{

	CLR_BIT(TIMSK, TOIE1);

	}
    else
	{

	}

    return;

    }

/*-------------------------------------------------------------------*/

void Pwm_Init(const Pwm_ConfigType* ConfigPtr)
    {

    if (Module_Flags.Pwm_Init == TRUE)
	{

	Module_Flags.Pwm_Init = FALSE;

	if (Module_Flags.Calling_init_func == TRUE)
	    {

	    if (PWM_DEV_ERROR_DETECT == STD_ON)
		{
		//cal dev erroe det error

		}

	    }
	else
	    {

	    extern Pwm_ConfigType pwmconfig[PwmIndex];

	    uint8 LoopCounter;

	    Module_Flags.Calling_init_func = TRUE;

	    //set fast pwm mode
	    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) | (0 << WGM10);
	    TCCR1B |= (1 << WGM13) | (1 << WGM12);

	    //disable ovf interrupt
	    CLR_BIT(TIMSK, TOIE1);

	    for (LoopCounter = 0; LoopCounter < PwmIndex; LoopCounter++)
		{

		DisableNotification(LoopCounter);

		Set_Polarity(LoopCounter, pwmconfig[LoopCounter].polarity);

		Update_DutyCycle(LoopCounter, pwmconfig[LoopCounter].DutyCycle);

		}

	    //set top
	    ICR1 = MaximumDutyCycle;

	    Set_Period(InitPeriod);

	    }

	Module_Flags.Pwm_Init = TRUE;

	}
    else
	{

	}
    return;
    }

#if PWM_DE_INIT_API==STD_ON

void Pwm_Deinit(void)
    {

    if (Module_Flags.Pwm_Deinit == TRUE)
	{

	Module_Flags.Pwm_Deinit = FALSE;

	extern Pwm_ConfigType pwmconfig[PwmIndex];

	uint8 LoopCounter;

	Module_Flags.Calling_init_func = FALSE;
	Module_Flags.NeedResetPolarity1 = FALSE;
	Module_Flags.NeedResetPolarity2 = FALSE;
	Module_Flags.Pwm_SetDutyCycleChannel1 = TRUE;
	Module_Flags.Pwm_SetDutyCycleChannel2 = TRUE;
	Module_Flags.Pwm_SetPeriodAndDutyChannel1 = TRUE;
	Module_Flags.Pwm_SetPeriodAndDutyChannel2 = TRUE;
	Module_Flags.Pwm_SetOutputToIdleChannel1 = TRUE;
	Module_Flags.Pwm_SetOutputToIdleChannel2 = TRUE;
	Module_Flags.Pwm_DisableNotificationChannel1 = TRUE;
	Module_Flags.Pwm_DisableNotificationChannel2 = TRUE;
	Module_Flags.Pwm_EnableNotificationChannel1 = TRUE;
	Module_Flags.Pwm_EnableNotificationChannel2 = TRUE;
	Module_Flags.Channel1Notifications = FALSE;
	Module_Flags.Channel2Notifications = FALSE;

	for (LoopCounter = 0; LoopCounter < PwmIndex; LoopCounter++)
	    {

	    Set_Idle(LoopCounter);

	    DisableNotification(LoopCounter);

	    }

	Module_Flags.Pwm_Deinit = TRUE;
	}
    else
	{
	}
    return;
    }

#else
#warning "Pwm_Deinit is disabled"
#endif

#if PWM_SET_DUTY_CYCLE_API==STD_ON

void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle)
    {

    switch (ChannelNumber)
	{

    case LogicalPwmChannel1:

	if (Module_Flags.Pwm_SetDutyCycleChannel1 == TRUE)
	    {

	    Module_Flags.Pwm_SetDutyCycleChannel1 = FALSE;
	    SetDutyCycle(LogicalPwmChannel1, DutyCycle);
	    Module_Flags.Pwm_SetDutyCycleChannel1 = TRUE;

	    }
	else
	    {

	    }
	break;

    case LogicalPwmChannel2:

	if (Module_Flags.Pwm_SetDutyCycleChannel2 == TRUE)
	    {

	    Module_Flags.Pwm_SetDutyCycleChannel2 = FALSE;
	    SetDutyCycle(LogicalPwmChannel2, DutyCycle);
	    Module_Flags.Pwm_SetDutyCycleChannel2 = TRUE;

	    }
	else
	    {

	    }
	break;

    default:
	break;
	}

    return;

    }

#else
#warning "Pwm_SetDutyCycle is disabled"
#endif

#if PWM_SET_PERIOD_AND_DUTY_API==STD_ON

void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (ChannelNumber)
	{
    case LogicalPwmChannel1:

	//hardware constrain changing period of one channel changes the period of the other channel
	if ((pwmconfig[LogicalPwmChannel1].ChannelClass == PWM_VARIABLE_PERIOD)
		&& (Module_Flags.Pwm_SetPeriodAndDutyChannel1 == TRUE))
	    {
	    Module_Flags.Pwm_SetPeriodAndDutyChannel1 = FALSE;
	    Set_Period(Period);
	    SetDutyCycle(LogicalPwmChannel1, DutyCycle);
	    Module_Flags.Pwm_SetPeriodAndDutyChannel1 = TRUE;
	    }
	else
	    {

	    }
	break;

    case LogicalPwmChannel2:

	//hardware constrain changing period of one channel changes the period of the other channel
	if ((pwmconfig[LogicalPwmChannel2].ChannelClass == PWM_VARIABLE_PERIOD)
		&& (Module_Flags.Pwm_SetPeriodAndDutyChannel2 == TRUE))
	    {
	    Module_Flags.Pwm_SetPeriodAndDutyChannel2 = FALSE;
	    Set_Period(Period);
	    SetDutyCycle(LogicalPwmChannel2, DutyCycle);
	    Module_Flags.Pwm_SetPeriodAndDutyChannel2 = TRUE;
	    }
	else
	    {

	    }

	break;

    default:

	break;

	}

    return;
    }

#else
#warning "Pwm_SetPeriodAndDuty is disabled"
#endif

#if PWM_SET_OUTPUT_TO_IDLE_API==STD_ON

void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber)
    {

    switch (ChannelNumber)
	{

    case LogicalPwmChannel1:

	if (Module_Flags.Pwm_SetOutputToIdleChannel1 == TRUE)
	    {

	    Module_Flags.Pwm_SetOutputToIdleChannel1 = FALSE;
	    Set_Idle(LogicalPwmChannel1);
	    Module_Flags.Pwm_SetOutputToIdleChannel1 = TRUE;

	    }
	else
	    {

	    }
	break;

    case LogicalPwmChannel2:

	if (Module_Flags.Pwm_SetOutputToIdleChannel2 == TRUE)
	    {

	    Module_Flags.Pwm_SetOutputToIdleChannel2 = FALSE;
	    Set_Idle(LogicalPwmChannel2);
	    Module_Flags.Pwm_SetOutputToIdleChannel2 = TRUE;

	    }
	else
	    {

	    }
	break;

    default:
	break;
	}
    return;
    }

#else
#warning "Pwm_SetOutputToIdle is disabled"
#endif

#if PWM_NOTIFICATION_SUPPORTED==STD_ON

void Pwm_DisableNotification(Pwm_ChannelType ChannelNumber)
    {

    switch (ChannelNumber)
	{

    case LogicalPwmChannel1:

	if (Module_Flags.Pwm_DisableNotificationChannel1 == TRUE)
	    {

	    Module_Flags.Pwm_DisableNotificationChannel1 = FALSE;
	    DisableNotification(LogicalPwmChannel1);
	    Module_Flags.Pwm_DisableNotificationChannel1 = TRUE;

	    }
	else
	    {

	    }
	break;

    case LogicalPwmChannel2:

	if (Module_Flags.Pwm_DisableNotificationChannel2 == TRUE)
	    {

	    Module_Flags.Pwm_DisableNotificationChannel2 = FALSE;
	    DisableNotification(LogicalPwmChannel2);
	    Module_Flags.Pwm_DisableNotificationChannel2 = TRUE;

	    }
	else
	    {

	    }
	break;

    default:
	break;
	}
    return;

    }

#else
#warning "Pwm_DisableNotification is disabled"
#endif

#if PWM_NOTIFICATION_SUPPORTED==STD_ON
void Pwm_EnableNotification(Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification)
    {

    extern Pwm_ConfigType pwmconfig[PwmIndex];

    switch (pwmconfig[ChannelNumber].HardwareChannelId)
	{

    case HardwarePwmChannel1:

	if (Module_Flags.Pwm_EnableNotificationChannel1 == TRUE)
	    {

	    Module_Flags.Pwm_EnableNotificationChannel1 = FALSE;

	    switch (pwmconfig[ChannelNumber].polarity)
		{

	    case PWM_HIGH:

		if (Notification == PWM_RISING_EDGE)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel1Notifications = TRUE;
		    }
		else if (Notification == PWM_FALLING_EDGE)
		    {
		    Module_Flags.Channel1Notifications = FALSE;
		    SET_BIT(TIMSK, OCIE1A);
		    }
		else if (Notification == PWM_BOTH_EDGES)
		    {
		    Module_Flags.Channel1Notifications = TRUE;
		    SET_BIT(TIMSK, TOIE1);
		    SET_BIT(TIMSK, OCIE1A);
		    }
		else
		    {
		    }
		break;

	    case PWM_LOW:

		if (Notification == PWM_RISING_EDGE)
		    {
		    Module_Flags.Channel1Notifications = FALSE;
		    SET_BIT(TIMSK, OCIE1A);
		    }
		else if (Notification == PWM_FALLING_EDGE)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel1Notifications = TRUE;
		    }
		else if (Notification == PWM_BOTH_EDGES)
		    {
		    Module_Flags.Channel1Notifications = TRUE;
		    SET_BIT(TIMSK, TOIE1);
		    SET_BIT(TIMSK, OCIE1A);
		    }
		else
		    {
		    }
		break;

	    default:
		break;
		}

	    Module_Flags.Pwm_EnableNotificationChannel1 = TRUE;

	    }
	else
	    {

	    }
	break;

    case HardwarePwmChannel2:

	if (Module_Flags.Pwm_EnableNotificationChannel1 == TRUE)
	    {

	    Module_Flags.Pwm_EnableNotificationChannel1 = FALSE;

	    switch (pwmconfig[ChannelNumber].polarity)
		{
	    case PWM_HIGH:

		if (Notification == PWM_RISING_EDGE)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel2Notifications = TRUE;
		    }
		else if (Notification == PWM_FALLING_EDGE)
		    {
		    Module_Flags.Channel2Notifications = FALSE;
		    SET_BIT(TIMSK, OCIE1B);
		    }
		else if (Notification == PWM_BOTH_EDGES)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel2Notifications = TRUE;
		    SET_BIT(TIMSK, OCIE1B);
		    }
		else
		    {
		    }

		break;

	    case PWM_LOW:

		if (Notification == PWM_RISING_EDGE)
		    {
		    Module_Flags.Channel2Notifications = FALSE;
		    SET_BIT(TIMSK, OCIE1B);
		    }
		else if (Notification == PWM_FALLING_EDGE)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel2Notifications = TRUE;
		    }
		else if (Notification == PWM_BOTH_EDGES)
		    {
		    SET_BIT(TIMSK, TOIE1);
		    Module_Flags.Channel2Notifications = TRUE;
		    SET_BIT(TIMSK, OCIE1B);
		    }
		else
		    {
		    }

		break;

	    default:
		break;
		}

	    Module_Flags.Pwm_EnableNotificationChannel1 = TRUE;

	    }
	else
	    {

	    }
	break;
    default:
	break;
	}
    return;

    }
#else
#warning "Pwm_EnableNotification is disabled"
#endif
