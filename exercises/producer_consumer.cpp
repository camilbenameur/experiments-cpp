// Producer/consumer: mutex + condition_variable + atomic
// Build: g++ -std=c++20 -Wall -Wextra -pthread producer_consumer.cpp -o prog && ./prog
//
// A producer thread pushes 5 measurements, a consumer thread processes
// them. The relative order of produced/consumed lines may vary from one
// run to the next: only the final total is deterministic

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex m;
std::condition_variable cv;
std::queue<int> q;
std::atomic<bool> done{false};

void producer() {
    for (int i = 1; i <= 5; ++i) {
        int value = i * 10;
        {
            std::lock_guard<std::mutex> lk(m);   // RAII: unlock guaranteed
            q.push(value);
        }   // the block keeps the critical section as small as possible
        cv.notify_one();
        std::cout << "produced: " << value << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    done = true;       // atomic: no mutex needed for a simple bool
    cv.notify_one();   // final wake-up so the consumer can exit
}

void consumer() {
    int count = 0;
    while (true) {
        int value = 0;
        {
            std::unique_lock<std::mutex> lk(m);
            // The predicate protects against spurious wakeups: we only
            // proceed when the condition truly holds
            cv.wait(lk, [] { return !q.empty() || done; });

            if (q.empty() && done) break;   // nothing left to come

            value = q.front();
            q.pop();
        }
        std::cout << "consumed: " << value << "\n";
        ++count;
    }
    std::cout << "done, total consumed = " << count << "\n";
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}

// Notes:
// 1. The mutex protects the queue (not a thread-safe structure): without
//    it, concurrent push/pop = data race = UB
// 2. cv.wait releases the mutex while waiting, otherwise the producer
//    could never take it to push -> deadlock. Reacquired on wake-up
// 3. Lock-free variant (SPSC): static ring buffer + tail index written
//    only by the producer (release) and head only by the consumer (acquire)
// 4. A volatile bool would not do for done: volatile brings neither
//    atomicity nor memory barriers. volatile = hardware/ISR, atomic = threads
