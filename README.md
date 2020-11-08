# Documentation

## Caff Parser

## Requirements
* To compile the project **gcc 6+**, **GNU make** and up to date **C library** is necessary
* To create coverage report **gcovr** is neccessary (if you have **pip**, you can install it with `pip install gcovr`)
* To create cppcheck xml report, **cppcheck** is necessary

## Compiling the project

Simply run `make` in the **CaffParser** folder.

## Trying out the tool

From **CaffParser** dir run `./parser <file-name.caff>` or `./parser < <file-name.caff>`. If the given input is a valid Caff file, then it will write out the Credits info and create a preview of all the Ciffs found in the **CaffParser/previews** folder.

## Running the tests

The tests can be run after compiling the project by running `./test` in the **CaffParser** folder.

## Creating coverage report

Coverage report can be created by running `make coverage` in the **CaffParser** folder. The generated html report can be found in **CaffParser/coverage**

We have reached almost 100% line coverage with our tests. There are a few lines unchecked. For those we have left comments why we consider that particular line is unreachable. There is one line in **ciff.cpp** in the BMP generator function which is untested. It needs a Ciff, where the image's width % 4 != 0 to use the padding, but it's not the core functionality of the parser.

Also there are uncovered lines in **test.cpp** but it's not in the SUT, so it does not have to be fully covered. **main.cpp** is only used as an interface for the parser, so it is not included in coverage report, the reported coverage for it is 0 (or it does not matter).

Current coverage report:

![coverage report](https://github.com/adamk90/PictoGraphy/blob/master/images/coverage.png)

## Using valgrind

By default, makefile compiles the project with debug options, so **valgrind** can be used without any modifications by for example running `valgrind ./test`. Our run of **valgrind**:

![valgrind output](https://github.com/adamk90/PictoGraphy/blob/master/images/valgrind_test_result.png)

## Using cppcheck

For creating the cppcheck report, run `make cppcheck`. The report will be named as **CaffParser/cppcheck_report.xml**.

Our initial run of cppcheck:

![cppcheck before](https://github.com/adamk90/PictoGraphy/blob/master/images/cppcheck_before.png)


After we have fixed the foundings:

![cppcheck after](https://github.com/adamk90/PictoGraphy/blob/master/images/cppcheck_after.png)

Only warning is we did not use --config-check flag, but then it writes that standard includes are not necessary for correct run, so we found it a suppressable warning.

## AFL Fuzzer

To run the AFL fuzzer, AFL needs to be installed and AFLCXX, AFLFUZZ env variables set to afl-g++ and afl-fuzz respectively.


`AFLCXX="path/to/afl/afl-g++" AFLFUZZ="path/to/afl/afl-fuzz" make afl`

We have run the fuzzer for almost 6 hours, but no crashes occured. There were 20 unique timeouts, but those are not considered bad (not hangs, just didnt complete under a limit), only slow. As AFL have not created any inputs for the timeouts, we could not examine them further. The result can be seen here:

![afl output](https://github.com/adamk90/PictoGraphy/blob/master/images/afl_output.png)
