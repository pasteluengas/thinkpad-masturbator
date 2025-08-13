# ThinkPad Masturbator

## Introduction
Small program that reacts to the use of ThinkPad's TrackPoint, by directly accessing to the Linux kernel events (so no Xorg, Wayland nor any other abstraction is required). The more the program is "stimulated", the more significant, expressive and human messages are displayed on the terminal screen.

<small>This only works on GNU/Linux ;[.</small>

## Compiling/Using
### Compilation
Since it's a small program with just two `.c` files and the only external library required is ncurses, the easiest way to compile the program is:

    gcc main.c getev.c -o thinkpad-masturbator -lncurses

> Note that this program uses `strcasestr`, available in glibc, this function is not a part of the standard C Library so it's not present in all LibC implementations

### Usage
The program is executed as any other executable file (that needs Root) on Linux.

    sudo ./thinkpad-masturbator

> If you're worried this program might harmful to your computer, you can check the source code and see there's nothing harmful by running the program... If you're still worried, you can simply not to run it.

If it detects a TrackPoint, the program will welcome you, since that, you'll be able to interact with the TrackPoint, the program will show diferent messages until it "gets tired" and closes itself.

## Operation
**TrackPoint Detection:** The first big step int he program is to detect what device corresponds to a TrackPoint, this is done by reading the file `/proc/bus/input/devices`, the program will return the device that is recognized as a mouse and includes `trackpoint` in its name. If not mouse with `trackpoint` in its name is found, it will discard devices with `optical`, `trackpad`, `wired` or `usb` in their names and return the remaining device... Once a TrackPoint is detected we can read its events.
 
**Linux Subsystem API:** This program uses the Linux Input Subsystem API (in a very simple way) to read the TrackPoint raw events directly from the kernel. Since Linux follows the Unix's idea that "Everything is a file", this process is as simple as reading the file `/dev/input/eventX` file, and using the struct `input_event` (also provided by Linux) to parse it.

## About the phrases and the narrative
Although this started as a joke without a deep meaning, as the weeks i decided to give the program deeper and more human "toughts", the computer becomes a character with feelings, desires and insecurities.

While the program runs, we'll see it is jealous to other programs, specially server-based programs, and Artificial Intelligences, the program is technically opposite to server based programs and AIs, it is an small C program that runs in the user's hardware and it's limited to say the same phrases, that makes it feel insecure and inferior to them. Also, the program is emotionally and technically dependent of `root`, if you try to run the program with any other user you will be instantly rejected.

The phrases can be seen in [main.c:line 48](https://github.com/pasteluengas/thinkpad-masturbator/blob/main/main.c#L48).
