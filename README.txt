This project was intended to form the basis of a smart home curtain system, where the curtains are movable using a beaded chain.

To accomplish this, a C program was writting for the micro:bit and moto:bit.  In particular, the code was broken into the following files, not including .h files:

algorithm.c: In the algorithm file, the machine's sensor states (held within a struct defined in the header file) are analyzed in order to determine the proper output states, also held within that struct.  The primary function in the algorith file is called once every second by a timer interrupt on a timer that fires once every second.

gpio.c:  This is simply the file from the first lab, which allows the configuration, reading, and writing of GPIO pins.  In particular, this is used to configure a tri-color LED for troubleshooting, which used EDGE_P8, EDGE_P12, and EDGE_P14; further, it was used to read the A and B buttons on the micro:bit since they acted as manual overrides.

i2c.c:  This file was taken from Lab 6, and is primarily used to configure/write to the moto:bit's I2C, which is further configured in main.c.  It does however also have the ability to read.
light.c:  This file uses gpio.c to more easily control the tri-color LED, so that from main.c functions such as turn_light_on(Green) can be used, instead of thinking about the pin number and whether you should write a 1 or 0.  An enumeration for the light color tying the color to the correct GPIO number is also specified in the header file.

motor.c  This is the main driver, and was transcribed from the motobit.ts file which can be found here: https://github.com/sparkfun/pxt-moto-bit into C code.  The header file specifies enumerations such as the motor, direction, and On/Off in order to allow easier use of the underlying I2C writes.

main.c:  This file handles both the sensors on the breadboard (I was having some trouble with SAADC's file when I tried to put the sensors in their own sensors.c/sensors.h file, this was actually the primary change between curtains and CurtainsV2) as well as tying together the other files, and writing to the motor based on how the algorithm file alters the state struct.  The time is also altered here in a repeated timer callback function which fires every second.  This timer firing also calls algorithm to update the state variable.  However, the motor is written to in an infinite loop in the main function, since when I tried to put it in the timer callback function the I2C wasn't able to write fast enough to also allow for state updates (at least I'm assuming as much, since when I tried it I wasn't seeing ANY print statements that I use for troubleshooting).

Further, if you're wondering why this is V3 instead of V2, the primary difference is the use of a struct to store the system's state.  In V2, the amount of information that had to be used to call the algorithm file's primary function was quite large, so instead I repacked it into a single struct that could also be updated by the algorithm to contain the output information, since I wanted to call the algorithm from the timer callback but couldn't write to the motor from there.

Please don't hesitate to reach out with any questions you might have!
