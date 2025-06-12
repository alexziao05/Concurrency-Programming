// Alex Huang; RED_ID: 130124082

#ifndef UTILS_H
#define UTILS_H
#include "shared_data.h"

void sleep_microseconds(long microseconds);
int get_total_num_of_lines (const char *filename);
void print_progress_bar (shared_data *shared_data);
bool isFileExist(const string& path);

#endif //UTILS_H
