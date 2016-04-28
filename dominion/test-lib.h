#include <stdio.h>

// Prints the start message for a test, given the name of the function it is testing.
void start(char* name);

// Asserts a statement without killing the test. Prints either success or failure.
void assert(int val, char* message);

// Ends the test. Prints where the test passed or failed based on the previously asserted
// statements. Additionally, this function resets the current state of test-lib.
void end();