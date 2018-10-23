#include <stdio.h>
#include <stdlib.h>
#include "resolution.h"
#include "inputoutput.h"


int main(int argc, const char* argv[])
{
    Problem* p;

    // ouverture de CPLEX et creation du PL avec le nom "probleme"
    p = openProb("tomates");

    if(argc == 2)
    {
        // lecture depuis un fichier et instanciation du PL
        readProb(p, argv[1]);

        // optimisation du PL
        optimizeProb(p);

        // recuperation de la solution
        getSolutionProb(p);


    }
    else printf("La commande est incorrecte.\nLa syntaxe correcte est:\n./resolution (fichier)\n");

    // liberation de l’espace memoire du PL et fermeture de CPLEX
    closeProb(p, EXIT_SUCCESS);

    return EXIT_SUCCESS;
}
