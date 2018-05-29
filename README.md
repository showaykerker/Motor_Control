# Motor_Control

This is a project using [Linkit 7697](https://www.seeedstudio.com/LinkIt-7697-p-2818.html) and a few arduino modules - including 20-slot encoder and deceleration dc motor - to control motor speed.

Note that in linkit 7697 package, the external interrupt is having a ==debounce time==, which makes it unable to calculate actual speed of the motor.

Inorder to fix this problem, it required to modify a little bit in 7697's library.

### For Linux

In ~/.arduino15/packages/Linkit/hardware/linkit_rtos/0.10.18/cores/arduino/wiring_interrupt.c:20 
``` arduino=
void attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode) {
	// by default we set the debounce time to 50 ms
	return attachInterruptWithDebounce(pin, callback, mode, 50); // <--
}
```

The default debounce time was set to 50ms, I tried 1ms and it worked well.
