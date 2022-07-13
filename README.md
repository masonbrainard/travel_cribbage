# travel_cribbage
This project was made as a proof of concept for an electronic cribbage board. Instead of having more than 360 leds to show the positions of each player, I would substitute those with 3 separate 3 digit 7 segment displays, with a RGB dial to increment score and show whose turn it is.

![alt text](https://github.com/masonbrainard/travel_cribbage/blob/main/tc_v1_on.jpg)

The design requirements for the Travel Cribbage Board V1 (TC_V1) included:
1.	A RGB dial that would follow your movement as it was turned.
2.	3 separate 3 digit 7 segment displays.
3.	Multiple methods of power sources.

With these in mind, I used a Atmega 328 microcontroller, and designed a printed circuit board (PCB) to hold the components and run them. The circuit board runs at 5V, but can be powered by a 9V Battery, AA Batteries, or a rechargeable LiPo battery. In all the pictures (as of writing) the version shown is a testing platform using a 9V battery. To this repository, I have uploaded the schematic, layout, code and pictures.

TC_V1 Schematic: https://github.com/masonbrainard/travel_cribbage/blob/main/travelcribbage_schematic.pdf

TC_V1 PCB Layout: https://github.com/masonbrainard/travel_cribbage/blob/main/travelcribbage_layout.pdf

With the travel size prototype completed, I am working on a full size one!

If you have any questions, please email me at masonbrainard@gmail.com
