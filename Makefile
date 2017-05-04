LIBRARY = libflex.a
OBJS = flex.o

CC = /usr/bin/clang
CFLAGS = -Wall -Werror

ifeq ($(MAKECMDGOALS),debug)
  CFLAGS += -DDEBUG -O0 -g
else
  CFLAGS += -O3
endif
debug: all

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	/bin/rm -f $(LIBRARY)
	/usr/bin/ar -cq $(LIBRARY) $(OBJS)

test: t
	@./t

t: $(OBJS) test.c
	$(CC) $(CFLAGS) test.c -o t $(OBJS)

%.o : %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	/bin/rm -f $(OBJS) $(LIBRARY) t
