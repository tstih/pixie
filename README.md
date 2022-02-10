![status.badge] [![language.badge]][language.url] [![standard.badge]][standard.url] [![license.badge]][license.url]

# pixie

pixie is a simple vector display emulator for Linux. You can send
commands to pixie using a named pipe. 

## Changes

~~~
10-Feb-2022  Added the blit mode (XOR or COPY)
09-Feb-2022  Added optional line pattern.
07-Feb-2022  First version.
~~~

## Compile

~~~
make
~~~

## Use

*pixie [--width n] [--height m] [--title s] [pipe]*

 > Default resolution is 1024x512, default title is pixie
 > and default named pipe is `/tmp/pixie.pipe`.

### Example

~~~
pixie --width 330 --height 410 --title Vectrex /tmp/pixie.pipe
~~~

 > When you run pixie it will block the terminal until you press
 > Ctrl+C or send commands to the named pipe. Therefore you will 
 > need to open two terminals: one for pixie and one for sending 
 > commands.

## Commands 

You can send pixie four basic commands

| Command                  | Desciption                                      | Example          |
|--------------------------|-------------------------------------------------|------------------|
| P<x,y>                   | Draw pixel at x,y.                              | P100,100         |
| L<x0,y0,x1,y1[,pattern]> | Draw line from x0,y0 to x1,y1 using bit pattern | L100,100,300,300 |
| I<n>                     | Set color intensity (0-255)                     | I128             |
| C                        | Clear screen                                    | C                |
| B<blitmode>              | Set blit mode: 0=NONE, 1=COPY, 2=XOR            | B2               |

### Example(s)

![screenshot](img/pixie.png)

## Why pixie?

To simulate procedural programming environment on an event driven GUI. You can, for example, develop vector game prototypes for 8-bit machines on Linux, or test your drawing algorithms on pixie.

## To do

My wish list includes (in this order) raster operations, colors, and a retro CRT shader. Stay tuned.

[language.url]:   https://en.wikipedia.org/wiki/ANSI_C
[language.badge]: https://img.shields.io/badge/language-C-blue.svg

[standard.url]:   https://en.wikipedia.org/wiki/C89/
[standard.badge]: https://img.shields.io/badge/standard-C89-blue.svg

[license.url]:    https://github.com/tstih/libcpm3-z80/blob/main/LICENSE
[license.badge]:  https://img.shields.io/badge/license-MIT-blue.svg

[status.badge]:  https://img.shields.io/badge/status-stable-green.svg