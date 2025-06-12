# Alex Huang; RED_ID: 130124082
# CXX Make variable for compiler
CC=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CCFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = consumer.o producer.o main.o utils.o

# Program name
PROGRAM = processrequests

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -pthread -o $(PROGRAM) $(OBJS)

main.o : main.cpp
	$(CC) $(CCFLAGS) main.cpp
	
producer.o: producer.cpp producer.h
	$(CC) $(CCFLAGS) producer.cpp
    
consumer.o: consumer.cpp consumer.h
	$(CC) $(CCFLAGS) consumer.cpp

utils.o: utils.cpp utils.h
	$(CC) $(CCFLAGS) utils.cpp

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -f *.o *~ $(PROGRAM)

