#include "test-lib.h"

int failState = 0;

int failed = 0;
int passed = 0;

void start(char* name) {
	printf("BEGINNING TEST of %s\n", name);
}

void assert(int val, char* message) {
	if(!val) {
		failState = 1;
		failed++;
	} else {
		passed++;
	}
	printf("   %s: %s\n", val ? "ASSERTION PASSED" : "ASSERTION FAILED", message);
}

void end() {
	printf("%s - result: %d/%d\n", failState ? "TEST FAILED" : "TEST SUCCESSFUL", passed, passed + failed);
	
	// reset fail state for next test
	failState = 0;
	
	// reset assertion counts for next test
	failed = 0;
	passed = 0;
}