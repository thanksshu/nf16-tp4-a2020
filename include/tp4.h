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
    int balance_factor;         // left tree height - right tree height
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

/**
 * @param balance create a AVL or not
 */
int ajouter_noeud(t_Index *index, t_Noeud *noeud, int balance);

/**
 * @param balance create a AVL or not
 */
int indexer_fichier(t_Index *index, char *filename);

void afficher_index(t_Index *index);

void afficher_occurence_mot(t_Index *index, char *mot);

/**
 * @param index the index to balance, will be freed when executing
 * @return new index if success, NULL if not
 */
t_Index *equilibrer_index(t_Index *index);

/* other functions */
/** free a tree with node as its root
 * @param node pointer of root node to start cleaning
 */
void _free_tree(t_Noeud *node);

/** add a node in a tree
 * @param ptr_root pointer of root node address to start being added
 * , using double pointer to make root node can be replaced, used for rotation
 * @param result final add result, -1 add duplicated, 1 success, 0 failed
 * @param self_grow if node_self grows higher, 0 no, 1 grow
 * @param balance 1 to balance, 0 no balance
 */
void _add_node(t_Noeud **ptr_root, t_Noeud *new,
               int *result, int *self_grow, int balance);

/** 
 * balance a minimal imbalanced BST
 * @param ptr_root the pointer of root node
 * , using double pointer to make root node can be replaced, used for rotation
 * @param result as return value: balanced(1) or not(0)
 */
void _balance_tree(t_Noeud **ptr_root, int *result);

void _rotate_right(t_Noeud **ptr_root);

void _rotate_left(t_Noeud **ptr_root);

/** 
 * search a node using a word
 * @param node pointer of root node to start searching
 * @return returns the found node, NULL for not found
 */
t_Noeud *_search_word(t_Noeud *node, char *word);

/**
 *  print out (almost) all infomation of a node recusively
 * @param node pointer of root node to start printing
 * @param first_char previous word's first character
 */
void _print_tree(t_Noeud *node, char *pre_first_char);

/** 
 * use inside of afficher_occurence_mot, to form a phrase recusively
 * @param node pointer of root node to start forming
 */
void _make_phrase(t_Noeud *node, int num_phrase,
                  char *array_phrase[MAX_PHRASE_LENTH]);

/**
 * recusively check balance factor in a tree
 * @return 1 if balance, 0 if not
 */
int _check_balance(t_Noeud *root);

/**
 * recusively build avl according to the old tree
 * @param root old tree root
 * @param index empty index for the avl
 * @return 1 if build success, 0 if not
 */
int _build_avl(t_Noeud **root, t_Index *index);
