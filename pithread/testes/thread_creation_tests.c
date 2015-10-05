

#include "../include/pidata.h"
#include <stdlib.h>
#include <stdio.h>

void* printTest1(void*) {
	printf("Test 1\n");
}

int main(int argc, char const *argv[]) {
	printf("START THREAD CREATION TESTS\n");

	picreate(-1, printTest1, NULL);
	return 0;
}

