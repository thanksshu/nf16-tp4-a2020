#ifndef TP3_TP3_H
#define TP3_TP3_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_position
{
    int numero_ligne;           // in which line
    int numero_phrase;          // in which phrase
    int ordre_ligne;            // order in line !mod!
    int ordre_phrase;           // order in phrase !mod!
    struct t_position *suivant; // next appearance
} t_Position;

typedef struct t_ListesPosition
{
    t_Position *debut;
    int nb_elements;
} t_ListePosition;

typedef struct t_noeud
{
    char *mot;
    int nb_occurences;
    t_ListePosition *positions;
    t_Noeud *filsGauche;
    t_Noeud *filsDroit;
} t_Noeud;

typedef struct t_index
{
    t_Noeud *racine;
    int nb_mots_differents;
    int nb_mots_total;
} t_Index;

//cette fonction créée une liste de position vide
t_ListePosition *creer_liste_positions();

//cette fonction ajoute un nouvel element dans une liste de position triée, !mod!
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

/*other functions*/
void make_word_lower(char *mot);

t_Noeud *recherche_appronfondie(t_Noeud *ptrnoeud, char *word);

//Cette fonction affiche le menu
void affichageMenu();

void menuPrincipal(void);
#endif //TP3_TP3_H
