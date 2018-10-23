#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <cplex.h>
#include "resolution.h"


void addConstraint(Problem* p);
void addVariable(Problem* p);
void modifyCoeff(Problem* p);
void solvePrint(Problem* p, int (*ptrFunction) (CPXENVptr, CPXLPptr));
void writeProblem(Problem* p);
void writeSolution(Problem* p);
void quit(Problem* p);


int main(int argc, char const *argv[])
{
    Problem* p;
    int (*ptrFunction) (CPXENVptr, CPXLPptr);
    int val;
    char buff[1024];

    system("clear");
    if(argc ==2 || argc == 3)
    {
        printf("Bienvenue dans l'interface C de CPLEX.\nVous avez choisi de lire le probleme contenu dans le fichier '%s'.\n", argv[1]);
        if(argc == 3)
        {
            printf("Choix de l'algorithme: ");
            if(!strcmp(argv[2], "primal"))
            {
                ptrFunction = CPXprimopt;
                printf("primam");
            }
            else if(!strcmp(argv[2], "dual"))
            {
                ptrFunction = CPXdualopt;
                printf("dual");
            }
            else if(!strcmp(argv[2], "barrier"))
            {
                ptrFunction = CPXbaropt;
                printf("barrier");
            }
            else if(!strcmp(argv[2], "sift"))
            {
                ptrFunction = CPXsiftopt;
                printf("sift");
            }
            else
            {
                printf("Choix de l'algorithme incorrect.\nChoix possibles: primal dual barrier sift\nAlgorithme choisi par defaut.");
                ptrFunction = CPXlpopt;
            }
            printf("\n\n");
        }
        else
        {
            printf("Algorithme choisi par defaut.\n\n");
            ptrFunction = CPXlpopt;
        }
        // ouverture de CPLEX et creation du PL avec le nom "probleme"
        p = createProb("probleme");

        // lecture depuis un fichier et instanciation du PL
        readProb(p, argv[1]);

        while(1)
        {
            printf("Quelle action voulez vous effectuer ?\n");
            printf("1: Ajouter une contrainte\n");
            printf("2: Ajouter une variable\n");
            printf("3: Modifier les coefficients ou les membres droits\n");
            printf("4: Resoudre le probleme et afficher ses solutions\n");
            printf("5: Sauvegarder le probleme\n");
            printf("6: Sauvegarder la solution\n");
            printf("7: Quitter le programme\n");
            printf("\nVotre choix:");

            scanf("%d", &val);
            system("clear");
            switch(val)
            {
                case 1:
                    addConstraint(p);
                    break;
                case 2:
                    addVariable(p);
                    break;
                case 3:
                    modifyCoeff(p);
                    break;
                case 4:
                    solvePrint(p, ptrFunction);
                    break;
                case 5:
                    writeProblem(p);
                    break;
                case 6:
                    writeSolution(p);
                    break;
                case 7:
                    quit(p);
                    return EXIT_SUCCESS;
                default:
                    system("clear");
            }
            system("clear");
        }
    }
    else printf("La commande est incorrecte.\nLa syntaxe correcte est:\n./resolution (fichier) [primal|dual|barrier|sift]\n");

    return EXIT_SUCCESS;
}

void addConstraint(Problem* p)
{
    int nbConstraint, nbCols, nbRows, i, j;
    int* constraintBegin,* constraintInd;
    double* constraintBound,* constraintCoeff;
    char* constraintSense;
    char c;

    nbCols = CPXgetnumcols(p->env, p->lp);
    nbRows = CPXgetnumrows(p->env, p->lp);

    printf("Combien de contraintes voulez vous ajouter ? ");
    scanf("%d",&nbConstraint);

    constraintBegin = (int*)malloc(nbConstraint*sizeof(int));
    constraintInd = (int*)malloc(nbCols*nbConstraint*sizeof(int));
    constraintCoeff = (double*)malloc(nbCols*nbConstraint*sizeof(double));
    constraintSense = (char*)malloc(nbConstraint*sizeof(char));
    constraintBound = (double*)malloc(nbConstraint*sizeof(double));
    if(constraintBegin == NULL || constraintInd == NULL || constraintCoeff == NULL || constraintBound == NULL || constraintSense == NULL)
    {
        printf("Echec de l'allocation memoire pour l'ajout des contraintes.\n");
        exit(EXIT_FAILURE);
    }

    if(nbConstraint > 0)
    {
        for(i=0; i<nbConstraint; i++)
            for(j=0; j<nbCols; j++)
                constraintInd[i*nbCols+j] = j;

        for(i=0; i<nbConstraint; i++)
            constraintBegin[i]= i*nbCols;

        for(i=0; i<nbConstraint; i++)
        {
            for(j=0; j<nbCols; j++)
            {
                printf("Entrez le coefficient %d de la contrainte c%d: ", j+1, nbRows+i+1);
                scanf("%lf", &constraintCoeff[i*nbCols+j]);
            }
            while(c=getchar() != '\n' && c != EOF);
            printf("Entrez le sens (L pour inferieur, G pour superieur ou E pour egal) de la contrainte c%d: ", nbRows+i+1);
            constraintSense[i]=getchar();
            printf("Entrez le membre droit de la contrainte c%d: ", nbRows+i+1);
            scanf("%lf", &constraintBound[i]);
        }

        addConstProb(p, nbConstraint, nbConstraint*nbCols, constraintBound, constraintSense, constraintBegin, constraintInd, constraintCoeff);
    }
}


void addVariable(Problem* p)
{
    int nbVariables, nbCols, nbRows, i, j;
    int* variableBegin,* variableInd;
    double* variableLowerBound,* variableUpperBound,* variableCoeff,* variableObjCoeff;
    char** variableNames;
    char temp[1024];

    nbCols = CPXgetnumcols(p->env, p->lp);
    nbRows = CPXgetnumrows(p->env, p->lp);

    printf("Combien de variables voulez vous ajouter ? ");
    scanf("%d", &nbVariables);

    variableObjCoeff = (double*)malloc(nbVariables*sizeof(double));
    variableBegin = (int*)malloc(nbVariables*sizeof(int));
    variableInd = (int*)malloc(nbRows*nbVariables*sizeof(int));
    variableCoeff = (double*)malloc(nbRows*nbVariables*sizeof(double));
    variableLowerBound = (double*)malloc(nbVariables*sizeof(double));
    variableUpperBound = (double*)malloc(nbVariables*sizeof(double));
    variableNames = (char**)malloc(nbVariables*sizeof(char*));

    if(variableObjCoeff == NULL || variableBegin == NULL || variableInd == NULL || variableCoeff == NULL || variableLowerBound == NULL || variableUpperBound == NULL || variableNames == NULL)
    {
        printf("Echec de l'allocation memoire pour l'ajout des variables.\n");
        exit(EXIT_FAILURE);
    }

    if(nbVariables > 0)
    {
        for(i=0; i<nbVariables; i++)
            for(j=0; j<nbRows; j++)
                variableInd[i*nbRows+j] = j;

        for(i=0; i<nbVariables; i++)
            variableBegin[i]= i*nbRows;

        for(i=0; i<nbVariables; i++)
        {
            printf("Entrez le nom de la variable %d: ", nbCols+i+1);
            scanf("%s", temp);
            variableNames[i] = (char*)malloc((strlen(temp)+1)*sizeof(char));
            strcpy(variableNames[i], temp);
            printf("Entrez le coefficient objectif de la variable %s: ", variableNames[i]);
            scanf("%lf", &variableObjCoeff[i]);
            for(j=0; j<nbRows; j++)
            {
                printf("Entrez le coefficient %d de la variable %s: ", j+1, variableNames[i]);
                scanf("%lf", &variableCoeff[i*nbRows+j]);
            }
            printf("Entrez la contrainte inferieure de la variable %s: ", variableNames[i]);
            scanf("%lf", &variableLowerBound[i]);
            printf("Entrez la contrainte superieure de la variable %s: ", variableNames[i]);
            scanf("%lf", &variableUpperBound[i]);
        }

        addVarProb(p, nbVariables, nbVariables*nbRows, variableObjCoeff, variableBegin, variableInd, variableCoeff, variableLowerBound, variableUpperBound, variableNames);
    }

    for(i=0; i<nbVariables; i++)
        free(variableNames[i]);
    free(variableNames);
    free(variableBegin);
    free(variableInd);
    free(variableLowerBound);
    free(variableUpperBound);
    free(variableObjCoeff);
    free(variableCoeff);
}

void modifyCoeff(Problem* p)
{
    int choice, nbCoeffs, nbCols, nbRows, i;
    int *numRow, *numCol;
    double *values;

    nbCols = CPXgetnumcols(p->env, p->lp);
    nbRows = CPXgetnumrows(p->env, p->lp);

    do {
        printf("Que souhaitez-vous modifier ? (1 pour les coefficients des contraintes, 2 pour les membres droits des contraintes)\n");
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("Combien de coefficients voulez-vous modifier ? \n");
            scanf("%d", &nbCoeffs);
            if (nbCoeffs > nbCols * nbRows)
            {
                printf("Vous souhaitez modifier plus de coefficients qu'il y en a dans la matrice...\n");
                continue;
            }
            numRow = (int*) malloc(sizeof(int) * nbCoeffs);
            numCol = (int*) malloc(sizeof(int) * nbCoeffs);
            values = (double*) malloc(sizeof(double) * nbCoeffs);

            for (i = 0; i < nbCoeffs; i++)
            {
                printf("Indiquez le numero de contrainte ou se situe le coeffcient %d a modifier : ", i+1);
                scanf("%d", &numRow[i]);
                numRow[i]--;
                printf("Indiquez le numero de la variable dont vous souhaitez modifier le coefficient %d : ", i+1);
                scanf("%d", &numCol[i]);
                numCol[i]--;
                printf("Indiquez la valeur du nouveau coefficient %d : ", i+1);
                scanf("%lf", &values[i]);
            }
            modifyCoeffProb(p,  nbCoeffs,  numRow,  numCol, values);
            free(numRow);
            free(numCol);
            free(values);
        }
        else if (choice == 2)
        {
            printf("Combien de membres droits voulez-vous modifier ? ");
            scanf("%d", &nbCoeffs);
            if (nbCoeffs > nbRows)
            {
                printf("Vous souhaitez modifier plus de membres droits qu'il n'y a de contraintes...\n");
                continue;
            }
            numRow = (int*) malloc(sizeof(int) * nbCoeffs);
            values = (double*) malloc(sizeof(double) * nbCoeffs);

            for (i = 0; i < nbCoeffs; i++)
            {
                printf("Indiquez le numero de contrainte ou se situe le membre droit %d a modifier : ", i+1);
                scanf("%d", &numRow[i]);
                numRow[i]--;
                printf("Indiquez la valeur du nouveau membre droit %d : ", i+1);
                scanf("%lf", &values[i]);
            }
            modifyRhsProb(p, nbCoeffs, numRow, values);
            free(numRow);
            free(values);
        }
    } while (choice != 1 && choice != 2);
}

void solvePrint(Problem* p, int (*ptrFunction) (CPXENVptr, CPXLPptr))
{
    optimizeProb(p, ptrFunction); // resolution du PL avec un algorithme choisi par l'utilisateur
    getSolutionProb(p); // recuperation de la solution du PL
    printVariablesProb(p); // affichage de la solution du PL
    system("clear");
}

void writeProblem(Problem* p)
{
    writeProb(p, "probleme.lp");
    printf("Probleme ecrit dans le fichier probleme.lp .\n");
}

void writeSolution(Problem* p)
{
    writeSolutionProb(p, "solution.sol");
    printf("Solution ecrite dans le fichier solution.sol .\n");
}

void quit(Problem* p)
{
    printf("Au revoir!\n");
    destroyProb(p); // liberation de l’espace memoire du PL
}
