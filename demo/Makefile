CC=gcc-5
override CFLAGS+=-I../mongo-c-driver/src/mongoc -I../mongo-c-driver/src/libbson/src/bson

EXECUTABLES=\
						load-data\
						fetch-data

all: load-data fetch-data

load-data: load-data.o
	../mongo-c-driver/libtool --mode=link $(CC) $(CFLAGS) -o $@ ../mongo-c-driver/libmongoc-1.0.la $^

fetch-data: fetch-data.o
	../mongo-c-driver/libtool --mode=link $(CC) $(CFLAGS) -o $@ ../mongo-c-driver/libmongoc-1.0.la $^

clean:
	rm -r *.o .libs
	rm $(EXECUTABLES)
