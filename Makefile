SRC=$(CURDIR)/src
INCLUDE=$(SRC)/include
CC?=gcc
CFLAGS=-Wall -Wextra -ggdb  -ffast-math
FILES=$(shell find $(SRC) -type f -name '*.c')

all: 
	@echo ">> Compiling <<"
	$(CC) $(CFLAGS)  -I$(INCLUDE) $(FILES) -o packetsniffer
	@echo ">> DONE <<"

clean:
	@rm -f packetsniffer
