PROGS := mm_proc
CFLAGS:= -O3 -Wall -Werror -Wextra
CC := gcc
TESTS := test500.txt test2.txt test32.txt

all : ${PROGS}

mm_% : mm_%.c mm_test.c mm_test.h
	${CC} mm_$*.c mm_test.c -o $@ -I . ${CFLAGS}


test: ${TESTS} ${PROGS}
	for x in ${TESTS}; do ./${PROGS} $$x > tmp ; diff -B tmp $$x ; done

mmf_proc : mm_proc.c mm_test.c mm_test.h
	${CC} -DDOUBLE_MM mm_proc.c mm_test.c -o $@ -I . ${CFLAGS}

clean :
	rm -f ${PROGS} *~