CC = gcc
SRCS = extio.c get_interval.c main.c
OBJS = $(SRCS:%.c=%.o) # intermediate object files

EXEC = get_interval # name of the final executable

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) -c $<

clean:
	rm -rvf *.o
