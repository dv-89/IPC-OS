#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>

#define BUFFER_SIZE 100

std::mutex mtx;
int buffer[BUFFER_SIZE];
int bufferIndex = 0;

void producer()
{
    std::srand(std::time(nullptr));
    
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        int data = std::rand() % 100;  // Generate a random integer between 0 and 99
        
        // Acquire lock to access the buffer
        std::lock_guard<std::mutex> lock(mtx);
        
        // Add data to the buffer
        buffer[bufferIndex] = data;
        ++bufferIndex;
        std::cout << "Producer generated: " << data << std::endl;
    }
}

void consumer()
{
    // Wait until the producer generates all the data
    while (bufferIndex < BUFFER_SIZE) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    // Acquire lock to access the buffer
    std::lock_guard<std::mutex> lock(mtx);
    
    // Read and print all the data from the buffer
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        std::cout << "Consumer received: " << buffer[i] << std::endl;
    }
}

int main()
{
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);
    
    producerThread.join();
    consumerThread.join();
    
    return 0;
}
