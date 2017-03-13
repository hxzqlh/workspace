.SUFFIXES: .c .cpp .o
CC=g++ -g -shared -fPIC
GCC=g++ -g
 
all: test1.so test2.so main clean
 
OBJ1=test1.o
OBJ2=test2.o
OBJ3=main.o
 
test1.so:$(OBJ1)
	$(CC) -o $@ $?

test2.so:$(OBJ2)
	$(CC) -o $@ $?
 
main:$(OBJ3)
	$(GCC)  -o $@ $? -ldl
 
.cpp.o:
	$(CC) -c $*.cpp

.c.o:
	$(CC) -c $*.c

clear:
	rm -rf *.o *.so dl_class_so

clean:
	rm -rf *.o 
