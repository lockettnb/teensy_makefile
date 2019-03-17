
To compile code for the Teensy from the command line you could use the Makefile
that comes with the Teensyduino install.

Just navigate to the ".../hardware/teensy/avr/cores/teensy3" directory,
create a main.cpp file and the Makefile will compile it.

Cool, but I prefer to keep my software project in the $HOME/src directory.
This took me on a long winding road of trying to better understand the
Teensy makefile so I could customize it. 

As the saying goes:
 "YOU ARE IN A MAZE OF TWISTING LITTLE PASSAGES, ALL DIFFERENT"

This repository has a modified version of the makefile for the core
teensy software. And a project makefile that will call the core 
makefile if a recompile is needed. Usually this is done if you 
change a compile time configuration option.   

BONUS: As part of this adventure through the twisty little passages I 
collected documentation on all of the GCC compiler/linker options. See
the doc/teensy_makefile.pdf for details.

BONUS BONUS: This is also a documentation makefile. This make is used to convert 
markdown documentation into HTML or PDF.

PS: The makefile is specific to the versions of the Arduino and 
Teensyduino software you are using.  In this case they are for
Arduino 1.8.8 and Teensyduino 1.45.  They can be easily (easy is a relative word)
changed for other versions.
