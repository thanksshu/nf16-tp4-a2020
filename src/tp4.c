#include "../include/tp4.h"

t_ListePosition *creer_liste_positions()
{
    t_ListePosition *list_position = (t_ListePosition *)calloc(1, sizeof(t_ListePosition));
    return list_position ? list_position : NULL;
}

int ajouter_position(t_ListePosition *listeP,
                     int numero_ligne, int numero_phrase,
                     int ordre_ligne, int ordre_phrase)
{
    // check if listeP is NULL
    if (!listeP)
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
    if (listeP->debut)
    {
        // already have one postion
        t_Position *temp_pos = listeP->debut;
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
        listeP->debut = position;
    }

    // element number plus one
    listeP->nb_elements += 1;
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

int ajouter_noeud(t_Index *index, t_Noeud *noeud, int balance)
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
    int result, grow = 0;
    _add_node(&(index->racine), noeud, &result, &grow, balance);

    if (result)
    // add success
    {
        index->nb_mots_total += 1;
        // return 1 means no duplication
        if (result == 1)
        {
            index->nb_mots_differents += 1;
        }
        return 1;
    }
    else
    // add failed
    {
        return 0;
    }
}

void _add_node(t_Noeud **ptr_root, t_Noeud *new, int *result, int *self_grow, int balance)
{
    // which child grows: 1 for left, -1 for right, 0 neither grows
    int child_grow = 0;

    /* add the new node, set final result, update balance */
    if (strcmp((*ptr_root)->mot, new->mot) == 0)
    // word duplicated with self
    {
        // add new position
        ajouter_position((*ptr_root)->positions,
                         new->positions->debut->numero_ligne,
                         new->positions->debut->numero_phrase,
                         new->positions->debut->ordre_ligne,
                         new->positions->debut->ordre_phrase);
        // no longer need the new node, free it
        new = _free_tree(new);

        (*ptr_root)->nb_occurences += 1;
        // child tree didn't grow
        child_grow = 0;
        *result = -1;
    }
    else if (strcmp((*ptr_root)->mot, new->mot) < 0)
    // word's node to add is bigger than self, try add to right
    {
        if ((*ptr_root)->filsDroit)
        // already have a node at right, try add node to next level
        {
            int right_grow = 0;
            _add_node(&((*ptr_root)->filsDroit), new, result, &right_grow,
                      balance);
            child_grow = right_grow ? -1 : 0;
        }
        else
        // add succeed
        {
            (*ptr_root)->filsDroit = new;
            // right tree grows higher
            child_grow = -1;
            // a different word added
            *result = 1;
        }
    }
    else if (strcmp((*ptr_root)->mot, new->mot) > 0)
    // word's node to add is smaller than self, try add to left
    {
        // try add to left
        if ((*ptr_root)->filsGauche)
        // already have a node at left, try add node to next level
        {
            int left_grow = 0;
            _add_node(&((*ptr_root)->filsGauche), new, result, &left_grow,
                      balance);
            child_grow = left_grow ? 1 : 0;
        }
        else
        // can add to left
        {
            (*ptr_root)->filsGauche = new;
            // left tree grows higher
            child_grow = 1;
            // a different word added
            *result = 1;
        }
    }

    /* recalculate self balance factor */
    int pre_balance_factor = (*ptr_root)->balance_factor;
    (*ptr_root)->balance_factor += child_grow;
    if (abs((*ptr_root)->balance_factor) > abs(pre_balance_factor))
    // self tree grows
    {
        // check if balance required
        if (balance)
        {
            int result = 0;
            _balance_tree(&(*ptr_root), &result);
            // if balance performed then self won't grow
            // if not self still grow
            *self_grow = result ? 0 : 1;
            return;
        }

        // no balance needed
        // self tree grows higher
        *self_grow = 1;
        return;
    }
    // self won't grow
    *self_grow = 0;
    return;
}

void _balance_tree(t_Noeud **ptr_root, int *result)
{
    // reset result
    *result = 0;

    // check if need balance
    if ((*ptr_root)->balance_factor == -2 &&
        (*ptr_root)->filsDroit->balance_factor == 1)
    {
        // right left rotation
        _rotate_right(&((*ptr_root)->filsDroit));
        _rotate_left(ptr_root);

        *result = 1;
    }
    else if ((*ptr_root)->balance_factor == 2 &&
             (*ptr_root)->filsGauche->balance_factor == -1)
    {

        // left right rotation
        _rotate_left(&((*ptr_root)->filsGauche));
        _rotate_right(ptr_root);

        *result = 1;
    }

    if (*result)
    {
        // update balance factor after double rotation
        (*ptr_root)->filsDroit->balance_factor = 0;
        (*ptr_root)->filsGauche->balance_factor = 0;
        switch ((*ptr_root)->balance_factor)
        {
        case 1:
            (*ptr_root)->filsDroit->balance_factor = -1;
            break;
        case -1:
            (*ptr_root)->filsGauche->balance_factor = 1;
            break;
        default:
            // two child's balance factor already set to 0
            break;
        }
        (*ptr_root)->balance_factor = 0;

        // end balance
        return;
    }

    if ((*ptr_root)->balance_factor == -2 &&
        (*ptr_root)->filsDroit->balance_factor == -1)
    {
        // left rotation
        _rotate_left(ptr_root);

        // update balance factor
        (*ptr_root)->filsGauche->balance_factor = 0;
        (*ptr_root)->balance_factor = 0;

        // end balance
        *result = 1;
        return;
    }

    if ((*ptr_root)->balance_factor == 2 &&
        (*ptr_root)->filsGauche->balance_factor == 1)
    {
        // right rotation
        _rotate_right(ptr_root);

        // update balance factor
        (*ptr_root)->filsDroit->balance_factor = 0;
        (*ptr_root)->balance_factor = 0;

        // end balance
        *result = 1;
        return;
    }

    // no balance needed
    *result = 0;
    return;
}

void _rotate_right(t_Noeud **ptr_root)
{
    t_Noeud *pivot = (*ptr_root)->filsGauche;
    // root's left child change
    (*ptr_root)->filsGauche = pivot->filsDroit;
    // left_child's right child change
    pivot->filsDroit = *ptr_root;
    // change the root
    *ptr_root = pivot;
}

void _rotate_left(t_Noeud **ptr_root)
{
    t_Noeud *pivot = (*ptr_root)->filsDroit;
    (*ptr_root)->filsDroit = pivot->filsGauche;
    pivot->filsGauche = *ptr_root;
    *ptr_root = pivot;
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
        index->racine = _free_tree(index->racine);
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
        if (character > 64 && character < 91 || character > 96 && character < 123)
        // capital or lower case character
        {
            word[strlen(word)] = character;
        }
        else if (!(character != ' ' && character != '.' && character != '\n'))
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
                // init mot, alloc with one more char to content \0
                node->mot = (char *)calloc(1, (MAX_WORD_LENTH + 1) * sizeof(char));
                strcpy(node->mot, word);
                // init occurence
                node->nb_occurences = 1;

                // add new node to tree
                if (!ajouter_noeud(index, node, 0))
                // fail to add
                {
                    if (node)
                        node = _free_tree(node);
                    if (index->racine)
                        index->racine = _free_tree(index->racine);
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
        else
        {
            // invalid word
            if (index->racine)
                index->racine = _free_tree(index->racine);
            index->racine = NULL;
            index->nb_mots_differents = 0;
            index->nb_mots_total = 0;
            return 0;
        }

        // read a new character
        character = (char)fgetc(file);
    }

    fclose(file);
    return word_count;
}

t_Noeud *_free_tree(t_Noeud *node)
{
    if (node)
    // free node
    {
        // postfix traversal
        // free left
        node->filsGauche = _free_tree(node->filsGauche);
        // free right
        node->filsDroit = _free_tree(node->filsDroit);
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
    // after free or nothing to free
    return NULL;
}

void afficher_index(t_Index *index)
{
    char *first_char = (char *)calloc(1, sizeof(char));
    // print recusively
    _print_tree(index->racine, first_char);
    free(first_char);
}

void _print_tree(t_Noeud *node, char *pre_first_char)
{
    // nothing to print
    if (node == NULL)
    {
        return;
    }

    // infix traversal
    // print left node
    _print_tree(node->filsGauche, pre_first_char);

    // detect changement of the first character
    if (*pre_first_char != *node->mot)
    {
        // print with capital case
        printf("\n%c\n", *node->mot - 32);
        *pre_first_char = *node->mot;
    }

    // print the word
    printf("├─ ");
    // capital the first char
    printf("%c", *(node->mot) - 32);
    // rest of char
    char *character = node->mot;
    while (*character)
    {
        character++;
        printf("%c", *character);
    }
    // end print
    printf("\n");

    t_Position *position = node->positions->debut;

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
    _print_tree(node->filsDroit, pre_first_char);
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

t_Index *equilibrer_index(t_Index *index)
{
    // check if balance
    if (_check_balance(index->racine))
    // no need
    {
        return index;
    }

    // prepare new index
    t_Index *index_avl = (t_Index *)calloc(1, sizeof(t_Index));
    if (_build_avl(&(index->racine), index_avl))
    // build success
    {
        // fix the total word count
        index_avl->nb_mots_total = index->nb_mots_total;
        // clean old index
        free(index);
        return index_avl;
    }
    // build failed
    index_avl->racine = _free_tree(index_avl->racine);
    free(index_avl);
    return NULL;
}

int _check_balance(t_Noeud *root)
{
    if (root)
    {
        // prefix check
        if (!(abs(root->balance_factor) < 2 &&
              _check_balance(root->filsGauche) &&
              _check_balance(root->filsDroit)))
        // not balanced
        {
            return 0;
        }
    }

    // balance
    return 1;
}

int _build_avl(t_Noeud **ptr_root, t_Index *index)
{
    if (*ptr_root)
    {
        // build with postfix traversal
        // build left then build right
        if (_build_avl(&((*ptr_root)->filsGauche), index) &&
            _build_avl(&((*ptr_root)->filsDroit), index))
        // build success
        {
            // build self
            // add the node to new index
            (*ptr_root)->balance_factor = 0;
            int result = ajouter_noeud(index, *ptr_root, 1);
            // remove the node from origin
            *ptr_root = NULL;
            return result;
        }
        // build fail
        else
        {
            return 0;
        }
    }
    // nothing to build
    return 1;
}

void menuPrincipal(void)
{
    int choix, import = 0;
    t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));

    do
    {
        printf("______________Bienvenue______________\n");
        printf("1 - Charger un fichier\n");
        printf("2 - Caractéristiques de l'index\n");
        printf("3 - Afficher l'index\n");
        printf("4 - Rechercher un mot\n");
        printf("5 - Afficher les occurences d'un mot\n");
        printf("6 - Equilibrer l'index\n");
        printf("7 - Quitter\n");

        //We ask for the user to choose an option
        choix = 0;
        scanf("%d", &choix);
        printf("\n\n");

        //We use a switch() to take care of the user's choice
        switch (choix)
        {
        case 1: //Charger un fichier
            printf("Entrez le nom du fichier à indexer :\n");
            char nomfichier[150];
            scanf("%s", nomfichier);

            int nbmots = indexer_fichier(index, nomfichier);

            if (nbmots != 0)
            {
                printf("Le fichier %s a été indéxé avec succès.\n", nomfichier);
                printf("Il contient %d mots.\n\n", nbmots);
                import = 1;
            }
            else
            {
                printf("Une erreur est survenue lors du chargement du fichier.\n\n");
            }

            break;

        case 2: //Caractéristiques de l'index
            if (import != 0)
            {
                printf("L'index contient %d mots différents.\n", index->nb_mots_differents);
                printf("L'index contient %d mots au total.\n", index->nb_mots_total);

                int equilibre = _check_balance(index->racine);
                if (equilibre == 1)
                {
                    printf("L'index est équilibré.\n\n");
                }
                else
                {
                    printf("L'index n'est pas équilibré.\n\n");
                }
            }
            else
            {
                printf("Erreur. Aucun fichier n'a été importé.\n\n");
            }
            break;

        case 3: //Afficher l'index
            if (import != 0)
            {
                afficher_index(index);
            }
            else
            {
                printf("Erreur. Aucun fichier n'a été importé.\n\n");
            }
            break;

        case 4: //Rechercher un mot
            if (import != 0)
            {
                printf("Entrez le mot que vous recherchez :\n");
                char motrecherche[MAX_WORD_LENTH];
                scanf("%s", motrecherche);

                t_Noeud *mottrouve = rechercher_mot(index, motrecherche);
                if (mottrouve != NULL)
                {
                    printf("%s a été trouvé :\n", motrecherche);
                    printf("|\n");
                    t_Position *mottrouveposition = mottrouve->positions->debut;
                    while (mottrouveposition != NULL)
                    {
                        printf("|-- Ligne %d, à la %d postion, dans la %d phrase.\n", mottrouveposition->numero_ligne, mottrouveposition->ordre_ligne, mottrouveposition->numero_phrase);
                        mottrouveposition = mottrouveposition->suivant;
                    }
                    printf("|\n");
                }
                else
                {
                    printf("Erreur. Le mot %s n'a pas été trouvé. \n\n", motrecherche);
                }
            }
            else
            {
                printf("Erreur. Aucun fichier n'a été importé.\n\n");
            }
            break;

        case 5: //Afficher les occurences d'un mot
            if (import != 0)
            {
                printf("Entrez le mot que vous recherchez :\n");
                char motrecherche[MAX_WORD_LENTH];
                scanf("%s", motrecherche);
                afficher_occurence_mot(index, motrecherche);
            }
            else
            {
                printf("Erreur. Aucun fichier n'a été importé.\n\n");
            }
            break;

        case 6: //Equilibrer l'index
            if (import != 0)
            {
                index = equilibrer_index(index);
                if (!index)
                {
                    printf("Erreur lors de la construction de l'index équilibré.\n\n");
                }
                else
                {
                    printf("L'index est maintenant équilibré.\n\n");
                }
            }
            else
            {
                printf("Erreur. Aucun fichier n'a été importé.\n\n");
            }
            break;

        case 7: //Quitter et libérer la mémoire
            if (import == 0)
            {
                free(index);
            }
            else
            {
                index->racine = _free_tree(index->racine);
                free(index);
            }
            printf("Au revoir ! \n");
            break;

        default:
            printf("Erreur de saisie. Veuillez entrez une option du programme.\n\n\n");
            break;
        }

    } while (choix != 7);
}
