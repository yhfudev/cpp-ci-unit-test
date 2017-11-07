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

Usage
-----

For source code, place the unit test cases either in your c source code file, or in a seperate file. for example:

```C
#include <string.h>

#define CIUT_ENABLED 1 /**< user defined, a global macro defined to 1 to active the unit test code */

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

and then, place the ciut main function in somewhere,

```C
#define CIUT_ENABLED    1 /**< user defined, a global macro defined to 1 to active the unit test code */
#define CIUT_PLACE_MAIN 1 /**< user defined, a local macro defined to 1 to place main() inside a c file, use once */
#include <ciut.h>
```

It's done.
See the example files examples/ciutexc.c and examples/test-lang.c.

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

