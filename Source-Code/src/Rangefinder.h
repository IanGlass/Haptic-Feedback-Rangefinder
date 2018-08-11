/**************************************************************************//**
 * @file
 * @brief Rangefinder main module
 * @author Ian Glass
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/

//
//
#ifndef TRIG_PIN
#define TRIG_PIN     3
#endif
#ifndef TRIG_PORT
#define TRIG_PORT    gpioPortB
#endif
#ifndef ECHO_PIN
#define ECHO_PIN     4
#endif
#ifndef ECHO_PORT
#define ECHO_PORT    gpioPortB
#endif
#ifndef PWM_PIN
#define PWM_PIN     4
#endif
#ifndef PWM_PORT
#define PWM_PORT    gpioPortF
#endif
#ifndef BUTTON_PIN
#define BUTTON_PIN     13
#endif
#ifndef BUTTON_PORT
#define BUTTON_PORT    gpioPortC
#endif
#ifndef CHARGE_PIN
#define CHARGE_PIN     0
#endif
#ifndef CHARGE_PORT
#define CHARGE_PORT    gpioPortA
#endif
//
//

//
//Optimal without signal cross-over
#define TRIGGER_CALL_FREQ 25
//Set to 40us high time
#define TRIGGER_HIGH_FREQ 25000
//PWM CALL frequency (100 times PWM frequency). Should be no greater than time to acquire 5 samples
#define PWM_FREQ 100000 //25000
//Systick call frequency
#define SYS_FREQ 10000
//Frequency for range indicator (set to 2 Hz atm)
#define RANGE_FREQ 200
//
//

















