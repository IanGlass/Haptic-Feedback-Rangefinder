# Haptic-Feedback-Rangefinder

The purpose of this project was to design a hand held rangefinder which uses vibration intensity (haptic feedback) to indicate distance. The sensing range can be cycled between 100, 200 and 400 cm using the push button. The project uses the HC-SR04 ultrasonic sensor and the EFM32TG microcontroller produced by SiliconLabs.

# Circuit

The Li-ion battery is V regulated to a nominal 3.5 V, providing power to the microcontroller. Battery charge is controlled with the MCP73831, which is powered through a 5 V USB source. MOSFETs are not required for normal operation but allows the micro to power up and indicate 'charging' with a few short vibration bursts from the motor. The device has a 50 mA average measured current usage. 

<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Power-Schematic.jpg" width="700">

The MCP1640 boost converter is required and adjusted to boost the 3.5 V regulated power source to 5 V, the operating voltage of the HC-SR04 sensor. 

<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Boost-Converter_Schematic.jpg" width="700">

The EFM32TG is programmed and debugged through the Serial Wire Debug header and can be powered through USB. The HC-SR04 sensor is connected through two GPIO ports. An op amp is included to boost the micro current drive and smooths out PWM to an analogue voltage.

<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Rangefinder-Schematic.jpg" width="700">

<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Circuit-Schematics/Rangefinder-PCB.JPG" width="420">

# Code and Sensor

The HC-SR04 has a sensing range of 2 cm to 400 cm and operates by measuring the time of flight of eight 40 kHz sound pulses sent when triggered with a 10 us high digital input signal. Distance is calculated by measuring the high time of an output pin connected to the micro, where the range = high time * 340 (m/s) / 2.

High precision timing is achieved with the SysTick ISR, which provides a foreground incremental global timer. Timer1 ISR handler produces 25 Hz triggers to sample the HC-SR04. The Even_GPIO ISR handler is triggered on an edge change of the HC-SR04 output and resets and saves the global timer when prompted. Background tasks are not used in this implementation.

# Case

The device consists of a battery, motor, PCB, switch, button, HC-SR04 and two case halves which can be 3D printed from PLA and super glued shut.

<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Assembly-1.JPG" width="420">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Assembly-2.JPG" width="420">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Assembly-3.JPG" width="420">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Bottom.JPG" width="420">
<img src="https://github.com/IanGlass/Haptic-Feedback-Rangefinder/blob/master/Case/Top.JPG" width="420">



