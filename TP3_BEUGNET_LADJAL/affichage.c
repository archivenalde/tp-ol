#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <cplex.h>

#include "affichage.h"

void afficherVariablesRes(Problem* p)
{
    int i;

    printf("---------------Variables-------------------\n");
    for (i = 0; i < CPXgetnumrows(p->env, p->lp); i++)
        printf("var[%d] = %lf\n", i+1, p->var[i]);

    printf("\n\n");
}

void afficherSolutionOptimaleRes(Problem* p)
{
	int i;

    printf("---------------Valeur objectif-------------------\n");
    printf("La resultat de la solution optimale est %lf\n", p->obj);

    printf("\n\n");
}

void afficherDualVariablesRes(Problem* p)
{
    int i;

    printf("---------------Variables Duals-------------------\n");
    for (i = 0; i < CPXgetnumrows(p->env, p->lp); i++)
        printf("dual[%d] = %lf\n", i+1, p->dual[i]);

    printf("\n\n");
}

void afficherVariablesEcartsRes(Problem* p)
{
    int i;


    printf("---------------Variables d'ecarts-------------------\n");
    for (i = 0; i < CPXgetnumrows(p->env, p->lp); i++)
        printf("slack[%d] = %lf\n", i+1, p->slack[i]);

    printf("\n\n");
}


void afficherCoutsReduitsRes(Problem* p)
{
    int i;


    printf("---------------Couts reduits-------------------\n");
    for (i = 0; i < CPXgetnumcols(p->env, p->lp); i++)
        printf("rc[%d] = %lf\n", i+1, p->reduceCosts[i]);

    printf("\n\n");
}


void afficherRes(Problem* p, ...)
{
	va_list va;
	const char* fctstr;

	va_start(va, p);
	fctstr = va_arg(va, const char*);

	while (fctstr != NULL)
	  {
		if (strcmp(fctstr, VARSTR) == 0)
			afficherVariablesRes(p);
		else if (strcmp(fctstr, OBJSTR) == 0)
			afficherSolutionOptimaleRes(p);
		else if (strcmp(fctstr, DUALSTR) == 0)
			afficherDualVariablesRes(p);
		else if (strcmp(fctstr, SLACKSTR) == 0)
			afficherVariablesEcartsRes(p);
		else if (strcmp(fctstr, REDUCECOSTSSTR) == 0)
			afficherCoutsReduitsRes(p);
		else if (fctstr != NULL)
			printf("Pas d'affichages correspondant a : %s.\n", fctstr);
		fctstr = va_arg(va, const char*);
	}

	va_end(va);
}
