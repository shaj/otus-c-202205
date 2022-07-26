all: prog fact

prog: prog.c
	$(CC) $(CFLAGS) -Wall -Wextra -Wpedantic -std=c11 -g -O0 $^ -o $@

fact: fact.c
	$(CC) $(CFLAGS) -Wall -Wextra -Wpedantic -std=c11 -g -O0 $^ -o $@

clean:
	$(RM) prog fact core* *.o

.PHONY: all clean
