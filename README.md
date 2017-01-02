
# Clock tools

A set of utilities to evaluate timestamping sources in POSIX systems:

* clockprobe -- prints all numeric ids available via `clock_gettime()`;
* clockprecision -- measures statistical properties of time sources;
* clockleaping -- records timestamps during UTC leap second events.



## clockprobe

Successively calls `clock_gettime()` with all clock ids from 0 to 254
in order to see which of them are actually supported by the operating
system and marked as valid at the time of probing.

This tool is helpful for checking whether an OS in fact supports:

* all the clock types defined in <time.h>;
* additional clock types not defined in <time.h>.

For example, an OS may respond to CLOCK_TAI even if that was not
defined in the supplied header files, or such headers are missing.
On the other hand, a particular clock type may be in fact absent
when running on specific platform with unsuitable hardware.

This tool, however, is *not* able to guess the meaning of any numeric
code it encounters, as various OSes have different clock identifiers
and even different symbolic names for the same or similar semantics.



## clockprecision

Gathers statistics on performance of various time sources:

* `clock_gettime()` with relevant clock types;
* `gettimeofday()`;
* `clock()`.

Reported properties are:

* resolution -- nominal clock scale resolution;
* all_minimum -- actual timestamp precision;
* all_average -- average precision among all valid samples;
* sel_minimum -- practical timestamp precision (95th percentile);
* sel_average -- practical average;
* sel_median -- practical median;
* sel_maximum -- practical maximum;
* sel_deviation -- practical deviation.

Note on terminology:

*	clock **resolution** is the granularity level of timestamp scale
	implemented by clock driver, that is how fine or coarse the scale
	is quantized theoretically;
*	clock **precision** is the granularity level of particular hardware,
	i. e. minimal non-zero difference between successive readings;
*	clock **accuracy** is the amount of difference between particular
	clock readings and some reference clock, at some point in time.

For example, with a nominal resolution of 1 nanosecond implemented
by an operating system, some clock source on particular hardware
may change its readings only once in 100 microseconds, and that is
its precision. However, its accuracy may in fact be no better than
10 milliseconds when the clock is synchronized over the Internet.

Command line syntax:

	clockprecision [options]

Options:

*	-?, --help      Print help page.
*	-l, --list      List available clock engines.
*	-a, --all       Benchmark all available clock engines.

Refer to the help page for clock selection and tuning options.



## clockleaping

Records `clock_gettime()` readings during UTC leap second events.

1.	The tool is supposed to be run on the day the leap event occurs.
	It automatically determines the local time which corresponds
	to UTC midnight, based on timezone setup, which must be correct.
2.	When started, the tool will detect relevant clock types that are
	available on host, and also filter out the duplicates, if any.
	The types selected for recording will be printed out, as well as
	the current time according to UTC for the user to check out.
3.	As the time goes by and approaches UTC midnight, the tool will
	mostly sleep with decreasing intervals (from 1 hour to 1 second),
	printing out the countdown.
4.	The recording will start 5 seconds before UTC midnight and will
	be carried out for about 10 seconds, making no pauses at all,
	so the only limiting factors will be `printf()` and stdout being
	redirected to a file. Therefore, depending on CPU and disk speed,
	the output may grow very large, so be sure to have free space.

During recording, timestamps will be output separated by tabs, with
each line representing a batch started by realtime wallclock reading
and followed by other clock types discovered. Although this format
is a natural choice for spreadsheet processing, beware of file size,
as it may take much time to import.
