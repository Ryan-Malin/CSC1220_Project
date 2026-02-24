# Virtual Memory Simulator

OS course project simulating page replacement algorithms.

## Team

- **Ryan**: Core simulation, trace parser, main logic
- **Connor**: FIFO, LRU, and Clock algorithms

## Files

**Done (Ryan):**

- `SimulatorState.h` - Page table and frame table
- `ReplacementPolicy.h` - Algorithm interface
- `TraceParser.h` - Reads trace files
- `Simulator.cpp` - Simulation logic
- `main.cpp` - Main program
- `SimpleFIFO.h` - Test algorithm

**TODO (Connor):**

- `FIFOPolicy.h` - FIFO algorithm
- `LRUPolicy.h` - LRU algorithm
- `ClockPolicy.h` - Clock algorithm

## How to Run

```bash
g++ main.cpp Simulator.cpp -o vm_sim
./vm_sim
```

## Connor's Task

Create 3 files that implement `ReplacementPolicy` interface. Use `SimpleFIFO.h` as a template.

Each needs these 4 functions:

```cpp
int pickVictimFrame(const SimulatorState& state);
void onPageAccess(int frame, bool isWrite);
void onPageLoaded(int frame, int page);
void onPageEvicted(int frame, int page);
```

**FIFO**: Track load order, evict oldest  
**LRU**: Track access times, evict least recently used  
**Clock**: Use reference bits, scan circularly
