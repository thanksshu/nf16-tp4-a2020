#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENTH 64
#define MAX_PHRASE_LENTH 128

typedef struct Position
{
    int numero_ligne;         // in which line, start with 0
    int numero_phrase;        // in which phrase, start with 0
    int ordre_ligne;          // order in line, start with 0
    int ordre_phrase;         // order in phrase, start with 0
    struct Position *suivant; // next position
} t_Position;

typedef struct ListesPosition
{
    t_Position *debut; // first position
    int nb_elements;   // number of positions in list
} t_ListePosition;

typedef struct Noeud
{
    char *mot;                  // word of this node
    int nb_occurences;          // number of duplicated words
    t_ListePosition *positions; // list of positions
    struct Noeud *filsGauche;   // left child
    struct Noeud *filsDroit;    // right child
    int balance;                // left tree height - right tree height
} t_Noeud;

typedef struct Index
{
    t_Noeud *racine;        // root
    int nb_mots_differents; // number of words in tree
    int nb_mots_total;      // number of words in file
} t_Index;

t_ListePosition *creer_liste_positions();

int ajouter_position(t_ListePosition *listeP,
                     int numero_ligne, int numero_phrase,
                     int ordre_ligne, int ordre_phrase);

t_Noeud *rechercher_mot(t_Index *index, char *mot);

int ajouter_noeud(t_Index *index, t_Noeud *noeud);

int indexer_fichier(t_Index *index, char *filename);

void afficher_index(t_Index *index);

void afficher_occurence_mot(t_Index *index, char *mot);

// TODO:
t_Index *equilibrer_index(t_Index *index);

/* other functions */
// free a tree with node as its root
void _free_btree(t_Noeud *node);

// add a node in a tree
int _add_node(t_Noeud *node_root, t_Noeud *node_new);

// search a node using a word
t_Noeud *_search_word(t_Noeud *node, char *word);

// print out (almost) all infomation of a node
void _print_node(t_Noeud *noeud, char *first_char);

// use inside of afficher_occurence_mot, to form a phrase
void _make_phrase(t_Noeud *node, int num_phrase,
                  char *array_phrase[MAX_PHRASE_LENTH]);