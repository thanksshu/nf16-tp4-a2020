#include "../include/tp4.h"

t_ListePosition *creer_liste_positions()
{
    t_ListePosition *list_position = (t_ListePosition *)calloc(1, sizeof(t_ListePosition));
    return list_position ? list_position : NULL;
}

int ajouter_position(t_ListePosition *liste_position,
                     int numero_ligne, int numero_phrase,
                     int ordre_ligne, int ordre_phrase)
{
    // check if liste_position is NULL
    if (!liste_position)
    {
        return -1;
    }

    // new t_Position
    t_Position *position = (t_Position *)calloc(1, sizeof(t_Position));
    if (!position)
    {
        return 0;
    }
    position->numero_ligne = numero_ligne;
    position->numero_phrase = numero_phrase;
    position->ordre_ligne = ordre_ligne;
    position->ordre_phrase = ordre_phrase;

    // check if list_position have a t_Position
    if (liste_position->debut)
    {
        // already have one postion
        t_Position *temp_pos = liste_position->debut;
        // to the end of list
        while (temp_pos->suivant)
        {
            temp_pos = temp_pos->suivant;
        }
        // append new pos to the next of the last position
        temp_pos->suivant = position;
    }
    else
    {
        // empty list
        liste_position->debut = position;
    }

    // element number plus one
    liste_position->nb_elements += 1;
    return 1;
}

t_Noeud *rechercher_mot(t_Index *index, char *mot)
{
    // make all characters lower case
    char *character = mot;
    while (*character)
    {
        if (*character < 91 && *character > 64)
        {
            *character = *character + 32;
        }
        character++;
    }

    // check if the tree is empty
    if (index->racine == NULL)
    {
        return NULL;
    }

    // search the word recusively
    return _search_word(index->racine, mot);
}

t_Noeud *_search_word(t_Noeud *node, char *word)
{
    // node don't exist
    if (node == NULL)
    {
        return NULL;
    }

    // word to search is bigger, search right
    if (strcmp(word, node->mot) > 0)
    {
        return _search_word(node->filsDroit, word);
    }

    // word to search is smaller, search left
    if (strcmp(word, node->mot) < 0)
    {
        return _search_word(node->filsGauche, word);
    }

    // find the word, return it
    if (strcmp(word, node->mot) == 0)
    {
        return node;
    }
}

int ajouter_noeud(t_Index *index, t_Noeud *noeud)
{
    // check if index and node don't exist
    if (!index || !noeud)
    {
        return 0;
    }

    // no node in index
    if (!(index->racine))
    {
        index->racine = noeud;
        // increase numbers
        index->nb_mots_total += 1;
        index->nb_mots_differents += 1;
        return 1;
    }

    // add node recusively
    int result = _add_node(index->racine, noeud);
    // add failed
    if (result == 0)
    {
        return 0;
    }
    else
    {
        index->nb_mots_total += 1;
        // return 1 means no duplication
        if (result == 1)
        {
            index->nb_mots_differents += 1;
        }
        return 1;
    }
}

int _add_node(t_Noeud *node_root, t_Noeud *node_new)
{
    // word duplicated
    if (strcmp(node_root->mot, node_new->mot) == 0)
    {
        // add new position
        ajouter_position(node_root->positions,
                         node_new->positions->debut->numero_ligne,
                         node_new->positions->debut->numero_phrase,
                         node_new->positions->debut->ordre_ligne,
                         node_new->positions->debut->ordre_phrase);
        // free new node
        _free_btree(node_new);

        node_root->nb_occurences += 1;
        return -1;
    }

    // word's node to add is bigger, try add to right
    if (strcmp(node_root->mot, node_new->mot) < 0)
    {
        // already have a node at right, try add node to next level
        if (node_root->filsDroit)
        {
            return _add_node(node_root->filsDroit, node_new);
        }
        else
        // add succeed
        {
            node_root->filsDroit = node_new;
            return 1;
        }
    }

    // word's node to add is smaller, try add to left
    if (strcmp(node_root->mot, node_new->mot) > 0)
    {
        // already have a node at right, try add node to next level
        if (node_root->filsGauche)
        {
            return _add_node(node_root->filsGauche, node_new);
        }
        else
        // add succeed
        {
            node_root->filsGauche = node_new;
            return 1;
        }
    }

    // something goes wrong
    return 0;
}

int indexer_fichier(t_Index *index, char *file_name)
{
    // check index
    if (!index)
    // no index
    {
        return 0;
    }

    if (index->racine)
    // index already have a tree
    {
        // clean up old tree
        _free_btree(index->racine);
        index->nb_mots_differents = 0;
        index->nb_mots_total = 0;
    }

    // open file
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        // file not exists, no word read
        return 0;
    }

    // read char one by one
    int line_word_order = 0, phrase_word_order = 0,
        line_count = 0, phrase_count = 0, word_count = 0;
    // init a empty string
    char word[MAX_WORD_LENTH] = "\0";
    // start pick character
    char character = (char)fgetc(file);
    while (character != EOF)
    {
        if (character != ' ' && character != '.' && character != '\n')
        // not word, line, phrase end, append the character to the word
        {
            word[strlen(word)] = character;
        }
        else
        // may be a word, line, phrase end
        // a ' ' may be a word end, excep "  "
        // a '.' end may be a word end, except ".." ". .", it must be a phrase end
        // a '\n' end may be a word end, except ".\n" ". \n", it must be a line end
        // => as a result, a word must not be empty to determine a new word
        {
            // check if word is not empty
            if (word[0])
            {
                // add this new word to tree
                // end the word
                word[strlen(word)] = '\0';
                // make all characters lower case
                char *character = word;
                while (*character)
                {
                    if (*character < 91 && *character > 64)
                    {
                        *character = *character + 32;
                    }
                    character++;
                }
                // create a new node, assume it's the first time the word appears
                t_Noeud *node = (t_Noeud *)calloc(1, sizeof(t_Noeud));
                // init position and list of positions
                t_ListePosition *list_position = (t_ListePosition *)calloc(1, sizeof(t_ListePosition));
                ajouter_position(list_position, line_count, phrase_count,
                                 line_word_order, phrase_word_order);
                node->positions = list_position;
                // init mot
                node->mot = (char *)calloc(1, sizeof(MAX_WORD_LENTH));
                strcpy(node->mot, word);
                // init occurence
                node->nb_occurences = 1;

                // add new node to tree
                if (!ajouter_noeud(index, node))
                // fail to add
                {
                    _free_btree(node);
                    _free_btree(index->racine);
                    index->nb_mots_differents = 0;
                    index->nb_mots_total = 0;
                    return 0;
                }

                // word ends
                word_count += 1;
                line_word_order += 1;
                phrase_word_order += 1;
                // reset word
                int i = 0;
                while (word[i])
                {
                    word[i] = '\0';
                    i += 1;
                }
            }

            // check if a phrase ends
            if (character == '.')
            {
                phrase_count += 1;
                phrase_word_order = 0;
            }

            // check if a line ends
            if (character == '\n')
            {
                line_count += 1;
                line_word_order = 0;
            }
        }

        // read a new character
        character = (char)fgetc(file);
    }

    fclose(file);
    return word_count;
}

void _free_btree(t_Noeud *node)
{
    if (!node)
    // empty node, nothing to free
    {
        return;
    }

    // postfix traversal
    // free left
    _free_btree(node->filsGauche);
    // free right
    _free_btree(node->filsDroit);
    // free self
    // free mot
    free(node->mot);
    // free list position
    t_Position *pos = node->positions->debut;
    while (pos)
    {
        t_Position *temp_pos = pos->suivant;
        free(pos);
        pos = temp_pos;
    }
    free(node->positions);
    free(node);
}

void afficher_index(t_Index *index)
{
    _print_node(index->racine, (char *)calloc(1, sizeof(char)));
}

void _print_node(t_Noeud *noeud, char *first_char)
{
    // nothing to print
    if (noeud == NULL)
    {
        return;
    }

    // prefix traversal
    // print left node
    _print_node(noeud->filsGauche, first_char);

    // detect changement of the first character
    if (*first_char != *(noeud->mot) - 32)
    {
        memset(first_char, *(noeud->mot) - 32, 1);
        printf("\n%c\n", *first_char);
    }

    // print the word
    printf("├─ ");
    // capital the first char
    printf("%c", *(noeud->mot) - 32);
    // rest of char
    char *character = noeud->mot;
    while (*character)
    {
        character++;
        printf("%c", *character);
    }
    // end print
    printf("\n");

    t_Position *position = noeud->positions->debut;

    while (position)
    {
        printf("├── (l:%d p:%d ol:%d op:%d)\n",
               position->numero_ligne,
               position->numero_phrase,
               position->ordre_ligne,
               position->ordre_phrase);
        position = position->suivant;
    }

    printf("│\n");

    // print right node
    _print_node(noeud->filsDroit, first_char);
}

void afficher_occurence_mot(t_Index *index, char *mot)
{

    // index don't exist
    if (!index)
    {
        return;
    }

    // search for the word
    t_Noeud *node_searched = rechercher_mot(index, mot);
    if (!node_searched)
    {
        printf("\nCe mot n'existe pas\n");
        return;
    }
    printf("\nMot \"%s\"\n", node_searched->mot);
    printf("Occurences = %d\n", node_searched->nb_occurences);

    // prepare phrases
    int pre_phrase = -1;
    int current_phrase = -1;
    t_Position *pos = node_searched->positions->debut;
    for (size_t i = 0; i < node_searched->positions->nb_elements; i++)
    {
        // read a phrase number
        current_phrase = pos->numero_phrase;

        // make a phrase
        char *phrase[MAX_PHRASE_LENTH] = {};
        _make_phrase(index->racine, current_phrase, phrase);
        int order_word_in_phrase = 0;

        // print info of word in phrase
        printf("├── (l:%d p:%d ol:%d op:%d) ",
               pos->numero_ligne,
               pos->numero_phrase,
               pos->ordre_ligne,
               pos->ordre_phrase);

        // print this phrase
        while (phrase[order_word_in_phrase])
        {
            // first word in phrase
            if (order_word_in_phrase == 0)
            {
                // capital first character
                printf("%c", *(phrase[order_word_in_phrase]) - 32);
                // handle the rest
                char *character = phrase[order_word_in_phrase];
                while (*character)
                {
                    character++;
                    printf("%c", *character);
                }
            }
            else
            {
                printf(" %s", phrase[order_word_in_phrase]);
            }
            order_word_in_phrase += 1;
        }
        printf(".\n");
        // pick up next phrase
        pos = pos->suivant;
        pre_phrase = current_phrase;
    }
}

void _make_phrase(t_Noeud *node, int num_phrase,
                  char *array_phrase[MAX_PHRASE_LENTH])
{
    // empty node
    if (!node)
    {
        return;
    }

    // infix traversal
    // left child
    _make_phrase(node->filsGauche, num_phrase, array_phrase);

    // self
    t_Position *pos = node->positions->debut;
    // traversal all postions
    for (size_t i = 0; i < node->positions->nb_elements; i++)
    {
        // find the position needed
        if (pos->numero_phrase == num_phrase)
        {
            // put string address in array
            array_phrase[pos->ordre_phrase] = node->mot;
        }
        pos = pos->suivant;
    }

    // right child
    _make_phrase(node->filsDroit, num_phrase, array_phrase);
}

// //foncton pour equilibrer l'arbre d'un index
// t_Index *equilibrer_index(t_Index *index)
// {
//     t_Noeud *ptrb; //pointer de boucle de noeuds dans l'arbre
//     t_Noeud *newracine = NULL;
//     ptrb = index->racine;

//     //parcours de l'arbre ancienne
//     equilibrer_parcours(ptrb, &newracine);

//     //modifier index
//     index->racine = newracine;

//     return index;
// }

// void parcours_index(t_Noeud *noeud, int height)
// {
//     if (noeud == NULL)
//     {
//         return;
//     }
//     printf("mot: %s, height: %d\n", noeud->mot, height);
//     printf("\t%d left:\n", height);
//     parcours_index(noeud->filsGauche, height + 1);
//     printf("\t%d right:\n", height);
//     parcours_index(noeud->filsDroit, height + 1);
// }

// //rotation elementaire
// void R_Rotate(t_Noeud **p)
// {
//     t_Noeud *L;
//     L = (*p)->filsGauche;
//     (*p)->filsGauche = L->filsDroit;
//     L->filsDroit = (*p);
//     *p = L;
// }

// //rotation elementaire
// void L_Rotate(t_Noeud **p)
// {
//     t_Noeud *R;
//     R = (*p)->filsDroit;
//     (*p)->filsDroit = R->filsGauche;
//     R->filsGauche = (*p);
//     *p = R;
// }

// void LeftBalance(t_Noeud **ptrt)
// {
//     t_Noeud *L, *Lr;
//     L = (*ptrt)->filsGauche;

//     switch (L->balance)
//     {
//     case 1:
//         (*ptrt)->balance = L->balance = 0;
//         R_Rotate(ptrt);
//     case -1:
//         Lr = L->filsDroit;
//         switch (Lr->balance)
//         {
//         case 1:
//             (*ptrt)->balance = -1;
//             L->balance = 0;
//             break;
//         case 0:
//             (*ptrt)->balance = L->balance = 0;
//             break;
//         case -1:
//             (*ptrt)->balance = 0;
//             L->balance = 1;
//             break;
//         }
//         Lr->balance = 0;
//         L_Rotate(&(*ptrt)->filsGauche);
//         R_Rotate(ptrt);
//     }
// }

// void RightBalance(t_Noeud **ptrt)
// {
//     t_Noeud *R, *Rl;
//     R = (*ptrt)->filsDroit;

//     switch (R->balance)
//     {
//     case -1:
//         (*ptrt)->balance = R->balance = 0;
//         L_Rotate(ptrt);
//         break;
//     case 1:
//         Rl = R->filsGauche;
//         switch (Rl->balance)
//         {
//         case 1:
//             (*ptrt)->balance = 0;
//             R->balance = -1;
//             break;
//         case 0:
//             (*ptrt)->balance = R->balance = 0;
//             break;
//         case -1:
//             (*ptrt)->balance = 1;
//             R->balance = 0;
//             break;
//         }
//         Rl->balance = 0;
//         R_Rotate(&(*ptrt)->filsDroit);
//         L_Rotate(ptrt);
//     }
// }

// //cette fonction permet de recreer l'arbre en respectant la propriete d'AVL
// int InsertAVL(t_Noeud **ptrt, char *word, int *taller, t_Noeud *noeud_ancien)
// {
//     if (!*ptrt)
//     {
//         *ptrt = (t_Noeud *)malloc(sizeof(t_Noeud));
//         (*ptrt)->mot = (char *)malloc(sizeof(MAX_WORD_LENTH));
//         strcpy((*ptrt)->mot, word);
//         (*ptrt)->filsGauche = (*ptrt)->filsDroit = NULL;
//         (*ptrt)->nb_occurences = noeud_ancien->nb_occurences;
//         (*ptrt)->balance = 0;
//         (*ptrt)->positions = noeud_ancien->positions;
//         *taller = 1;
//     }
//     else
//     {
//         if (strcmp(word, (*ptrt)->mot) == 0) /*noeud deja existe */
//         {
//             *taller = 0;
//             return 0;
//         }
//         if (strcmp(word, (*ptrt)->mot) < 0) /*parcourir a gauche*/
//         {
//             if (!InsertAVL(&(*ptrt)->filsGauche, word, taller, noeud_ancien))
//             {
//                 return 0;
//             }
//             if (*taller)
//             {
//                 switch ((*ptrt)->balance)
//                 {
//                 case 1:
//                     LeftBalance(ptrt);
//                     *taller = 0;
//                     break;
//                 case 0:
//                     (*ptrt)->balance = 1;
//                     *taller = 1;
//                     break;
//                 case -1:
//                     (*ptrt)->balance = 0;
//                     *taller = 0;
//                     break;
//                 }
//             }
//         }

//         else /*parcourir a droit*/
//         {
//             if (!InsertAVL(&(*ptrt)->filsDroit, word, taller, noeud_ancien))
//             {
//                 return 0;
//             }
//             if (*taller)
//             {
//                 switch ((*ptrt)->balance)
//                 {
//                 case 1:
//                     (*ptrt)->balance = 0;
//                     *taller = 0;
//                     break;
//                 case 0:
//                     (*ptrt)->balance = -1;
//                     *taller = 1;
//                     break;
//                 case -1:
//                     RightBalance(ptrt);
//                     *taller = 0;
//                     break;
//                 }
//             }
//         }
//     }
// }

// void equilibrer_parcours(t_Noeud *ptr_ancien, t_Noeud **new)
// {
//     int taller, insert;
//     taller = 0;
//     if (!ptr_ancien)
//     {
//         return;
//     }
//     insert = InsertAVL(new, ptr_ancien->mot, &taller, ptr_ancien);
//     equilibrer_parcours(ptr_ancien->filsGauche, new);
//     equilibrer_parcours(ptr_ancien->filsDroit, new);
// }

//Cette fonction est le menu dans lequel nous allons intéragir afin d'effectuer des actions
// void menuPrincipal(void)
// {
//     int uinput, instanceImport = 0;
//     t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));
//     FILE *f;

//     //Cette boucle va permettre d'affiche le menu tant que nous ne désirons pas de quitter le programme
//     do
//     {
//         //on va afficher le menu
//         affichageMenu();
//         //l'utilisateur va entrer un nombre (entre 1 et 7) afin d'effectuer l'action qu'il désire
//         scanf("%d", &uinput);
//         printf("\n");
//         switch (uinput)
//         {
//         case 1:
//         {
//             /* Importation d'un fichier */
//             char char_input[125];
//             //cette boucle est juste là pour permettre à l'utilisateur de rentrer un nom de fichier correct afin de pouvoir importer
//             while (1)
//             {
//                 printf("Veuillez saisir le nom du fichier d'une instance:\n");
//                 scanf("%s", char_input);
//                 f = fopen(char_input, "r");
//                 //on check si le fichier existe
//                 if (f)
//                 {
//                     //création de l'instance
//                     fclose(f);
//                     int nb_mots_lu = indexer_fichier(index, char_input);
//                     printf("Le fichier contient %d mots\n", nb_mots_lu);
//                     instanceImport = 1;
//                     break;
//                 }
//                 else
//                 {
//                     //le fichier n'existe pas, on demande à l'utilisateur s'il veut continuer à chercher ou non
//                     printf("Le fichier \"%s\" n'existe pas! Continuer? y/n:", char_input);
//                     scanf("%s", char_input);
//                     if (!strcmp("y", char_input))
//                     {
//                         continue;
//                     }
//                     else
//                     {
//                         break;
//                     }
//                 }
//             }
//             break;
//         }
//         case 2:
//         {
//             /* Caracteristiques de l'index */

//             break;
//         }
//         case 3:
//         {

//             break;
//         }
//         case 4:
//         {
//             /* Rechercher un mot */

//             break;
//         }
//         case 5:
//         {
//             /* Afficher les occurences d'un mot */

//             break;
//         }
//         case 6:
//         {
//             /* Equilibrer l'index */

//             break;
//         }
//         case 7:
//         {
//             /* Quitter l'application + liberation des resources memoires */

//             //on check si on a importé un fichier précédemment
//             //if (instanceImport) libererRessource(instance);
//             printf("Vous quittez l'application");
//             break;
//         }
//         default:
//             printf("Choix invalide !\n\n\n");
//         }
//     } while (uinput != 7);
// }
