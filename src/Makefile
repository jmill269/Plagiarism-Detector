##################################################
# Makefile
# Jonathan Miller
#
###################################################
# USAGE:
#
# to compile:
# make
#
# to compile tests and run tests
# make test
#
# remove compilation output files:
# make clean
#

CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -g -O3 -I../include
INC_DIRS = ../include
OBJ_DIR = ./obj
BIN_DIR = ../bin

#make sure output directories exist
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

all: $(BIN_DIR)/plagiarism $(BIN_DIR)/unitTest

test: $(BIN_DIR)/unitTest #@echo "running tests"
	$(BIN_DIR)/unitTest

$(BIN_DIR)/plagiarism: $(OBJ_DIR)/plagiarism.o $(OBJ_DIR)/metric.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/metric.o $(OBJ_DIR)/plagiarism.o -o $(BIN_DIR)/plagiarism

$(BIN_DIR)/unitTest: $(OBJ_DIR)/unitTestsMetric.o $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/metric.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/unitTestDriver.o $(OBJ_DIR)/unitTestsMetric.o $(OBJ_DIR)/metric.o -o $(BIN_DIR)/unitTest

$(OBJ_DIR)/plagiarism.o: plagiarism.cpp
	$(CC) $(CFLAGS) -c plagiarism.cpp -o $(OBJ_DIR)/plagiarism.o

$(OBJ_DIR)/unitTestDriver.o: unitTestDriver.cpp
	$(CC) $(CFLAGS) -c unitTestDriver.cpp -o $(OBJ_DIR)/unitTestDriver.o

$(OBJ_DIR)/unitTestsMetric.o: unitTestsMetric.cpp
	$(CC) $(CFLAGS) -c unitTestsMetric.cpp -o $(OBJ_DIR)/unitTestsMetric.o

$(OBJ_DIR)/metric.o: metric.cpp $(INC_DIRS)/metric.hpp
	$(CC) $(CFLAGS) -c metric.cpp -o $(OBJ_DIR)/metric.o


###############################
# "utility" rules
###############################
# the .PHONY line says these targets aren't expected to generate files named after the target
.PHONY: clean test

clean:
	@echo "Removing generated files..."
	@rm -f plagiarism $(OBJ_DIR)/*.o *.gcda *.gcno *.gcov $(BIN_DIR)/plagiarism *~ *# ./*~ ./*#
	@rm -f $(BIN_DIR)/*
