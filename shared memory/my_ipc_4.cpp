// // //Use the shared memory and semaphores for the implementation of the logical ring-buffer
// // //(that can store up to 10 data items) and the synchronization.


#include<semaphore.h>
#include<fstream>
#include<iostream>
#include<thread>


using namespace std;

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];

int in = 0; // points to next free element
int out = 0; // points to the first full buffer element
int count = 0; // counts the number of data items in the buffer
int producedCount = 0;

sem_t semEmpty;
sem_t semFull;

void producer_process() {
    ofstream generatedFile("producer_data_ipc_4.txt");
    while (producedCount < 100) {
        int x = (rand() % 100)+1;
        sem_wait(&semEmpty);
        buffer[in] = x;
        in = (in + 1) % BUFFER_SIZE; // Wrap-around behavior for the in pointer
        count++;
        producedCount++;
        sem_post(&semFull);
        //producedCount tells how many integers are generated.
        cout << producedCount << " - generated " << x << endl;
        generatedFile << x << endl; 
    }
    generatedFile.close();
}

void consumer_process() {
    ofstream consumedFile("consumed_data_ipc_4.txt");
    while (1) {
        sem_wait(&semFull);
        int y = buffer[out];
        out = (out + 1) % BUFFER_SIZE; // Wrap-around behavior for the out pointer
        count--;
        sem_post(&semEmpty);
        cout << "consumed " << y << endl;
        consumedFile << y << endl;
    }
    consumedFile.close();
}

int main() {
    srand(time(NULL));
    sem_init(&semEmpty, 0, BUFFER_SIZE - 1);
    sem_init(&semFull, 0, 0);

    thread producer(producer_process);
    thread consumer(consumer_process);

    producer.join();
    consumer.join();

    sem_destroy(&semEmpty);
    sem_destroy(&semFull);

    return 0;
}
