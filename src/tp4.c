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
    if (IndexNotFound(index))
    {
        return NULL;
    }
    if (mot == NULL)
        ;
    {
        printf("Recherche impossible, probleme avec le mot entre!");
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
    ajouter_noeud_approfondie(index->racine, noeud, &flag);
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

            // word end
            word_count += 1;
            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word
            break;
        case '.':
            // is space
            printf("%s\n", word);

            // word end
            word_count += 1;
            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word

            // phrase ends
            phrase_count += 1;
            phrase_word_order = 0;

            // skip next space
            character = (char)fgetc(file);
            break;
        case '\n':
            // is new line
            printf("%s\n", word);

            // word ends
            word_count += 1;
            line_word_order += 1;
            phrase_word_order += 1;
            memset(word, '\0', sizeof(word)); // reset word

            // line ends
            line_count += 1;
            line_word_order = 0;
            break;
        default:
            // a character in a word
            word[strlen(word)] = character;
            break;
        }

        // read a new character
        character = (char)fgetc(file);
    }

    return word_count;
}

//cette fonction affiche les mots classés par ordre alphabétique
void afficher_index(t_Index *index)
{
}

/*other functions*/
void make_word_lower(char *mot)
{
    char *ptrchar;
    strcpy(ptrchar, mot);
    while (*ptrchar != '\0')
    {
        if (*ptrchar < 91 && *ptrchar > 64)
        {
            *ptrchar = *ptrchar + 32;
        }
        ptrchar++;
    }
}

t_Noeud *recherche_appronfondie(t_Noeud *ptrnoeud, char *word)
{
    t_Noeud *ptrn;
    char *copyword;

    strcpy(copyword, word);
    make_word_lower(copyword);
    if (strcmp(copyword, ptrn->mot) > 0)
    {
        return recherche_appronfondie(ptrn->filsDroit, word);
    }
    else if (strcmp(copyword, ptrn->mot) < 0)
    {
        return recherche_appronfondie(ptrn->filsGauche, word);
    }
    if (strcmp(copyword, ptrn->mot) == 0)
    {
        return ptrn;
    }
}

//ajouter un noeud de maniere recurrence
void ajouter_noeud_approfondie(t_Noeud *ptrnb, t_Noeud *nouveau, int *ajouteflag)
{
    if (ptrnb->mot < nouveau->mot)
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
    if (ptrnb->mot > nouveau->mot)
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

//This function for creation of a new node
t_Noeud *create_noeud(char *mot, t_ListePosition *position)
{
    t_Noeud *nouveau = (t_Noeud *)malloc(sizeof(t_Noeud));
    strcpy(nouveau->mot, mot);
    nouveau->filsDroit = nouveau->filsGauche = NULL;
    nouveau->positions = position;
    nouveau->nb_occurences = 0;
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

            //cette boucle est juste là pour permettre à l'utilisateur de rentrer un nom de fichier correct afin de pouvoir importer
            while (1)
            {
                char char_input[64];
                printf("Veuillez saisir le nom du fichier d'une instance:\n");
                scanf("%s", char_input);
                f = fopen(char_input, "r");
                //on check si le fichier existe
                if (f)
                {
                    //création de l'instance
                    fclose(f);
                    printf("Importation du fichier effectue\n");
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
            /* Afficher l'index */

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
