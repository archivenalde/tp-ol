#ifndef RESOLUTION_H
#define RESOLUTION_H

#define VARIABLESLENGTH 1024

#include <stdbool.h>
#include <cplex.h>
#include "inputoutput.h"

struct problem_s {
    CPXENVptr env;
    CPXLPptr lp;
    double obj, *var, *dual, *slack, *reduceCosts;
    int optResult;
};
typedef struct problem_s Problem;

// ouverture de CPLEX et creation d’un PL
Problem* openProb(const char* name);

// lecture depuis un fichier et instanciation d’un PL
void readProb(Problem* p, const char* fileName);

// resolution d’un PL
void optimizeProb(Problem* p);

// recuperation de la solution d'un PL
void getSolutionProb(Problem* p);

// liberation de l’espace memoire d'un PL et fermeture de CPLEX
void closeProb(Problem* p, int exit_status);

// affichage de l'erreur retournee par CPLEX
bool catchError(Problem* p, int status);


#endif // RESOLUTION_H
