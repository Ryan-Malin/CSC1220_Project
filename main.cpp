#include "SimulatorState.h"
#include "ReplacementPolicy.h"
#include "TraceParser.h"
#include "FIFOPolicy.h"
#include "LRUPolicy.h"
#include "ClockPolicy.h"
#include <iostream>
using namespace std;

// Declare the functions from Simulator.cpp
void processMemoryAccess(char operation, int pageNum,
                         SimulatorState &state,
                         ReplacementPolicy *policy);
void printMemoryState(const SimulatorState &state);
void printStatistics(const SimulatorState &state);

void runSimulation(string traceFile, int numFrames)
{
    cout << "\n"
         << string(60, '=') << endl;
    cout << "TRACE FILE: " << traceFile << endl;
    cout << string(60, '=') << endl;

    int maxPages = 100;

    // Parse the trace file
    TraceParser parser;
    vector<TraceParser::Access> accesses = parser.parseFile(traceFile);

    if (accesses.empty())
    {
        cout << "Could not load trace file. Skipping." << endl;
        return;
    }

    cout << endl;

    // Array of algorithm names
    string algorithms[] = {"FIFO", "LRU", "Clock"};

    // Test each algorithm
    for (int alg = 0; alg < 3; alg++)
    {
        cout << "---- " << algorithms[alg] << " ----" << endl;

        // Create fresh state for this algorithm
        SimulatorState state(numFrames, maxPages);

        // Create the appropriate policy
        ReplacementPolicy *policy = nullptr;

        if (algorithms[alg] == "FIFO")
        {
            policy = new FIFOPolicy();
        }
        else if (algorithms[alg] == "LRU")
        {
            policy = new LRUPolicy();
        }
        else if (algorithms[alg] == "Clock")
        {
            policy = new ClockPolicy();
        }

        // Run the simulation (quiet mode - no detailed output)
        for (int i = 0; i < accesses.size(); i++)
        {
            char op = accesses[i].operation;
            int page = accesses[i].pageNum;
            processMemoryAccess(op, page, state, policy);
        }

        // Print compact results
        cout << "  Accesses: " << state.totalAccesses
             << " | Hits: " << state.pageHits
             << " | Faults: " << state.pageFaults
             << " | Writebacks: " << state.writebacks
             << " | Hit Rate: " << (100.0 * state.pageHits / state.totalAccesses) << "%"
             << endl;

        // Clean up
        delete policy;
    }
}

int main()
{
    cout << string(60, '=') << endl;
    cout << "VIRTUAL MEMORY SIMULATOR - TESTING SUITE" << endl;
    cout << string(60, '=') << endl;

    int numFrames = 4;
    cout << "\nConfiguration: " << numFrames << " frames\n"
         << endl;

    // Test all trace files
    runSimulation("test_trace.txt", numFrames);
    runSimulation("loop_trace.txt", numFrames);
    runSimulation("challenge_trace.txt", numFrames);
    runSimulation("locality_trace.txt", numFrames);

    return 0;
}