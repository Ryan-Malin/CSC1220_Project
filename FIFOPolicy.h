#pragma once
#include <queue>
#include "ReplacementPolicy.h"

class FIFOPolicy : public ReplacementPolicy {
public:
    int pickVictimFrame(const SimulatorState& state) override {
        // Pop stale entries until we find a currently-resident frame.
        while (!order_.empty()) {
            int f = order_.front();
            order_.pop();
            if (f >= 0 && f < (int)state.frame_to_page.size() && state.frame_to_page[f] != -1) {
                return f;
            }
        }

        // Fallback: pick first resident frame (shouldn't happen if queue is consistent).
        for (int f = 0; f < (int)state.frame_to_page.size(); ++f) {
            if (state.frame_to_page[f] != -1) return f;
        }
        return 0;
    }

    void onPageAccess(int /*frame*/, bool /*isWrite*/) override {
        // FIFO ignores accesses; it only cares about load order.
    }

    void onPageLoaded(int frame, int /*page*/) override {
        order_.push(frame);
    }

    void onPageEvicted(int /*frame*/, int /*page*/) override {
        // Nothing required; stale frames are discarded lazily in pickVictimFrame().
    }

private:
    std::queue<int> order_;
};
