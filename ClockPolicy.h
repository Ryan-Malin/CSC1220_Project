#pragma once
#include <vector>
#include "ReplacementPolicy.h"

class ClockPolicy : public ReplacementPolicy {
public:
    int pickVictimFrame(const SimulatorState& state) override {
        ensureSized_(state);

        int n = (int)state.frame_to_page.size();
        if (n == 0) return 0;

        while (true) {
            int f = hand_;

            // Defensive: if empty frame exists, return it.
            if (state.frame_to_page[f] == -1) {
                advance_(n);
                return f;
            }

            if (ref_[f]) {
                ref_[f] = false;   // second chance used up
                advance_(n);
            } else {
                advance_(n);
                return f;
            }
        }
    }

    void onPageAccess(int frame, bool /*isWrite*/) override {
        if (frame >= 0 && frame < (int)ref_.size()) ref_[frame] = true;
    }

    void onPageLoaded(int frame, int /*page*/) override {
        if (frame >= (int)ref_.size()) ref_.resize(frame + 1, false);
        if (frame >= 0) ref_[frame] = true;
    }

    void onPageEvicted(int frame, int /*page*/) override {
        if (frame >= 0 && frame < (int)ref_.size()) ref_[frame] = false;
    }

private:
    void ensureSized_(const SimulatorState& state) {
        if (ref_.size() != state.frame_to_page.size()) {
            ref_.assign(state.frame_to_page.size(), false);
            hand_ = 0;
        }
    }

    void advance_(int n) { hand_ = (hand_ + 1) % n; }

    std::vector<bool> ref_;
    int hand_ = 0;
};
