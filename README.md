# Concurrency Programming: Producer-Consumer Pattern

A comprehensive C++ implementation of the Producer-Consumer problem using POSIX threads (pthreads), semaphores, and mutexes. This project demonstrates fundamental concepts in concurrent programming through a file processing application with real-time progress visualization.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Command Line Options](#command-line-options)
- [Examples](#examples)
- [Understanding the Output](#understanding-the-output)
- [Implementation Details](#implementation-details)
- [File Structure](#file-structure)
- [Troubleshooting](#troubleshooting)
- [Educational Value](#educational-value)
- [License](#license)

## Overview

This project implements a classic Producer-Consumer synchronization pattern where:

- **Producer Thread**: Reads lines from a text file and adds them to a shared buffer queue
- **Consumer Thread**: Processes lines from the buffer queue and writes statistics to an output file
- **Main Thread**: Displays a real-time progress bar showing processing status

The implementation uses semaphores for synchronization and demonstrates key concurrency concepts including mutual exclusion, producer-consumer coordination, and thread-safe data structures.

## Features

- ✅ **Thread-Safe Operations**: Uses semaphores and mutexes for proper synchronization
- ✅ **Configurable Parameters**: Customizable timing, queue size, and progress display
- ✅ **Real-Time Progress Bar**: Visual feedback with customizable progress markers
- ✅ **File Processing**: Reads any text file and processes it line by line
- ✅ **Statistics Output**: Generates detailed processing statistics
- ✅ **Error Handling**: Comprehensive input validation and error reporting
- ✅ **Cross-Platform**: Compatible with Unix-like systems (Linux, macOS)

## Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Producer      │    │  Shared Buffer  │    │    Consumer     │
│   Thread        │────│     (Queue)     │────│     Thread      │
│                 │    │                 │    │                 │
│ • Reads file    │    │ • Thread-safe   │    │ • Processes     │
│ • Adds to queue │    │ • Size limited  │    │   lines         │
│ • Respects      │    │ • Semaphore     │    │ • Writes stats  │
│   timing        │    │   protected     │    │ • Respects      │
└─────────────────┘    └─────────────────┘    │   timing        │
                                              └─────────────────┘
                              │
                              ▼
                    ┌─────────────────┐
                    │   Main Thread   │
                    │                 │
                    │ • Progress bar  │
                    │ • Synchronizes  │
                    │   display       │
                    └─────────────────┘
```

## Prerequisites

- C++ compiler with C++11 support (g++, clang++)
- POSIX-compliant operating system (Linux, macOS, Unix)
- pthread library
- make utility

## Installation

1. **Clone or download the project:**
   ```bash
   git clone <repository-url>
   cd Concurrency-Programming
   ```

2. **Compile the project:**
   ```bash
   make
   ```

3. **Verify compilation:**
   ```bash
   ls -la processrequests
   ```

## Usage

### Basic Syntax
```bash
./processrequests [OPTIONS] <input_file>
```

### Quick Start
```bash
# Basic usage with default settings
./processrequests sample.txt

# Custom timing and queue size
./processrequests -t 100 -c 75 -q 50 sample.txt
```

## Command Line Options

| Option | Description | Default | Range/Constraints |
|--------|-------------|---------|-------------------|
| `-t <time>` | Producer time (microseconds) | 50 | > 0 |
| `-c <time>` | Consumer time (microseconds) | 50 | > 0 |
| `-q <size>` | Maximum queue size | 100 | ≥ 1 |
| `-p <marks>` | Total progress marks | 50 | ≥ 10 |
| `-h <interval>` | Hash mark interval | 10 | 1-10 |

### Parameter Details

- **Producer Time (`-t`)**: Microseconds to sleep between reading each line (simulates I/O delay)
- **Consumer Time (`-c`)**: Microseconds to sleep after processing each line (simulates processing time)
- **Queue Size (`-q`)**: Maximum number of lines that can be buffered simultaneously
- **Progress Marks (`-p`)**: Total number of characters in the progress bar
- **Hash Interval (`-h`)**: Every Nth character will be a '#' instead of '-'

## Examples

### Example 1: Basic File Processing
```bash
# Create a test file
echo -e "Line 1\nLine 2\nLine 3\nLine 4\nLine 5" > test.txt

# Run with default settings
./processrequests test.txt
```

**Output:**
```
----------#----------#----------#----------#----------
```

**Generated `processOutputs.txt`:**
```
1 6
1 6
1 6
1 6
1 6
```

### Example 2: Fast Producer, Slow Consumer
```bash
# Fast producer (10μs), slow consumer (200μs)
./processrequests -t 10 -c 200 test.txt
```

This demonstrates **queue buildup** as the producer adds items faster than the consumer processes them.

### Example 3: Slow Producer, Fast Consumer
```bash
# Slow producer (200μs), fast consumer (10μs)
./processrequests -t 200 -c 10 test.txt
```

This shows **queue starvation** where the consumer often waits for the producer.

### Example 4: Large Queue with Custom Progress Bar
```bash
# Large queue with detailed progress tracking
./processrequests -q 500 -p 100 -h 5 large_file.txt
```

**Progress Bar Pattern:**
```
----#----#----#----#----#----#----#----#----#----#
```
Every 5th character is a '#', giving more detailed progress feedback.

### Example 5: Stress Testing with Extreme Timing
```bash
# Very fast processing to test synchronization
./processrequests -t 1 -c 1 -q 10 test.txt

# Very slow processing to observe queue behavior
./processrequests -t 1000 -c 1000 -q 5 test.txt
```

### Example 6: Processing Different File Types

**Processing a log file:**
```bash
./processrequests -t 25 -c 100 /var/log/system.log
```

**Processing source code:**
```bash
./processrequests -t 75 -c 50 main.cpp
```

**Processing a large dataset:**
```bash
./processrequests -t 10 -c 30 -q 1000 data.csv
```

## Understanding the Output

### 1. Progress Bar
The progress bar shows real-time processing status:
- `-`: Regular progress marker
- `#`: Every Nth marker (configurable with `-h`)
- Length: Configurable with `-p`

**Example with `-p 20 -h 4`:**
```
---#---#---#---#---#
```

### 2. Output File (`processOutputs.txt`)
Each line contains two numbers:
```
<queue_size_before_removal> <line_length>
```

**Example output:**
```
3 15    # Queue had 3 items, processed line was 15 characters
2 8     # Queue had 2 items, processed line was 8 characters
1 12    # Queue had 1 item, processed line was 12 characters
```

### 3. Queue Behavior Analysis

**Pattern Recognition:**
- **Increasing queue sizes**: Producer is faster than consumer
- **Decreasing queue sizes**: Consumer is faster than producer
- **Stable queue sizes**: Balanced producer/consumer rates
- **Queue size of 1**: Tight synchronization, minimal buffering

## Implementation Details

### Synchronization Mechanisms

1. **Semaphores Used:**
   - `empty`: Tracks empty slots in queue (initialized to `max_queue_size`)
   - `full`: Tracks filled slots in queue (initialized to 0)
   - `mutex`: Ensures exclusive access to shared data (initialized to 1)

2. **Critical Sections:**
   - Queue operations (push/pop)
   - Progress counter updates
   - Progress bar printing

3. **Thread Lifecycle:**
   - Producer exits after reading entire file
   - Consumer exits when queue is empty AND no active producers
   - Main thread waits for both to complete

### Memory Management
- Automatic cleanup of semaphores
- Proper file handle management
- Thread-safe queue operations using STL containers

## File Structure

```
Concurrency-Programming/
├── main.cpp           # Main program and command-line parsing
├── producer.cpp       # Producer thread implementation
├── producer.h         # Producer function declarations
├── consumer.cpp       # Consumer thread implementation
├── consumer.h         # Consumer function declarations
├── shared_data.h      # Shared data structure definitions
├── utils.cpp          # Utility functions (timing, progress, file ops)
├── utils.h            # Utility function declarations
├── Makefile          # Build configuration
├── README.md         # This documentation
└── processOutputs.txt # Generated output file (after running)
```

### Key Components

- **`shared_data` struct**: Contains all shared variables and synchronization primitives
- **`receiverequests()`**: Producer function running in separate thread
- **`processrequests()`**: Consumer function running in separate thread
- **Progress tracking**: Real-time visual feedback system

## Troubleshooting

### Common Issues

1. **Permission Denied**
   ```bash
   chmod +x processrequests
   ```

2. **File Not Found**
   ```bash
   # Ensure file exists and is readable
   ls -la yourfile.txt
   cat yourfile.txt  # Test readability
   ```

3. **Compilation Errors**
   ```bash
   # Clean and rebuild
   make clean
   make
   ```

4. **Segmentation Fault**
   - Check file permissions
   - Verify file is not empty
   - Ensure valid command-line arguments

### Debugging

**Compile with debug symbols:**
```bash
make clean
make
gdb ./processrequests
```

**Run with verbose output:**
```bash
# Check file properties first
wc -l yourfile.txt
file yourfile.txt
```

## Educational Value

This project demonstrates several key concepts in concurrent programming:

### 1. **Producer-Consumer Pattern**
- **Problem**: Coordinating threads with different processing rates
- **Solution**: Bounded buffer with semaphore-based synchronization

### 2. **Semaphore Usage**
- **Counting Semaphores**: `empty` and `full` track resource availability
- **Binary Semaphore**: `mutex` provides mutual exclusion

### 3. **Thread Synchronization**
- **Race Conditions**: Prevented through proper locking
- **Deadlock Avoidance**: Consistent lock ordering
- **Resource Management**: Bounded queue prevents memory overflow

### 4. **Performance Analysis**
- **Throughput**: Effect of timing parameters on overall performance
- **Latency**: Queue size impact on response time
- **Scalability**: Understanding bottlenecks in concurrent systems

### Learning Exercises

1. **Modify timing parameters** and observe queue behavior
2. **Add multiple producers** by modifying the thread creation code
3. **Implement priority queues** for different types of requests
4. **Add statistics collection** for performance analysis
5. **Experiment with different semaphore configurations**

## Advanced Usage

### Multiple Test Files
```bash
# Create various test files
echo -e "Short\nLines\nHere" > short.txt
python3 -c "print('\n'.join([f'Line {i} with some content' for i in range(1000)]))" > long.txt

# Compare behavior
./processrequests -t 50 -c 50 short.txt
./processrequests -t 50 -c 50 long.txt
```

### Performance Benchmarking
```bash
# Time the execution
time ./processrequests -t 10 -c 10 large_file.txt

# Compare different configurations
time ./processrequests -t 100 -c 10 -q 10 large_file.txt
time ./processrequests -t 100 -c 10 -q 1000 large_file.txt
```

### Integration with Shell Scripts
```bash
#!/bin/bash
# test_suite.sh
for queue_size in 10 50 100 500; do
    echo "Testing with queue size: $queue_size"
    ./processrequests -q $queue_size -t 50 -c 50 test.txt
    echo "Queue size $queue_size completed"
done
```

## License

This project is created for educational purposes. Feel free to use, modify, and distribute for learning concurrent programming concepts.

---

**Author**: Alex Huang  
**Course**: Concurrency Programming  
**Focus**: Producer-Consumer Synchronization, Thread Safety, Semaphore Usage