all: clean executable execute 

executable:lista.o mem.o proc.o p0.o p1.o p2.o p3.o
	gcc -Wall -o p0 lista.o mem.o proc.o p0.o p1.o p2.o p3.o

p0.o:
	gcc -c -g p0.c 

p1.o: 
	gcc -c -g p1.c

p2.o: 
	gcc -c -g p2.c

p3.o: 
	gcc -c -g p3.c

lista.o:
	gcc -c -g lista.c

mem.o:
	gcc -c -g mem.c

proc.o:
	gcc -c -g proc.c

execute: 
	./p0

valgrind: clean executable
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-report.txt ./p0

clean:
	rm -f *.o *.txt p0
