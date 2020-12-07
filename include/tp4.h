#ifndef TP3_TP3_H
#define TP3_TP3_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENTH 65
#define MAX_phrase 50

typedef struct Position
{
    int numero_ligne;         // in which line
    int numero_phrase;        // in which phrase
    int ordre_ligne;          // order in line ! mod !
    int ordre_phrase;         // order in phrase ! mod !
    struct Position *suivant; // next appearance
} t_Position;

typedef struct ListesPosition
{
    t_Position *debut;
    int nb_elements;
} t_ListePosition;

typedef struct Noeud
{
    char *mot;
    int nb_occurences;
    t_ListePosition *positions;
    struct Noeud *filsGauche;
    struct Noeud *filsDroit;
    int bf;     // la difference de la hauteur entre le sous-arbre gauche et celle droit
} t_Noeud;

typedef struct Index
{
    t_Noeud *racine;
    int nb_mots_differents;
    int nb_mots_total;
} t_Index;

//STRUCTURE DES ELEMENTS DE LA PILE
typedef struct elem {
    t_Noeud* noeud;
    struct elem* suivant;
} Elem;

//STRUCTURE DE LA PILE
typedef struct pile {
    Elem* firstElem;
} Pile;

//cette fonction créée une liste de position vide
t_ListePosition *creer_liste_positions();

//cette fonction ajoute un nouvel element dans une liste de position triée, ! mod !
int ajouter_position(t_ListePosition *liste_position,
                     int numero_ligne, int numero_phrase,
                     int ordre_ligne, int ordre_phrase);

//cette fonction créée un index vide
t_Index *creer_index();

//cette fonction recherche un mot dans un index
t_Noeud *rechercher_mot(t_Index *index, char *mot);

//cette fonction permet d'ajouter un noeud dans l'index
int ajouter_noeud(t_Index *index, t_Noeud *noeud);

//cette fonction permet d'indexer un fichier lu
int indexer_fichier(t_Index *index, char *filename);

//cette fonction affiche les mots classés par ordre alphabétique
void afficher_index(t_Index *index);

void afficher_occurence_mot(t_Index *index, char *mot);

t_Index *equilibrer_index(t_Index *index);

/*other functions*/
void make_word_lower(char *mot);

void parcours_index(t_Noeud *noeud, int height);

t_Noeud *recherche_appronfondie(t_Noeud *ptrn, char *word);

int IndexNotFound(t_Index *ptrindex);

int NoeudNotFound(t_Noeud *ptrn);

t_Noeud *create_noeud(char *mot, t_ListePosition *position);

void ajouter_noeud_approfondie(t_Noeud *racine, t_Noeud *nouveau, int *ajouteflag);

void traitement_word(t_Index *index, char *word, int line_count, int line_word_order, int phrase_count, int phrase_word_order);

void traitementnoeud(char **array, int n_phrase, t_Noeud *noeud_a_traiter);

void parcours(char **array, int n_phrase, t_Noeud *noeud);

void traitementphrase(t_Position *word_position, char **array, t_Noeud *racine);

Pile* creer_pile();

//Cette fonction affiche le menu
void affichageMenu();

void menuPrincipal(void);
#endif //TP3_TP3_H
