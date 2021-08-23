Simplified Hardware Simulator Learning Tool

build with "./build"

run with "./run " followed by a path to a 'tile data' file in the 'tds' directory

This program simulates a a world of simplified physics based on a few simple rules that allow the user to create the building blocks for digital logic, and by extension, the building blocks for all digital computing devices. The elements and properties of this simplified world are as follows:

0.) None (n): No property. Transmits no power to all sides
1.) Wire (w): Transmits any received power to all other sides
2.) Source (s): Transmits power to all sides unconditionally
3.) Switch (t): Transmits power to side opposite receiving if and only if exactly one side is receiving power
4.) Diode (d): Transmits power to its 'up' side if and only if the side opposite its up is powered
# 5.) Cross (c): Transmits power to side opposite side receiving power if and only if powered

There is a sample of 'tile data' files in the 'tds' directory from which can be tested. These are mainly simple logic gates, but in theory any arbitrarily complex system could be designed using the same simple set of physical rules.
