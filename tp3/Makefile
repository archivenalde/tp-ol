all: tp3

tp3: tp3.o
    gcc tp3.o -o tp3 -L/LOCAL/CPLEX_Studio1251/cplex/lib/x86-64_sles10_4.1/static_pic -lcplex -lm -lpthread

tp3.o: tp3.c
    gcc -c tp3.c -o tp3.o -I/LOCAL/CPLEX_Studio1251/cplex/include/ilcplex
