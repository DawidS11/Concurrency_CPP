#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include <mutex>
#include <assert.h>

/*
Two threads adding to the result and three threads subtracting from it.
Semaphore is set two 1 so both subtraction and addition may occur first.
Mutexes are used in order not to subtract/add at the same time.
*/

// g++ -std=c++20 counting_semaphore.cpp -o prog

std::counting_semaphore gSemaphore{1};

int main(){

    int result = 2;

    std::mutex subtractLock;
    auto subtract = [&](int i){
        gSemaphore.acquire();
        subtractLock.lock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            result -= 2;
            std::cout << "Subtract " << i << ", result is: " << result << std::endl;
        subtractLock.unlock();
    };

    std::thread subtract1(subtract, 1);
    std::thread subtract2(subtract, 2);
    std::thread subtract3(subtract, 3);

    std::mutex addLock;
    auto add = [&](int i) {
        addLock.lock();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            result += 2;
            std::cout << "Add " << i << ", result is: " << result << std::endl;
        addLock.unlock();
        gSemaphore.release();
    };

    std::thread add1(add, 1);
    std::thread add2(add, 2);

    subtract1.join();
    subtract2.join();
    subtract3.join();
    add1.join();
    add2.join();

    assert(result == 0);
}