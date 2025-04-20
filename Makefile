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
TEST=test
TEST_PROG=test_cutils

all: $(LIB)/libcutils.a

$(LIB)/libcutils.a: $(OBJECTS)
	ar -rv $@ $^

$(BUILD)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(LIB)/libcutils.a
	$(MAKE) -C $(TEST) test

runtest: test
	$(TEST)/$(TEST_PROG)

.PHONY: clean
clean:
	rm -f $(BUILD)/*.o
	rm -f $(LIB)/*.a
	rm -f $(TEST)/*.o
	rm -f $(TEST)/$(TEST_PROG)
