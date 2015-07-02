CC=gcc-5
CFLAGS=-I../mongo-c-driver/src/mongoc -I../mongo-c-driver/src/libbson/src/bson

EXECUTABLES=\
						double-load-data\
						fetch-data

all: double-load-data fetch-data

double-load-data: double-load-data.o
	../mongo-c-driver/libtool --mode=link $(CC) $(CFLAGS) -o $@ ../mongo-c-driver/libmongoc-1.0.la $^

fetch-data: fetch-data.o
	../mongo-c-driver/libtool --mode=link $(CC) $(CFLAGS) -o $@ ../mongo-c-driver/libmongoc-1.0.la $^

clean:
	rm -r *.o .libs
	rm $(EXECUTABLES)
