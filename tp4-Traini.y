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
%left reSTAR
%right reSYMB
%right LPAR
%left RPAR
%left reDERIVATE
%left QUESTION

%type <regex> expr

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
    |	STR IN expr	    { if (match($3, $1)) { printf("TRUE"); } else { printf("FALSE"); } }


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
