HOMELOCAL = /global/home/users/rdmaworkshop08/local
WORKING = /global/home/users/rdmaworkshop08/wdc/RDMA-Programming-Competition-2024
UCCSRC = $(HOMELOCAL)/ucc/src
CC = gcc
CFLAGS = -I$(WORKING) -I$(UCCSRC)/components/tl/ucp -I$(UCCSRC)/components/tl/ucp/allreduce
LDFLAGS = -lucc -lucp -lucs
TARGET = allreduce

all: $(TARGET)

$(TARGET): allreduce.o
	$(CC) -o $(TARGET) allreduce.o $(LDFLAGS)

allreduce.o: allreduce.c
	$(CC) $(CFLAGS) -c allreduce.c

clean:
	rm -f $(TARGET) *.o
