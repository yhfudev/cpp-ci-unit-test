ciut
====

An unit test framwork for C and C++.


[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/yhfudev/cpp-ci-unit-test/blob/master/COPYING)
[![Travis Build status](https://travis-ci.org/yhfudev/cpp-ci-unit-test.svg?branch=master)](https://travis-ci.org/yhfudev/cpp-ci-unit-test)
[![Appveyor Build status](https://ci.appveyor.com/api/projects/status/d849gxe54umc9nmp/branch/master?svg=true)](https://ci.appveyor.com/project/yhfudev/cpp-ci-unit-test/branch/master)
[![codecov](https://codecov.io/gh/yhfudev/cpp-ci-unit-test/branch/master/graph/badge.svg)](https://codecov.io/gh/yhfudev/cpp-ci-unit-test)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/12089/badge.svg)](https://scan.coverity.com/projects/yhfudev-cpp-ci-unit-test)


We are pleased to present ciut, a comprehensive unit test framework designed specifically for C and C++ programming languages. This powerful framework offers a range of features that simplify the process of writing and executing unit tests. Here are some notable features of ciut:

1. Seamless Test Integration: With ciut, adding tests to your codebase is hassle-free. There is no need to manually add test cases to suites, as the test cases are automatically registered.
1. Lightweight and Portable: ciut consists of a single header file and does not require any additional library files. This makes it highly portable and easy to integrate into your existing projects.
1. Minimal Learning Curve: We have carefully designed ciut to have a minimal number of APIs, allowing you to quickly learn and utilize the framework efficiently.
1. Automatic Test Case Naming: If you don't specify a name for a test case, ciut automatically generates one for you, saving you time and effort.
1. Simple Test Skipping: Easily skip a test case by specifying the '`.skip=1`' argument within `TEST_CASE()`. Toggle the skipping on or off with just a single character.
1. Flexible Test Placement: Whether you prefer embedding the unit test code within your main code or keeping it in a separate file, ciut accommodates both options.
1. Accurate Error Reporting: Identify the exact file and line number where a failure occurs, facilitating precise debugging.
1. Continuous Testing: Experience uninterrupted testing even after reporting an assertion failure or encountering a segmentation fault.
1. Float Equality Support: Seamlessly check for equality among floating-point values, ensuring accurate comparisons.
1. Command-Line Interface: ciut generates a test executable that provides a wide range of command-line arguments, offering flexibility and customization options for running your tests.
1. Selective Test Execution: Tailor your test runs by providing filter arguments via the command line, allowing for targeted and efficient testing.

In summary, ciut offers a comprehensive and user-friendly unit test framework for C and C++ development. Its features simplify the testing process, enhance debugging capabilities, and provide flexibility in test execution.


Simple Usage
------------

To incorporate unit test cases into your source code, you have two options: either include them within your C source code file or place them in a separate file (e.g., mytests.c). Here's a simple example to illustrate:

```C
#include <string.h>

// #define CIUT_ENABLED 1 /**< It is a user-defined global macro that should be set to 1 in order to activate the unit test code */

#if defined(CIUT_ENABLED) && (CIUT_ENABLED == 1)
#include <ciut.h>

TEST_CASE() {
    REQUIRE(0 == 1);
}

TEST_CASE( .description="Test float point equation.", .skip=0 ) {
    CIUT_DBL_EQUAL(0.1 + 0.2, 0.3);
}

TEST_CASE( .description="Test the __FUNCTION__ macro.", .skip=0 ) {
    SECTION("test func") {
        REQUIRE(0 == strcmp(__FUNCTION__, "ciut_function_auto_1"));
    }
}
#endif /* CIUT_ENABLED */
```

To use the ciut framework, you need to place the `ciut_main()` function in the `main()` function. Here's an example in a file named "ciutexc.c":
```C
// #define CIUT_ENABLED    1 /**< It is a user-defined global macro that should be set to 1 in order to activate the unit test code */
#define CIUT_PLACE_MAIN 1 /**< It is user-defined, a local macro that should be set to 1 in order to place main() inside a C file. This macro should be used only once */
#include <ciut.h>

int main(int argc, char * argv[])
{
    return ciut_main(argc, argv);
}
```

It's done. If you're looking for reference examples, you can find them in the ciut framework's official examples folder. Specifically, you can refer to the "ciutexc.c" and "test-lang.c" files located in the "examples" directory.

To compile the test executable, you can use the gcc compiler as an example. Here's a command you can use:
```bash
gcc -DCIUT_ENABLED=1 -o ciutexc mytests.c ciutexc.c
```
After executing this command, the gcc compiler will compile and link both files together to generate the test executable.

To run the executable with specific arguments and get more details about available options, you can use the following command:
```bash
ciutexc -h

Usage:
    ./ciutexc <options>
Options:
    -h            show help
    -l            list of test cases
    -f <filter>   the filter string
    -t <title>    unit test title
    -x <xml file> log to XML file
    -c | -        log to stdout
```


API Manual
----------

In this section each API function will be described in detail.
You may get the project document by:
```bash
cd doc && make clean && make
```

### CIUT_TEST_CASE() or TEST_CASE()

It's a declaration of a test block.

The arguments supported by this macro include:
* .name: a C string for name of test case
* .description: a C string description of this test case
* .skip: if this test should skipped by default. 0 - will be executed by default, 1 - will be skipped by default.

For example:
```C
CIUT_TEST_CASE(.name="test title", .description="This is a test.", .skip=1) {
    CIUT_SECTION("Test section 1") {
        CIUT_ASSERT(0 == 0);
    }
}
```

### CIUT_SECTION(title) or SECTION(title)

This is a section declaration in a test block, to separate the test codes to multiple sections.
```C
CIUT_SECTION("section title") {
}
```

### CIUT_ASSERT(condition) or REQUIRE(condition)

It's a assertion for a condition, if it is true, then the test is passed,
otherwise the test failed.
```C
CIUT_ASSERT(0 == 0);
```

### CIUT_DBL_EQUAL(val1, val2)

This macro is an assertion that two float values are equal in the scale of 1e-15.
If the difference of values are out of range(>1e-15), then test failed.
```C
CIUT_DBL_EQUAL(0.1 + 0.2, 0.3);
```

### CIUT_LOG()

The is a help function for logging messages.
```C
CIUT_LOG("This is a test");
CIUT_LOG("The values is %d", 1);
```

### switch CIUT_PLACE_MAIN
This is a switch for user place the main function in the source code by define it to '1', such as:
```C
#define CIUT_HANDLE_SIGSEGV 1 /**< user defined in ciut main, set to 0 if you want to dig into where is the segmentation fault happened using gdb */
#define CIUT_PLACE_MAIN 1 /**< user defined, a local macro defined to 1 to place main() inside a c file, use once */
#include <ciut.h>

int main(int argc, char * argv[])
{
    return ciut_main(argc, argv);
}
```

### switch CIUT_HANDLE_SIGSEGV

This is a switch for the code handle if segmentation fault happen.
It need to be used with the function ciut_main(), see above example.


TODO List
---------

* handle expected segmentation fault;
* supports fuzzing(fuzz testing) automated testing technique (https://en.wikipedia.org/wiki/Fuzzing);
  AFL (american fuzzy lop, http://lcamtuf.coredump.cx/afl/)




