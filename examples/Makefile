CC=gcc
CFLAGS=-I.

ODIR=out

DEPS = ../src/cli.h ../src/cli_defs.h


_OBJ = main.o cli.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cli: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o