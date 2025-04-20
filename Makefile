CC=gcc
BUILD=build
SRC=src
LIB=lib
OBJECTS = $(BUILD)/exceptions.o \
		  $(BUILD)/list.o \
		  $(BUILD)/map.o \
		  $(BUILD)/stack.o \
		  $(BUILD)/memory.o
INCLUDE=-I include -I include/cutils
CFLAGS=$(INCLUDE) -std=c11

all: $(LIB)/libcutils.a

$(LIB)/libcutils.a: $(OBJECTS)
	ar -rv $@ $^

$(BUILD)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f build/*.o
	rm -f lib/*.a