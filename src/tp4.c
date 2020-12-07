#include "../include/tp4.h"

// cette fonction créée une liste de positions vide
t_ListePosition *creer_liste_positions()
{
    t_ListePosition *listeP = (t_ListePosition *)malloc(sizeof(t_ListePosition));
    if (!listeP)
    {
        printf("Erreur de creation de la liste de positions !\n");
        return NULL;
    }
    listeP->debut = NULL;
    listeP->nb_elements = 0;
    return listeP;
}

// cette fonction ajoute un nouvel element dans une liste de position triée
int ajouter_position(t_ListePosition *liste_position,
                     int numero_ligne, int numero_phrase,
                     int ordre_ligne, int ordre_phrase)
{
    // allocate a new t_Position
    t_Position *pos = (t_Position *)calloc(1, sizeof(t_Position));
    if (!pos)
    {
        printf("Erreur de creation du nouvel element! !\n");
        return 0;
    }
    pos->numero_ligne = numero_ligne;
    pos->numero_phrase = numero_phrase;
    pos->ordre_ligne = ordre_ligne;
    pos->ordre_phrase = ordre_phrase;

    // check if list have a t_Position
    if (liste_position->debut)
    {
        // already have one
        t_Position *temp_pos = liste_position->debut;

        // to the end of list
        while (temp_pos->suivant)
        {
            temp_pos = temp_pos->suivant;
        }

        // check if orders are correct
        if ((temp_pos->numero_ligne <= pos->numero_ligne) &&
            (temp_pos->numero_phrase <= pos->numero_phrase) &&
            (temp_pos->ordre_ligne <= pos->ordre_ligne) &&
            (temp_pos->ordre_phrase <= pos->ordre_phrase))
        {
            // right
            temp_pos->suivant = pos;
        }
        else
        {
            // not right
            return 0;
        }
    }
    else
    {
        // nope
        liste_position->debut = pos;
    }

    // element number +1
    liste_position->nb_elements += 1;
    return 1;
}

// cette fonction créée un index vide
t_Index *creer_index()
{
    t_Index *index = (t_Index *)malloc(sizeof(t_Index));
    if (!index)
    {
        printf("Erreur de creation de l'index !\n");
        return NULL;
    }
    index->racine = NULL;
    index->nb_mots_differents = 0;
    index->nb_mots_total = 0;
    return index;
}

//cette fonction recherche un mot dans un index
t_Noeud *rechercher_mot(t_Index *index, char *mot)
{
    t_Noeud *ptrnoeud;
    make_word_lower(mot);

    if (index->racine == NULL)
    {
        return NULL;
    }
    ptrnoeud = index->racine;
    return recherche_appronfondie(ptrnoeud, mot);
}

//cette fonction permet d'ajouter un noeud dans l'index, avec detection integree
int ajouter_noeud(t_Index *index, t_Noeud *noeud)
{
    int flag; //When the addition is successful,flag values 1
    if (IndexNotFound(index))
    {
        return 0;
    }
    if (NoeudNotFound(noeud))
    {
        return 0;
    }
    flag = 0;
    if (index->racine == NULL)
    {
        index->racine = noeud;
        flag = 1;
    }
    else
    {
        ajouter_noeud_approfondie(index->racine, noeud, &flag);
    }
    return flag;
}

//cette fonction permet d'indexer un fichier lu
int indexer_fichier(t_Index *index, char *file_name)
{
    // open file
    FILE *file = fopen(file_name, "r");
    if (!file)
    {
        // file not exists, no word read
        printf("error read\n");
        return 0;
    }

    // read char one by one
    int line_word_order = 0, phrase_word_order = 0,
        line_count = 0, phrase_count = 0, word_count = 0;
    // init a empty string
    char word[MAX_WORD_LENTH];
    memset(word, '\0', sizeof(word));
    // start pick character
    char character = (char)fgetc(file);
    while (character != EOF)
    {
        switch (character)
        {
        case ' ':
            // is space
            printf("%s\n", word);

            traitement_word(index, word, line_count, line_word_order, phrase_count, phrase_word_order);

            // word end
            word_count += 1;
            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word
            break;
        case '.':
            // is space
            printf("%s\n", word);
            
            traitement_word(index, word, line_count, line_word_order, phrase_count, phrase_word_order);

            // phrase ends
            phrase_count += 1;
            phrase_word_order = 0;

            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word

            // skip next space
            character = (char)fgetc(file);
            break;
        case '\n':
            // is new line
            printf("%s\n", word);

            // line ends
            line_count += 1;
            line_word_order = 0;

            traitement_word(index, word, line_count, line_word_order, phrase_count, phrase_word_order);

            // word ends
            word_count += 1;
            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word

            break;
        default:
            // a character in a word
            word[strlen(word)] = character;
            break;
        }

        // read a new character
        character = (char)fgetc(file);
    }

    fclose(file);
    return word_count;
}

//cette fonction affiche les mots classés par ordre alphabétique
//L'objectif est de faire un affichage infixe
void afficher_index(t_Index* index) {
    t_Noeud* noeud = index->racine;
    Pile* pile = creer_pile();
    char currLettre = '0';

    //Cette boucle va parcourir tous les noeuds de l'index jusqu'à la fin de l'arbre et tant que la pile n'est pas vide
    while(noeud != NULL || pile->firstElem != NULL) {
        //Cette boucle parcourt les noeuds de l'arbre
        while(noeud != NULL) {
            empiler(pile, noeud);
            noeud = noeud->filsGauche;
        }
        noeud = depiler(pile);
        //Cette fonction va permettre l'affichage des informations d'un mot et de changer si besoin l'alphabet dans lequel nous nous trouvons
        currLettre = dataNoeud(currLettre, noeud);
        noeud = noeud->filsDroit;
    }
    //On va libérer les ressources allouées pour la pile
    libererPile(pile);
}

void afficher_occurence_mot(t_Index *index, char *mot)
{
    t_Noeud *trouve, *ptrb; /*pointer de boucle*/
    t_Position *ptrpo;
    int i, j;
    char *arrayofword[MAX_phrase];
    if (IndexNotFound(index))
    {
        return;
    }

    trouve = rechercher_mot(index, mot);
    if (!trouve)
    {
        printf("Ce mot n'existe pas!");
        return;
    }
    printf("Mot = '%s'\n", trouve->mot);
    printf("Occurences = %d\n", trouve->nb_occurences);
    ptrpo = trouve->positions->debut;
    for (i = 0; i < trouve->positions->nb_elements; i++)
    {
        printf("| Ligne %d, mot %d :", ptrpo->numero_ligne, ptrpo->ordre_ligne);
        traitementphrase(ptrpo, arrayofword, index->racine);
        j = 0;
        while (arrayofword[j] != NULL)
        {
            printf(" %s", arrayofword[j]);
            j++;
        }
        printf(".\n");
        ptrpo = ptrpo->suivant;
    }
}

//foncton pour equilibrer l'arbre d'un index
t_Index *equilibrer_index(t_Index *index)
{
    t_Noeud *ptrb;   //pointer de boucle de noeuds dans l'arbre
    t_Noeud *newracine = NULL;
    ptrb = index->racine;

    //parcours de l'arbre ancienne
    equilibrer_parcours(ptrb, &newracine);

    //modifier index
    index->racine = newracine;

    return index;
}



/*other functions*/
void make_word_lower(char *mot)
{
    char *ptrchar = mot;
    while (*ptrchar != '\0')
    {
        if (*ptrchar < 91 && *ptrchar > 64)
        {
            *ptrchar = *ptrchar + 32;
        }
        ptrchar++;
    }
}

t_Noeud *recherche_appronfondie(t_Noeud *ptrn, char *word)
{
    if (ptrn == NULL)
    {
        return NULL;
    }
    if (strcmp(word, ptrn->mot) > 0)
    {
        return recherche_appronfondie(ptrn->filsDroit, word);
    }
    else if (strcmp(word, ptrn->mot) < 0)
    {
        return recherche_appronfondie(ptrn->filsGauche, word);
    }
    if (strcmp(word, ptrn->mot) == 0)
    {
        return ptrn;
    }
}

//ajouter un noeud de maniere recurrence
void ajouter_noeud_approfondie(t_Noeud *ptrnb, t_Noeud *nouveau, int *ajouteflag)
{
    if (strcmp(ptrnb->mot, nouveau->mot) < 0)
    {
        if (ptrnb->filsDroit != NULL)
        {
            ajouter_noeud_approfondie(ptrnb->filsDroit, nouveau, ajouteflag);
        }
        if (ptrnb->filsDroit == NULL)
        {
            ptrnb->filsDroit = nouveau;
            *ajouteflag = 1;
        }
    }
    if (strcmp(ptrnb->mot, nouveau->mot) > 0)
    {
        if (ptrnb->filsGauche != NULL)
        {
            ajouter_noeud_approfondie(ptrnb->filsGauche, nouveau, ajouteflag);
        }
        if (ptrnb->filsGauche == NULL)
        {
            ptrnb->filsGauche = nouveau;
            *ajouteflag = 1;
        }
    }
}

//This function if for creation of a new node
t_Noeud *create_noeud(char *mot, t_ListePosition *position)
{
    t_Noeud *nouveau = (t_Noeud *)malloc(sizeof(t_Noeud));
    nouveau->mot = (char *)malloc(MAX_WORD_LENTH);
    strcpy(nouveau->mot, mot);
    nouveau->filsDroit = nouveau->filsGauche = NULL;
    nouveau->positions = position;
    nouveau->nb_occurences = 1;
}

/*Function for treatment of a word when we create a index of a file*/
void traitement_word(t_Index *index, char *word, int line_count, int line_word_order, int phrase_count, int phrase_word_order)
{
    t_Noeud *nouveau, *trouve;
    t_ListePosition *nliste;
    int ajoutflag;

    if (IndexNotFound(index))
    {
        printf("Pas d'index!");
        return;
    }
    if (word == NULL)
    {
        printf("Recherche impossible, probleme avec le mot entre!");
        return;
    }

    trouve = rechercher_mot(index, word);
    if (trouve)
    {
        if (!(ajouter_position(trouve->positions, line_count, phrase_count, line_word_order, phrase_word_order)))
        {
            printf("Echec de l'ajout de la position!");
        }
        trouve->nb_occurences++;
    }
    else if (!trouve)
    {
        nliste = (t_ListePosition *)malloc(sizeof(t_ListePosition));
        nliste->nb_elements = 1;
        nliste->debut = NULL;
        ajouter_position(nliste, line_count, phrase_count, line_word_order, phrase_word_order);
        nouveau = create_noeud(word, nliste);
        ajoutflag = ajouter_noeud(index, nouveau);
        if (!ajoutflag)
        {
            printf("Echec de l'ajout du noeud!");
        }
    }
}

int IndexNotFound(t_Index *ptrindex)
{
    if (ptrindex == NULL)
    {
        printf("Pas d'index!!");
        return 1;
    }
    return 0;
}

int NoeudNotFound(t_Noeud *ptrn)
{
    if (ptrn == NULL)
    {
        printf("Noeud ajoutee n'exits pas!!");
        return 1;
    }
    return 0;
}

void traitementnoeud(char **array, int n_phrase, t_Noeud *noeud_a_traiter)
{
    t_Position *ptrpo;
    ptrpo = noeud_a_traiter->positions->debut;
    while (ptrpo)
    {
        if (ptrpo->numero_phrase == n_phrase)
        {
            array[ptrpo->ordre_phrase] = noeud_a_traiter->mot;
        }
        ptrpo = ptrpo->suivant; /*最后是否为NULL？？*/
    }
}
void parcours(char **array, int n_phrase, t_Noeud *noeud)
{
    if (noeud == NULL)
    {
        return;
    }
    traitementnoeud(array, n_phrase, noeud);
    parcours(array, n_phrase, noeud->filsGauche);
    parcours(array, n_phrase, noeud->filsDroit);
}

void parcours_index(t_Noeud *noeud, int height)
{
    if (noeud == NULL)
    {
        return;
    }
    printf("mot: %s, height: %d\n", noeud->mot, height);
    printf("\t%d left:\n", height);
    parcours_index(noeud->filsGauche, height + 1);
    printf("\t%d right:\n", height);
    parcours_index(noeud->filsDroit, height + 1);
}

void traitementphrase(t_Position *word_position, char **array, t_Noeud *racine)
{
    int n_phrase;
    n_phrase = word_position->numero_phrase;
    parcours(array, n_phrase, racine);
}

//rotation elementaire
void R_Rotate(t_Noeud **p)
{
    t_Noeud *L;
    L = (*p)->filsGauche;
    (*p)->filsGauche = L->filsDroit;
    L->filsDroit = (*p);
    *p = L;
}

//rotation elementaire
void L_Rotate(t_Noeud **p)
{
    t_Noeud *R;
    R = (*p)->filsDroit;
    (*p)->filsDroit = R->filsGauche;
    R->filsGauche = (*p);
    *p = R;
}

void LeftBalance(t_Noeud **ptrt)
{
    t_Noeud *L, *Lr;
    L = (*ptrt)->filsGauche;

    switch (L->bf)
    {
        case 1:
            (*ptrt)->bf = L->bf = 0;
            R_Rotate(ptrt);
        case -1:
            Lr = L->filsDroit;
            switch(Lr->bf)
            {
                case 1:
                    (*ptrt)->bf = -1;
                    L->bf = 0;
                    break;
                case 0:
                    (*ptrt)->bf = L->bf = 0;
                    break;
                case -1:
                    (*ptrt)->bf = 0;
                    L->bf = 1;
                    break;
            }
            Lr->bf = 0;
            L_Rotate(&(*ptrt)->filsGauche);
            R_Rotate(ptrt);
    }
}

void RightBalance(t_Noeud **ptrt)
{
    t_Noeud *R, *Rl;
    R = (*ptrt)->filsDroit;

    switch (R->bf)
    {
        case -1:
            (*ptrt)->bf = R->bf = 0;
            L_Rotate(ptrt);
        case 1:
            Rl = R->filsGauche;
            switch(Rl->bf)
            {
                case 1:
                    (*ptrt)->bf = 0;
                    R->bf = -1;
                    break;
                case 0:
                    (*ptrt)->bf = R->bf = 0;
                    break;
                case -1:
                    (*ptrt)->bf = 1;
                    R->bf = 0;
                    break;
            }
            Rl->bf = 0;
            R_Rotate(&(*ptrt)->filsDroit);
            L_Rotate(ptrt);
    }
}

//cette fonction permet de recreer l'arbre en respectant la propriete d'AVL
int InsertAVL(t_Noeud **ptrt, char *word, int *taller)
{
    if (!*ptrt)
    {
        *ptrt = (t_Noeud *)malloc(sizeof(t_Noeud));
        strcpy( (*ptrt)->mot, word);
        (*ptrt)->filsGauche = (*ptrt)->filsDroit = NULL;
        (*ptrt)->bf = 0;
        *taller = 1;
    }
    else 
    {
        if ( strcmp(word, (*ptrt)->mot) == 0 )   /*noeud deja existe */
        {
            *taller = 0;
            return 0;
        }
        if ( strcmp(word, (*ptrt)->mot) < 0)    /*parcourir a gauche*/
        {
            if ( !InsertAVL(&(*ptrt)->filsGauche, word, taller) )
            {
                return 0;
            }
            if (*taller)
            {
                switch( (*ptrt)->bf )
                {
                    case 1:
                        LeftBalance(ptrt);
                        *taller = 0;
                        break;
                    case 0:
                        (*ptrt)->bf = 1;
                        *taller = 1;
                        break;
                    case -1:
                        (*ptrt)->bf = 0;
                        *taller = 0;
                        break;
                }
            }
        }

        else                      /*parcourir a droit*/
        {
            if ( !InsertAVL(&(*ptrt)->filsDroit, word, taller) )
            {
                return 0;
            }
            if (*taller)
            {
                switch( (*ptrt)->bf )
                {
                    case 1:
                        (*ptrt)->bf = 0;
                        *taller = 0;
                        break;
                    case 0:
                        (*ptrt)->bf = -1;
                        *taller = 1;
                        break;
                    case -1:
                        RightBalance(ptrt);
                        *taller = 0;
                        break;
                }
            }
        }
    }
}

void equilibrer_parcours(t_Noeud *ptr_ancien, t_Noeud **new)
{
    int taller, insert;
    taller = 0;
    if (!ptr_ancien)
    {
        return;
    }
    insert = InsertAVL(new, ptr_ancien->mot, &taller);
    equilibrer_parcours(ptr_ancien->filsGauche, new);
    equilibrer_parcours(ptr_ancien->filsDroit, new);
}

//Cette fonction permet d'afficher toutes les informations sur un noeud et retourne le premier caractère du mot
char* dataNoeud(char* currLettre, t_Noeud* noeud) {
    //On stocke la première lettre du mot
    char* c = noeud->mot[0];
    t_ListePosition* listePos = noeud->positions;
    t_Position* allPos = listePos->debut;

    //on check si la première lettre du mot est différent de la lettre actuelle afin de changer l'affichage lors d'un changement de lettre dans l'alphabet
    if(currLettre != c) {
        currLettre = c;
        printf("\n%c\n", currLettre);
    }
    printf("|-- %s\n", noeud->mot);

    //Cette boucle parcourt la liste des positions du mot afin de les afficher
    while(allPos != NULL) {
        printf("|---- (l:%d, o:%d, p:%d)\n", allPos->numero_ligne, allPos->ordre_ligne, allPos->numero_phrase);
        allPos = allPos->suivant;
    }
    printf("|\n");
    return currLettre;
}


//Cette fonction affiche le menu
void affichageMenu()
{
    printf("Bienvenue\n");
    printf("1 - Charger un fichier\n");
    printf("2 - Caracteristiques de l'index\n");
    printf("3 - Afficher l'index\n");
    printf("4 - Rechercher un mot\n");
    printf("5 - Afficher les occurences d'un mot\n");
    printf("6 - Equilibrer l'index\n");
    printf("7 - Quitter le programme\n");
}

//Cette fonction est le menu dans lequel nous allons intéragir afin d'effectuer des actions
void menuPrincipal(void)
{
    int uinput, instanceImport = 0;
    t_Index *index = (t_Index *)calloc(1, sizeof(t_Index));
    FILE *f;

    //Cette boucle va permettre d'affiche le menu tant que nous ne désirons pas de quitter le programme
    do
    {
        //on va afficher le menu
        affichageMenu();
        //l'utilisateur va entrer un nombre (entre 1 et 7) afin d'effectuer l'action qu'il désire
        scanf("%d", &uinput);
        printf("\n");
        switch (uinput)
        {
        case 1:
        {
            /* Importation d'un fichier */
            char char_input[125];
            //cette boucle est juste là pour permettre à l'utilisateur de rentrer un nom de fichier correct afin de pouvoir importer
            while (1)
            {
                printf("Veuillez saisir le nom du fichier d'une instance:\n");
                scanf("%s", char_input);
                f = fopen(char_input, "r");
                //on check si le fichier existe
                if (f)
                {
                    //création de l'instance
                    fclose(f);
                    int nb_mots_lu = indexer_fichier(index, char_input);
                    printf("Le fichier contient %d mots\n", nb_mots_lu);
                    instanceImport = 1;
                    break;
                }
                else
                {
                    //le fichier n'existe pas, on demande à l'utilisateur s'il veut continuer à chercher ou non
                    printf("Le fichier \"%s\" n'existe pas! Continuer? y/n:", char_input);
                    scanf("%s", char_input);
                    if (!strcmp("y", char_input))
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            break;
        }
        case 2:
        {
            /* Caracteristiques de l'index */

            break;
        }
        case 3:
        {


            break;
        }
        case 4:
        {
            /* Rechercher un mot */

            break;
        }
        case 5:
        {
            /* Afficher les occurences d'un mot */

            break;
        }
        case 6:
        {
            /* Equilibrer l'index */

            break;
        }
        case 7:
        {
            /* Quitter l'application + liberation des resources memoires */

            //on check si on a importé un fichier précédemment
            //if (instanceImport) libererRessource(instance);
            printf("Vous quittez l'application");
            break;
        }
        default:
            printf("Choix invalide !\n\n\n");
        }
    } while (uinput != 7);
}
