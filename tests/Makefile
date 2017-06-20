FLEX_DIR = ..
CFLAGS = -Wall -Werror -O3 -I$(FLEX_DIR)

TESTS_SRC = test_default_values.c test_children.c test_managed_ptr.c test_basis.c test_grow.c test_shrink.c test_direction.c test_order.c test_align_self.c test_align_items.c test_wrap.c test_justify_content.c test_align_content.c test_position.c test_margin.c test_padding.c

all: run

units.c:        $(TESTS_SRC)
	ruby generate_units.rb $(TESTS_SRC)

run:    test
	@./test

test:   test.c units.c
	$(CC) $(CFLAGS) test.c $(FLEX_DIR)/flex.c -o test

clean:
	/bin/rm -f test units.c
