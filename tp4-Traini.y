%{
#include "regex.h"

int yylex();
void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
}

%}

/* les types des non-terminaux */
%union {
    regex regex;        /* pour les regex */
    char symbol;        /* pour les symboles */
    char* str;          /* pour les chaines */
}

/* déclaration des tokens correspondant aux regex */
%token reZERO reONE rePLUS reDOT reSTAR
%token <symbol> reSYMB

/* autres tokens */
%token LPAR RPAR    /* parenthèses */
%token NL END       /* fin de ligne et fin de fichier */
%token <str> STR    /* chaines de symboles, avec guillemets */
%token QUESTION     /* caractère '?' point d'intérogation */
%token reDERIVATE   /* pour la dérivation, caractère '/' */
%token IN	    /* permet de tester si une chaine IN une regex */

%right rePLUS
%left reDOT
%right reSTAR
%left reDERIVATE
%left QUESTION

%type <regex> expr
%type <regex> term
%type <regex> atom

%start toplevel

%%

toplevel:
        END                 { printf("\n"); exit(0); }
    |   line toplevel

line:
    cmd NL                  { printf("\n# "); }

cmd:
                            { printf("?"); }
    |   expr                { print_regex($1); }
    |	STR IN expr	    { if (match($3, $1)) { printf("TRUE"); } else { printf("FALSE"); } } /* match ne fonctionne pas, donc le résultat ici est incorrect */
    |	expr reDERIVATE STR { print_regex(derivate($1, $3)); } /* Ajouté ici à la question 10 */

/*
expr:
        reZERO              { $$ = zero(); }
    |   reONE               { $$ = one(); }
    |   reSYMB              { $$ = symbol($1); }
    |   LPAR expr RPAR      { $$ = $2; }
    |	expr QUESTION	    { $$ = plus($1, one()); }
    |   expr rePLUS expr    { $$ = plus($1, $3); }
    |   expr reDOT expr     { $$ = cat($1, $3); }
    |	expr reSTAR	    { $$ = star($1); }
    |	expr reDERIVATE STR { $$ = derivate($1, $3); }
    |	reSYMB expr	    { $$ = cat(symbol($1), $2); }
    |	expr reSYMB	    { $$ = cat($1, symbol($2)); }
*/

expr:
        expr rePLUS expr    { $$ = plus($1, $3); }
    |	expr reDOT expr	    { $$ = cat($1, $3); }
    |	LPAR expr RPAR	    { $$ = $2; }
    |	expr reSTAR	    { $$ = star($1); }
    |	expr QUESTION	    { $$ = plus($1, one()); }
    |	term		    { $$ = $1; }

term:
   	atom reDOT atom     { $$ = cat($1, $3); }
    |	atom term	    { $$ = cat($1, $2); }
    |	atom		    { $$ = $1; }

atom:
        reZERO              { $$ = zero(); }
    |   reONE               { $$ = one(); }
    |   reSYMB              { $$ = symbol($1); }
    |	atom reSTAR	    { $$ = star($1); }

/*
 * QUESTIONS
 *
 * Question 3 :
 * %right rePLUS reDOT lis a+b+c de cette façon -> a+(b+c)
 * %left rePLUS reDOT lis a+b+c de cette façon -> (a+b)+c
 * Le côté que l'on renseigne après le %, est celui qui sera interpreté en premier par Yacc
 * Donc par exemple a+b.c donne a + (b . c) si on met %right rePLUS et (a) + b.c si on met %left rePLUS.
 * Les parenthèses sont là pour désigner la partie qui est interprétée en premier
 *
 * Question 10 :
 * Les résultats des tests sont :
 * >>> a+bc
 * a + bc
 * # ab+c
 * ab + c
 * # abc*
 * abc*
 * # a.b*c+d
 * ab*c + d
 * # (ab+c)*d+e
 * (ab + c)*d + e
 * #
 */