#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <cplex.h>

struct problem_s {
    CPXENVptr env;
    CPXLPptr lp;
    double *var, *dual, obj, *slack, *reduceCosts;
    int optResult;
};
typedef struct problem_s Problem;

// ouverture de CPLEX et creation d’un PL
Problem* createProb(const char* probName);
// lecture depuis un fichier et instanciation d’un PL
void readProb(Problem* p, const char* fileName);
// ecriture d'un PL dans un fichier
void writeProb(Problem* p, const char* fileName);
// ecriture de la solution d'un PL dans un fichier
void writeSolutionProb(Problem* p, const char* fileName);
// ajout de contraintes a un PL
void addConstProb(Problem* p, int newRowsCount, int newCoeffCount, double* constraintBound, char* constraintSense, int* constraintBegin, int* constraintInd, double* constraintCoeff);
// ajout de variables a un PL
void addVarProb(Problem* p, int newColsCount, int newCoeffCount, double* variableObjCoeff, int* variableBegin, int* variableInd, double* variableCoeff, double* variableLowerBound, double* variableUpperBound, char** variableNames);
// modification des coefficients des contraintes
void modifyCoeffProb(Problem* p, int nbCoeffs, int *numRow, int *numCol, double *values);
// modification des membres droits
void modifyRhsProb(Problem* p, int nbRhs, int* numRow, double* values);
// resolution d’un PL
void optimizeProb(Problem* p, int (*ptrOptFct)(CPXENVptr, CPXLPptr));
// recuperation de la solution d'un PL
void getSolutionProb(Problem* p);
// affichage de la solution d'un PL
void printVariablesProb(Problem* p);
// liberation de l’espace memoire d'un PL et fermetur de CPLEX
void destroyProb(Problem* p);

// affichage de l'erreur retournee par CPLEX
void printErrorMessage(Problem* p, int status);


#endif // RESOLUTION_H
