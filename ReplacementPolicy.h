#ifndef REPLACEMENT_POLICY_H
#define REPLACEMENT_POLICY_H

#include "SimulatorState.h"

// Abstract base class - Connor will inherit from this for FIFO, LRU, Clock
class ReplacementPolicy
{
public:
    virtual ~ReplacementPolicy() {}

    virtual int pickVictimFrame(const SimulatorState &state) = 0; // when memory is full, which frame should we evict?

    virtual void onPageAccess(int frame, bool isWrite) = 0; // called when a page is accessed.

    virtual void onPageLoaded(int frame, int page) = 0; // Called when a new page is loaded into a frame

    virtual void onPageEvicted(int frame, int page) = 0; // Called when a page is evicted from a frame
};

#endif