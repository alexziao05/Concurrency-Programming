// Alex Huang; RED_ID: 130124082

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/stat.h>
#include <cstring>
#include "shared_data.h"
#include "utils.h"

#define MICRO_TO_SECOND 1000000

using namespace std;

int get_total_num_of_lines (const char *filename) {
    /* Initialize total number of lines */
    int total_num_of_lines = 0;
    /* Initialize a file object and set to read only mode */
    FILE *file = fopen(filename, "r");

    /* fgetc returns an int not a char from 0 to 255 */
    int character;
    /* fgetc(file) reads 1 character from the file */
    /* EOF is -1 */
    /* If character is not -1, continue reading the file */
    while ((character = fgetc(file)) != EOF) {
      /* Since this file is a Unix text file format, it ends with (\n) */
      if (character == '\n') {
        /* If so, increase the number of lines */
        total_num_of_lines++;
      }
    }

    /* Close the file */
    fclose(file);
    /* Return the total number of lines */
    return total_num_of_lines;
}

void print_progress_bar (shared_data *shared_data) {
    int total_num_of_processed_lines = shared_data->total_num_of_processed_lines;
    int total_num_of_lines = shared_data->total_num_of_lines;
    int total_progress_marks = shared_data->total_progress_marks;
    int hash_mark_every_Nth_char = shared_data->hash_mark_every_Nth_char;
    int previous_num_of_markers = shared_data->previous_num_of_markers;

    /* Given equation */
    double percent_complete = (double) total_num_of_processed_lines / total_num_of_lines;
    int current_num_of_markers = percent_complete * total_progress_marks;

    /* Only print to the standard ouput if progress has been made */
    /* This means there should be more markers than the previous number of markers */
	if (current_num_of_markers > previous_num_of_markers) {
          /* We want to see how many new markers has been added */
          /* Which is why we don't start at 0 but from the previous_num_of_markers */
      	  /* previous_num_of_markers has been initialized to 0 because it starts with 0 markers */
          while (previous_num_of_markers < current_num_of_markers) {
            /* Since we are printing a "#" at every Nth char, we don't want to use a zero-based index */
            int position = previous_num_of_markers + 1;
            /* If the position modulus hash_mark_every_Nth_char, it means it's a factor of it */
            if (position % hash_mark_every_Nth_char == 0) {
              printf("#");
            } else {
              printf("-");
            }
            previous_num_of_markers++;
            /* Ensure that the character appear immediately in the terminal */
            fflush(stdout);
          }

          /* We have to store the value of previous_num_of_markers back into the shared_data */
          /* So that the next time, curr_num is greater than prev_num, we have an updated value of prev_num */
          shared_data->previous_num_of_markers = current_num_of_markers;
	}
}

// Check if file exists and is accessible
bool isFileExist(const string& path) {
    ifstream file(path);
    return file.good(); // Returns true if file exists and accessible
}

// Code from concurrency programming assignment pdf
void sleep_microseconds(long microseconds) {
    struct timespec req;
    req.tv_sec = microseconds / MICRO_TO_SECOND; // Convert microseconds to seconds
    req.tv_nsec = (microseconds % MICRO_TO_SECOND) * 1000; // Convert remaining to nanoseconds

    if (nanosleep(&req, nullptr) == -1) {
        exit(0);
    }
}