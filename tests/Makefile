FLEX_DIR = ..

CC = /usr/bin/clang
CFLAGS = -Wall -Werror -O3 -I$(FLEX_DIR)

TESTS_SRC = test_default_values.c test_children.c test_managed_ptr.c test_basis.c test_grow.c test_shrink.c test_direction.c test_order.c test_align_self.c test_align_items.c test_wrap.c test_justify_content.c test_align_content.c test_position.c test_margin.c test_padding.c test_self_sizing.c
TESTS_OBJ = $(patsubst %.c,%.o,$(TESTS_SRC))
OBJS = $(TESTS_OBJ) units.o

all: run

units.c:        $(TESTS_SRC) generate_units.rb
	/usr/bin/ruby generate_units.rb $(TESTS_SRC)

run:    test
	@./test

test:   test.c test.h $(FLEX_DIR)/flex.c $(OBJS)
	$(CC) $(CFLAGS) -include test.h test.c $(FLEX_DIR)/flex.c -o test $(OBJS)

%.o : %.c test.h $(FLEX_DIR)/flex.h
	$(CC) $(CFLAGS) -include test.h -c $< -o $@

clean:
	/bin/rm -f test units.c $(OBJS)
