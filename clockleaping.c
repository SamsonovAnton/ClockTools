

// #if defined(__gnu_linux__) || defined(__linux__) || defined(__linux) || defined(linux)
// 	#include <linux/time.h>
// #elif defined(__FreeBSD__)
// 	// Nothing special currently.
// #elif defined(__CYGWIN__)
// 	// Nothing special currently.
// #endif


#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#if defined(__gnu_linux__) || defined(__linux__) || defined(__linux) || defined(linux)
	#ifndef CLOCK_TAI
		#define CLOCK_TAI 11	// Quirk for openSUSE and perhaps other Linux systems as well.
	#endif
// #elif defined(__FreeBSD__)
// 	// Nothing special currently.
// #elif defined(__CYGWIN__)
// 	// Nothing special currently.
// #else
// 	// Nothing special currently.
#endif



// Typical bindings for POSIX clocks.

typedef size_t clocktype_t;

#define CLOCKTYPE_COUNT UINT8_C(27)
#define CLOCKTYPE_UNSUPPORTED UINT8_MAX


#define CLOCKTYPE_REALTIME UINT8_C(0)
#define CLOCKTYPE_REALTIME_HR UINT8_C(1)
#define CLOCKTYPE_REALTIME_PRECISE UINT8_C(2)
#define CLOCKTYPE_REALTIME_COARSE UINT8_C(3)
#define CLOCKTYPE_REALTIME_FAST UINT8_C(4)

#define CLOCKTYPE_SOFTTIME UINT8_C(5)

#define CLOCKTYPE_MONOTONIC UINT8_C(6)
#define CLOCKTYPE_MONOTONIC_PRECISE UINT8_C(7)
#define CLOCKTYPE_MONOTONIC_COARSE UINT8_C(8)
#define CLOCKTYPE_MONOTONIC_FAST UINT8_C(9)
#define CLOCKTYPE_MONOTONIC_RAW UINT8_C(10)

#define CLOCKTYPE_BOOTTIME UINT8_C(11)
#define CLOCKTYPE_UPTIME UINT8_C(12)
#define CLOCKTYPE_UPTIME_PRECISE UINT8_C(13)
#define CLOCKTYPE_UPTIME_FAST UINT8_C(14)

#define CLOCKTYPE_REALTIME_ALARM UINT8_C(15)
#define CLOCKTYPE_BOOTTIME_ALARM UINT8_C(16)

#define CLOCKTYPE_PROCESS_CPUTIME_ID UINT8_C(17)
#define CLOCKTYPE_THREAD_CPUTIME_ID UINT8_C(18)

#define CLOCKTYPE_VIRTUAL UINT8_C(19)
#define CLOCKTYPE_PROF UINT8_C(20)
#define CLOCKTYPE_PROFILE UINT8_C(21)
#define CLOCKTYPE_SGI_CYCLE UINT8_C(22)

#define CLOCKTYPE_HIGHRES UINT8_C(23)
#define CLOCKTYPE_SECOND UINT8_C(24)

#define CLOCKTYPE_UTC UINT8_C(25)
#define CLOCKTYPE_TAI UINT8_C(26)



const char* ClockName[] = {

	"CLOCK_REALTIME",
	"CLOCK_REALTIME_HR",
	"CLOCK_REALTIME_PRECISE",
	"CLOCK_REALTIME_COARSE",
	"CLOCK_REALTIME_FAST",

	"CLOCK_SOFTTIME",

	"CLOCK_MONOTONIC",
	"CLOCK_MONOTONIC_PRECISE",
	"CLOCK_MONOTONIC_COARSE",
	"CLOCK_MONOTONIC_FAST",
	"CLOCK_MONOTONIC_RAW",

	"CLOCK_BOOTTIME",
	"CLOCK_UPTIME",
	"CLOCK_UPTIME_PRECISE",
	"CLOCK_UPTIME_FAST",

	"CLOCK_REALTIME_ALARM",
	"CLOCK_BOOTTIME_ALARM",

	"CLOCK_PROCESS_CPUTIME_ID",
	"CLOCK_THREAD_CPUTIME_ID",

	"CLOCK_VIRTUAL",
	"CLOCK_PROF",
	"CLOCK_PROFILE",
	"CLOCK_SGI_CYCLE",

	"CLOCK_HIGHRES",
	"CLOCK_SECOND",

	"CLOCK_UTC",
	"CLOCK_TAI"

};


// System-specific bindings for POSIX clocks.

clockid_t ClockId[] = {


#if defined(CLOCK_REALTIME)
	CLOCK_REALTIME,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_REALTIME_HR)
	CLOCK_REALTIME_HR,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_REALTIME_PRECISE)
	CLOCK_REALTIME_PRECISE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_REALTIME_COARSE)
	CLOCK_REALTIME_COARSE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_REALTIME_FAST)
	CLOCK_REALTIME_FAST,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_SOFTTIME)
	CLOCK_SOFTTIME,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_MONOTONIC)
	CLOCK_MONOTONIC,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_MONOTONIC_PRECISE)
	CLOCK_MONOTONIC_PRECISE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_MONOTONIC_COARSE)
	CLOCK_MONOTONIC_COARSE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_MONOTONIC_FAST)
	CLOCK_MONOTONIC_FAST,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_MONOTONIC_RAW)
	CLOCK_MONOTONIC_RAW,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_BOOTTIME)
	CLOCK_BOOTTIME,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_UPTIME)
	CLOCK_UPTIME,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_UPTIME_PRECISE)
	CLOCK_UPTIME_PRECISE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_UPTIME_FAST)
	CLOCK_UPTIME_FAST,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_REALTIME_ALARM)
	CLOCK_REALTIME_ALARM,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_BOOTTIME_ALARM)
	CLOCK_BOOTTIME_ALARM,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_PROCESS_CPUTIME_ID)
	CLOCK_PROCESS_CPUTIME_ID,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_THREAD_CPUTIME_ID)
	CLOCK_THREAD_CPUTIME_ID,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_VIRTUAL)
	CLOCK_VIRTUAL,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_PROF)
	CLOCK_PROF,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_PROFILE)
	CLOCK_PROFILE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_SGI_CYCLE)
	CLOCK_SGI_CYCLE,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_HIGHRES)
	CLOCK_HIGHRES,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_SECOND)
	CLOCK_SECOND,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif


#if defined(CLOCK_UTC)
	CLOCK_UTC,
#else
	CLOCKTYPE_UNSUPPORTED,
#endif

#if defined(CLOCK_TAI)
	CLOCK_TAI
#else
	CLOCKTYPE_UNSUPPORTED
#endif

};

int ClockAvailable[CLOCKTYPE_COUNT];


#define ClocksToProbe UINT8_C(12)
const clocktype_t ClockToProbe[] = {
	CLOCKTYPE_REALTIME,
	CLOCKTYPE_REALTIME_PRECISE,
	CLOCKTYPE_REALTIME_COARSE,
	CLOCKTYPE_MONOTONIC,
	CLOCKTYPE_MONOTONIC_PRECISE,
	CLOCKTYPE_MONOTONIC_COARSE,
	CLOCKTYPE_MONOTONIC_RAW,
	CLOCKTYPE_BOOTTIME,
	CLOCKTYPE_UPTIME,
	CLOCKTYPE_HIGHRES,
	CLOCKTYPE_UTC,
	CLOCKTYPE_TAI
};

size_t ClocksToRecord = UINT8_C(0);
clocktype_t ClockToRecord[ClocksToProbe];



void PrintTimespec(const struct timespec* pdtValue) {
	if (pdtValue) {
		if (pdtValue->tv_nsec <= INT32_C(999999999)) {
			printf("%010" PRIu32 ".%09" PRIu32, (uint32_t)pdtValue->tv_sec, (uint32_t)pdtValue->tv_nsec);
		} else if (pdtValue->tv_nsec <= INT32_C(1999999999)) {
			printf("%010" PRIu32 "+1.%09" PRIu32, (uint32_t)pdtValue->tv_sec, (uint32_t)(pdtValue->tv_nsec - INT32_C(1000000000)));
		} else {
			printf("%010" PRIu32 "?%li", (uint32_t)pdtValue->tv_sec, pdtValue->tv_nsec);
		} // end if
	} else {
		printf("%10" PRIu32 ".%09" PRIu32, UINT32_C(0), UINT32_C(0));
	} // end if
} // end function PrintTimespec


void PrintTm(const struct tm* poValue) {
	printf("%04i-%02i-%02i %02i:%02i:%02i",
		poValue->tm_year + 1900, poValue->tm_mon + 1, poValue->tm_mday,
		poValue->tm_hour, poValue->tm_min, poValue->tm_sec);
} // end function PrintTimespec


int GetAvailableClocks(void) {

	size_t ndxClock;
	clocktype_t ordClock;
	clockid_t idClock;
	struct timespec dtValue;
	char cStatus;

	printf("Defined clocks ([+] = operational, [-] = immobilized, [X] = missing):\n");
	for(ordClock = 0; ordClock < CLOCKTYPE_COUNT; ++ordClock) {
		idClock = ClockId[ordClock];
		if (idClock == CLOCKTYPE_UNSUPPORTED) {
			ClockAvailable[ordClock] = 0; continue;
		} else if (clock_gettime(idClock, &dtValue) == 0) {
			ClockAvailable[ordClock] = 1; cStatus = '+';
		} else {
			ClockAvailable[ordClock] = 0; cStatus = ((errno == EINVAL) ? 'X' : '-' );
		} // end if
		printf("\t[%c] %s (%" PRIu32 ")\n", cStatus, ClockName[ordClock], (uint32_t)idClock);
	} // end for

	if (!ClockAvailable[CLOCKTYPE_REALTIME]) {
		printf("\nError: CLOCK_REALTIME is not available.\n");
		return 0;
	} // end if

 	printf("Recorded clocks ([+] = selected, [D] = duplicate):\n");
	for(ndxClock = 0; ndxClock < ClocksToProbe; ++ndxClock) {
		size_t ndxClockEarlier;
		ordClock = ClockToProbe[ndxClock];
		idClock = ClockId[ordClock];
		if (!ClockAvailable[ordClock]) continue;
		for(ndxClockEarlier = 0; ndxClockEarlier < ClocksToRecord; ++ndxClockEarlier) {
			if (ClockToRecord[ndxClockEarlier] == ordClock) break;
		} // end for
		if (ndxClockEarlier == ClocksToRecord) {
			ClockToRecord[ClocksToRecord++] = ordClock; cStatus = '+';
		} else {
			cStatus = 'D';
		} // end if
		printf("\t[%c] %s (%" PRIu32 ")\n", cStatus, ClockName[ordClock], (uint32_t)idClock);
	} // end for

	return 1;
} // end function GetAvailableClocks


struct timespec ComputeStartTime(uint nSeconds) {

	struct timespec dtNowHQ;
	time_t dtNowLQ;
	struct tm* poNow;

	struct tm oThen;
	time_t dtThenLQ;
	struct timespec dtThenHQ;

	char *pszTZ;

	// Force UTC as local time.
	pszTZ = getenv("TZ");
	setenv("TZ", "", 1);
	tzset();

	clock_gettime(CLOCK_REALTIME, &dtNowHQ);
	dtNowLQ = dtNowHQ.tv_sec;
	poNow = localtime(&dtNowLQ);
	printf("Current time is "); PrintTm(poNow); printf(" UTC ["); PrintTimespec(&dtNowHQ); printf("].\n");

	oThen = *poNow;
	oThen.tm_hour = 23;
	oThen.tm_min = 59;
	oThen.tm_sec = 60 - nSeconds;
	dtThenLQ = mktime(&oThen);
	dtThenHQ.tv_sec = dtThenLQ;
	dtThenHQ.tv_nsec = 0;
	printf("Shall record at "); PrintTm(&oThen); printf(" UTC ["); PrintTimespec(&dtThenHQ); printf("].\n");

	if (pszTZ) setenv("TZ", pszTZ, 1);
	else unsetenv("TZ");
	tzset();

	//dtThenHQ.tv_sec = dtNowHQ.tv_sec + 5;
	return dtThenHQ;
} // end function ComputeStartTime


void WaitUntil(struct timespec* pdtStart) {

	while (1) {

		struct timespec dtNow;
		uint32_t nSecondsRemaining, nSecondsSleep;

		clock_gettime(CLOCK_REALTIME, &dtNow);
		if (dtNow.tv_sec >= pdtStart->tv_sec) break;

		nSecondsRemaining = pdtStart->tv_sec - dtNow.tv_sec;

		if (nSecondsRemaining <= 10) {
			printf("Recording will start in %" PRIu32 " seconds.", nSecondsRemaining);
			if (nSecondsRemaining <= 1) { printf("\n"); break; }
			nSecondsSleep = 1;
		} else {
			printf("Still %" PRIu32 " seconds until recording starts.", nSecondsRemaining);
			if (nSecondsRemaining <= 30) {
				nSecondsSleep = 5;
			} else if (nSecondsRemaining <= 60) {
				nSecondsSleep = 10;
			} else if (nSecondsRemaining <= 120) {
				nSecondsSleep = 30;
			} else if (nSecondsRemaining <= 600) {
				nSecondsSleep = 60;
			} else if (nSecondsRemaining <= 1800) {
				nSecondsSleep = 300;
			} else if (nSecondsRemaining <= 3600) {
				nSecondsSleep = 600;
			} else if (nSecondsRemaining <= 7200) {
				nSecondsSleep = 1800;
			} else {
				nSecondsSleep = 3600;
			} // end if
		} // end if

		printf(" Sleeping for %" PRIu32 " seconds...\n", nSecondsSleep);
		fflush(stdout);
		sleep(nSecondsSleep);

	} // end while


} // end function WaitUntil


void Record(uint nSeconds) {

	size_t ndxClock;
	clocktype_t ordClock;
	clockid_t idClock;
	struct timespec dtPrevRealtime;

	printf("%s", ClockName[CLOCKTYPE_REALTIME]);
	for(ndxClock = 1; ndxClock < ClocksToRecord; ++ndxClock) {
		ordClock = ClockToRecord[ndxClock];
		idClock = ClockId[ordClock];
		printf("\t%s", ClockName[ordClock]);
	} // end for
	printf("\n");

	clock_gettime(CLOCK_REALTIME, &dtPrevRealtime);

	while (nSeconds) {

		struct timespec dtCurRealtime;
		struct timespec dtCurAux;

		if (clock_gettime(CLOCK_REALTIME, &dtCurRealtime) == 0) {
			PrintTimespec(&dtCurRealtime);
		} else {
			PrintTimespec(NULL);
		} // end if

		for(ndxClock = 1; ndxClock < ClocksToRecord; ++ndxClock) {
			idClock = ClockId[ClockToRecord[ndxClock]];
			printf("\t");
			if (clock_gettime(idClock, &dtCurAux) == 0) {
				PrintTimespec(&dtCurAux);
			} else {
				PrintTimespec(NULL);
			} // end if
		} // end for
		printf("\n");

		if (dtCurRealtime.tv_sec != dtPrevRealtime.tv_sec) {
			--nSeconds;
		} // end if

		dtPrevRealtime = dtCurRealtime;

	} // end while

} // end function Record


int main(int argc, char **argv) {

	const uint nSecondsProlog = 5;
	const uint nSecondsTotal = 2 * nSecondsProlog;

	struct timespec dtStart;

	printf("\nClock leaping test\n\n");
	if (!GetAvailableClocks()) return 1; printf("\n");
	dtStart = ComputeStartTime(nSecondsProlog); printf("\n");
	WaitUntil(&dtStart); printf("\n");
	Record(nSecondsTotal); printf("\n");

	return 0;
} // end function main
