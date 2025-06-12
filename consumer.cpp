// Alex Huang; RED_ID: 130124082

#include <fstream>
#include "consumer.h"
#include "shared_data.h"
#include "utils.h"
#include <semaphore.h>
#include <pthread.h>

void* processrequests (void* shared_data_ptr) {
    shared_data* shared_data = static_cast< ::shared_data * >(shared_data_ptr);
    ofstream outputFile("processOutputs.txt");
    while (!(shared_data->buffer.empty() && shared_data->num_of_active_producers == 0)) {
        /* Make sure we have filled slots to consume */
        sem_wait(&shared_data->full);
        /* Access buffer exclusively */
        sem_wait(&shared_data->mutex);
        /* Before popping, get the length of the line and the current size of the queue */
        string line = shared_data->buffer.front();
        int curr_queue_size = shared_data->buffer.size();
        /* Remove line from the buffer */
        shared_data->buffer.pop();
        /* Unlock mutex */
        sem_post(&shared_data->mutex);
        /* Inform producer */
        sem_post(&shared_data->empty);
        /* Output queue size before removal and line length to processOutputs.txt */
        outputFile << curr_queue_size << " " << line.length() << "\n";
        /* Sleep at the end to represent the time it took to "consume" the request. */
        sleep_microseconds(shared_data->consume_time);

        /* Access buffer to add 1 to totalNumOfProcessedLines */
        sem_wait(&shared_data->mutex);
        /* Add 1 */
        shared_data->total_num_of_processed_lines++;
        /* Unlock buffer */
        sem_post(&shared_data->mutex);
    }
    outputFile.close();
    pthread_exit(nullptr);
}