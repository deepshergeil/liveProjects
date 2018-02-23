warmup2: warmup2.o my402list.o
	gcc -o warmup2 -g -pthread warmup2.o my402list.o -lm -lnsl
warmup2.o: warmup2.c my402list.h defs.h
	gcc -g -c -Wall warmup2.c -D_POSIX_PTHREAD_SEMANTICS
my402list.o: my402list.c my402list.h defs.h
	gcc -g -c -Wall my402list.c
clean:
	rm -f *.o warmup2






