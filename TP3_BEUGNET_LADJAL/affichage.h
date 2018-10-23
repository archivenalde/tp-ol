#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "resolution.h"

#define VARSTR "variables"
#define DUALSTR "variables_duals"
#define OBJSTR "solution_objectif"
#define SLACKSTR "variables_ecarts"
#define REDUCECOSTSSTR "couts_reduits"

void afficherVariablesRes(Problem* p);
void afficherSolutionOptimaleRes(Problem* p);
void afficherDualVariablesRes(Problem* p);
void afficherVariablesEcartsRes(Problem* p);
void afficherCoutsReduitsRes(Problem* p);
void afficherRes(Problem* p, ...);


#endif
