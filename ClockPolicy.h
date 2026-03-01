#pragma once
#include <vector>
#include "ReplacementPolicy.h"

// The ClockPolicy class implements the clock page replacement algorithm, which is a more efficient version of the second-chance algorithm. It uses a circular buffer (or "clock") to keep track of which pages have been accessed recently
class ClockPolicy : public ReplacementPolicy
{
public:
    int pickVictimFrame(const SimulatorState &state) override
    {
        ensureSized_(state);

        int n = (int)state.frame_to_page.size();
        if (n == 0)
            return 0;

        while (true)
        {
            int f = hand_;

            // Defensive: if empty frame exists, return it.
            if (state.frame_to_page[f] == -1)
            {
                advance_(n);
                return f;
            }

            if (ref_[f])
            {
                ref_[f] = false; // second chance used up
                advance_(n);
            }
            else
            {
                advance_(n);
                return f;
            }
        }
    }
    // For this policy, we only care about whether the page was accessed or not, so we ignore the isWrite parameter.
    void onPageAccess(int frame, bool /*isWrite*/) override
    {
        if (frame >= 0 && frame < (int)ref_.size())
            ref_[frame] = true;
    }
    // When a new page is loaded into a frame, we want to mark that frame as recently accessed (i.e., give it a "second chance").
    void onPageLoaded(int frame, int /*page*/) override
    {
        if (frame >= (int)ref_.size())
            ref_.resize(frame + 1, false);
        if (frame >= 0)
            ref_[frame] = true;
    }
    // When a page is evicted from a frame, we want to clear the reference bit for that frame since it's now empty.
    void onPageEvicted(int frame, int /*page*/) override
    {
        if (frame >= 0 && frame < (int)ref_.size())
            ref_[frame] = false;
    }

    // Note: The ensureSized_ function is called at the beginning of pickVictimFrame to make sure the ref_ vector is the correct size. This way, if the number of frames changes during the simulation, we can handle it gracefully without running into out-of-bounds errors.
private:
    void ensureSized_(const SimulatorState &state)
    {
        if (ref_.size() != state.frame_to_page.size())
        {
            ref_.assign(state.frame_to_page.size(), false);
            hand_ = 0;
        }
    }

    void advance_(int n) { hand_ = (hand_ + 1) % n; }

    std::vector<bool> ref_;
    int hand_ = 0;
};
