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

The code used to program the Atmega328 is in C++ also a part of the repository. Some features include the Dial lighting up the color of the player whose turn it is and the leds within the dial turning as the score is adjusted. There are three buttons below the dial, the left button is the 2/3 player switch which will cycle between 2 players or 3 players of any two colors so long as at least one of the players scores is 0. The center button is the Plus/Minus mode toggle which on a given players turn will count from zero to add to their current score, signified by "P" or "-" in the hundreds place. Finally, the rightmost button is an undo button which will take the score back to where it started on the previous turn or back to the previous player and their previous score. 

With the travel size prototype completed, I am working on a full size one!

If you have any questions, please email me at masonbrainard@gmail.com
