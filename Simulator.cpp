#include "SimulatorState.h"
#include "ReplacementPolicy.h"
#include <iostream>
using namespace std;

// This is THE main function - simulates one memory access
void processMemoryAccess(char operation, int pageNum,
                         SimulatorState &state,
                         ReplacementPolicy *policy)
{

    state.totalAccesses++;
    bool isWrite = (operation == 'W');

    // STEP 1: Check if page is already in memory (look up in page table)
    int frame = state.page_to_frame[pageNum];

    if (frame != -1)
    {
        // ============================================
        // PAGE HIT - Page is already in memory!
        // ============================================
        state.pageHits++;

        cout << "HIT: Page " << pageNum << " already in Frame " << frame << endl;

        // Notify the algorithm (for LRU/Clock tracking)
        policy->onPageAccess(frame, isWrite);

        // If it's a write, mark the frame as dirty
        if (isWrite)
        {
            state.frame_dirty[frame] = true;
            cout << "  -> Frame " << frame << " is now DIRTY" << endl;
        }
    }
    else
    {
        // ============================================
        // PAGE FAULT - Page is NOT in memory!
        // ============================================
        state.pageFaults++;

        cout << "FAULT: Page " << pageNum << " not in memory" << endl;

        // STEP 2: Find a free frame (if any exist)
        int targetFrame = -1;
        for (int i = 0; i < state.numFrames; i++)
        {
            if (state.frame_to_page[i] == -1)
            {
                targetFrame = i; // Found a free frame!
                cout << "  -> Found free Frame " << targetFrame << endl;
                break;
            }
        }

        // STEP 3: If no free frame, we must EVICT someone
        if (targetFrame == -1)
        {
            cout << "  -> Memory FULL, must evict..." << endl;

            // Ask the replacement algorithm which frame to evict
            targetFrame = policy->pickVictimFrame(state);
            int victimPage = state.frame_to_page[targetFrame];

            cout << "  -> Evicting Page " << victimPage << " from Frame " << targetFrame << endl;

            // STEP 4: Check if victim is dirty (needs writeback)
            if (state.frame_dirty[targetFrame])
            {
                state.writebacks++;
                cout << "  -> WRITEBACK required (dirty page)" << endl;
            }

            // Update victim's page table entry (it's no longer in memory)
            state.page_to_frame[victimPage] = -1;

            // Notify the algorithm about the eviction
            policy->onPageEvicted(targetFrame, victimPage);
        }

        // STEP 5: Load the new page into the frame
        state.frame_to_page[targetFrame] = pageNum;
        state.page_to_frame[pageNum] = targetFrame;
        state.frame_dirty[targetFrame] = isWrite; // Dirty if first access is write

        cout << "  -> Loaded Page " << pageNum << " into Frame " << targetFrame << endl;

        // Notify the algorithm about the new page
        policy->onPageLoaded(targetFrame, pageNum);
        policy->onPageAccess(targetFrame, isWrite);
    }

    cout << endl; // Blank line between accesses for readability
}

// Helper function to print current memory state
void printMemoryState(const SimulatorState &state)
{
    cout << "=== Current Memory State ===" << endl;
    for (int i = 0; i < state.numFrames; i++)
    {
        if (state.frame_to_page[i] != -1)
        {
            cout << "Frame " << i << ": Page " << state.frame_to_page[i];
            if (state.frame_dirty[i])
            {
                cout << " (DIRTY)";
            }
            cout << endl;
        }
        else
        {
            cout << "Frame " << i << ": [empty]" << endl;
        }
    }
    cout << endl;
}

// Helper function to print final statistics
void printStatistics(const SimulatorState &state)
{
    cout << "======================================" << endl;
    cout << "=== Final Simulation Statistics ===" << endl;
    cout << "======================================" << endl;
    cout << "Total Accesses:  " << state.totalAccesses << endl;
    cout << "Page Hits:       " << state.pageHits << endl;
    cout << "Page Faults:     " << state.pageFaults << endl;
    cout << "Writebacks:      " << state.writebacks << endl;
    cout << "--------------------------------------" << endl;

    double hitRate = 100.0 * state.pageHits / state.totalAccesses;
    double faultRate = 100.0 * state.pageFaults / state.totalAccesses;

    cout << "Hit Rate:        " << hitRate << "%" << endl;
    cout << "Fault Rate:      " << faultRate << "%" << endl;
    cout << "======================================" << endl;
}