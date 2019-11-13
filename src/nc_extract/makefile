CC = gcc
SRCS = extio.c nc_extract.c main.c
OBJS = $(SRCS:%.c=%.o)
LINKERFLAGS = -lnetcdf -lhdf5_hl -lhdf5 -lz -lm
EXEC = nc_extract

all: clean $(OBJS)
	$(CC) $(LINKERFLAGS) -o $(EXEC) $(OBJS)

%.o:%.c
	$(CC) -c $<

clean:
	rm -rvf *.o
