TEST_CFLAGS=$(CFLAGS) -I ../include -I ../include/cutils
TEST_OBJECTS=test_cutils.o test_cutils_list.o

%.o: %.c
	$(CC) -c $(TEST_CFLAGS) -o $@ $<

test: test_cutils

test_cutils: $(TEST_OBJECTS)
	$(CC) $(TEST_CFLAGS) -o $@ $(TEST_OBJECTS) -L ../lib -l:libcutils.a