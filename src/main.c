#include "../include/tp4.h"

#include <signal.h>

int main()
{
    // menuPrincipal();
    // return 0;

    // test code
    t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));
    printf("nombre mot lu: %d\n",
          indexer_fichier(index, "/home/thanksshu/Documents/Onedrive/Etudes/NF16/tp/tp4/test.txt"));

    printf("%d %d\n", index->nb_mots_total, index->nb_mots_differents);

    printf("balance: %d\n", _check_balance(index->racine));

    afficher_index(index);

    t_Noeud *node_found = rechercher_mot(index, "day");
    node_found ? printf("%s", node_found->mot) : printf("NULL");

    afficher_occurence_mot(index, "day");

    // balance
    index = equilibrer_index(index);

    printf("%d %d\n", index->nb_mots_total, index->nb_mots_differents);

    printf("balance: %d\n", _check_balance(index->racine));

    afficher_index(index);

    node_found = rechercher_mot(index, "day");
    node_found ? printf("%s", node_found->mot) : printf("NULL");

    afficher_occurence_mot(index, "day");

    _free_tree(index->racine);
    free(index);

    return 0;
}
