#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <chrono>

struct Mailbox {
    std::mutex m;
    std::condition_variable cv;
    std::unique_ptr<int> msg; // empty = no message
    bool done = false;
};

void producer(Mailbox& box) {
    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // fake work

        auto p = std::make_unique<int>(i * 10);

        {
            std::lock_guard<std::mutex> lock(box.m);
            box.msg = std::move(p); // handoff ownership to mailbox
        }
        box.cv.notify_one(); // wake consumer
    }

    {
        std::lock_guard<std::mutex> lock(box.m);
        box.done = true;
    }
    box.cv.notify_one();
}

void consumer(Mailbox& box) {
    while (true) {
        std::unique_ptr<int> local;

        {
            std::unique_lock<std::mutex> lock(box.m);

            box.cv.wait(lock, [&] {
                return box.msg != nullptr || box.done;
            });

            if (box.msg) {
                local = std::move(box.msg); // take ownership out
            } else if (box.done) {
                break;
            }
        }

        if (local) {
            std::cout << "Got message: " << *local << "\n";
        }
    }
}

int main() {
    Mailbox box;

    std::thread t1(producer, std::ref(box));
    std::thread t2(consumer, std::ref(box));

    t1.join();
    t2.join();
}
