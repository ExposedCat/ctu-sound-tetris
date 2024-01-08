# Sound Tetris | CTU MM1 Project
by [Chipi & Chapa Corp.](https://exposedcat.notion.site/Chipi-Chapa-Corp-Home-f21182d34d3b41be999243534f871a9e) : Artem Prokop, Hanna Hliavitskaya

![Gameplay](video/gameplay.gif)

Simple classic Tetris written in C++ using [IIM AV library](https://projects.iim.cz/support:iimavlib). 


## Short Description
Game generates sounds for all events happening. Speed Level System accelerates not only game, but also all the sounds - bigger score = more intense sounds!

## Project Structure
- `src` is a main folder dir with all the sources
- `dependenies` must contain compiled IIMAV library and `includes` dir with its headers
- `background-sound` and `brick-sound` files manage respective sounds within a game
- `controller` contains main game logic
- `utils` contains utilites (e.g. random generator)
- `brick` represents bricks (tetrominoes) logic
- `main` is a game entrypoint that combines all the pieces together 
## Build and start
### via Script
There is a script that builds & starts the game on Linux:
`./start-linux.sh`

### Manual
To build the project you can use `cmake`:
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
Then you should be able to start game via
- `./main`

## Controls
- `Q` : Quit app
- `A` : Rotate figure
- `Arrow Left` : Move left
- `Arrow Right` : Move right
- `Arrow Down`  : Move down
- `Arrow Up`  : Send brick to the bottom