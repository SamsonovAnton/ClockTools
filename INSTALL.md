
# Clock tools

A set of utilities to evaluate timestamping sources in POSIX systems.
Refer to [README.md](README.md) for usage description.



## System requirements

Operating systems and compilers known to be compatible:

* GNU/Linux based systems with kernel 2.4+ and GCC 3.3+;
* Cygwin 1.7 with GCC 3.4+;
* FreeBSD 10+ with clang 3.3+.

Other POSIX systems are likely to be compatible.



## Building and installing

All tools are implemented as single source file programs. Therefore,
compiling them is pretty straightforward, such as:

	cc -o clockprobe clockprobe.c

For convenience, a Makefile is provided that supports common actions:

	make
	make clean
	make install
	make -j2 CFLAGS='-O3 -g0' prefix='/opt/clocktools' install

The last one is an advanced usage example that compiles in 2 threads
with maximum optimization and minumum debugging, and then installs
the tools to an alternative directory tree.

For out-of-tree builds, system-specific additions are provided
that are only compatible with GNU and BSD flavors of *make*:

	cd ...../build
	make -f ../source/GNUmakefile
		# or
	make -f ../source/BSDmakefile

Other systems may require their own non-portable additions.
