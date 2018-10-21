#include <stdlib.h>
#include <stdio.h>

#include <cplex.h>

struct resolution_s {
    CPXENVptr env;
    CPXLPptr lp;
    double *var, *dual, obj, *slack, *reduceCosts;
    int optResult, numRows, numCols;
};

typedef struct resolution_s Resolution;

//Affecte env et lp
int startRes(Resolution* r, const char* nomProb)
{
    int statut;

    r->env = CPXopenCPLEX(&statut);
    r->lp = CPXcreateprob(r->env, &statut, nomProb);

    return statut;
}

int readRes(Resolution* r, const char* fileName)
{
    int statut;

    statut = CPXreadcopyprob(r->env, r->lp, fileName, NULL);

    return statut;
}

int optRes(Resolution* r, int (*ptrOptFct)(CPXENVptr, CPXLPptr))
{
    int statut;

    statut = (*ptrOptFct)(r->env, r->lp);

    return statut;
}

int solRes(Resolution* r)
{
    int statut;

    r->numRows = CPXgetnumrows(r->env, r->lp);
    r->numCols = CPXgetnumcols(r->env, r->lp);

    r->var = malloc(sizeof(double) * r->numCols);
    r->dual = malloc(sizeof(double) * r->numRows);
    r->slack = malloc(sizeof(double) * r->numRows);
    r->reduceCosts = malloc(sizeof(double) * r->numCols);
    statut = CPXsolution(r->env, r->lp, &r->optResult, &r->obj, r->var, r->dual, r->slack, r->reduceCosts);

    return statut;
}

void afficherVariablesRes(Resolution *r)
{
    int i;
    printf("---------------Variables-------------------\n");
    for (i = 0; i < r->numCols; i++)
        printf("var[%d] = %lf\n", i+1, r->var[i]);

    printf("\n\n");
}

int destroyRes(Resolution *r)
{
    int statut = 0;
    if (r != NULL)
    {
        if (r->var != NULL) free(r->var);
        if (r->dual != NULL) free(r->dual);
        if (r->slack != NULL) free(r->slack);
        if (r->reduceCosts != NULL) free(r->reduceCosts);

        statut = CPXfreeprob (r->env, &r->lp);
        statut = CPXcloseCPLEX (&r->env);

        free(r);
    }

    return statut;
}

int main()
{
    int statut = 0; // statut technique des fonctions
    int i;

    Resolution* r = malloc(sizeof(Resolution));

    // ouverture de Cplex
    //r->env = CPXopenCPLEX (&statut);

    // creation d’un PL
    //r->lp = CPXcreateprob(r->env, &statut, "probleme");

    statut = startRes(r, "probleme");

    // instanciation d’un PL
    statut = readRes(r, "p3.lp");

    // resolution d’un PL
    statut = optRes(r, CPXprimopt);

    //Recuperation de la solution
    statut = solRes(r);

    afficherVariablesRes(r);
    // sauvegarde de la solution dans un fichier
    //statut = CPXsolwrite(env, lp, "resolution.txt");

    // liberation de l’espace memoire
    statut = destroyRes(r);
    // on s’en va !
    return 0;
}
