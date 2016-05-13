#include "softassert.h"

void softassert(int val, char *message) {
	if(!val) {
		printf("Assertion failed: %s\n", message);
	}
}