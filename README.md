# NAVI - A Navigational Aid for the Visually Impaired
The NAVI (Navigational Aid for the Visually Impaired) is a compact, lightweight, handheld, assistive device whose purpose is to help the visually impaired navigate their surroundings.

Drawing on the principles of echolocation, the NAVI uses a [Parallax PING)))] (https://www.parallax.com/product/28015) Ultrasonic Distance Sensor to determine the distance of nearby obstacles from the device. Using an algorithm, this distance is then used to provide the appropriate audio and/or haptic feedback (through speakers and/or a vibration motor) to the user to alert them of the obstacle's presence. The feedback is also scaled so that it becomes faster and stronger as the obstacle gets closer (refer to the `hapticFeedback()` and `wait()` functions), thus allowing the user to estimate the relative distance of the obstacle as well. The NAVI also allows for customization, as push buttons can be used to toggle distance and feedback modes, and the device can be easily recharged through its USB port.

In testing, the NAVI detected obstacles, estimated their distances, and provided the appropriately scaled feedback with near 100% accuracy. It proved to have a quick learning curve in human trials as well. The NAVI offers advantages over the common white walking cane, as it is compact, accurate, and can detect obstacles of a wide range of heights and distances. With further research, it can become a very viable tool for the visually impaired.

The software for the NAVI (`NAVI.ino`) is what is mainly included in this Repository. The NAVI was controlled by an [Arduino Micro] (https://www.arduino.cc/en/Main/ArduinoBoardMicro), and hence the code is specifically built to run on one.

The NAVI was entered as a project to several science fairs. It won 1st place at the [Pennsylvania Junior Academy of Science] (http://www.pjas.net/), 4th place at the [Pittsburgh Regional Science and Engineering Fair] (https://www.scitechfestival.org/mainsf.asp), and accumulated a total of seven sponsor awards.

Below is an image of the NAVI. Its components are housed in a 3D printed enclosure, with the PING))) in the front, on/off and mode switch buttons on the top, speaker and headphone jack in the back, and vibration motor on the bottom:

![alt tag](https://github.com/sarthak-n/NAVI/blob/master/img/NAVI.JPG)