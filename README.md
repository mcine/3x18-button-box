button box that has 3 x 18 buttons.

Made using sparkfun arduino pro micro
2x6 button matrix (8 push buttons and 2 2 way momentary switches)
3 position switch for modes
2 encoders with push buttons
total = 18 digital inputs.

using also library:
https://github.com/MHeironimus/ArduinoJoystickLibrary

mode switch is used to select if we are using buttons 1-18, 19-38 or 39-56

to prevent unwanted doubleclicks there a bit of prevention code for that.
encoder "push" can be quite short so that is made longer in code

This might get tons of tweaking but should work as a starting point

I don't really care about licenses but since the joystick library is lgpl, It shoudl be
safe to use the same..