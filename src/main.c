#include <stdio.h>
#include "../include/tp4.h"

int main()
{
    // menuPrincipal();
    t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));
    // indexer_fichier(index, "/home/thanksshu/Documents/Onedrive/Etudes/NF16_TP/tp4/test.txt");
    indexer_fichier(index, "C:\\Users\\linso\\Desktop\\study_files\\TC04\\NF16\\TP\\nf16-tp4-2020-a\\test.txt");
    char mot[MAX_WORD_LENTH] = "An";
    afficher_occurence_mot(index, mot);
    // parcours_index(index->racine, 0);
    return 0;
}
