ciut
====

An unit test framwork for C and C++.


License | Travis | Appveyor | Coverity
------- | ------ | -------- | --------
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/yhfudev/cpp-ci-unit-test/blob/master/COPYING) | [![Travis Build status](https://travis-ci.org/yhfudev/cpp-ci-unit-test.svg?branch=master)](https://travis-ci.org/yhfudev/cpp-ci-unit-test) | [![Appveyor Build status](https://ci.appveyor.com/api/projects/status/d849gxe54umc9nmp/branch/master?svg=true)](https://ci.appveyor.com/project/yhfudev/cpp-ci-unit-test/branch/master) | [![Coverity Scan Build Status](https://scan.coverity.com/projects/12089/badge.svg)](https://scan.coverity.com/projects/yhfudev-cpp-ci-unit-test)


We introduce ciut as an unit test framwork for C and C++, has following features:

* adding tests with minimal hassle, no manual code to add test cases to suites, the test cases are registered automatically;
* single header file, no extra library files, portable and easy to integrate;
* minimized number of APIs to learn quick;
* don't need specify the test name, it's generated automatically if not given;
* the test case can be skipped by specify the argument '.skip=1' in the TEST_CASE(), switch it on/off by only one single char!
* the unit test code can be placed in both with the code or in a separated file;
* reports file and line number of the fail point;
* continue testing next after reporting assertion fail or segmentation fault;
* support float equality checking;
* test cases can be selected to run by providing filter arguments in command line;


Simple Usage
------------

For source code, place the unit test cases either in your c source code file, or in a seperate file. for example, mytests.c:

```C
#include <string.h>

// #define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

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

and then, place the ciut main function in somewhere, for example, ciutexc.c

```C
// #define CIUT_ENABLED    1 /**< user defined, a global macro defined to 1 to active the unit test code */
#define CIUT_PLACE_MAIN 1 /**< user defined, a local macro defined to 1 to place main() inside a c file, use once */
#include <ciut.h>

int main(int argc, char * argv[])
{
    return ciut_main(argc, argv);
}
```

It's done.
See the example files examples/ciutexc.c and examples/test-lang.c.

To compile the test executable, for example, by gcc:
```bash
gcc -DCIUT_ENABLED=1 -o ciutexc mytests.c ciutexc.c
```

To run the executable, you can also specify arguments for it, run help to get more details
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

