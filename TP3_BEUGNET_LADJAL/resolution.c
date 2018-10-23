#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cplex.h>

#include "resolution.h"
#include "affichage.h"

#define VARIABLESLENGTH 1024

//Affecte env et lp
Problem* createProb(const char* probName)
{
    int status;
    Problem* p;

    p = malloc(sizeof(Problem));
    if (p == NULL)
    {
        printf("Echec de l'allocation memoire pour la structure resolution.\n");
        destroyProb(p);
        exit(EXIT_FAILURE);
    }

    p->env = CPXopenCPLEX(&status);
    if (p->env == NULL)
    {
        printf("Echec de l'ouverture de CPLEX.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }

    p->lp = CPXcreateprob(p->env, &status, probName);
    if (p->lp == NULL)
    {
        printf("Echec de la creation du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }

    return p;
}

void readProb(Problem* p, const char* fileName)
{
    int status;

    status = CPXreadcopyprob(p->env, p->lp, fileName, NULL);
    if (status)
    {
        printf("Erreur de lecture du probleme dans le fichier \"%s\".\n", fileName);
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void writeProb(Problem* p, const char* fileName)
{
    int status;

    status = CPXwriteprob(p->env, p->lp, fileName, NULL);
    if (status)
    {
        printf("Erreur d'ecriture du probleme dans le fichier \"%s\".\n", fileName);
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void writeSolutionProb(Problem* p, const char* fileName)
{
    int status;

    status = CPXsolwrite(p->env, p->lp, fileName);
    if (status)
    {
        printf("Erreur d'ecriture de la solution dans le fichier \"%s\".\n", fileName);
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void addConstProb(Problem* p, int newRowsCount, int newCoeffCount, double* constraintBound, char* constraintSense, int* constraintBegin, int* constraintInd, double* constraintCoeff)
{
    int status, nbRows, i;
    char** rowNames;

    nbRows = CPXgetnumrows(p->env, p->lp);
    rowNames = (char**) malloc(nbRows*sizeof(char*));
    if(rowNames == NULL)
    {
        printf("Echec de l'allocation memoire pour l'ajout de contraintes.\n");
        destroyProb(p);
        exit(EXIT_FAILURE);
    }

    for(i=0; i<newRowsCount; i++)
    {
        rowNames[i]= (char*) malloc(sizeof(char)*((nbRows+1)/10 +2));
        sprintf(rowNames[i], "c%d", nbRows+i+1);
        if(rowNames[i] == NULL)
        {
            printf("Echec de l'allocation memoire pour l'ajout de contraintes.\n");
            destroyProb(p);
            exit(EXIT_FAILURE);
        }
    }

    status = CPXaddrows (p->env, p->lp, 0, newRowsCount, newCoeffCount, constraintBound, constraintSense, constraintBegin, constraintInd, constraintCoeff, NULL, rowNames);

    for(i=0; i<newRowsCount; i++)
        free(rowNames[i]);
    free(rowNames);

    if (status)
    {
        printf("Erreur pendant l'optimisation du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void addVarProb(Problem* p, int newColsCount, int newCoeffCount, double* variableObjCoeff, int* variableBegin, int* variableInd, double* variableCoeff, double* variableLowerBound, double* variableUpperBound, char** variableNames)
{
    int status;

    status = CPXaddcols (p->env, p->lp, newColsCount, newCoeffCount, variableObjCoeff, variableBegin, variableInd, variableCoeff, variableLowerBound, variableUpperBound, variableNames);
    if (status)
    {
        printf("Erreur pendant l'optimisation du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void modifyCoeffProb(Problem* p, int nbCoeffs, int *numRow, int *numCol, double *values)
{
    int status;

    status = CPXchgcoeflist (p->env, p->lp, nbCoeffs, numRow, numCol, values);

    if (status)
    {
        printf("Erreur pendant la modification des coefficients de la matrice du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void modifyRhsProb(Problem* p, int nbRhs, int* numRow, double* values)
{
    int status;

    status = CPXchgrhs (p->env, p->lp, nbRhs, numRow, values);

    if (status)
    {
        printf("Erreur pendant la modification des membres droits du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void optimizeProb(Problem* p, int (*ptrOptFct)(CPXENVptr, CPXLPptr))
{
    int status;

    status = (*ptrOptFct)(p->env, p->lp);
    if (status)
    {
        printf("Erreur pendant l'optimisation du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void getSolutionProb(Problem* p)
{
    int status, nbCols, nbRows;

    nbCols = CPXgetnumcols(p->env, p->lp);
    nbRows = CPXgetnumrows(p->env, p->lp);

    p->var = malloc(sizeof(double) * nbCols);
    p->dual = malloc(sizeof(double) * nbRows);
    p->slack = malloc(sizeof(double) * nbRows);
    p->reduceCosts = malloc(sizeof(double) * nbCols);
    if(p->var == NULL || p->dual == NULL || p->slack == NULL || p->reduceCosts == NULL)
    {
        printf("Echec de l'allocation memoire pour la solution.\n");
        destroyProb(p);
        exit(EXIT_FAILURE);
    }

    status = CPXsolution(p->env, p->lp, &p->optResult, &p->obj, p->var, p->dual, p->slack, p->reduceCosts);
    if(status)
    {
        printf("Echec de la recuperation de la solution du probleme.\n");
        printErrorMessage(p, status);
        destroyProb(p);
        exit(EXIT_FAILURE);
    }
}

void printVariablesProb(Problem* p)
{
    int status, surplus, i, n;
    char** varNames;
    char* nameStore;
    char choice[25];


    n = CPXgetnumcols(p->env, p->lp);
    varNames = malloc(sizeof(char*)*n);
    nameStore = malloc(sizeof(char)*VARIABLESLENGTH);

    if(varNames == NULL || nameStore == NULL)
        printf("Echec de l'allocation memoire pour la recuperation des noms de variable.\n");
    else
    {
        status = CPXgetcolname (p->env, p->lp, varNames, nameStore, VARIABLESLENGTH, &surplus, 0, n-1);
        if(status)
        {
            if(status == CPXERR_NEGATIVE_SURPLUS && surplus < 0)
            {
                nameStore = realloc(nameStore, VARIABLESLENGTH-surplus);
                if(nameStore == NULL)
                {
                    printf("Echec de la reallocation memoire pour la recuperation des noms de variable.\n");
                    free(varNames);
                    destroyProb(p);
                    exit(EXIT_FAILURE);
                }
                status = CPXgetcolname (p->env, p->lp, varNames, nameStore, VARIABLESLENGTH-surplus, &surplus, 0, n-1);
            }

            if(status)
            {
                printf("Echec de la recuperation des noms de variable.\n");
                printErrorMessage(p, status);
                if(varNames != NULL) free(varNames);
                if(nameStore != NULL) free(nameStore);
                destroyProb(p);
                exit(EXIT_FAILURE);
            }
        }

        do {
            printf("Que voulez-vous afficher ? '%s', '%s', '%s', '%s', '%s' ou '%s' : ", VARSTR, DUALSTR, OBJSTR, SLACKSTR, REDUCECOSTSSTR, "rien");
            scanf("%s", &choice);
            system("clear");
            afficherRes(p, choice, NULL);
        } while (strcmp(choice, "rien"));
    }

    if(varNames != NULL) free(varNames);
    if(nameStore != NULL) free(nameStore);
}

void destroyProb(Problem* p)
{
    int status;

    if (p != NULL)
    {
        if (p->var) free(p->var);
        if (p->dual) free(p->dual);
        if (p->slack) free(p->slack);
        if (p->reduceCosts) free(p->reduceCosts);

        if(p->lp != NULL)
        {
            status = CPXfreeprob(p->env, &p->lp);
            if(status)
            {
                printf("Echec de la destruction du probleme.\n");
                printErrorMessage(p, status);
            }
        }

        if(p->env != NULL)
        {
            status = CPXcloseCPLEX(&p->env);
            if(status)
            {
                printf("Echec de la fermeture de CPLEX.\n");
                printErrorMessage(p, status);
            }
        }

        free(p);
    }
}

void printErrorMessage(Problem* p, int status)
{
    char errorMessage[CPXMESSAGEBUFSIZE];

    CPXgeterrorstring (p->env, status, errorMessage);
    fprintf (stderr, "%s", errorMessage);
}
