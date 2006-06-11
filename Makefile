all: runBufferTest runWriteBufferTest runReaderTest runStructTest runStructReaderTest

%.o: %.c
	gcc -g -c $< -o $@

Test/BufferTest: Test/BufferTest.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

Test/WriteBufferTest: Test/WriteBufferTest.o WriteBuffer.o WriteBufferTypes.o Session.o
	gcc $^ -o $@

Test/ReaderTest: Test/ReaderTest.o Reader.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

Test/StructTest: Test/StructTest.o Struct.o Session.o
	gcc $^ -o $@

Test/StructReaderTest: Test/StructReaderTest.o StructReader.o Struct.o Reader.o Buffer.o BufferTypes.o Session.o
	gcc $^ -o $@

.PHONY: runBufferTest
.PHONY: runWriteBufferTest
.PHONY: runReaderTest
.PHONY: runStructTest
.PHONY: runStructReaderTest

runBufferTest: Test/BufferTest
	cd Test; ./BufferTest

runWriteBufferTest: Test/WriteBufferTest
	cd Test; ./WriteBufferTest

runReaderTest: Test/ReaderTest
	cd Test; ./ReaderTest

runStructTest: Test/StructTest
	cd Test; ./StructTest

runStructReaderTest: Test/StructReaderTest
	cd Test; ./StructReaderTest > StructReaderTest.output
	cd Test; diff ReaderTest.data StructReaderTest.output

.PHONY: clean

clean:
	rm Test/*Test Test/*.output Test/*.o Test/*~ Test/core* *.o *~ core*
