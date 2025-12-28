# LED-Light-Chain-Timer
A simple timer for LED chains. Switch it on and the LED chain lights for 6h, then it turns off for 18h, and so on.

The usage of a CH32V003J4M6 allows to directly connect the schematic to a 3.6V LiIon-Battery. To protect the battery from discharging to dangerus voltage levels, there is a protection board added, that also allows to recharge. 

olarized dark             |  Solarized Ocean
:-------------------------:|:-------------------------:
<img src="Doc/Ressources/00_IMG_20251215_135505.jpg" alt="drawing" width="300"/> |
<img src="Doc/Ressources/05_IMG_20251215_135531.jpg" alt="drawing" width="300"/>

To make it look nice, everything is packed in a wooden box.
The left image shows an older version with a switch. It turned out, that it could be omitted.
We could hide the plug of the LED string inside the housing, and of course run the cable to the outside through a hole.

Under the black heat shrink tubing, next to the connector, is a small piece of circuit board on which a resistor limits the current through the LEDs to below 10mA.I set each of the different LED strings to this current, the brightness is okay like this.

<img src="Doc/Ressources/10_IMG_20251215_135719.jpg" alt="drawing" width="300"/>
<img src="Doc/Ressources/15_IMG_20251215_135726.jpg" alt="drawing" width="300"/>

Here is the breadboard setup shown, with a battery on which a TP4096 protection board is mounted.

<img src="Doc/Ressources/20_IMG_20251123_140422.jpg" alt="drawing" width="300"/>

On the left is the CH32v003J4N6 CPU, mounted on an adapter board. On the right is a transistor circuit, which was later omitted.

<img src="Doc/Ressources/25_IMG_20251123_140336.jpg" alt="drawing" width="300"/>

Again, the battery with protection board.

