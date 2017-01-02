
prefix=/usr/local
exec_prefix=$(prefix)
bindir=$(exec_prefix)/bin

EXECUTABLES=clockprobe clockprecision clockleaping

all: $(EXECUTABLES)

clean:
	rm $(EXECUTABLES)

install: all
	install -m 0755 -d $(bindir)
	install -m 0755 $(EXECUTABLES) $(bindir)

clockleaping: clockleaping.c
	$(CC) $(CFLAGS) -o $@ $<

clockprecision: clockprecision.c
	$(CC) $(CFLAGS) -o $@ $<

clockprobe: clockprobe.c
	$(CC) $(CFLAGS) -o $@ $<
