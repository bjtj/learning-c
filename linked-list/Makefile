TARGET=linked_list
OBJS=main.o list.o

all: ${TARGET}

${TARGET}: ${OBJS}
	gcc -Wall -o $@ ${OBJS}

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm -rf ${TARGET} ${OBJS}
