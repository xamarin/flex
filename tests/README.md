# Test suite

This directory contains the test suite for the project.

Tests are written in C directly using the C API and are separated per flexbox property or functionality.

It is preferable to cover each new change in the code base (even bug fixes) with one or more tests.

## Running the tests

You can run the test suite very easily:

```
$ make
```

If you see `OK` at the end you won!

## Adding tests

Adding a test is easy:

- If you are adding a test to an existing unit (one of the `test_*.c` files), simply add the test as a new function with the appropriate `test_<name of unit>` prefix. `make` will then pick up the new test and build/run it for you.

- If you are adding a test as a new unit, you have to create a new source file for the unit (example, `test_foo.c`) and add it into the `Makefile`. Then you can simply run `make`.
