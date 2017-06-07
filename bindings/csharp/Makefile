FLEX_DIR = ../..

all: run

base.cs:        $(FLEX_DIR)/flex.h
	/usr/bin/ruby generate_base.rb

libflex.dylib:  $(FLEX_DIR)/flex.c $(FLEX_DIR)/flex.h
	/usr/bin/clang -I$(FLEX_DIR) $(FLEX_DIR)/flex.c -O3 -dynamiclib -undefined dynamic_lookup -o libflex.dylib -arch x86_64 -arch i386

test.exe:       base.cs flex.cs test.cs
	mcs base.cs flex.cs test.cs -out:test.exe -d:DEBUG -debug

run:   test.exe libflex.dylib
	mono --debug test.exe

clean:
	/bin/rm -f base.cs libflex.dylib test.exe

