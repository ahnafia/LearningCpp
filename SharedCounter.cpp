#include <iostream>
#include <thread>
#include <memory>
#include <mutex>

struct Counter {
    int value = 0;
    std::mutex m;
};

void increment(std::shared_ptr<Counter> counter, int times) {
    for (int i = 0; i < times; ++i) {
        std::lock_guard<std::mutex> lock(counter->m);
        ++counter->value;
    }
}

int main() {
    auto counter = std::make_shared<Counter>();

    std::thread t1(increment, counter, 1'000'000);
    std::thread t2(increment, counter, 1'000'000);

    t1.join();
    t2.join();

    std::cout << "Final value: " << counter->value << "\n";
}
