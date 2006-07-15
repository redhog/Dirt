export CFLAGS:=-g
export LIBS:=
CC=gcc

MAJOR:=$(shell tla tree-version | sed -e "s+.*--.*--++g")
MINOR:=$(shell tla logs -r | head -1 | sed -e "s+.*-++g")


all: libDirt.so.$(MAJOR).$(MINOR) test

%.o: src/%.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

libDirt.so.$(MAJOR).$(MINOR): Writer.o StructReader.o Struct.o Reader.o WriteBuffer.o WriteBufferTypes.o Buffer.o BufferTypes.o Session.o
	$(CC) $(CFLAGS) $(LIBS) -fPIC -shared -Wl,-soname,libDirt.so.0.1 -o $@ $^

libDirt.so.$(MAJOR): libDirt.so.$(MAJOR).$(MINOR)
	ln -s $< $@

libDirt.so: libDirt.so.$(MAJOR)
	ln -s $< $@

.PHONY: clean
clean: test-clean
	-rm *.so.* *.so *.o *~ core* src/*~ Dirt/*~

.PHONY: test
test: libDirt.so
	make -C Test

.PHONY: test-clean
test-clean:
	make -C Test clean
