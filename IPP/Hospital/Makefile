CC=gcc
OPT=-c

.PHONY:clean debug

hospital: hospital.o parse.o structure.o
	$(CC) -o $@ $^

%.o: %.c %.h
	$(CC) -o $@ $(OPT) $<

%.dbg.o: %.c %.h
	$(CC) -g -o $@ $(OPT) $<

hospital.dbg.o: hospital.c
	$(CC) -g -o $@ $(OPT) $<

clean:
	rm -f *.o hospital hospital.dbg

debug: hospital.dbg.o parse.dbg.o structure.dbg.o
	$(CC) -o hospital.dbg $^
