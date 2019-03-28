
CC = clang

AR = ar rcs
RM = -rm -rf

all : mysqlite

INCLUDE = ./src

CFLAGS = -Wall -O -fPIC
CFLAGS += -mmacosx-version-min=10.10

OBJS = $(patsubst %.c,%.o, $(wildcard src/sql*.c))

%.o:%.c
	$(CC) -c $(CFLAGS) -I$(INCLUDE) $< -o $@

mysqlite : $(OBJS)
	$(AR) libsqlite3.a $(OBJS)

clean:
	$(RM) src/*.o
