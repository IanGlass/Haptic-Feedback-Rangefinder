# Haptic-Feedback-Rangefinder

The purpose of this project was to design a hand held rangefinder which uses vibration intensity (haptic feedback) to indicate distance. The sensing range can be cycled between 100, 200 and 400 cm using the push button, where the intensity of vibration indicates the distance from the maximum distance. A switch is also present to switch the device on and off. The device also has charge indication to let the user know it is charging, by way of a couple of vibrations as feedback. The device will not operate when in charge mode. This project uses the HC-SR04 ultrasonic sensor and the EFM32TG microcontroller produced by SiliconLabs.

# Circuit

The Li-ion battery is voltage regulated to a nominal 3.5 V, providing power to the microcontroller. Battery charge is controlled with the MCP73831, which is powered through a 5 V USB source. MOSFETs are not required for normal operation but allows the micro to power up and indicate 'charging' with a few short vibration bursts from the motor (Charge tag leading to PA0 on the micro). The device has a 50 mA average measured current usage. 

<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Power-Schematic.jpg" width="700">
</p>

The MCP1640 boost converter is required and adjusted to boost the 3.5 V regulated power source to 5 V, the operating voltage of the HC-SR04 sensor.

<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Boost-Converter-Schematic.jpg" width="700">
</p>

The EFM32TG is programmed and debugged through the Serial Wire Debug header and can be powered through USB. The HC-SR04 sensor is connected to the EFM32TG through two GPIO ports (Echo and Trig). These two pins are used to trigger range acquisition and to echo the result. An LMV321 operational amplifier is used in non-inverting mode to boost the microcontroller drive current to the motor and to smooth out the PWM signal to an analogue voltage. The voltage divider between PD6 and PD5 allows the micro to periodically check 

<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Rangefinder-Schematic.jpg" width="700">
</p>
<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Rangefinder-PCB.JPG" width="420">
</p>

# Code

The code runs on event driven foreground tasks only (IRQs) and does not use any background operation. The HC-SR04 has a sensing range of 2 cm to 400 cm and operates by measuring the time of flight of eight 40 kHz sound pulses sent when triggered with a 10 us high digital input signal. Distance is calculated by measuring the high time of an output pin connected to the micro, where the range = high time * 340 (m/s) / 2.
The following code excerpt runs on the *TIMER1* peripheral to send 10 us long GPIO high signals to the HC-SR04 TRIG pin at a rate of 25 Hz, giving a sample rate of 25 Hz before averaging.
```c
void TIMER1_IRQHandler(void) {
	/* Reset overflow flag */
	TIMER_IntClear(TIMER1, TIMER_IF_OF);
	/* Turn trigger on if off before */
	if (GPIO_PinInGet(TRIG_PORT, TRIG_PIN) == 0) {
		GPIO_PinOutSet(TRIG_PORT, TRIG_PIN);
		/* Set overflow value to trigger timer in 10 us */
		TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER)/TRIGGER_HIGH_FREQ/16);
	}
	/* Switch back to regular trigger timing */
	else {
		GPIO_PinOutClear(TRIG_PORT, TRIG_PIN);
		/* Set overflow value to call timer at 25Hz */ 
		TIMER_TopSet(TIMER1, CMU_ClockFreqGet(cmuClock_HFPER)/TRIGGER_CALL_FREQ);
	}
}
```

The *SYSTICK* IRQ increments a counter variable every 10 us, which is used for th Tof measurement. This IRQ is triggered on both edges of the *ECHO* pin. If the pin is high, the function resets the *SYSTICK* counter, and when the pin is low, the counter is indicative of ToF. This IRQ performs 5 point averaging when 5 samples are acquired, reducing the overall sampling rate of the system to 5 Hz. This IRQ also detects a charge event, when the device is plugged into USB, which triggers another IRQ to indicate charging through haptic feedback and stops operation. The PWM signal is calculated as a percentage of the current average distance and the maximum detection range set by pushing the button.
```c
void GPIO_EVEN_IRQHandler(void) {
	/* If echo high start timer */
	if (GPIO_PinInGet(ECHO_PORT, ECHO_PIN) == 1) {
		Time = 0;
	}
	/* If echo low stop timer */
	else if (GPIO_PinInGet(ECHO_PORT, ECHO_PIN) == 0) {
		//Distance in cm, definitely works!
		Distance[SampleCount] = Time*340/(1000000/SYS_FREQ)/2;
		SampleCount++;
	}
	if ((SampleCount >= 4) && (!Charging)) {
		SampleCount = 0;
		//Needs to scale 30-90% as motor has min starting voltage. PWM and distance inversely proportional
		PWM = 100 - ((Distance[0] + Distance[1] + Distance[2] + Distance[3])/4*100/Range);
		//Can go -Ve if Distance>range
		if (PWM < 0) {
			PWM = 0;
		}
	}
	
	/* Detect of charging from USB */
	if (GPIO_PinInGet(CHARGE_PORT, CHARGE_PIN) == 1) {
		Charging = 1;
	}
	if (GPIO_PinInGet(CHARGE_PORT, CHARGE_PIN) == 0) {
		Charging = 0;
		/* Reset indicated charge when unplugged */
		Charge_Indicated = 0;
	}
	GPIO_IntClear(1 << ECHO_PIN);
	GPIO_IntClear(1 << CHARGE_PIN);
}
```

This IRQ has three functions: drive the PWM output to the op amp and motor, perform counting to debounce the button used for range switching and perform haptic feedback on a charge event. Motor PWM is achieved by using a counter which is incremented on each call of the IRQ. Button debounce is achieved in a similar manner. 
```c
void TIMER0_IRQHandler(void) {
	TIMER_IntClear(TIMER0, TIMER_IF_OF);
	PWMTime++;
	/* Turn PWM off if timer < PWM variable */
	if (PWMTime >= PWM) {
		GPIO_PinOutClear(PWM_PORT, PWM_PIN);
	}
	else {
		GPIO_PinOutSet(PWM_PORT, PWM_PIN);
	}
	if (PWMTime >= 100) {
		PWMTime = 0;
	}

	Debounce++;
	if (Debounce > 400) {
		Button_Enable = 1;
	}
	if (!Sensor_On) {
		/* Set motor on time to 1/2 total time */
		PWM = 50;
		/* Change pwm timer freq to 5Hz */
		TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/RANGE_FREQ);
		Indicator_Counter++;
		/* If finished indicating range, turn everything back on */
		if (Indicator_Counter/100 >= Indicator_Counter_Max) {
			/* Reset PWM to normal frequency */
			TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);
			/* Turn trigger timer back on */
			TIMER_IntEnable(TIMER1, TIMER_IF_OF);
			Sensor_On = 1;
		}
	}
	if ((Charging) && (!Charge_Indicated)) {
		PWM = 50;
		/* Change pwm timer freq to 5Hz */
		TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/RANGE_FREQ);
		Charge_Counter++;
		if (Charge_Counter >= 100) {
			/* Reset PWM to normal frequency */
			TIMER_TopSet(TIMER0, CMU_ClockFreqGet(cmuClock_HFPER)/PWM_FREQ);
			Charge_Counter = 0;
			Charge_Indicated = 1;
			PWM = 0;
		}
	}
}
```

This IRQ exclusively checks for a button push event and switches the range between 100, 200 and 400 cm. This range is used in previous IRQs to scale the vibration intensity with currently sensed distance.
```c
void GPIO_ODD_IRQHandler(void) {
	/* Button check for button pressed and perform debounce */
	if ((GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN) == 1) && Button_Enable && !Prev_State && !Charging){
		Debounce = 0;
		Button_Enable = 0;
		/* Switch Range */
		if (Range == 100) {
			Range = 200;
			Indicator_Counter_Max = 2;
		}
		else if (Range == 200) {
			Range = 400;
			Indicator_Counter_Max = 4;
		}
		else {
			Range = 100;
			Indicator_Counter_Max = 1;
		}
		/* Turn off trigger timer */
		TIMER_IntDisable(TIMER1, TIMER_IF_OF);
		Indicator_Counter = 0;
		Sensor_On = 0;
		Prev_State = 1;
	}
	/* Make sure button has been released before allowing another push */
	else if ((GPIO_PinInGet(BUTTON_PORT, BUTTON_PIN) == 0) && Prev_State) {
		Prev_State = 0;
	}
	GPIO_IntClear(1 << BUTTON_PIN);
}
```

# Case

The device consists of a (top to bottom) a top case piece, the HC-SR04 sensor, the populated PCB, a Li battery, the motor and the bottom piece. The case is 3D printed from PLA and can be super glued together.

<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Assembly-2.JPG" width="420">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Assembly-3.JPG" width="420">
</p>
<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Bottom.JPG" width="900">
</p>
<p align="center">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Top.JPG" width="900">
</p>



