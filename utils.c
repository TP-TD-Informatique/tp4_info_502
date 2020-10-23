/**********************************************************************
 *** TP info502 (théorie des langages)
 **********************************************************************/
#include "regex.h"

/*
 * fonctions auxiliaires pour créer des regex
 */

regex zero() {
    regex zero = GC_MALLOC(sizeof(struct regex_s));
    zero->regex_type = ZERO;
    zero->first_son = NULL;
    zero->second_son = NULL;
    zero->symbol = 0;
    return zero;
}

regex one() {
    regex one = GC_MALLOC(sizeof(struct regex_s));
    one->regex_type = ONE;
    one->first_son = NULL;
    one->second_son = NULL;
    one->symbol = 0;
    return one;
}

regex symbol(char c) {
    regex symb = GC_MALLOC(sizeof(struct regex_s));
    symb->regex_type = SYMBOL;
    symb->first_son = NULL;
    symb->second_son = NULL;
    symb->symbol = c;
    return symb;
}

regex plus(regex r1, regex r2) {
    regex r = GC_MALLOC(sizeof(struct regex_s));
    r->regex_type = PLUS;
    r->first_son = r1;
    r->second_son = r2;
    r->symbol = 0;
    return r;
}

regex cat(regex r1, regex r2) {
    regex r = GC_MALLOC(sizeof(struct regex_s));
    r->regex_type = CONCAT;
    r->first_son = r1;
    r->second_son = r2;
    r->symbol = 0;
    return r;
}

regex star(regex r1) {
    regex r = GC_MALLOC(sizeof(struct regex_s));
    r->regex_type = STAR;
    r->first_son = r1;
    r->second_son = NULL;
    r->symbol = 0;
    return r;
}

/*
void free_regex(regex r) {
    switch (r->regex_type){
        case ZERO:
        case ONE:
        case SYMBOL:
            free(r);
            return;
            break;
        case PLUS:
        case CONCAT:
            free_regex(r->first_son);
            free_regex(r->second_son);
            free(r);
            return;
            break;
        case STAR:
            free_regex(r->first_son);
            free(r);
            break;
        default:
            return;
    }
}

regex copy_regex(regex r) {
    regex r1, r2;
    switch (r->regex_type){
        case ZERO:
            return zero();
            break;
        case ONE:
            return one();
            break;
        case SYMBOL:
            return symbol(r->symbol);
            break;
        case PLUS:
            r1 = copy_regex(r->first_son);
            r2 = copy_regex(r->second_son);
            return plus(r1, r2);
            break;
        case CONCAT:
            r1 = copy_regex(r->first_son);
            r2 = copy_regex(r->second_son);
            return cat(r1, r2);
            break;
        case STAR:
            r1 = copy_regex(r->first_son);
            return star(r1);
            break;
        default:
            return NULL;
    }
}
*/

/* compare deux arbres syntaxiques de regex */
int regex_equal(regex r1, regex r2) {
    switch (r1->regex_type){
        case ZERO:
            return r2->regex_type == ZERO;
            break;
        case ONE:
            return r2->regex_type == ONE;
            break;
        case SYMBOL:
            return r2->regex_type == SYMBOL && r2->symbol == r1->symbol;
            break;
        case PLUS:
            return r2->regex_type == PLUS &&
                   regex_equal(r1->first_son, r2->first_son) &&
                   regex_equal(r1->second_son, r2->second_son);
            break;
        case CONCAT:
            return r2->regex_type == CONCAT &&
                   regex_equal(r1->first_son, r2->first_son) &&
                   regex_equal(r1->second_son, r2->second_son);
            break;
        case STAR:
            return r2->regex_type == STAR &&
                   regex_equal(r1->first_son, r2->first_son);
            break;
        default:
            return -1;
    }
}


/* vérifie si une regex apparait dans un tableau (pour l'égalité des arbres
 * syntaxiques */
static int _inside(regex r, regex *T, int nb_regex) {
    for (int i=0; i<nb_regex; i++) {
        if (regex_equal(r, T[i])) return 1;
    }
    return 0;
}

/* récupères tous les termes d'une somme dans un tableau:
 *   r est la regex qu'il faut séparer en morceaux
 *   T est le tableau des termes
 *   nb_regex est le nombre d'éléments dans le tableau
 *   sizeT est la taille utilisable du tableau */
/* NOTE: mutuellement recursive avec simplify_better */
static void _get_terms(regex r, regex **T, int *nb_regex, int *sizeT) {
    switch (r->regex_type){
        case ZERO:
            break;
        case PLUS:
            _get_terms(r->first_son, T, nb_regex, sizeT);
            _get_terms(r->second_son, T, nb_regex, sizeT);
            break;
        default:
            r = simplify_better(r);
            if (!_inside(r, *T, *nb_regex)) {
                if (*nb_regex >= *sizeT) {
                    *sizeT *= 2;
                    /* printf("%p -> ", *T); */
                    *T = GC_REALLOC(*T, *sizeT * sizeof(regex));
                    /* printf("%p\n", *T); */
                }
                (*T)[*nb_regex] = r;
                *nb_regex = *nb_regex+1;
            }
            return;
    }
}

/* simplifie une somme en supprimant les morceaux qui apparaissent plusieurs
 * fois */
static regex simplify_sum(regex r) {
    int nb_regex = 0;
    int sizeT = 2;
    regex *T = GC_MALLOC(sizeT * sizeof(regex));
    regex result;
    _get_terms(r, &T, &nb_regex, &sizeT);
    if (nb_regex == 0) {
        result = zero();
    } else {
        result = T[0];
        for (int i=1; i<nb_regex; i++) {
            result = plus(result, T[i]);
        }
    }
    return result;
}

/* fonction principale : simplifie une regex : en utilisant
 *    0* = 1* = 1
 *    R** = R*
 *    R.0 = 0.R = 0
 *    1.R = R.1 = R
 *    R + S = = S + R
 *    R + R = R
 *    R + 0 = R
 * Attention, toutes les simplifications ne sont pas faites.
 */
/* NOTE : mutuellement récursive avec _get_terms */
regex simplify_better(regex r) {
    if (r == NULL) {
        fprintf(stderr, "ERREUR dans simplify_better\n");
        exit(1);
    };
    regex r1, r2;
    switch (r->regex_type){
        case ZERO:
        case ONE:
        case SYMBOL:
            return r;
            break;
        case PLUS:
            return simplify_sum(plus(r->first_son,r->second_son));
            break;
        case CONCAT:
            r1 = simplify_better(r->first_son);
            r2 = simplify_better(r->second_son);
            if (r1->regex_type == ZERO || r2->regex_type == ZERO) {
                return zero();
            } else if (r1->regex_type == ONE) {
                return r2;
            } else if (r2->regex_type == ONE) {
                return r1;
            } else {
                return cat(r1, r2);
            }
            break;
        case STAR:
            r1 = simplify_better(r->first_son);
            if (r1->regex_type == ZERO) {
                return one();
            } else if (r1->regex_type == ONE) {
                return r1;
            } else if (r1->regex_type == STAR) {
                return r1;
            } else {
                return star(r1);
            }
            break;
        default:
            fprintf(stderr, "ERREUR dans simplify_better\n");
            exit(1);
    }
}


void print_C_regex(regex r) {
    if (r == NULL) {
        fprintf(stderr, "ERREUR dans print_C_regex\n");
        exit(1);
    };
    switch (r->regex_type){
        case ZERO:
            printf("zero()");
            break;
        case ONE:
            printf("one()");
            break;
        case SYMBOL:
            printf("symbol('%c')", r->symbol);
            break;
        case PLUS:
            printf("add(");
            print_C_regex(r->first_son);
            printf(",");
            print_C_regex(r->second_son);
            printf(")");
            break;
        case CONCAT:
            printf("cat(");
            print_C_regex(r->first_son);
            printf(",");
            print_C_regex(r->second_son);
            printf(")");
            break;
        case STAR:
            printf("star(");
            print_C_regex(r->first_son);
            printf(")");
            break;
        default:
            fprintf(stderr, "ERREUR dans print_C_regex\n");
            exit(1);
    }
}
