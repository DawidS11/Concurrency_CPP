#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

/*
The number of elements in 'vals' vector will (most likely) not be eqaul 'num'.
The order of them will (most likely) not be consecutive.
*/

// g++ -fsanitize=thread -std=c++20 trylock.cpp -o prog

int main() {
    size_t num = 9;
    std::mutex myLock;
    std::vector<int> vals;
    std::vector<std::thread> threads;    

    auto lambda = [&](int i){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if(myLock.try_lock()){
            vals.push_back(i);
            myLock.unlock();
        }
    };

    for(int i = 0; i < num; ++i) {
        threads.push_back(std::thread(lambda, i));
    }

    for(auto& t : threads) {
        t.join();
    }
    
    std::cout << std::endl;
    for(std::vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) {
        std::cout << *it << std::endl;
    }
}