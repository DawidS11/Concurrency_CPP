#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore>
#include <assert.h>

/*
Two threads adding to the result and three threads subtracting from it.
Semaphore of subtraction is set two 1 so the subtraction will occur first.
Subtraction and addition should occur alternately.
*/

// g++ -fsanitize=thread -std=c++20 binary_semaphore.cpp -o prog

std::binary_semaphore addSemaphore{0};
std::binary_semaphore subtractSemaphore{1};

int main(){

    int result = 2;

    auto subtract = [&](int i){
        subtractSemaphore.acquire();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            result -= 2;
            std::cout << "Subtract " << i << ", result is: " << result << std::endl;
        addSemaphore.release();
    };

    std::thread subtract1(subtract, 1);
    std::thread subtract2(subtract, 2);
    std::thread subtract3(subtract, 3);

    auto add = [&](int i) {
        addSemaphore.acquire();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            result += 2;
            std::cout << "Add " << i << ", result is: " << result << std::endl;
        subtractSemaphore.release();
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