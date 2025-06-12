// Alex Huang; RED_ID: 130124082

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <cstring>
#include "shared_data.h"
#include "consumer.h"
#include "producer.h"
#include "utils.h"

int main(int argc, char **argv) {
    int option_idx;
    shared_data shared_data;

    // Code structure from "threadexample.cpp" in concurrency programming assignment
    while ((option_idx = getopt(argc, argv, "t:c:q:p:h:")) != -1) {
        switch (option_idx) {
            case 't':
                shared_data.produce_time = atoi(optarg);
                if (shared_data.produce_time <= 0) {
                    cout << "Time for producing a request must be a number and greater than 0" << endl;
                    exit(1);
                }
                break;
            case 'c':
                shared_data.consume_time = atoi(optarg);
                if (shared_data.consume_time <= 0) {
                    cout << "Time for consuming a request must be a number and greater than 0" << endl;
                    exit(1);
                }
                break;
            case 'q':
                shared_data.max_queue_size = atoi(optarg);
                if (shared_data.max_queue_size < 1) {
                    cout << "Max size of request queue must be a number and greater than 0" << endl;
                    exit(1);
                }
                break;
            case 'p':
                shared_data.total_progress_marks = atoi(optarg);
                if (shared_data.total_progress_marks < 10) {
                    cout << "Number of progress marks must be a number and at least 10" << endl;
                    exit(1);
                }
                break;
            case 'h':
                shared_data.hash_mark_every_Nth_char = atoi(optarg);
                if (shared_data.hash_mark_every_Nth_char <= 0 || shared_data.hash_mark_every_Nth_char > 10) {
                    cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                    exit(1);
                }
                break;
            default:
                exit(1);
        }
    }

    if (optind < argc) {
        shared_data.mandatory_file = argv[optind];
        /* Check if file exists and can be opened */
        if (!isFileExist(shared_data.mandatory_file)) {
            cout << "Unable to open file " << shared_data.mandatory_file << endl;
            exit(1);
        }
        /* Cast to const char data type */
        const char *filename = const_cast<char *>(shared_data.mandatory_file.c_str());
        shared_data.total_num_of_lines = get_total_num_of_lines(filename);
    } else {
        exit(1);
    }

    // In the beginning...
    sem_init(&shared_data.mutex, 0, 1); //...the mutex is available
    sem_init(&shared_data.empty, 0, shared_data.max_queue_size); //...the entire queue is empty
    sem_init(&shared_data.full, 0, 0); //...the queue is not full, no slots are full

    pthread_attr_t pthread_attr_default;
    pthread_attr_init(&pthread_attr_default);
    pthread_t producer, consumer;
    pthread_create(&producer, &pthread_attr_default, receiverequests, &shared_data);
    pthread_create(&consumer, &pthread_attr_default, processrequests, &shared_data);

    // Print progress bar while it is processing
    while (shared_data.total_num_of_processed_lines < shared_data.total_num_of_lines) {
      sem_wait(&shared_data.mutex);
      print_progress_bar(&shared_data);
      sem_post(&shared_data.mutex);
    }

    // Catch cases where process finishes before progress bar gets printed
    sem_wait(&shared_data.mutex);
    print_progress_bar(&shared_data);
    sem_post(&shared_data.mutex);

    pthread_join(producer, nullptr);
    pthread_join(consumer, nullptr);

    cout << "\n" << shared_data.total_num_of_processed_lines << " requests were received and processed." << endl;

    return 0;
}