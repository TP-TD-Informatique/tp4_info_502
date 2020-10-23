/**********************************************************************
 *** TP info502 (théorie des langages)
 **********************************************************************/

#ifndef _REGEX_H
#define _REGEX_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>


/*
 * type pour les regex
 */
/* les 6 différents types de regex à la Kleene */
enum CASE_REGEX {
    ZERO,
    ONE,
    SYMBOL,
    PLUS,
    CONCAT,
    STAR
};

/* une regex est une structure arborescente
 * chaque noeud a un type (voir au dessus)
 * pour une somme ou un produit, il y a deux fils
 * pour une etoile, il n'y a qu'un seul fils (le deuxième est NULL)
 * pour un symbole, il n'y a pas de fils et le champs "symbol" contient ... le * symbol
 * pour 0 et 1, il n'y a pas de fils, et le champs "symbol" contient 0 */
typedef struct regex_s {
    enum CASE_REGEX regex_type;
    struct regex_s *first_son;
    struct regex_s *second_son;
    char symbol;
} *regex;


/*
 * des constructeurs pour créer plus facilement des regex
 */
regex zero() ;
regex one() ;
regex symbol(char c) ;
regex plus(regex r1, regex r2) ;
regex cat(regex r1, regex r2) ;
regex star(regex r1) ;

/*
 * des constructeurs intellingent pour créer des regex
 */
regex smart_plus(regex r1, regex r2) ;
regex smart_cat(regex r1, regex r2) ;
regex smart_star(regex r1) ;

/* libérer l'espace mémoire d'une regex */
/* void free_regex(regex r); */
/* recopier une regex */
/* regex copy_regex(regex r); */

/*
 * autres fonctions utilitaires
 */
/* compare les arbres syntaxiques de deux regex */
int regex_equal(regex r1, regex r2) ;

/* simplifie une regex en utilisant les égalités de base */
regex simplify(regex r) ;

/* simplifie une regex en utilisant les égalités de base et la commutativité /
 * idempotence de l'addition */
regex simplify_better(regex r) ;


/*
 * à écrire
 */
/* affiche une regex */
void print_regex(regex r) ;
void print_C_regex(regex r) ;

/* renvoie 1 si une regex contient le mot vide, et 0 sinon */
int contains_epsilon(regex r) ;

/* calcul la dérivé d'une regex selon un symbol */
regex derivative(regex r, char c) ;

/* calcul de la dérivé d'une regex selon une chaine de caractères */
regex derivate(regex r, char *str);

/* renvoie 1 si une regex contient une chaine, et 0 sinon */
int match(regex r, char* s) ;

/* renvoie 1 si le langage d'une regex est vide, et 0 sinon */
int empty(regex r) ;

/* renvoie 1 si le langage d'une regex est infini, et 0 sinon */
int infinite(regex r);

/* calcule une regex qui reconnait le renversé d'une autre regex */
regex reverse(regex r);

/* calcule une regex qui reconnait les préfixes d'une autre regex */
regex prefix(regex r);

/* calcule une regex qui reconnait les suffixes d'une autre regex */
regex suffix(regex r);

/* calcule une regex qui reconnait les sous-chaines d'une autre regex */
regex subword(regex r);

/* la fonction de test qui est appelée lorsqu'aucun argument n'est donné sur
 * la ligne de commandes */
void test();

#endif  /* _REGEX_H */
