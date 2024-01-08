# Sound Tetris | CTU MM1 Project
by [Chipi & Chapa Corp.](https://exposedcat.notion.site/Chipi-Chapa-Corp-Home-f21182d34d3b41be999243534f871a9e) : Artem Prokop, Hanna Hliavitskaya

![Gameplay](video/gameplay.gif)

Simple classic Tetris written on C++ using [IIM AV library](https://projects.iim.cz/support:iimavlib). 

All in-game sounds are generated, see `background-sound.cpp` and `brick-sound.cpp` for impementation.

For understanding game logic check out `controller` class.

## Build and start
To build the project you can start the BASH script `prepare_linux.sh`

``./prepare_linux.sh``

To start the app you can run

``./build/main``

## Controls
- `Q` : Quit app
- `A` : Rotate figure
- `Arrow Left` : Move left
- `Arrow Right` : Move right
- `Arrow Down`  : Move down
- `Arrow Up`  : Send brick to the bottom