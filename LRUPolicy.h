#pragma once
#include <vector>
#include <limits>
#include "ReplacementPolicy.h"

// Implements the Least Recently Used (LRU) page replacement policy.
class LRUPolicy : public ReplacementPolicy
{
public: // Destructor
    int pickVictimFrame(const SimulatorState &state) override
    {
        ensureSized_(state);

        unsigned long long bestTime = std::numeric_limits<unsigned long long>::max();
        int victim = -1;

        for (int f = 0; f < (int)state.frame_to_page.size(); ++f)
        {
            if (state.frame_to_page[f] == -1)
                continue; // empty
            if (lastUsed_[f] < bestTime)
            {
                bestTime = lastUsed_[f];
                victim = f;
            }
        }

        if (victim == -1)
        {
            for (int f = 0; f < (int)state.frame_to_page.size(); ++f)
                if (state.frame_to_page[f] != -1)
                    return f;
            return 0;
        }
        return victim;
    }

    // Called when a page is accessed. We update the last used time for the frame.
    void onPageAccess(int frame, bool /*isWrite*/) override
    {
        if (frame >= 0 && frame < (int)lastUsed_.size())
        {
            lastUsed_[frame] = ++tick_;
        }
    }

    void onPageLoaded(int frame, int /*page*/) override
    {
        if (frame >= (int)lastUsed_.size())
            lastUsed_.resize(frame + 1, 0);
        if (frame >= 0)
            lastUsed_[frame] = ++tick_;
    }

    void onPageEvicted(int frame, int /*page*/) override
    {
        if (frame >= 0 && frame < (int)lastUsed_.size())
            lastUsed_[frame] = 0;
    }

    // Private helper to ensure our lastUsed_ vector is the right size for the current number of frames.
private:
    void ensureSized_(const SimulatorState &state)
    {
        if (lastUsed_.size() != state.frame_to_page.size())
        {
            lastUsed_.assign(state.frame_to_page.size(), 0);
            tick_ = 0;
        }
    }

    std::vector<unsigned long long> lastUsed_;
    unsigned long long tick_ = 0;
};
