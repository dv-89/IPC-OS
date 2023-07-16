// #include <iostream>
// #include <sys/types.h>
// #include <unistd.h>
// #include <fstream>

// using namespace std;

// void producer_process(int writeEnd) {
//     ofstream outputFile("produced_data_1.txt");

//     for (int i = 0; i < 100; ++i) {
//         int data = (rand()%100 )+ 1;
//         write(writeEnd, &data, sizeof(data));
//         outputFile << data << endl;
//         cout << i << " - Produced: " << data << endl;
//     }

//     outputFile.close();
//     close(writeEnd);
// }

// void consumer_process(int readEnd) {
//     int data;
//     ofstream outputFile("consumed_data_1.txt");

//     while (read(readEnd, &data, sizeof(data)) > 0) {
//         outputFile << data << endl;
//         cout << "Consumed: " << data << endl;
//     }

//     outputFile.close();
//     close(readEnd);
// }

// int main() {
//     int fd[2];
//     pid_t pid;

//     int result = pipe(fd);

//     if (result == -1) {
//         cout << "Error creating pipe." << endl;
//         return 1;
//     }

//     // Create a child process
//     pid = fork();

//     if (pid < 0) {
//         cout << "Fork failed, cannot create child process." << endl;
//         return 1;
//     }

//     if (pid > 0) {
//         // Parent process
//         // Close unused end - parent only writes, so close read end
//         close(fd[0]);
//         producer_process(fd[1]);
//         close(fd[1]);
//     } else {
//         // Child process
//         // Close unused end - child only reads, so close write end
//         close(fd[1]);
//         consumer_process(fd[0]);
//         close(fd[0]);
//     }

//     return 0;
// }




#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>

using namespace std;

void producer_process(int writeEnd) {
    ofstream outputFile("produced_data.txt");

    for (int i = 0; i < 100; ++i) {
        int data = (rand() % 100) + 1;
        write(writeEnd, &data, sizeof(data));
        outputFile << data << endl;
        cout << "Produced: " << data << endl;
    }

    outputFile.close();
    close(writeEnd);
}

void consumer_process(int readEnd) {
    int data;
    ofstream outputFile("consumed_data.txt");

    while (read(readEnd, &data, sizeof(data)) > 0) {
        outputFile << data << endl;
        cout << "Consumed: " << data << endl;
    }

    outputFile.close();
    close(readEnd);
}

int main() {
    int fd[2];

    int result = pipe(fd);

    if (result == -1) {
        cout << "Error creating pipe." << endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cout << "Fork failed, cannot create child process." << endl;
        return 1;
    }

    if (pid > 0) {
        // Parent process
        close(fd[0]); // Close unused read end
        thread producer(producer_process, fd[1]); // Pass the write end to the producer thread
        producer.join(); // Wait for the producer thread to finish
        close(fd[1]); // Close the write end
    } else {
        // Child process
        close(fd[1]); // Close unused write end
        thread consumer(consumer_process, fd[0]); // Pass the read end to the consumer thread
        consumer.join(); // Wait for the consumer thread to finish
        close(fd[0]); // Close the read end
    }

    return 0;
}

