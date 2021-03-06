export INSTALLDIR=/usr/local

export CFLAGS:=-g -Wall -pedantic-errors -std=gnu99
export LIBS:=
export CC:=gcc

MAJOR:=$(shell tla tree-version | sed -e "s+.*--.*--++g")
MINOR:=$(shell tla logs -r | head -1 | sed -e "s+.*-++g")


all: libDirt.so.$(MAJOR).$(MINOR) test

%.o: src/%.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

libDirt.so.$(MAJOR).$(MINOR): StructWriter.o Writer.o StructReader.o Struct.o Reader.o WriteBuffer.o WriteBufferTypes.o Buffer.o BufferTypes.o Session.o
	$(CC) $(CFLAGS) $(LIBS) -fPIC -shared -Wl,-soname,libDirt.so.0.1 -o $@ $^

libDirt.so.$(MAJOR): libDirt.so.$(MAJOR).$(MINOR)
	ln -s $< $@

libDirt.so: libDirt.so.$(MAJOR)
	ln -s $< $@

.PHONY: clean
clean: test-clean
	-rm -rf *.so.* *.so *.o *~ core* src/*~ Dirt/*~

.PHONY: test
test: libDirt.so
	make -C Test

.PHONY: test-clean
test-clean:
	make -C Test clean

.PHONY: install-libs
install-libs: libDirt.so libDirt.so.$(MAJOR) libDirt.so.$(MAJOR).$(MINOR)
	cp -d $^ $(INSTALLDIR)/lib

.PHONY: install-headers
install-headers: Dirt
	cp -dr $^ $(INSTALLDIR)/include

.PHONY: install
install: install-libs install-headers


.PHONY: uninstall
uninstall:
	rm -rf $(INSTALLDIR)/lib/{libDirt.so,libDirt.so.$(MAJOR),libDirt.so.$(MAJOR).$(MINOR)} $(INSTALLDIR)/include/Dirt
