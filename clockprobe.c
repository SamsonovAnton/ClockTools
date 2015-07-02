
#include <errno.h>
#include <time.h>
#include <stdio.h>


void ProbeClocks() {
	clockid_t idClock;
	printf("Supported clock ids: ");
	for (idClock = 0; idClock <= 254; ++idClock) {
		struct timespec dtValue;
		if (clock_gettime(idClock, &dtValue) == 0) {
			printf("%u, ", (unsigned int)idClock);
		} else if (errno != EINVAL) {
			printf("%u (errno=%u), ", (unsigned int)idClock, (unsigned int)errno);
		} // end if
	} // end for
	printf("end.\n");
} // end function ProbeClocks


int main(int argc, char **argv) {
	ProbeClocks();
	return 0;
} // end function main
