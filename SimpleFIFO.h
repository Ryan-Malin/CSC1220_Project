#ifndef SIMPLE_FIFO_H
#define SIMPLE_FIFO_H

#include "ReplacementPolicy.h"
#include <queue>
using namespace std;

// Super simple FIFO just for testing
class SimpleFIFO : public ReplacementPolicy
{
private:
    queue<int> loadOrder; // Track order frames were loaded

public:
    int pickVictimFrame(const SimulatorState &state) override
    {
        // Return the oldest frame (front of queue)
        return loadOrder.front();
    }

    void onPageAccess(int frame, bool isWrite) override
    {
        // FIFO doesn't care about accesses
    }

    void onPageLoaded(int frame, int page) override
    {
        // Add frame to back of queue
        loadOrder.push(frame);
    }

    void onPageEvicted(int frame, int page) override
    {
        // Remove from front of queue
        loadOrder.pop();
    }
};

#endif