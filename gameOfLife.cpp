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

            // If the cell value is odd, then we are alive this tick
            if (itYCells->second % 2 == 1) {;
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
                        if (j > 0 && currentYCoordinate + j < currentYCoordinate) {
                            cout << "Skipping processing cell below of " << currentYCoordinate << "..." << endl;
                            continue;
                        }
                        else if (j < 0 && currentYCoordinate + j > currentYCoordinate) {
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
    for (auto itUpdateCellsX = cellsToUpdate.begin(); itUpdateCellsX != cellsToUpdate.end(); ++itUpdateCellsX) {
        long long currentXCoordinate = itUpdateCellsX->first;
        for (auto itUpdateCellsY = itUpdateCellsX->second.begin(); itUpdateCellsY != itUpdateCellsX->second.end(); ++itUpdateCellsY) {
            long long currentYCoordinate = *itUpdateCellsY;

            // Retrieve the cell state from our datastructure.
            CellState currentCellState = CellState::Dead;
            if (cells->find(currentXCoordinate) != cells->end()) {
                if ((*cells)[currentXCoordinate].find(currentYCoordinate) != (*cells)[currentXCoordinate].end()) {
                    currentCellState = (*cells)[currentXCoordinate][currentYCoordinate];
                }
            }

            int neighboringAliveCells = 0;
            
            // given the cell state, update the board using its neighboring cell's states
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0) {
                        continue;
                    }   

                    // check for overflow, and avoid processing cells outside of the cellular bounds.
                    if (i > 0 && currentXCoordinate + i < currentXCoordinate) {
                        cout << "Skipping processing cell right of " << currentXCoordinate << "..." << endl;
                        continue;
                    }
                    else if (i < 0 && currentXCoordinate + i > currentXCoordinate) {
                        cout << "Skipping processing cell left of " << currentXCoordinate << "..." << endl;
                        continue;
                    }
                    if (j > 0 && currentYCoordinate + j < currentYCoordinate) {
                        cout << "Skipping processing cell below of " << currentYCoordinate << "..." << endl;
                        continue;
                    }
                    else if (j < 0 && currentYCoordinate + j > currentYCoordinate) {
                        cout << "Skipping processing cell above of " << currentYCoordinate << "..." << endl;
                        continue;
                    }

                    // Retrieve the cell state from our datastructure.
                    CellState currentNeighboringCellState = CellState::Dead;
                    if (cells->find(currentXCoordinate + i) != cells->end()) {
                        if ((*cells)[currentXCoordinate + i].find(currentYCoordinate + j) != (*cells)[currentXCoordinate + i].end()) {
                            currentNeighboringCellState = (*cells)[currentXCoordinate + i][currentYCoordinate + j];
                        }
                    }

                    if (currentNeighboringCellState == CellState::Alive || currentNeighboringCellState == CellState::BecomingDead) {
                        neighboringAliveCells++;
                        cout << "( " << currentXCoordinate << " , " << currentYCoordinate<< " ) has alive neighbor ( " << currentXCoordinate + i << " , " << currentYCoordinate + j  << ") " << endl;
                    }
                }
            }

            // if the current cell is alive, then an alive neighbor count of 2 or 3 will sustain it, else it dies.
            if (currentCellState == CellState::Alive) {
                (*cells)[currentXCoordinate][currentYCoordinate] = neighboringAliveCells >= 2 && neighboringAliveCells <= 3 ? CellState::Alive : CellState::BecomingDead;
                cout << "Alive cell ( " << currentXCoordinate << " , " << currentYCoordinate << " ) has " << neighboringAliveCells << " neighbors." << endl;
            }
            // if the current cell is dead, then an alive neighbor count of 3 will become alive, else it remains dead.
            if (currentCellState == CellState::Dead) {
                CellState state = neighboringAliveCells == 3 ? CellState::BecomingAlive : CellState::Dead;
                (*cells)[currentXCoordinate][currentYCoordinate] = state;
                cout << "Dead cell ( " << currentXCoordinate << " , " << currentYCoordinate << " ) has " << neighboringAliveCells << " neighbors. Setting to " << ((state == CellState::BecomingAlive) ? "Alive" : "Dead") << endl;
                
                // if this cell stayed dead between frames, remove it from our board to minimize bloat.
                if (state == CellState::Dead) {
                    if (cellsToRemove.find(currentXCoordinate) != cellsToRemove.end()) {
                        cellsToRemove.insert(pair<long long, unordered_set<long long>>(currentXCoordinate, unordered_set<long long>()));
                    }
                    cellsToRemove[currentXCoordinate].insert(currentYCoordinate);
                    cout << "Clearing (" << currentXCoordinate << " , " << currentYCoordinate << " ) to be removed from board.";
                }
            }
        }
    }

    // Step 3: prune any irrelevant cells
    for (auto itXCells = cellsToRemove.begin(); itXCells != cellsToRemove.end(); ++itXCells) {
        long long currentXCoordinate = itXCells->first;
        for (auto itYCells = itXCells->second.begin(); itYCells != itXCells->second.end(); ++itYCells) {
            long long currentYCoordinate = *itYCells;

            if (cells->find(currentXCoordinate) != cells->end()) {
                auto& currentColumn = (*cells)[currentXCoordinate];
                if (currentColumn.find(currentYCoordinate) != currentColumn.end()) {
                    currentColumn.erase(currentYCoordinate);
                    cout << "Clearing (" << currentXCoordinate << " , " << currentYCoordinate << " ) from board.";
                }
                if (currentColumn.empty()) {
                    cells->erase(currentXCoordinate);
                    cout << "Clearing (" << currentXCoordinate << " , " << " ) from board.";
                }
            }
        }
    }

}

void printOutputBoard(board* cells) {
    cout << "#Life 1.06" << endl;
    for (auto itXCells = cells->begin(); itXCells != cells->end(); ++itXCells) {
        for (auto itYCells = itXCells->second.begin(); itYCells != itXCells->second.end(); ++itYCells) {
            // if the cell is alive this tick, print its coordinates.
            if (itYCells->second % 2 == 1) {
                cout << itXCells->first << " " << itYCells->first << endl;
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
    
    // In this implementation We use nested hash maps, with X and Y coordinates as the respective keys, with an integer enum representing the state.
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
                if (cells.find(x) == cells.end()) {
                    cells.insert(pair<long long, unordered_map<long long, CellState>>(x, unordered_map<long long, CellState>()));
                }
                cells[x][y] = CellState::Alive;
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