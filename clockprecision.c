
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS

#include <stdint.h>
#include <inttypes.h>
#include <memory.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint32_t nanoseconds_t;
#define NANOSECONDS_C(x) UINT32_C(x)
#define NANOSECONDS_MAX UINT32_MAX
#define PRIuNANOSECONDS PRIu32
#define SECONDS_TO_NANOSECONDS NANOSECONDS_C(1000000000)
#define MILLISECONDS_TO_NANOSECONDS NANOSECONDS_C(1000000)
#define MICROSECONDS_TO_NANOSECONDS NANOSECONDS_C(1000)

typedef uint32_t microseconds_t;
#define MICROSECONDS_C(x) UINT32_C(x)
#define MICROSECONDS_MAX UINT32_MAX
#define PRIuMICROSECONDS PRIu32
#define SECONDS_TO_MICROSECONDS MICROSECONDS_C(1000000)


#ifdef CLOCK_REALTIME
	#define ENGINE_CLOCK_GETTIME_REALTIME
#endif
#ifdef CLOCK_MONOTONIC
	#define ENGINE_CLOCK_GETTIME_MONOTONIC
#endif
#ifdef CLOCK_PROCESS_CPUTIME_ID
	#define ENGINE_CLOCK_GETTIME_PROCESS_CPUTIME_ID
#endif
#ifdef CLOCK_THREAD_CPUTIME_ID
	#define ENGINE_CLOCK_GETTIME_THREAD_CPUTIME_ID
#endif
#if defined(ENGINE_CLOCK_GETTIME_REALTIME) \
	| defined(ENGINE_CLOCK_GETTIME_MONOTONIC) \
	| defined(ENGINE_CLOCK_GETTIME_PROCESS_CPUTIME_ID) \
	| defined(ENGINE_CLOCK_GETTIME_THREAD_CPUTIME_ID)
	#define ENGINE_CLOCK_GETTIME_ANY
#endif
#define ENGINE_GETTIMEOFDAY
#define ENGINE_CLOCK_CPUTIME



struct clock_precision_stats {
	nanoseconds_t resolution;
	nanoseconds_t all_minimum;
	nanoseconds_t all_average;
	nanoseconds_t sel_minimum;
	nanoseconds_t sel_average;
	nanoseconds_t sel_median;
	nanoseconds_t sel_maximum;
	nanoseconds_t sel_deviation;
}; // end struct clock_precision_stats

typedef nanoseconds_t (*clock_precision_querier)(const void* engine_args);

struct clock_engine_conf {
	const char* name;
	const void* args;
	clock_precision_querier resolution_querier;
	clock_precision_querier precision_measurer;
}; // end struct clock_engine_conf

struct clock_benchmark_conf {
	size_t batch_count;
	size_t trial_count;
	microseconds_t pause;
	nanoseconds_t* samples;
	size_t sel_margin;
}; // end struct clock_benchmark_conf



int compare_nanoseconds(const void* left, const void* right) {
	if (*(nanoseconds_t*)left < *(nanoseconds_t*)right) return -1;
	if (*(nanoseconds_t*)left > *(nanoseconds_t*)right) return +1;
	return 0;
} // end function compare_nanoseconds


uint8_t log2_uint32(uint32_t value) {
	uint8_t result = 0;
	while (value >>= 1) ++result;
	return result;
} // log2_uint32

uint8_t log2_uint64(uint64_t value) {
	uint8_t result = 0;
	while (value >>= 1) ++result;
	return result;
} // log2_uint64

uint64_t sqrt_uint64(uint64_t value) {
	// Babylonian method.
	switch(value) {
		case UINT64_C(0): return UINT64_C(0);
		case UINT64_C(1): return UINT64_C(1);
		case UINT64_C(2): return UINT64_C(1);
		case UINT64_C(3): return UINT64_C(1);
		case UINT64_C(4): return UINT64_C(2);
	} // end switch
	uint8_t bits = 1 + log2_uint64(value);
	uint64_t root = (value >> (bits >> 1)), root_prev = root;
	while (1) {
		uint64_t root_new = (root + (value / root)) >> 1;
		if ((root_new == root) || (root_new == root_prev)) {
			if (root_new < root) root = root_new;
			break;
		} // end if
		root_prev = root; root = root_new;
	} // end while
	return root;
} // end function sqrt_uint64


void measure_clock_precision(
	const struct clock_engine_conf* engine_conf,
	const struct clock_benchmark_conf* benchmark_conf,
	struct clock_precision_stats* result
) {

	const clock_precision_querier
		resolution_querier = engine_conf->resolution_querier,
		precision_measurer = engine_conf->precision_measurer;
	const void* const engine_args = engine_conf->args;

	result->resolution = resolution_querier(engine_args);
	if (!result->resolution) return;

	size_t sample_count =
		benchmark_conf->batch_count * benchmark_conf->trial_count;

	nanoseconds_t* samples = benchmark_conf->samples;
	if (!samples)
		samples = malloc(sample_count * sizeof(nanoseconds_t));

	size_t index = 0;
	size_t batch = benchmark_conf->batch_count;
	while (batch--) {
		if (benchmark_conf->pause)
			usleep(rand() % benchmark_conf->pause);
		size_t trial = benchmark_conf->trial_count;
		while (trial--) {
			samples[index++] = precision_measurer(engine_args);
		} // end while
	} // end while

	qsort(samples,
		sample_count, sizeof(nanoseconds_t), compare_nanoseconds);

	size_t all_minimum_index = 0;
	size_t all_maximum_index = sample_count - 1;
	for (index = 0; index <= all_maximum_index; ++index) {
		nanoseconds_t sample = samples[index];
		if (!sample) { ++all_minimum_index; continue; }
	} // end for

	if (all_minimum_index > all_maximum_index) {
		result->all_minimum = 0;
		result->all_average = 0;
		result->sel_minimum = 0;
		result->sel_median = 0;
		result->sel_average = 0;
		result->sel_maximum = 0;
		result->sel_deviation = 0;
		goto quit;
	} // end if

	size_t all_count = all_maximum_index - all_minimum_index + 1;

	size_t sel_margin =
		(all_count * benchmark_conf->sel_margin) / 100;
	size_t sel_minimum_index = all_minimum_index + sel_margin;
	size_t sel_maximum_index = all_maximum_index - sel_margin;
	size_t sel_median_index = (sel_minimum_index + sel_maximum_index) >> 1;
	size_t sel_count = sel_maximum_index - sel_minimum_index + 1;


	result->all_minimum = samples[all_minimum_index];
	result->sel_minimum = samples[sel_minimum_index];
	result->sel_median = samples[sel_median_index];
	result->sel_maximum = samples[sel_maximum_index];

	if (result->sel_minimum == result->sel_maximum) {
		result->sel_average = result->sel_minimum;
		result->sel_deviation = 0;
		uint64_t sample_sum = 0;
		for (index = all_minimum_index; index <= all_maximum_index; ++index) {
			sample_sum += samples[index];
		} // end for
		result->all_average = sample_sum / all_count;
	} else {
		uint64_t sample_sum = 0;
		for (index = sel_minimum_index; index <= sel_maximum_index; ++index) {
			sample_sum += samples[index];
		} // end for
		nanoseconds_t average_sel = sample_sum / sel_count;
		for (index = all_minimum_index; index <= sel_minimum_index; ++index) {
			sample_sum += samples[index];
		} // end for
		for (index = sel_maximum_index; index <= all_maximum_index; ++index) {
			sample_sum += samples[index];
		} // end for
		uint64_t difference_sum = 0;
		for (index = sel_minimum_index; index <= sel_maximum_index; ++index) {
			nanoseconds_t sample = samples[index];
			nanoseconds_t diff = ((sample > average_sel) ?
				(sample - average_sel) : (average_sel - sample));
			difference_sum += ((uint64_t)diff) * diff;
		} // end for
		result->all_average = sample_sum / all_count;
		result->sel_average = average_sel;
		result->sel_deviation = sqrt_uint64(difference_sum / sel_count);
	} // end if

	// This quirky adjustment is required on some systems that report
	// the resolution as reciprocal number (ticks per second).
	if (result->resolution > result->sel_minimum)
		result->resolution = SECONDS_TO_NANOSECONDS / result->resolution;

quit:
	if (!benchmark_conf->samples) free(samples);
} // end function measure_clock_precision

void print_nanoseconds(nanoseconds_t value, int allow_zero) {
	if (value) {
		if (value <= NANOSECONDS_C(9999)) {
			printf("%4" PRIuNANOSECONDS " ns", value);
		} else if (value <= NANOSECONDS_C(9999999)) {
			printf("%4" PRIuNANOSECONDS " us",
				value / MICROSECONDS_TO_NANOSECONDS);
		} else {
			printf("%4" PRIuNANOSECONDS " ms",
				value / MILLISECONDS_TO_NANOSECONDS);
		} // end if
		printf(" (2**-%" PRIu8 ")",
			log2_uint32(SECONDS_TO_NANOSECONDS / value));
	} else if (allow_zero) {
		printf("   0 ns (2**-oo)");
	} else {
		printf("undetermined");
	} // end if
} // end function print_nanoseconds

void print_clock_precision(
	const struct clock_precision_stats* result
) {
	printf("\n\tresolution    : "); print_nanoseconds(result->resolution, 0);
	printf("\n\tall_minimum   : "); print_nanoseconds(result->all_minimum, 0);
	printf("\n\tall_average   : "); print_nanoseconds(result->all_average, 0);
	printf("\n\tsel_minimum   : "); print_nanoseconds(result->sel_minimum, 0);
	printf("\n\tsel_average   : "); print_nanoseconds(result->sel_average, 0);
	printf("\n\tsel_median    : "); print_nanoseconds(result->sel_median, 0);
	printf("\n\tsel_maximum   : "); print_nanoseconds(result->sel_maximum, 0);
	printf("\n\tsel_deviation : "); print_nanoseconds(result->sel_deviation, 1);
	printf("\n");
} // end function print_clock_precision

void print_benchmark_banner(
	const struct clock_benchmark_conf* benchmark_conf
) {
	printf("\nBenchmarking in %zu batches of %zu measurements each,\n"
		"which gives %zu sampling opportunities in total.\n",
		benchmark_conf->batch_count, benchmark_conf->trial_count,
		benchmark_conf->batch_count * benchmark_conf->trial_count);
	printf("Pausing for up to %" PRIuMICROSECONDS
		" microseconds before each batch.\n",
		benchmark_conf->pause);
	printf("Selection margin is %zu %% of best and worst samples.\n\n",
		benchmark_conf->sel_margin);
} // end function print_benchmark_banner

void benchmark_clock_engine(
	const struct clock_engine_conf* engine_conf,
	const struct clock_benchmark_conf* benchmark_conf
) {
	printf("Benchmarking %s...", engine_conf->name); fflush(stdout);
	struct clock_precision_stats result;
	measure_clock_precision(
		engine_conf, benchmark_conf, &result);
	print_clock_precision(&result);
} // end function benchmark_clock_engine

void benchmark_single_clock(
	const struct clock_engine_conf* engine,
	const struct clock_benchmark_conf* benchmark_conf
) {
	print_benchmark_banner(benchmark_conf);
	benchmark_clock_engine(engine, benchmark_conf);
	printf("\n");
} // end function benchmark_single_clock

void benchmark_all_clocks(
	size_t engine_count,
	const struct clock_engine_conf* engines,
	const struct clock_benchmark_conf* benchmark_conf
) {
	print_benchmark_banner(benchmark_conf);
	size_t index;
	for (index = 0; index < engine_count; ++index) {
		benchmark_clock_engine(engines + index, benchmark_conf);
	} // end for
	printf("\n");
} // end function benchmark_all_clocks


#ifdef ENGINE_CLOCK_GETTIME_ANY

nanoseconds_t query_clock_gettime(const void* engine_args) {
	clockid_t engine_type = *(const clockid_t*)engine_args;
	struct timespec ts_resolution;
	if (clock_getres(engine_type, &ts_resolution) != 0) return 0;
	return (nanoseconds_t)(ts_resolution.tv_nsec);
} // end function query_clock_gettime

nanoseconds_t measure_clock_gettime(const void* engine_args) {
	clockid_t engine_type = *(const clockid_t*)engine_args;
	struct timespec ts_start, ts_stop;
	clock_gettime(engine_type, &ts_start);
	clock_gettime(engine_type, &ts_stop);
	if (ts_stop.tv_sec != ts_start.tv_sec) return 0;
	if (ts_stop.tv_nsec <= ts_start.tv_nsec) return 0;
	return (nanoseconds_t)(ts_stop.tv_nsec - ts_start.tv_nsec);
} // end function measure_clock_gettime

#endif


#ifdef ENGINE_GETTIMEOFDAY

nanoseconds_t query_gettimeofday(const void* engine_args) {
	return MICROSECONDS_TO_NANOSECONDS;
} // end function query_gettimeofday

nanoseconds_t measure_gettimeofday(const void* engine_args) {
	struct timeval tv_start, tv_stop;
	gettimeofday(&tv_start, NULL);
	gettimeofday(&tv_stop, NULL);
	if (tv_stop.tv_sec != tv_start.tv_sec) return 0;
	if (tv_stop.tv_usec <= tv_start.tv_usec) return 0;
	return ((nanoseconds_t)(tv_stop.tv_usec - tv_start.tv_usec))
		* MICROSECONDS_TO_NANOSECONDS;
} // end function measure_gettimeofday

#endif


#ifdef ENGINE_CLOCK_CPUTIME

nanoseconds_t query_clock(const void* engine_args) {
	return SECONDS_TO_NANOSECONDS / CLOCKS_PER_SEC;
} // end function query_clock

nanoseconds_t measure_clock(const void* engine_args) {
	clock_t c_start = clock();
	clock_t c_stop = clock();
	if (c_stop <= c_start) return 0;
	return ((nanoseconds_t)(c_stop - c_start))
		* (SECONDS_TO_NANOSECONDS / CLOCKS_PER_SEC);
} // end function measure_clock

#endif


void configure_clock_engines(
	size_t* engine_count,
	struct clock_engine_conf** engines
) {

	size_t count = 0;

	#ifdef ENGINE_CLOCK_GETTIME_REALTIME
	++count;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_MONOTONIC
	++count;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_PROCESS_CPUTIME_ID
	++count;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_THREAD_CPUTIME_ID
	++count;
	#endif

	#ifdef ENGINE_GETTIMEOFDAY
	++count;
	#endif

	#ifdef ENGINE_CLOCK_CPUTIME
	++count;
	#endif

	*engine_count = count;
	if (!count) { engines = NULL; return; }

	*engines = malloc(count * sizeof(struct clock_engine_conf));
	if (!*engines) return;

	size_t index = 0;
	struct clock_engine_conf* engine;

	#ifdef ENGINE_CLOCK_GETTIME_REALTIME
	engine = *engines + index++;
	engine->name = "clock_gettime(CLOCK_REALTIME)";
	static const clockid_t arg_clock_gettime_REALTIME = CLOCK_REALTIME;
	engine->args = &arg_clock_gettime_REALTIME;
	engine->resolution_querier = query_clock_gettime;
	engine->precision_measurer = measure_clock_gettime;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_MONOTONIC
	engine = *engines + index++;
	engine->name = "clock_gettime(CLOCK_MONOTONIC)";
	static const clockid_t arg_clock_gettime_MONOTONIC = CLOCK_MONOTONIC;
	engine->args = &arg_clock_gettime_MONOTONIC;
	engine->resolution_querier = query_clock_gettime;
	engine->precision_measurer = measure_clock_gettime;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_PROCESS_CPUTIME_ID
	engine = *engines + index++;
	engine->name = "clock_gettime(CLOCK_PROCESS_CPUTIME_ID)";
	static const clockid_t arg_clock_gettime_PROCESS_CPUTIME_ID =
		CLOCK_PROCESS_CPUTIME_ID;
	engine->args = &arg_clock_gettime_PROCESS_CPUTIME_ID;
	engine->resolution_querier = query_clock_gettime;
	engine->precision_measurer = measure_clock_gettime;
	#endif

	#ifdef ENGINE_CLOCK_GETTIME_THREAD_CPUTIME_ID
	engine = *engines + index++;
	engine->name = "clock_gettime(CLOCK_THREAD_CPUTIME_ID)";
	static const clockid_t arg_clock_gettime_THREAD_CPUTIME_ID =
		CLOCK_THREAD_CPUTIME_ID;
	engine->args = &arg_clock_gettime_THREAD_CPUTIME_ID;
	engine->resolution_querier = query_clock_gettime;
	engine->precision_measurer = measure_clock_gettime;
	#endif

	#ifdef ENGINE_GETTIMEOFDAY
	engine = *engines + index++;
	engine->name = "gettimeofday()";
	engine->args = NULL;
	engine->resolution_querier = query_gettimeofday;
	engine->precision_measurer = measure_gettimeofday;
	#endif

	#ifdef ENGINE_CLOCK_CPUTIME
	engine = *engines + index++;
	engine->name = "clock()";
	engine->args = NULL;
	engine->resolution_querier = query_clock;
	engine->precision_measurer = measure_clock;
	#endif

} // end function configure_clock_engines

void release_clock_engines(
	size_t engine_count,
	struct clock_engine_conf* engines
) {
	if ((engine_count) && (engines)) free(engines);
} // end function release_clock_engines


#define BATCH_COUNT_MIN UINT16_C(1)
#define BATCH_COUNT_MAX UINT16_C(16384)
#define BATCH_COUNT_DEF UINT16_C(512)

#define TRIAL_COUNT_MIN UINT16_C(8)
#define TRIAL_COUNT_MAX UINT16_C(49152)
#define TRIAL_COUNT_DEF UINT16_C(8192)

#define BATCH_PAUSE_MIN MICROSECONDS_C(0)
#define BATCH_PAUSE_MAX MICROSECONDS_C(50000)
#define BATCH_PAUSE_DEF MICROSECONDS_C(7500)

#define SELECTION_MARGIN_MIN UINT8_C(1)	// Ignored worst and best, per cent.
#define SELECTION_MARGIN_MAX UINT8_C(25)
#define SELECTION_MARGIN_DEF UINT8_C(5)

void print_help(const char* self_path) {
	if (self_path && strlen(self_path)) {
		const char* slash_pos = strrchr(self_path, '/');
		if (slash_pos && (++slash_pos != 0)) self_path = slash_pos;
	} else {
		self_path = "clockprecision";
	} // end if
	printf("\nClock precision measurement utility.\n");
	printf("\nSyntax:\n\t%s [options]\n\n", self_path);
	printf("Options:\n\n");
	printf("-?, --help\n\tPrint this help.\n\n");
	printf("-l, --list\n\tList all available (compiled) clock engines.\n\n");
	printf("-a, --all\n\tBenchmark all clock engines.\n\n");
	printf("-e, --engine {index}\n"
		"\tBenchmark particular clock engine specified by its index.\n"
		"\tRun with -l or --list to view all available engines.\n\n");
	printf("-b, --batches {count} \t| range: %zu to %zu, default: %zu\n"
		"\tSet number of batches for benchmarking.\n\n",
		BATCH_COUNT_MIN, BATCH_COUNT_MAX, BATCH_COUNT_DEF);
	printf("-t, --trials {count} \t| range: %zu to %zu, default: %zu\n"
		"\tSet number of trials in each benchmarking batch.\n\n",
		TRIAL_COUNT_MIN, TRIAL_COUNT_MAX, TRIAL_COUNT_DEF);
	printf("-p, --pause {duration} \t| range: %" PRIuMICROSECONDS
		" to %" PRIuMICROSECONDS ", default: %" PRIuMICROSECONDS "\n"
		"\tSet maximum sleep time before each batch, in microseconds.\n"
		"\tActual duration will be randomized on every ocassion.\n\n",
		BATCH_PAUSE_MIN, BATCH_PAUSE_MAX, BATCH_PAUSE_DEF);
	printf("-m, --margin {portion} \t| range: %zu to %zu, default: %zu\n"
		"\tSet selection margin for statistical analysis, per cent.\n"
		"\tBoth best and worst samples of that amount will be excluded.\n\n",
		SELECTION_MARGIN_MIN, SELECTION_MARGIN_MAX, SELECTION_MARGIN_DEF);
} // end function print_help

void print_version() {
	printf("This program has no exact version.\n");
	printf("Is was compiled on " __DATE__ " " __TIME__ ".\n");
} // end function print_help

void print_clock_engines(
	size_t engine_count,
	const struct clock_engine_conf* engines
) {
	printf("Available clock engines:\n");
	size_t index;
	for (index = 0; index < engine_count; ++index) {
		printf("%2d. %s\n", (index + 1), (engines + index)->name);
	} // end for
	printf("\n");
} // end function benchmark_all_clocks


enum work_mode_type {
	work_mode_syntax_error,
	work_mode_config_error,
	work_mode_help,
	work_mode_version,
	work_mode_list_engines,
	work_mode_benchmark_all_clocks,
	work_mode_benchmark_single_clock,
	work_mode_benchmark_interactive
};

enum work_result_type {
	work_result_success = 0,
	work_result_syntax_error = 1,
	work_result_config_error = 2,
	work_result_failure = 3
};

int main(int argc, char **argv) {

	size_t engine_count;
	struct clock_engine_conf* engines;
	configure_clock_engines(&engine_count, &engines);
	if (!engine_count) {
		fprintf(stderr, "Error: no clock engines available.\n");
		return work_result_failure;
	} else if (!engines) {
		fprintf(stderr, "Error: failed to configure clock engines.\n");
		return work_result_failure;
	} // end if

	struct clock_benchmark_conf benchmark_conf;
	benchmark_conf.batch_count = BATCH_COUNT_DEF;
	benchmark_conf.trial_count = TRIAL_COUNT_DEF;
	benchmark_conf.pause = BATCH_PAUSE_DEF;
	benchmark_conf.samples = NULL;
	benchmark_conf.sel_margin = SELECTION_MARGIN_DEF;

	enum work_mode_type work_mode = work_mode_benchmark_all_clocks;
	enum work_result_type work_result = work_result_success;
	const char* error_message = NULL;

	size_t engine_index;

	size_t argi = 1;
	while (argi < argc) {
		const char* argt = argv[argi++];
		if ((strcmp(argt, "-?") == 0)
				|| (strcmp(argt, "--help") == 0)) {
			work_mode = work_mode_help;
			break;
		} else if ((strcmp(argt, "-V") == 0)
				|| (strcmp(argt, "--version") == 0)) {
			work_mode = work_mode_version;
			break;
		} else if ((strcmp(argt, "-l") == 0)
				|| (strcmp(argt, "--list") == 0)) {
			work_mode = work_mode_list_engines;
			break;
		} else if ((strcmp(argt, "-a") == 0)
				|| (strcmp(argt, "--all") == 0)) {
			work_mode = work_mode_benchmark_all_clocks;
		} else if ((strcmp(argt, "-e") == 0)
				|| (strcmp(argt, "--engine") == 0)) {
			if (argi >= argc) {
				work_mode = work_mode_syntax_error;
				error_message = "No engine index specified.";
				break;
			} // end if
			if (sscanf(argv[argi++], "%zu", &engine_index) <= 0) {
				work_mode = work_mode_syntax_error;
				error_message = "Malformed engine index specified.";
				break;
			} // end if
			if ((engine_index < 1u) || (engine_index > engine_count)) {
				work_mode = work_mode_config_error;
				error_message = "Incorrect engine index specified.";
				break;
			} // end if
			work_mode = work_mode_benchmark_single_clock;
			--engine_index;
		} else if ((strcmp(argt, "-b") == 0)
				|| (strcmp(argt, "--batches") == 0)) {
			if (argi >= argc) {
				work_mode = work_mode_syntax_error;
				error_message = "No batch count specified.";
				break;
			} // end if
			if (sscanf(argv[argi++], "%zu", &benchmark_conf.batch_count) <= 0) {
				work_mode = work_mode_syntax_error;
				error_message = "Malformed batch count specified.";
				break;
			} // end if
			if ((benchmark_conf.batch_count < BATCH_COUNT_MIN)
					|| (benchmark_conf.batch_count > BATCH_COUNT_MAX)) {
				work_mode = work_mode_config_error;
				error_message = "Specified batch count is out of range.";
				break;
			} // end if
		} else if ((strcmp(argt, "-t") == 0)
				|| (strcmp(argt, "--trials") == 0)) {
			if (argi >= argc) {
				work_mode = work_mode_syntax_error;
				error_message = "No trial count specified.";
				break;
			} // end if
			if (sscanf(argv[argi++], "%zu", &benchmark_conf.trial_count) <= 0) {
				work_mode = work_mode_syntax_error;
				error_message = "Malformed trial count specified.";
				break;
			} // end if
			if ((benchmark_conf.trial_count < TRIAL_COUNT_MIN)
					|| (benchmark_conf.trial_count > TRIAL_COUNT_MAX)) {
				work_mode = work_mode_config_error;
				error_message = "Specified trial count is out of range.";
				break;
			} // end if
		} else if ((strcmp(argt, "-p") == 0)
				|| (strcmp(argt, "--pause") == 0)) {
			if (argi >= argc) {
				work_mode = work_mode_syntax_error;
				error_message = "No pause duration specified.";
				break;
			} // end if
			if (sscanf(argv[argi++], "%" PRIuMICROSECONDS,
					&benchmark_conf.pause) <= 0) {
				work_mode = work_mode_syntax_error;
				error_message = "Malformed pause duration specified.";
				break;
			} // end if
			if ((benchmark_conf.pause < BATCH_PAUSE_MIN)
					|| (benchmark_conf.pause > BATCH_PAUSE_MAX)) {
				work_mode = work_mode_config_error;
				error_message = "Specified pause duration is out of range.";
				break;
			} // end if
		} else if ((strcmp(argt, "-m") == 0)
				|| (strcmp(argt, "--margin") == 0)) {
			if (argi >= argc) {
				work_mode = work_mode_syntax_error;
				error_message = "No selection margin specified.";
				break;
			} // end if
			if (sscanf(argv[argi++], "%zu", &benchmark_conf.sel_margin) <= 0) {
				work_mode = work_mode_syntax_error;
				error_message = "Malformed selection margin specified.";
				break;
			} // end if
			if ((benchmark_conf.sel_margin < SELECTION_MARGIN_MIN)
					|| (benchmark_conf.sel_margin > SELECTION_MARGIN_MAX)) {
				work_mode = work_mode_config_error;
				error_message = "Specified selection margin is out of range.";
				break;
			} // end if
		} else {
			work_mode = work_mode_syntax_error;
			error_message = "Unrecognized option.";
			break;
		} // end if
	} // end while

	switch (work_mode) {
		case work_mode_benchmark_all_clocks:
		case work_mode_benchmark_single_clock:
			benchmark_conf.samples = malloc(sizeof(nanoseconds_t)
				* (benchmark_conf.batch_count * benchmark_conf.trial_count));
			if (!benchmark_conf.samples) {
				work_mode = work_mode_config_error;
				error_message = "Failed to allocate memory for samples.";
			} // end if
			break;
	} // end switch

	switch (work_mode) {
		case work_mode_syntax_error:
			work_result = work_result_syntax_error;
			fprintf(stderr, "Syntax error");
			if (error_message) fprintf(stderr, ": %s", error_message);
			fprintf(stderr, "\nRun with -? or --help to view options.");
			break;
		case work_mode_config_error:
			work_result = work_result_config_error;
			fprintf(stderr, "Configuration error");
			if (error_message) fprintf(stderr, ": %s", error_message);
			fprintf(stderr, "\n");
			break;
		case work_mode_help:
			print_help(argc ? argv[0] : NULL);
			break;
		case work_mode_version:
			print_version();
			break;
		case work_mode_list_engines:
			print_clock_engines(engine_count, engines);
			break;
		case work_mode_benchmark_all_clocks:
			benchmark_all_clocks(engine_count, engines, &benchmark_conf);
			break;
		case work_mode_benchmark_single_clock:
			benchmark_single_clock(engines + engine_index, &benchmark_conf);
			break;
	} // end switch

	if (benchmark_conf.samples) free(benchmark_conf.samples);
	release_clock_engines(engine_count, engines);

	return work_result;
} // end function main
