#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

void showHelp() {
    cout    << "Game of Life v1.06 Solver \n"
            << "Usage: gameOfLife <path to Life v1.06 file>\n";
}

void main (int argc, char** argv){
    if (argc < 2) {
        showHelp();
        return;
    }
    cout << "Solving Game Of Life!" << endl;
    char* filePath = argv[1];
    cout << "Using starting file of: \"" << filePath << "\"";

    // This is our datastructure for representing the boardstate.
    // Given the constraints that this game is in 64-bit signed integer space: (LLONG_MIN, LLONG_MAX), its infeasible to store the state of _EVERY_ cell in a 2D array, as it has a size complexity of N*M, where N = LLONG_MAX, M = LLONG_MAX, for a theoretical max size of (2^64)^2 = 3.40e38 bits
    // Therefore, we can use an alternate datastructure to only track the current alive cells.
    // Using hash map allows us to retain the constant-time access to individual cells, similar to our nested arrays, while giving us a better worst-case size-complexity pf 2*N, where N is the number of alive cells in our simulation.
    // Also, in order to minimize memory usage due to our large simulation size, we will be doing each simulation step in-place.
    
    // In this implementation We use a pair of X,Y coordinates as our key, with an int representing the cell state, enumerated as:
    // 0 = dead
    // 1 = alive
    // 2 = dead -> alive next step
    // 3 = alive -> dead next step
    unordered_map<pair<long long, long long>, int> cells();

    // read from the input file, 
    ifstream lifeFile (filePath);
    string line = {};
    int numInitialAliveCells = 0;
    if (lifeFile.is_open()) {
        // The first line of a life v1.06 indicates the version.
        // TODO: If we wanted to be throrough, validate the Life file version is what we expected.
        // For now, just skip it.
        getline(lifeFile, line);

        string xString, yString;
        long long x, y = 0L;
        // populate our initial datastructure with the provided alive cells.
        while (getline(lifeFile, line)) {
            cout << line << endl;
            istringstream iss(line);
            iss >> x >> y;
            cout << "X: " << x << " Y: " << y;
            numInitialAliveCells++;
        }
        lifeFile.close();
    }

    cout << "Starting with initial board of " << numInitialAliveCells << "." << endl;
}