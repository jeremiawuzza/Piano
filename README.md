# Piano

written in C++11 with SFML 2.3 & SFGUI

This is based on the nifty little piano of AlexanderX. But I wanted to clean up the multiple arrays so I have created a struct that contains all the variables needed for a piano note - the filename, the SFML keyboard number to use and the key colour, and three that are not initialized at first: keyShape, buffer and sound. These 17 struct items are pushed back into a vector of key structs.

Amazingly the buffer and sound can be intitalised within the vector and the black and white keys sort themselves out based on their colour automatically. In other words the piano could be extended either way if you had the sound files. 
The full set of piano notes is available at http://theremin.music.uiowa.edu/MISpiano.html.

Another trick I have added is to use a detatched thread to play the note. This enables me to set delays using the sf::sleep function in the callee without holding up the main program, so the long dwell time on the piano notes can be shortened (this can be configured by tweaking the milliseconds in the callee. I guess other effects could be added using the thread such as pitch changes or even flanging). 

One problem I haven't solved is repeated keying if you hold a key down. Although the keyreleased has to be triggered to allow the note to sound something is stopping that lockout working. Maybe the keyboard times out? Or maybe it's just the convoluted nature of threads. What would be ideal would be for the note to sound as the key is held and then decay quickly after release.

SFGUI has been used to create the widgets controlling pitch and decay, as well as showing which note you are hitting. SFGUI is a real treasure and well worth incorporating in a project

