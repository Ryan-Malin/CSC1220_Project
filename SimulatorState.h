#ifndef SIMULATOR_STATE_H
#define SIMULATOR_STATE_H

#include <vector>
using namespace std;

struct SimulatorState
{
    // Configuration
    int numFrames; // running count of frames.
    int numPages;  // count of pages.

    vector<int> page_to_frame; // this is our mapping. -1 menas not in a frame. Otherwise, 'page_to_frame[4] = 2' means that page 2 is in frame 4.

    vector<int> frame_to_page; // this is the reverse mapping of the above.

    vector<bool> frame_dirty; // this means it's been modified and needs to be written back to disk if we evict it.

    // Stats
    int pageFaults;    // How many times we had to load from disk
    int pageHits;      // How many times page was already in memory, which is low cost and efficient!
    int writebacks;    // How many dirty pages we had to write back
    int totalAccesses; // Total number of memory accesses

    // Constructor - sets everything up
    SimulatorState(int frames, int pages)
    {
        numFrames = frames;
        numPages = pages;

        page_to_frame.resize(pages, -1); // Initialize page table (all pages start NOT in memory)

        frame_to_page.resize(frames, -1); // Initialize frame table (all frames start empty)

        frame_dirty.resize(frames, false); // Initialize dirty bits (all frames start clean)

        // Initialize statistics to zero
        pageFaults = 0;
        pageHits = 0;
        writebacks = 0;
        totalAccesses = 0;
    }
};

#endif