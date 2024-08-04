CC = gcc
CFLAGS = -I/global/home/users/rdmaworkshop08/wdc/RDMA-Programming-Competition-2024 -I/global/home/users/rdmaworkshop08/local/ucc/src -I/global/home/users/rdmaworkshop08/local/ucc/src/ucc -I/global/home/users/rdmaworkshop08/local/ucc/src/components/tl/ucp
LDFLAGS = -lucc -lucp -lucs
TARGET = allreduce

all: $(TARGET)

$(TARGET): allreduce.o
	$(CC) -o $(TARGET) allreduce.o $(LDFLAGS)

allreduce.o: allreduce.c
	$(CC) $(CFLAGS) -c allreduce.c

clean:
	rm -f $(TARGET) *.o
