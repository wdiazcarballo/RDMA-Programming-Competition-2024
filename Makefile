CC = gcc
CFLAGS = -I/global/home/users/rdmaworkshop08/local/ucc/src/ucc
LDFLAGS = -lucc -lucp -lucs
TARGET = allreduce

all: $(TARGET)

$(TARGET): allreduce.o
	$(CC) -o $(TARGET) allreduce.o $(LDFLAGS)

allreduce.o: allreduce.c
	$(CC) $(CFLAGS) -c allreduce.c

clean:
	rm -f $(TARGET) *.o
