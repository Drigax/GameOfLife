This is a C++ implementation of [Conway's game of life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules).

This solution assumes a 64 bit signed integer space for the maximum game board size:[-2^63, 2^63-1], and implements this solution using hashtables and hashsets.

Build instructions:

`mkdir build`

`cd build`

`cmake ..`

If on Windows, `Project.sln` will be generated in the build folder. build the solution and execute gameOfLife.exe with an input [ASCII Life v1.06 file](https://www.conwaylife.com/wiki/Life_1.06).

Runtime Usage:

`gameOfLife <path to Life v1.06 file> [number of iterations (default: 10)]`
