#include "../include/TP3.h"

/*
Conseil pour la manipulation des chaines de caractères.
Les bibliothèques C contiennent plusieurs fonctions qui vous seront utiles lors du TP, vous avez <ctype.h> pour les fonctions sur les caractères et <string.h> pour les chaines de caractères.
Avant d'implémenter vos propres traitements sur les chaines de caractères,  vérifiez s'il existe des fonctions C qui le font. Ca vous éviteras du travail en plus.
Si elles ne sont pas dans ces deux bibliothèques, elle peuvent être dans une autre.
Google, stackoverflow et la documentation du langage C sont vos amis
https://fr.cppreference.com/w/c
Lien vers la documentation C
*/

//cette fonction créée une liste de positions vide
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

//cette fonction ajoute un nouvel element dans une liste de position triée
int ajouter_position(t_ListePosition *listeP, int ligne, int ordre, int num_phrase)
{
    t_ListePosition *list = listeP;
    t_Position *newPos = (t_Position *)malloc(sizeof(t_Position));
    if (!newPos)
    {
        printf("Erreur de creation du nouvel element! !\n");
        return 0;
    }
    newPos->numero_ligne = ligne;
    newPos->numero_phrase = num_phrase;
    newPos->ordre = ordre;
    //newPos->suivant

    return 1;
}

//cette fonction créée un index vide
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
    t_Noeud * ptrnoeud;
    if (IndexNotFound(index))
    {
        return NULL;
    }
    if (mot == NULL);
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
int indexer_fichier(t_Index *index, char *filename)
{
    return;
}

//cette fonction affiche les mots classés par ordre alphabétique
void afficher_index(t_Index *index)
{
}

/*other functions*/
void make_word_lower(char *mot)
{
    char *ptrchar;
    ptrchar = mot;
    while (ptrchar != '/0')
    {
        if (*ptrchar < 91 && *ptrchar > 64)
        {
            *ptrchar = *ptrchar + 32;
        }
        ptrchar ++;
    }
}

t_Noeud *recherche_appronfondie(t_Noeud *ptrnoeud, char *word)
{
    t_Noeud * ptrn;
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
t_Noeud *create_noeud(char *mot,t_ListePosition *position)
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
