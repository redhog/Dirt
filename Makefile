all: runBufferTest runReaderTest runStructTest runStructReaderTest

.PHONY: runBufferTest

runBufferTest: BufferTest
	./BufferTest

BufferTest: BufferTest.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

.PHONY:  runReaderTest

runReaderTest: ReaderTest
	./ReaderTest

ReaderTest: ReaderTest.o Reader.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

.PHONY: runStructTest

runStructTest: StructTest
	./StructTest

StructTest: StructTest.o Struct.o Session.o
	gcc $^ -o $@

.PHONY: runStructReaderTest

runStructReaderTest: StructReaderTest
	./StructReaderTest > StructReaderTest.output
	diff ReaderTest.data StructReaderTest.output

StructReaderTest: StructReaderTest.o StructReader.o Struct.o Reader.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

%.o: %.c
	gcc -g -c $< -o $@

.PHONY: clean

clean:
	rm *Test *.output *.o *~ core*
