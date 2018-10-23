#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#define TAILLE_MAX 25


struct input_s
{
    int demEnt, demJus, demSauce;
    double prixEnt, prixJus, prixSauce;
    double coutEnt, coutJus, coutSauce;
    int nbBase, nbSup;
    int* qualiteBase,* quantiteBase;
    double* prixBase;
    int* qualiteSup,* quantiteSup;
    double* prixSup;
};
typedef struct input_s Input;

Input* creerInput();
void recupererInput(Input* in, const char* fileName);
void ajouterBaseInput(Input* in, int qualite, int quantite, double prix);
void ajouterSupInput(Input* in, int qualite, int quantite, double prix);
void detruireInput(Input* in, int exit_status);

#endif // INPUTOUTPUT_H
