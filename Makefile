all : run

run : build
	./servidor 1 & (sleep 2s ; ./cliente file.txt 5)

build : servidor cliente

servidor : servidor.cpp servidor.h archivo.o
	g++ servidor.cpp archivo.o -o servidor

cliente : cliente.cpp cliente.h  archivo.o
	g++ cliente.cpp archivo.o -o cliente

archivo.o : archivo.cpp archivo.h
	g++ -c archivo.cpp

clean :
	rm -f escritor_* cliente servidor *.o 0.txt 1.txt 2.txt 3.txt 4.txt
