CC = gcc
CFLAGS = -g -pg
LDFLAGS = -llzo2

OBJ = data.o pool.o part.o map.o

TESTS = test_compress test_server test_client

all: $(OBJ) $(TESTS)

tests: $(TESTS) $(OBJ)

test_compress: $(OBJ) test_compress.o
	$(CC) -w $(CFLAGS) -o test_compress test_compress.o $(OBJ) $(LDFLAGS) 
test_server: $(OBJ) test_server.o
	$(CC) -w $(CFLAGS) -o test_server test_server.o $(OBJ) $(LDFLAGS) 
test_client: $(OBJ) test_client.o
	$(CC) -w $(CFLAGS) -o test_client test_client.o $(OBJ) $(LDFLAGS) 

%.o: %.c
	$(CC) -w $(CFLAGS) -c $<

clean:
	rm -rf *.o
