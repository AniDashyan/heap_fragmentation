#include <iostream>
#include <vector>
#include <random>
#include <thread>

static size_t totalMem = 0;

void* operator new[](size_t size) {
    void* ptr = std::malloc(size);
    if (!ptr) throw std::bad_alloc();
    totalMem += size;
    return ptr;
}

void operator delete[](void* ptr) noexcept {
    std::free(ptr);
}

size_t getLargestBlock(size_t maxSize) {
    size_t size = maxSize / 2;
    size_t testMem = 0; // Track allocated memory
    while (size > 1024) {
        try {
            char* ptr = new char[size];
            testMem += size;
            delete[] ptr;
            totalMem -= size; // Undo the test allocation
            return size;
        } catch (const std::bad_alloc&) {
            totalMem -= testMem; // Undo any failed test allocations
            testMem = 0; // Reset testMem
            size /= 2;
        }
    }
    totalMem -= testMem; // Undo any remaining test allocations
    return 0;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> heapDist(100 * 1024 * 1024, 500 * 1024 * 1024);
    std::uniform_real_distribution<double> allocProb(0.3, 0.7);
    std::exponential_distribution<double> sizeDist(1.0 / (256 * 1024.0));

    size_t heapSize = heapDist(gen);
    std::cout << "Heap Size: " << heapSize / (1024 * 1024) << " MB\n";

    std::vector<std::pair<char*, size_t>> blocks;
    const int iter = 20;
    bool stopSimulation = false; // Flag to stop the simulation

    for (int i = 0; i < iter && !stopSimulation; ++i) {
        bool alloc = std::uniform_real_distribution<double>(0, 1)(gen) < allocProb(gen);
        if (alloc || blocks.empty()) {
            size_t size = std::max<size_t>(1024, std::min<size_t>(sizeDist(gen), 10 * 1024 * 1024));
            try {
                char* ptr = new char[size];
                blocks.push_back({ptr, size});
                std::cout << "Alloc " << size / 1024 << " KB (Block " << blocks.size() << ") Total: " << totalMem / 1024 << " KB\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } catch (const std::bad_alloc&) {
                std::cout << "Allocation failed due to lack of memory. Stopping simulation.\n";
                stopSimulation = true;
            }
        } else {
            int idx = std::uniform_int_distribution<int>(0, blocks.size() - 1)(gen);
            totalMem -= blocks[idx].second;
            delete[] blocks[idx].first;
            std::cout << "Dealloc block " << idx + 1 << " Total: " << totalMem / 1024 << " KB\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            blocks.erase(blocks.begin() + idx);
        }

        if (i == 0 || i == iter / 2 || i == iter - 1 || stopSimulation) {
            size_t largest = getLargestBlock(heapSize);
            size_t freeMem = (heapSize > totalMem) ? (heapSize - totalMem) : 0;
            double frag = (freeMem > 0) ? (1.0 - static_cast<double>(largest) / freeMem) : 1.0;
            if (frag < 0) frag = 0;
            if (frag > 1) frag = 1;

            std::cout << "Free Mem: " << freeMem / 1024 << " KB, Largest: " << largest / 1024 << " KB, Frag: " << frag * 100 << "%\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (frag >= 1.0 || freeMem == 0) {
                std::cout << "Heap is fully fragmented or out of memory. Stopping simulation.\n";
                stopSimulation = true;
            }
        }
    }

    for (auto& b : blocks) {
        totalMem -= b.second;
        delete[] b.first;
    }

    std::cout << "Done. Final Total: " << totalMem / 1024 << " KB\n";
    return 0;
}