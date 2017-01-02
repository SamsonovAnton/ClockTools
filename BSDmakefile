
MAKEFILE_DIR:=$(:!dirname "$(:!realpath "$(.MAKE.MAKEFILES:[-1])"!)"!)
.SUFFIXES: .c
.PATH.c: $(MAKEFILE_DIR)

.sinclude "Makefile"
