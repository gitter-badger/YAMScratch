Note to editors. Do not mess with the HTML. Write around it. It looks better this way.

#Hardware #
===============

###Overview###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Speed Controller###
<<<<<<< HEAD
&nbsp;&nbsp;&nbsp;&nbsp;A servo motor controller is used to interpret an input signal, often at logic level from a micro controller, and produce an output signal. The servo motor controller can be modeled as two stage cascade closed loop transfer function. The controller has builtin position feedback sensor, which it uses to hold the steering at the set angle. Manually turning the servo past the set will and releasing the horn will result in the servo returning to the set position. Although the servo has a builtin feedback loop, this error signal is not exposed to the microcontroller, therefore we must treat the speed controller as a single transfer function in an overall open loop tranfer function. 
=======
&nbsp;&nbsp;&nbsp;&nbsp;A servo motor controller is used to interpret an input signal, often at logic level from a micro controller, and produce an output signal. The servo motor controller can be modeled as two stage cascade closed loop transfer function. The controller has builtin position feedback and 
>>>>>>> origin/master

###Compass###
&nbsp;&nbsp;&nbsp;&nbsp;To determine the heading of the DEVICE, a magnetic field sensor IC was used as a sort of electric compass. The magnetic field sensor was used to yield the heading of the device with respect to a fixed magnetic heading. The compass could be calibrated to the desired magnetic field orientation. By bringing the calibrate line low, the physical orientation of the compass would be set to zero degrees. The compass is then rotated 90 degrees clockwise and the calibrate line brought low again to set the compass reference east direction. This calibration process is done four times to establish the four cardinal directions for the compass reference frame. By convention the compass zero reading was aligned with earth magnetic north, and a heading of zero degrees will hereafter be referred to interchangeably with the direction north. Proper calibration is important to receive meaningful information from the compass. Failing to calibrate the compass with cardinal directions 90 degrees apart will lead to undefined  behavior in an associated software that assumes perpendicular cardinal directions. The compass will linearly interpolate direction heading between two cardinal directions, using the scale that the directions are in fact perpendicular. Calibrating the compass with an angle greater than 90 degree between one cardinal direction will lead to decreased sensitivity to heading changes, while a calibrated angle less than 90 will provide increased sensitivity to heading change, up to the noise limit inherent to the device. This specific calibration process tricks allow for more advanced designs, however in the interest of simplicity, perpendicular cardinal directions during calibration were used.
<br>&nbsp;&nbsp;&nbsp;&nbsp;A serial protocol, I2C, was used to obtain readings from the compass. The compass was connected to the serial bus of the micro controller. This compass has an address of 0xC0, and has 14 data registers that can be accessed. Register zero contains the software revision number, while register 1 contains the current heading in a single char. Registers 2 and 3 contain the heading measured in tenths of a degree in two bytes as a number between 0 and 3599. The compass takes about 33ms to generate a new heading reading, so the compass is polled every 40ms. For greatest accuracy the heading is read in tenth degree mode from registers 2 and 3. The compass detects the vector sum of the magnetic fields at a specific point in space. This means that contributions from nearby electronics and metal concentrating field lines can distort the magnetic field to have a different gradient than only the earth's magnetic field alone. Inside the building, where structural re-bar and power lines all warp the local magnetic, a constant magnetic heading may not necessarily correspond to a constant inertial heading.

###Ranger###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###LCD/keypad###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Accelerometer###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Serial Bus###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Pin Connections from Priority Table###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

#Software Description#

###Overview###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Inititalization###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Use of PCA###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Pulse Streams###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Setting the Crossbar###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###I2C Read and Write###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Use of I2C (SMB)###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Reading Analog Input, A/D results###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Keypad Input and LCD output###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Control loops###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

###Timing###
&nbsp;&nbsp;&nbsp;&nbsp;This text is filler and contains no useful information at all.<br> Please replace me with something prettier.

#Results and Conculsions#
============================
analysis of results, description of performance, what was learned,
problems and solutions

#Code#
```c
#include <stdlib.h>
#include <stdio.h>
#include "c8051_SDCC.h"
#include "i2c.h"

void main(void) {

}

```
