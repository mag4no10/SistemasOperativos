#Makefile Jonay Faas Garcia

SHELL := /bin/bash
CXX = g++
CXXFLAGS = -Wall -std=c++17
######################################
files = ./src/*.cc
######################################
BOLD   := $(shell tput bold)
CLEAR  := $(shell tput sgr0)
######################################
all: mini_terminal

mini_terminal: $(files)
	@$(CXX) $(CXXFLAGS) -o mini_terminal $(files)
	@mv mini_terminal ./bin/

######################################
.PHONY: clean
clean:
	@echo "Cleaning up the directory..."
	@rm -rf *.o *gch *.out programa
######################################
.PHONY: help
help:
	@echo "${BOLD}make${CLEAR} -> compile"
	@echo "${BOLD}make clean${CLEAR} -> clean [.out/.o] files"
	@echo "${BOLD}make test1${CLEAR} -> not_full_buffer copy"
	@echo "${BOLD}make test2${CLEAR} -> recursive_full_buffer copy"

######################################
.PHONY: test1
test1:
	@chmod u+x ./test/test1.sh
	@./test/test1.sh

.PHONY: test2
test2:
	@chmod u+x ./test/test2.sh
	@./test/test2.sh