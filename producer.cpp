// Alex Huang; RED_ID: 130124082

#include <fstream>
#include "producer.h"
#include "shared_data.h"
#include <semaphore.h>
#include <pthread.h>
#include "utils.h"

using namespace std;

void* receiverequests (void* shared_data_ptr) {
    /* Declare objects */
    shared_data* shared_data = static_cast< ::shared_data * >(shared_data_ptr);
    ifstream file(shared_data->mandatory_file);
    string line;
    /* Iterate through the file line by line */
    while (getline(file, line)) {
        /* Sleep for set microseconds. Sleep at the beginning to represent time to "produce" the request. */
        sleep_microseconds(shared_data->produce_time);
        /* Make sure we have room */
        sem_wait(&shared_data->empty);
        /* Access buffer exclusively */
        sem_wait(&shared_data->mutex);
        /* Add line to buffer */
        shared_data->buffer.push(line);
        /* Unlock mutex */
        sem_post(&shared_data->mutex);
        /* Inform consumer */
        sem_post(&shared_data->full);
    }
    file.close();

    /* Mark producer as done -- no more lines to process */
    /* Get access to shared data */
    sem_wait(&shared_data->mutex);
    /* Decrement num of active producers to 0. No more producers are running. */
    shared_data->num_of_active_producers--;
    /* Release the lock */
    sem_post(&shared_data->mutex);

    /* Exit thread */
    pthread_exit(nullptr);
}