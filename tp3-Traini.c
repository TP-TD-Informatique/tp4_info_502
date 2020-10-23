/**********************************************************************
 *** fichier à rendre pour le TP3 d'info502 (théorie des langages)
 *** étudiants: Roudoudou et compagnie
 **********************************************************************/


#include "regex.h"

/* affichage naif d'une regex */
void print_regex(regex r) {
    if (r == NULL) {
        fprintf(stderr, "ERREUR dans print_regex\n");
        exit(1);
    };
    switch (r->regex_type){
        case ZERO:
            printf("0");
            break;
        case ONE:
            printf("1");
            break;
        case SYMBOL:
            putchar(r->symbol);
            break;
        case PLUS:
            printf("(");
            print_regex(r->first_son);
            printf(") + (");
            print_regex(r->second_son);
            printf(")");
            break;
        case CONCAT:
            printf("(");
            print_regex(r->first_son);
            printf(").(");
            print_regex(r->second_son);
            printf(")");
            break;
        case STAR:
            printf("(");
            print_regex(r->first_son);
            printf(")*");
            break;
        default:
            fprintf(stderr, "ERREUR dans print_regex\n");
            exit(1);
    }
}

/* renvoie 1 si une regex contient le mot vide, et 0 sinon */
int contains_epsilon(regex r) {
    return -1;
}

/* calcul la dérivé d'une regex selon un symbol */
regex derivative(regex r, char c) {
    return NULL;
}

/* renvoie 1 si une regex contient une chaine, et 0 sinon */
int match(regex r, char* s) {
    return -1;
}


/* renvoie 1 si le langage d'une regex est vide, et 0 sinon */
int empty(regex r) {
    return -1;
}


/* renvoie 1 si le langage d'une regex est infini, et 0 sinon */
int infinite(regex r) {
    return -1;
}


/* calcule une regex qui reconnait le renversé d'une autre regex */
regex reverse(regex r) {
    return NULL;
}


/* calcule une regex qui reconnait les préfixes d'une autre regex */
regex prefix(regex r) {
    return NULL;
}

/* calcule une regex qui reconnait les suffixes d'une autre regex */
regex suffix(regex r) {
    return NULL;
}


/* la fonction de test qui est appelée lorsqu'aucun argument n'est donné sur
 * la ligne de commandes */
void test() {
    printf("Fonction de test...\n");
    printf("Il faut faire quelque chose de plus intéressant...\n");
    printf("(à définir dans le fichiers tp3-???.c)\n");
}
