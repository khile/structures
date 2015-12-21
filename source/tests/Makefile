PROG=list_tests

# add new sources here
SRCS=list_tests.c list.c

OBJS=${SRCS:.c=.o}

CLEAN=${OBJS} ${PROG}

CC=gcc

CFLAGS= -g -Wall -pthread

${PROG}: ${OBJS}
	${CC} ${CFLAGS} -o ${PROG} ${OBJS}

clean:
	rm -f ${CLEAN}

# dependency list
list.o: list.h list.c
