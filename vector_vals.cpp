#include <iostream>
#include <thread>
#include <vector>
#include <iomanip>
#include <mutex>
#include <atomic>

// g++ -fsanitize=thread -std=c++20 vector_vals.cpp -o prog
bool isRunning = true;
time_t start = time(0);
std::mutex myLock;
std::atomic<time_t> timeLeft = 10;

void change_vals(std::vector<int>& vec, int index) {
    while(timeLeft > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200*(index+1)));
        myLock.lock();
            vec[index] = rand() % 100;
        myLock.unlock();
    }
}

int main() {
    srand(time(0));
    std::vector<int> vals(4);
    std::vector<std::thread> threads;

    for(int i = 0; i < 4; ++i) {
        threads.push_back(std::thread(change_vals, std::ref(vals), i));
    }

    while(timeLeft > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        for(int i = 0; i < 4; ++i) {
            myLock.lock();
                std::cout << std::setw(5) << vals[i] << std::setw(5);
            myLock.unlock();
        }
        timeLeft -= (time(0) - start);
        std::cout << "   | time left: " << timeLeft << std::endl;
    }
    for(auto& t : threads) {
        t.join();
    }
}