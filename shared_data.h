// Alex Huang; RED_ID: 130124082

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <queue>
#include <string>
#include <semaphore.h>

using namespace std;

struct shared_data {
    /* Queue */
    queue<string> buffer;
    /* Semaphores */
    sem_t empty; // Empty slots in the queue (Producer)
    sem_t full; // Filled slots in the queue (Consumer)
    sem_t mutex; // Mutually exclusive access
    /* Progress Bar */
    int total_num_of_lines = 0;
    int total_num_of_processed_lines = 0;
    /* Default Values -- Explicitly Initialized */
    int produce_time = 50;
    int consume_time = 50;
    int max_queue_size = 100;
    int total_progress_marks = 50;
    int hash_mark_every_Nth_char = 10;
    string mandatory_file;
    int num_of_active_producers = 1;
    int previous_num_of_markers = 0;
};

#endif //SHARED_DATA_H
