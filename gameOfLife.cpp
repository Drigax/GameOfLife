#include <iostream>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

const int DEFAULT_NUMBER_OF_ITERATIONS = 10;

const enum CellState {
    Dead = 0,
    Alive = 1,
    BecomingDead = 2,
    BecomingAlive = 3
};

// Setup typedefs for our game board types to make code easier to read.
typedef unordered_map<long long, unordered_map<long long , CellState>> board;

void showHelp() {
    cout    << "Game of Life v1.06 Solver \n"
            << "Usage: gameOfLife <path to Life v1.06 file> [number of iterations (default: 10)]\n";
}

void gameOfLifeStep(board* cells) {
    unordered_map<long long, unordered_set<long long>> cellsToUpdate{};
    unordered_map<long long, unordered_set<long long>> cellsToRemove{};
    
    // Step 1: iterate through board, updating becoming states, and populating cells to update this tick.
    for (auto itXCells = cells->begin(); itXCells != cells->end(); ++itXCells) {
        long long currentXCoordinate = itXCells->first;
        for (auto itYCells = itXCells->second.begin(); itYCells != itXCells->second.end(); ++itYCells) {
            long long currentYCoordinate = itYCells->first;

            // If the cell value is even, then we are dead this tick
            if (itYCells->second % 2 == 0) {;
                itYCells->second = CellState::Alive;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        // check for overflow, and avoid processing cells outside of the cellular bounds.
                        if (i > 0 && currentXCoordinate + i < currentXCoordinate) {
                            cout << "Skipping processing cell right of " << currentXCoordinate << "..." << endl;
                            continue;
                        }
                        else if (i < 0 && currentXCoordinate + i > currentXCoordinate) {
                            cout << "Skipping processing cell left of " << currentXCoordinate << "..." << endl;
                            continue;
                        }
                        if (j > 0 && currentYCoordinate + i < currentYCoordinate) {
                            cout << "Skipping processing cell below of " << currentYCoordinate << "..." << endl;
                            continue;
                        }
                        else if (j < 0 && currentYCoordinate + i > currentYCoordinate) {
                            cout << "Skipping processing cell above of " << currentYCoordinate << "..." << endl;
                            continue;
                        }

                        if (cellsToUpdate.find(currentXCoordinate + i) == cellsToUpdate.end()) {
                            cellsToUpdate[currentXCoordinate + i] = unordered_set<long long>();
                        }

                        cellsToUpdate[currentXCoordinate + i].insert(currentYCoordinate + j);
                        cout << "Adding ( " << currentXCoordinate + i << " , " << currentYCoordinate + j << " ) to cells to process this tick" << endl;
                    }
                }
            }
            else {
                itYCells->second = CellState::Dead;
            }
        }
    }

    // Step 2: iterate through cells to update, and set any becoming states
    for (auto itUpdateCells = cellsToUpdate.begin(); itUpdateCells != cellsToUpdate.end(); ++itUpdateCells) {

    }

    // Step 3: prune any irrelevant cells

}

void printOutputBoard(board* cells) {
    cout << "#Life 1.06" << endl;
    for (auto itXCells = cells->begin(); itXCells != cells->end(); ++itXCells) {
        for (auto itYCells = itXCells->second.begin(); itYCells != itXCells->second.end(); ++itYCells) {
            // if the cell is alive this tick, print its coordinates.
            if (itYCells->second % 2 == 1) {
                cout << itXCells->first << itYCells->first << endl;
            }
        }
    }
}

void main (int argc, char** argv){
    if (argc < 2) {
        showHelp();
        return;
    }
    cout << "Solving Game Of Life!" << endl;
    char* filePath = argv[1];
    cout << "Using starting file of: \"" << filePath << "\"" << endl;

    int numberOfIterations = DEFAULT_NUMBER_OF_ITERATIONS;
    if (argc > 2) {
        try {
            numberOfIterations = atoi(argv[2]);
        }
        catch (exception e) {
            cerr << "Invalid number of iterations: " << argv[2];
        }
    }

    // This is our datastructure for representing the boardstate.
    // Given the constraints that this game is in 64-bit signed integer space: (LLONG_MIN, LLONG_MAX), its infeasible to store the state of _EVERY_ cell in a 2D array, as it has a size complexity of N*M, where N = LLONG_MAX, M = LLONG_MAX, for a theoretical max size of (2^64)^2 = 3.40e38 bits
    // Therefore, we can use an alternate datastructure to only track the current alive cells.
    // Using hash map allows us to retain the constant-time access to individual cells, similar to our nested arrays, while giving us a better worst-case size-complexity pf 2*N, where N is the number of alive cells in our simulation.
    // Also, in order to minimize memory usage due to our large simulation size, we will be doing each simulation step in-place.
    
    // In this implementation We use a pair of X,Y coordinates as our key, with an integer enum representing the state.
    board cells{};

    // Read from the input file to setup our initial board state.
    ifstream lifeFile (filePath);
    string line = {};
    unsigned long long numInitialAliveCells = 0;
    if (lifeFile.is_open()) {
        // The first line of a life v1.06 indicates the version.
        // TODO: If we wanted to be throrough, validate the Life file version is what we expected.
        // For now, just skip it.
        getline(lifeFile, line);

        string xString, yString;
        long long x, y = 0L;
        // populate our initial datastructure with the provided alive cells.
        while (getline(lifeFile, line)) {
            try {
                //cout << line << endl;
                istringstream iss(line);
                iss >> x >> y;
                cout << "X: " << x << " Y: " << y << endl;
                numInitialAliveCells++;
            }
            catch (exception e) {
                cerr << "Invalid input: " << line << endl << "Exiting..." << endl;
                return;
            }
        }
        lifeFile.close();
    }
    else {
        cout << "Unable to open file: " << filePath << endl;
        return;
    }

    cout << "Starting with initial board of " << numInitialAliveCells << " cells." << endl;
    cout << "Simulating " << numberOfIterations << " simulation steps..." << endl;

    // Run the game of life simulation the number of times specified, then print the result.
    for (int i = 0; i < numberOfIterations; ++i) {
        gameOfLifeStep(&cells);
    }

    // finally, print the resulting game board
    cout << endl << "Resulting board is: " << endl;
    printOutputBoard(&cells);
}