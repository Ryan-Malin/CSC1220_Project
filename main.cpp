#include "SimulatorState.h"
#include "ReplacementPolicy.h"
#include "TraceParser.h"

// Connor's Files
#include "FIFOPolicy.h"
#include "LRUPolicy.h"
#include "ClockPolicy.h"

// template stuff
#include <iostream>
using namespace std;

// Declare the functions from Simulator.cpp
void processMemoryAccess(char operation, int pageNum,
                         SimulatorState &state,
                         ReplacementPolicy *policy);
void printMemoryState(const SimulatorState &state);
void printStatistics(const SimulatorState &state);

int main()
{
    cout << "====================================" << endl;
    cout << "  Virtual Memory Simulator" << endl;
    cout << "====================================" << endl;
    cout << endl;

    // CONFIGURATION
    int numFrames = 4;  // 4 frames of physical memory
    int maxPages = 100; // Support up to 100 different pages
    string traceFile = "test_trace.txt";

    cout << "Configuration:" << endl;
    cout << "  Frames: " << numFrames << endl;
    cout << "  Trace file: " << traceFile << endl;
    cout << "  Algorithm: FIFO" << endl;
    cout << endl;

    // STEP 1: Initialize the simulator state
    SimulatorState state(numFrames, maxPages);

    // STEP 2: Create the replacement policy (FIFO for now)
    ReplacementPolicy *policy = new SimpleFIFO();

    // STEP 3: Parse the trace file
    TraceParser parser;
    vector<TraceParser::Access> accesses = parser.parseFile(traceFile);

    if (accesses.empty())
    {
        cout << "No accesses to simulate. Exiting." << endl;
        delete policy;
        return 1;
    }

    cout << endl;
    cout << "====================================" << endl;
    cout << "  Starting Simulation" << endl;
    cout << "====================================" << endl;
    cout << endl;

    // STEP 4: Run the simulation - process each access
    for (int i = 0; i < accesses.size(); i++)
    {
        char op = accesses[i].operation;
        int page = accesses[i].pageNum;

        cout << "Access " << i << ": " << op << " " << page << endl;

        processMemoryAccess(op, page, state, policy);

        // Optionally print memory state after each access
        // printMemoryState(state);
    }

    // STEP 5: Print final statistics
    printStatistics(state);

    // STEP 6: Clean up
    delete policy;

    return 0;
}