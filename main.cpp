#include <iostream>
#include <thread>
#include <vector>
#include <iomanip>

// g++ -std=c++20 main.cpp -o prog
bool isRunning = true;

void change_vals(std::vector<int>& vec, int index) {
    while(isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        vec[index] = rand() % 100;
    }
}

int main() {
    srand(2024);
    std::vector<int> vals(4);
    std::vector<std::thread> threads;
    time_t start, timeLeft;

    for(int i = 0; i < 4; ++i) {
        threads.push_back(std::thread(change_vals, std::ref(vals), i));
    }

    start = time(0);
    while(isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        system("clear");
        for(int i = 0; i < 4; ++i) {
            std::cout << vals[i] << std::setw(5);
        }
        timeLeft = 10 - (time(0) - start);
        isRunning = timeLeft > 0;
        std::cout << "   | time left: " << timeLeft << std::endl;
    }
    for(auto& t : threads) {
        t.join();
    }
    
    
}