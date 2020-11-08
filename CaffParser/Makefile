CXX = g++
CFLAGS = -Wall -O2 -std=c++11
TESTFLAGS = -Wall -O0 -ggdb3 -std=c++11 -g --coverage
BUILD_DIR = ./build

.PHONY: coverage cppcheck

all: main test

main: main.o ciff.o utils.o caff.o
	${CXX} ${TESTFLAGS} utils.o ciff.o caff.o main.o -o parser

test: utils.o test.o ciff.o caff.o 
	${CXX} ${TESTFLAGS} utils.o test.o ciff.o caff.o -o test

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

main.o: main.cpp ciff.h caff.h ciff.o utils.o caff.o
	${CXX} ${TESTFLAGS} -c main.cpp

coverage: all
	./test
	gcovr -r . --html --html-details -o coverage/coverage.html

cppcheck:
	cppcheck -v --enable=all --language=c++ --std=c++11 --xml *.cpp *.h 2> cppcheck_report.xml

clean:
	rm *.o *.gcda *.gcno test coverage/* test_previews/* previews/* cppcheck_report.xml

check-afl-env:
ifndef AFLCXX
		$(error AFLCXX is undefined)
endif
ifndef AFLFUZZ
		$(error AFLFUZZ is undefined)
endif

afl: check-afl-env
	${AFLCXX} ${TESTFLAGS} utils.cpp ciff.cpp caff.cpp main.cpp -o afl-test
	${AFLFUZZ} -i afl_testdir -o afl_findings -- ./afl-test @@ 	



