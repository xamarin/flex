CFLAGS = -Wall -Werror -O3
FLEX_DIR = ..

all: run

run:
	$(CC) $(CFLAGS) test.c -I$(FLEX_DIR) $(FLEX_DIR)/flex.c -o test
	@./test

clean:
	/bin/rm -f test
