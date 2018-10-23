all: resolution

resolution: main.o resolution.o affichage.o
	gcc main.o resolution.o affichage.o -o resolution -L/LOCAL/CPLEX_Studio1251/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

main.o: main.c resolution.h
	gcc -c main.c -o main.o -I/LOCAL/CPLEX_Studio1251/cplex/include/ilcplex

resolution.o: resolution.c resolution.h affichage.h
	gcc -c resolution.c -o resolution.o -I/LOCAL/CPLEX_Studio1251/cplex/include/ilcplex

affichage.o: affichage.c affichage.h
	gcc -c affichage.c -o affichage.o -I/LOCAL/CPLEX_Studio1251/cplex/include/ilcplex

clean:
	rm *.o
	rm resolution
