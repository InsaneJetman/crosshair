![Crosshair](https://raw.githubusercontent.com/InsaneJetman/crosshair/master/img/cross64.png)

# Crosshair
A crosshair for Windows

This program creates a simple crosshair in that floats above other windows.
By default it will be placed in the middle of the screen, but it can be moved around as needed.
Generally, it won't work for full-screen applications, but it might for some.

## Usage

For example, if you're emulating a game such as *007 GoldenEye*,
just run `crosshair`, and drag the crosshair to the centre of the emulator window.
Now you have a nice crosshair for aiming.

To close `crosshair`, switch to the application and press `Alt+F4`, or just right-click it on the task bar and choose "Close window".

## Compiling

Use the included Code::Blocks project, or compile from the command line using the MinGW compiler:

    windres resource.rc resource.o
    g++ -s -O2 -static -mwindows -o crosshair.exe crosshair.cpp resource.o

## Binary Releases

Get the lastest binary release [here](https://github.com/InsaneJetman/crosshair/releases/latest).

## License

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
- Copyright © 2019 InsaneJetman
