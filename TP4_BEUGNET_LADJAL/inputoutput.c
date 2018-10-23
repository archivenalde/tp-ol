#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputoutput.h"

Input* creerInput()
{
    Input* in = NULL;

    in = (Input*) malloc(sizeof(Input));
    if(in == NULL)
    {
        perror("Erreur: echec de l'allocation memoire pour la structure input.\n");
        detruireInput(in, EXIT_FAILURE);
    }

    in->nbBase = 0;
    in->nbSup = 0;
    in->qualiteBase = NULL;
    in->quantiteBase = NULL;
    in->prixBase = NULL;
    in->qualiteSup = NULL;
    in->quantiteSup = NULL;
    in->prixSup = NULL;

    return in;
}

void recupererInput(Input* in, const char* fileName)
{
    FILE* fichier = NULL;
    int x, y;
    double z;
    char c, chaine[TAILLE_MAX] = "";

    if(in == NULL)
    {
        perror("Erreur: tentative d'ajout de donnees dans une structure non initialisee.\n");
        detruireInput(in, EXIT_FAILURE);
    }

    fichier = fopen(fileName, "r");
    if(fichier == NULL)
    {
        fprintf(stderr, "Erreur: probleme d'ouverture du fichier '%s'.\n", fileName);
        detruireInput(in, EXIT_FAILURE);
    }

    while(fgetc(fichier) != EOF)
    {
        fseek(fichier, -1, SEEK_CUR);
        while(c = fgetc(fichier) == '\n');
        fseek(fichier, -1, SEEK_CUR);
        fgets(chaine, TAILLE_MAX, fichier);
        if(!strcmp("Demande", chaine))
        {
            fscanf(fichier, "%s %s %s\n", chaine, chaine, chaine);
            fscanf(fichier, "%d %d %d\n", in->demEnt, in->demJus, in->demSauce);
        }
        else if(!strcmp("Prix de vente", chaine))
        {
            fscanf(fichier, "%s %s %s\n", chaine, chaine, chaine);
            fscanf(fichier, "%d %d %d\n", in->prixEnt, in->prixJus, in->prixSauce);
        }
        else if(!strcmp("Coût hors tomates", chaine))
        {
            fscanf(fichier, "%s %s %s\n", chaine, chaine, chaine);
            fscanf(fichier, "%d %d %lf\n", in->coutEnt, in->coutJus, in->coutSauce);
        }
        else if(!strcmp("Lot de base", chaine))
        {
            fscanf(fichier, "%s %s %s\n", chaine, chaine, chaine);
            fscanf(fichier, "%d %d %lf\n", &x, &y, &z);
            ajouterBaseInput(in, x, y, z);
        }
        else if(!strcmp("Lots supplémentaires", chaine))
        {
            fscanf(fichier, "%s %s %s\n", chaine, chaine, chaine);
            fscanf(fichier, "%d %d %lf\n", &x, &y, &z);
            ajouterSupInput(in, x, y, z);
        }
    }

    fclose(fichier);
}

void ajouterBaseInput(Input* in, int qualite, int quantite, double prix)
{
    int* iptr = NULL;
    double* dptr = NULL;

    if(in == NULL)
    {
        perror("Erreur: tentative d'ajout de lot de base dans une structure non initialisee.\n");
        detruireInput(in, EXIT_FAILURE);
    }

    if(in->nbBase == 0)
    {
        in->qualiteBase = (int*) malloc(sizeof(int));
        in->quantiteBase = (int*) malloc(sizeof(int));
        in->prixBase = (double*) malloc(sizeof(double));
        if(in->qualiteBase == NULL || in->quantiteBase == NULL || in->prixBase == NULL)
        {
            perror("Erreur: echec de l'allocation memoire pour l'ajout de lot de base.\n");
            detruireInput(in, EXIT_FAILURE);
        }
    }
    else
    {
        iptr = (int*) realloc(in->qualiteBase, in->nbBase+1);
        if(iptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot de base.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->qualiteBase = iptr;

        iptr = (int*) realloc(in->quantiteBase, in->nbBase+1);
        if(iptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot de base.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->quantiteBase = iptr;

        dptr = (double*) realloc(in->prixBase, in->nbBase+1);
        if(dptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot de base.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->prixBase = dptr;
    }

    in->qualiteBase[in->nbBase] = qualite;
    in->quantiteBase[in->nbBase] = quantite;
    in->prixBase[in->nbBase] = prix;
    in->nbBase++;
}

void ajouterSupInput(Input* in, int qualite, int quantite, double prix)
{
    int* iptr = NULL;
    double* dptr = NULL;

    if(in == NULL)
    {
        perror("Erreur: tentative d'ajout de lot supplementaire dans une structure non initialisee.\n");
        detruireInput(in, EXIT_FAILURE);
    }

    if(in->nbSup == 0)
    {
        in->qualiteSup = (int*) malloc(sizeof(int));
        in->quantiteSup = (int*) malloc(sizeof(int));
        in->prixSup = (double*) malloc(sizeof(double));
        if(in->qualiteSup == NULL || in->quantiteSup == NULL || in->prixSup == NULL)
        {
            perror("Erreur: echec de l'allocation memoire pour l'ajout de lot supplementaire.\n");
            detruireInput(in, EXIT_FAILURE);
        }
    }
    else
    {
        iptr = (int*) realloc(in->qualiteSup, in->nbSup+1);
        if(iptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot supplementaire.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->qualiteSup = iptr;

        iptr = (int*) realloc(in->quantiteSup, in->nbSup+1);
        if(iptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot supplementaire.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->quantiteSup = iptr;

        dptr = (double*) realloc(in->prixSup, in->nbSup+1);
        if(dptr == NULL)
        {
            perror("Erreur: echec de la reallocation memoire pour l'ajout de lot supplementaire.\n");
            detruireInput(in, EXIT_FAILURE);
        }
        in->prixSup = dptr;
    }

    in->qualiteSup[in->nbSup] = qualite;
    in->quantiteSup[in->nbSup] = quantite;
    in->prixSup[in->nbSup] = prix;
    in->nbSup++;
}

void detruireInput(Input* in, int exit_status)
{
    if(in != NULL)
    {
        if(in->qualiteBase  != NULL)    free(in->qualiteBase);
        if(in->quantiteBase != NULL)    free(in->quantiteBase);
        if(in->prixBase     != NULL)    free(in->prixBase);
        if(in->qualiteSup   != NULL)    free(in->qualiteSup);
        if(in->quantiteSup  != NULL)    free(in->quantiteSup);
        if(in->prixSup      != NULL)    free(in->prixSup);
        free(in);
    }

    exit(exit_status);
}
