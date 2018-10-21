#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <cplex.h>

#define ERREUR_CREATION_RES(fct) \
	if (r == NULL) \
    { \
    	printf("La structure de resolution n'est pas créée (%s).\n", fct);\
    	return -1;\
    }

#define ERREUR_STATUT(fct) \
	if (statut != 0) \
    { \
    	printf("Erreur, statut != 0 (%s).\n", fct);\
    }

struct resolution_s {
    CPXENVptr env;
    CPXLPptr lp;
    double *var, *dual, obj, *slack, *reduceCosts;
    int optResult, numRows, numCols;
};

typedef struct resolution_s Resolution;

Resolution* createRes(void)
{
	Resolution* res = (Resolution*) malloc(sizeof(Resolution));
	return res;
}

//Affecte env et lp
int startRes(Resolution* r, const char* nomProb)
{
    int statut;

    ERREUR_CREATION_RES("startRes")

    r->env = CPXopenCPLEX(&statut); 
    r->lp = CPXcreateprob(r->env, &statut, nomProb); 
   	ERREUR_STATUT("startRes")

    return statut;
}

int readRes(Resolution* r, const char* fileName)
{
    int statut;

   	ERREUR_CREATION_RES("readRes")

    statut = CPXreadcopyprob(r->env, r->lp, fileName, NULL);
    ERREUR_STATUT("readRes")

    return statut;
}

int startANDreadRes(Resolution* r, const char* nomProb, const char* fileName)
{
	int statut;

	ERREUR_CREATION_RES("startANDreadRes")

	statut = startRes(r, nomProb);
	statut = readRes(r, fileName);

	return statut;
}

int optRes(Resolution* r, int (*ptrOptFct)(CPXENVptr, CPXLPptr))
{
    int statut;

    ERREUR_CREATION_RES("optRes")

    statut = (*ptrOptFct)(r->env, r->lp);

    return statut;
}

int solRes(Resolution* r)
{
    int statut;

    ERREUR_CREATION_RES("solRes")

    r->numRows = CPXgetnumrows(r->env, r->lp);
    r->numCols = CPXgetnumcols(r->env, r->lp);

    r->var = realloc(sizeof(double) * r->numCols);
    r->dual = realloc(sizeof(double) * r->numRows);
    r->slack = realloc(sizeof(double) * r->numRows);
    r->reduceCosts = realloc(sizeof(double) * r->numCols);
    statut = CPXsolution(r->env, r->lp, &r->optResult, &r->obj, r->var, r->dual, r->slack, r->reduceCosts);

    return statut;
}

void afficherVariablesRes(Resolution *r)
{
    int i;

    ERREUR_CREATION_RES("afficherVariablesRes")

    printf("---------------Variables-------------------\n");
    for (i = 0; i < r->numCols; i++)
        printf("var[%d] = %lf\n", i+1, r->var[i]);

    printf("\n\n");
}

void afficherSolutionOptimaleRes(Resolution *r)
{
	int i;

    ERREUR_CREATION_RES("afficherSolutionOptimaleRes")

    printf("---------------Valeur objectif-------------------\n");
    printf("La resultat de la solution optimale est %lf\n", r->obj);

    printf("\n\n");
}

void afficherVarSolRes(Resolution *r)
{
	afficherVariablesRes(r);
	afficherSolutionOptimaleRes(r);
}


void afficherDualVariablesRes(Resolution *r)
{
    int i;

    ERREUR_CREATION_RES("afficherDualVariablesRes")

    printf("---------------Variables Duals-------------------\n");
    for (i = 0; i < r->numRows; i++)
        printf("dual[%d] = %lf\n", i+1, r->dual[i]);

    printf("\n\n");
}

void afficherVariablesEcartsRes(Resolution *r)
{
    int i;

    ERREUR_CREATION_RES("afficherVariablesEcartsRes")

    printf("---------------Variables d'ecarts-------------------\n");
    for (i = 0; i < r->numRows; i++)
        printf("slack[%d] = %lf\n", i+1, r->slack[i]);

    printf("\n\n");
}


void afficherCoutsReduitsRes(Resolution *r)
{
    int i;

    ERREUR_CREATION_RES("afficherCoutsReduitsRes")

    printf("---------------Couts reduits-------------------\n");
    for (i = 0; i < r->numCols; i++)
        printf("rc[%d] = %lf\n", i+1, r->reduceCosts[i]);

    printf("\n\n");
}


void afficherRes(Resolution* r, ...)
{
	va_list va;
	const char* fctstr;

	va_start(va, r);

	do {
		fctstr = va_arg(va, const char);
		if (strcmp(fctstr, "variables"))
			afficherVariablesRes(r);
		else if (strcmp(fctstr, "solution optimales") == 0)
			afficherSolutionOptimaleRes(r);
		else if (strcmp(fctstr, "dual variables") == 0)
			afficherDualVariablesRes(r);
		else if (strcmp(fctstr, "variables ecarts") == 0)
			afficherVariablesEcartsRes(r);
		else if (strcmp(fctstr, "couts reduits") == 0)
			afficherCoutsReduitsRes(r);
		else if (fctstr != NULL)
			printf("Pas d'affichages correspondant a : %s.\n", fctstr);
	} while (fctstr != NULL)

	va_end(va);
}


int destroyRes(Resolution *r)
{
    int statut = 0;

    ERREUR_CREATION_RES("destroyRes")

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


int main(int argc, char const *argv[])
{
    int statut = 0; // statut technique des fonctions
    int i;

    if (argc > 2)
    {
    	printf("L'execution de ce programme se fait de cette maniere : ./tp3 algorithme-de-resolution. Par defaut c'est equivalent a 'optimize'.\n");
    	return EXIT_FAILURE;
    }

    Resolution* r = createRes();

    // ouverture de Cplex
    //r->env = CPXopenCPLEX (&statut);

    // creation d’un PL
    //r->lp = CPXcreateprob(r->env, &statut, "probleme");

    //statut = startRes(r, "probleme");

    // instanciation d’un PL
    //statut = readRes(r, "p3.lp");

    statut = startANDreadRes(r, "probleme", "p3.lp");

    // resolution d’un PL
    if (strcmp(argv[1], "primal"))
    	statut = optRes(r, CPXprimopt);
    else if (strcmp(argv[1], "dual"))
    	statut = optRes(r, CPXdualopt);
    else
    	statut = optRes(r, CPXlpopt);

    //Recuperation de la solution
    statut = solRes(r);

    afficherVariablesRes(r);
    // sauvegarde de la solution dans un fichier
    //statut = CPXsolwrite(env, lp, "resolution.txt");

    // liberation de l’espace memoire
    statut = destroyRes(r);
    // on s’en va !
    return EXIT_SUCCESS;
}
