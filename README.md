# Heap Fragmentation

## Overview

This project is a C++ program that simulates heap fragmentation, a common issue in dynamic memory management. The simulation randomly allocates and deallocates memory blocks, calculates the heap fragmentation percentage, and stops when fragmentation reaches 100% or memory runs out. The goal is to demonstrate how fragmentation occurs in a heap over time, providing insights into memory management challenges in software systems.

## Build & Run

Follow these steps to build and run the project:

### Clone the Repository:
```bash
git clone https://github.com/AniDashyan/heap_fragmentation
cd heap_fragmentation
```


### Build the Project:
```bash
cmake -S . -B build
cmake --build build --config Debug
```

### Run the Executable:
```bash
./build/heap_frag
```

## Example Output
```
Heap Size: 351 MB
Alloc 352 KB (Block 1) Total: 352 KB
Free Mem: 360020 KB, Largest: 180186 KB, Frag: 49.951%
Alloc 126 KB (Block 2) Total: 479 KB
Dealloc block 2 Total: 352 KB
Dealloc block 1 Total: 0 KB
Alloc 667 KB (Block 1) Total: 667 KB
Dealloc block 1 Total: 0 KB
Alloc 180 KB (Block 1) Total: 180 KB
Alloc 8 KB (Block 2) Total: 188 KB
Dealloc block 1 Total: 8 KB
Dealloc block 1 Total: 0 KB
Alloc 10 KB (Block 1) Total: 10 KB
Free Mem: 360363 KB, Largest: 180186 KB, Frag: 49.9986%
Alloc 157 KB (Block 2) Total: 167 KB
Alloc 35 KB (Block 3) Total: 202 KB
Alloc 87 KB (Block 4) Total: 290 KB
Alloc 526 KB (Block 5) Total: 816 KB
Dealloc block 5 Total: 290 KB
Alloc 469 KB (Block 5) Total: 759 KB
Alloc 116 KB (Block 6) Total: 875 KB
Alloc 856 KB (Block 7) Total: 1732 KB
Dealloc block 5 Total: 1263 KB
Free Mem: 359110 KB, Largest: 180186 KB, Frag: 49.8241%
Done. Final Total: 0 KB
```
## Explanation

### Understanding the Output

The output provides a step-by-step log of the heap fragmentation simulation:

- **Heap Size**: The simulated heap size for this run, randomly chosen between 100 MB and 500 MB. In the example, it's 401 MB (410624 KB).

- **Allocation (Alloc)**: Shows memory allocations, including the size of the block (e.g., 399 KB), the block number (e.g., Block 1), and the total allocated memory (Total: 399 KB). Block sizes are generated using an exponential distribution (average 256 KB) and clamped between 1 KB and 10 MB.

- **Deallocation (Dealloc)**: Indicates when a block is deallocated (e.g., Dealloc block 1 Total: 0 KB). The block number is randomly chosen, and the total allocated memory is updated accordingly.

- **Fragmentation Stats**:
  - **Free Mem**: The estimated free memory in the heap, calculated as Heap Size - Total Allocated Memory. For example, Free Mem: 410225 KB means 410225 KB of the heap is free.
  - **Largest**: The largest contiguous block that can be allocated, estimated by attempting allocations starting at half the heap size and halving until successful. For example, Largest: 205312 KB (roughly half of 401 MB).
  - **Frag**: The fragmentation percentage, calculated as:
    ```
    Fragmentation = 1 - (Largest Allocatable Block / Total Free Memory)
    ```
    If Free Mem is 0, fragmentation is set to 100%. In the example, Frag: 49.9% indicates moderate fragmentation because the largest allocatable block (205312 KB) is about half of the free memory (410225 KB).

- **Stopping Condition**: The simulation stops if fragmentation reaches 100% or if an allocation fails (simulating out-of-memory conditions). In this example, fragmentation never reaches 100%, so the simulation completes all 20 iterations.

- **Final Total**: The total allocated memory after cleanup, which should be 0 KB since all blocks are deallocated. In the example, Final Total: 0 KB confirms proper memory management.

### Why is Heap Fragmentation a Bad Thing?

Heap fragmentation occurs when free memory in the heap is split into small, non-contiguous blocks, making it difficult to allocate large contiguous blocks even if the total free memory is sufficient. This is a significant issue in software systems for several reasons:

1. **Inefficient Memory Usage**: Fragmentation reduces the effective usable memory. For example, if you have 100 MB of free memory split into 1000 small blocks of 100 KB each, you can't allocate a single 50 MB block, even though there's enough total free memory. This leads to wasted memory and potential allocation failures.

2. **Performance Degradation**: Fragmented memory can slow down allocation and deallocation operations because the memory allocator has to search for suitable blocks, often traversing a fragmented free list. This increases the time complexity of memory management operations.

3. **Out-of-Memory Errors**: In systems with fixed heap sizes (e.g., embedded systems), fragmentation can cause allocation failures, even when there's free memory available. For instance, if an application needs a 10 MB block but the largest contiguous block is only 5 MB due to fragmentation, the allocation will fail, potentially crashing the program.

4. **Difficulty in Recovery**: Unlike stack memory, which is automatically reclaimed, heap memory relies on explicit deallocation. Fragmentation can persist unless the system implements a defragmentation mechanism (e.g., compacting the heap), which is complex and not always feasible in real-time systems due to the need to update pointers.

In the example output, fragmentation remains around 49.9%, indicating that the largest allocatable block is about half of the free memory. If fragmentation were to increase to 100% (e.g., Free Mem becomes 0 or the largest block becomes too small), the simulation stops, mimicking a real-world scenario where the heap becomes unusable. Understanding and mitigating heap fragmentation is crucial for building efficient, reliable software, especially in memory-constrained environments.
