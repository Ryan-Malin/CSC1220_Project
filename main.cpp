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

int main()
{
    cout << "====================================" << endl;
    cout << "  Virtual Memory Simulator" << endl;
    cout << "====================================" << endl;
    cout << endl;

    // CONFIGURATION
    int numFrames = 4;  // 4 frames of physical memory
    int maxPages = 100; // Support up to 100 different pages

    // I need to select the correct file for testing.
    string traceFile = "test_trace.txt";
    // string traceFile = "loop_trace.txt";
    // string traceFile = "challenge_trace.txt";

    cout << "Configuration:" << endl;
    cout << "  Frames: " << numFrames << endl;
    cout << "  Trace file: " << traceFile << endl;
    cout << endl;

    // Parse the trace file once
    TraceParser parser;
    vector<TraceParser::Access> accesses = parser.parseFile(traceFile);

    if (accesses.empty())
    {
        cout << "No accesses to simulate. Exiting." << endl;
        return 1;
    }

    cout << endl;

    // Array of algorithm names
    string algorithms[] = {"FIFO", "LRU", "Clock"};

    // Test each algorithm
    for (int alg = 0; alg < 3; alg++)
    {
        cout << "====================================" << endl;
        cout << "  Testing: " << algorithms[alg] << endl;
        cout << "====================================" << endl;
        cout << endl;

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

        // Run the simulation with this algorithm
        for (int i = 0; i < accesses.size(); i++)
        {
            char op = accesses[i].operation;
            int page = accesses[i].pageNum;

            // Uncomment next line to see detailed output
            // cout << "Access " << i << ": " << op << " " << page << endl;

            processMemoryAccess(op, page, state, policy);
        }

        // Print results for this algorithm
        printStatistics(state);

        // Clean up
        delete policy;

        cout << endl;
    }

    return 0;
}