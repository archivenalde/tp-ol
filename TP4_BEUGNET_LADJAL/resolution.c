#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cplex.h>
#include "resolution.h"
#include "inputoutput.h"

Problem* openProb(const char* name)
{
    int status;
    Problem* p = NULL;

    p = (Problem*) malloc(sizeof(Problem));
    if (p == NULL)
    {
        perror("Erreur: echec de l'allocation memoire pour la structure resolution.\n");
        closeProb(p, EXIT_FAILURE);
    }
    p->env = NULL;
    p->lp = NULL;
    p->var = NULL;
    p->dual = NULL;
    p->slack = NULL;
    p->reduceCosts = NULL;

    p->env = CPXopenCPLEX(&status);
    if (p->env == NULL)
    {
        catchError(p, status);
        closeProb(p, EXIT_FAILURE);
    }

    if(name == NULL)
        closeProb(p, EXIT_FAILURE);

    p->lp = CPXcreateprob(p->env, &status, name);
    if (p->lp == NULL)
    {
        catchError(p, status);
        closeProb(p, EXIT_FAILURE);
    }

    return p;
}

void readProb(Problem* p, const char* fileName)
{
    int status;

    if(p == NULL || p->env == NULL || p->lp == NULL || fileName == NULL)
        closeProb(p, EXIT_FAILURE);

    status = CPXreadcopyprob(p->env, p->lp, fileName, NULL);
    if(catchError(p, status))
        closeProb(p, EXIT_FAILURE);
}

void optimizeProb(Problem* p)
{
    int status;

    if(p == NULL || p->env == NULL || p->lp == NULL)
        closeProb(p, EXIT_FAILURE);

    status = CPXlpopt(p->env, p->lp);
    if(catchError(p, status))
        closeProb(p, EXIT_FAILURE);
}

void getSolutionProb(Problem* p)
{
    int status, nbCols, nbRows;

    if(p == NULL || p->env == NULL || p->lp == NULL)
    {
        perror("Erreur: recherche de solutions sur un probleme non initialise.\n");
        closeProb(p, EXIT_FAILURE);
    }

    nbCols = CPXgetnumcols(p->env, p->lp);
    nbRows = CPXgetnumrows(p->env, p->lp);

    p->var = (double*) malloc(sizeof(double) * nbCols);
    p->dual = (double*) malloc(sizeof(double) * nbRows);
    p->slack = (double*) malloc(sizeof(double) * nbRows);
    p->reduceCosts = (double*) malloc(sizeof(double) * nbCols);
    if(p->var == NULL || p->dual == NULL || p->slack == NULL || p->reduceCosts == NULL)
    {
        perror("Erreur: echec de l'allocation memoire pour la solution.\n");
        closeProb(p, EXIT_FAILURE);
    }

    status = CPXsolution(p->env, p->lp, &p->optResult, &p->obj, p->var, p->dual, p->slack, p->reduceCosts);
    if(catchError(p, status))
        closeProb(p, EXIT_FAILURE);
}

/*
void printSolutionProb(Problem* p)
{
    int status, surplus, nbCol, i;
    char **varName = NULL, *nameStore = NULL;

    if(p->env == NULL || p->lp == NULL)
    {
        perror("Erreur: tentative d'affichage de solutions sur un probleme non initialise.\n")
        closeProb(p, EXIT_FAILURE);
    }

    if(p->var == NULL)
    {
        perror("Erreur: tentative d'affichage de solutions sur un probleme non resolu.\n")
        closeProb(p, EXIT_FAILURE);
    }

    nbCol = CPXgetnumcols(p->env, p->lp);
    varName = (char**) malloc(nbCol * sizeof(char*));
    nameStore = (char*) malloc(VARIABLESLENGTH * sizeof(char));

    if(varName == NULL || nameStore == NULL)
    {
        perror("Erreur: echec de l'allocation memoire pour la recuperation des noms de variable.\n");
        closeProb(p, EXIT_FAILURE);
    }

    status = CPXgetcolname (p->env, p->lp, varName, nameStore, VARIABLESLENGTH, &surplus, 0, nbCol-1);
    if(status == CPXERR_NEGATIVE_SURPLUS)
    {
        nameStore = realloc(nameStore, VARIABLESLENGTH-surplus);
        if(nameStore == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour la recuperation des noms de variable.\n");
            free(varName);
            destroyProb(p, EXIT_FAILURE);
        }

        status = CPXgetcolname (p->env, p->lp, varName, nameStore, VARIABLESLENGTH-surplus, &surplus, 0, nbCol-1);
        if(catchError(p ,status))
        {
            free(varName);
            free(nameStore);
            closeProb(p, EXIT_FAILURE);
        }
    }

    printf("Le gain maximal avec ces parametres est de : %d€", p->obj);


    free(varName);
    free(nameStore);
}
*/

void closeProb(Problem* p, int exit_status)
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
            catchError(p, status);
        }

        if(p->env != NULL)
        {
            status = CPXcloseCPLEX(&p->env);
            catchError(p, status);
        }

        free(p);
    }

    exit(exit_status);
}

bool catchError(Problem* p, int status)
{
    char errorMessage[CPXMESSAGEBUFSIZE] = "";

    if(status)
    {
        CPXgeterrorstring (p->env, status, errorMessage);
        fprintf(stderr, "%s", errorMessage);
        return true;
    }

    return false;
}
