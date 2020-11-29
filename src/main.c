#include <stdio.h>
#include "../include/tp4.h"

int main()
{
    // menuPrincipal();
    t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));
    indexer_fichier(index, "/home/thanksshu/Documents/Onedrive/Etudes/NF16_TP/tp4/test.txt");
    return 0;
}
