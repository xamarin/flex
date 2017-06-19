FLEX_DIR = ../../..
CS_FILES = ../base.cs ../flex.cs test.cs test_*.cs

all: run

../base.cs:        $(FLEX_DIR)/flex.h
	(cd .. && /usr/bin/ruby tools/generate_base.rb)

libflex.dylib:  $(FLEX_DIR)/flex.c $(FLEX_DIR)/flex.h
	/usr/bin/clang -I$(FLEX_DIR) $(FLEX_DIR)/flex.c -O3 -dynamiclib -undefined dynamic_lookup -o libflex.dylib -arch x86_64 -arch i386

test.exe:       $(CS_FILES)
	mcs $(CS_FILES) -out:test.exe -d:DEBUG -debug

run:   test.exe libflex.dylib
	mono --debug test.exe

clean:
	/bin/rm -f ../base.cs libflex.dylib test.exe
