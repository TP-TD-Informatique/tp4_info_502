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

%left rePLUS reDOT

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
    |   expr                { print_C_regex($1); }

expr:
        reZERO              { $$ = zero(); }
    |   reONE               { $$ = one(); }
    |   reSYMB              { $$ = symbol($1); }
    |   LPAR expr RPAR      { $$ = $2; }
    |   expr rePLUS expr    { $$ = plus($1, $3); }
    |   expr reDOT expr     { $$ = cat($1, $3); }
    |	expr reSTAR	    { $$ = star($1); }
