CXX = g++
CFLAGS = -Wall -O2 -std=c++11
TESTFLAGS = -Wall -O0 -ggdb3 -std=c++11 -g --coverage
BUILD_DIR = ./build

.PHONY: coverage

all: utils.o test.o ciff.o caff.o 
	${CXX} ${TESTFLAGS} utils.o test.o ciff.o caff.o -o test

utils.o: utils.cpp utils.h
	${CXX} ${TESTFLAGS} -c utils.cpp

ciff.o: utils.h ciff.cpp ciff.h
	${CXX} ${TESTFLAGS} -c ciff.cpp

caff.o: ciff.h caff.cpp ciff.o utils.o caff.h
	${CXX} ${TESTFLAGS} -c caff.cpp

test.o: test.cpp ciff.h caff.h ciff.o utils.o caff.o
	${CXX} ${TESTFLAGS} -c test.cpp 

coverage: all
	./test
	gcovr -r . --html --html-details -o coverage/coverage.html

clean:
	rm *.o *.gcda *.gcno test coverage/* test_previews/*



