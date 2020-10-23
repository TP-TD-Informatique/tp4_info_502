/**********************************************************************
 *** fichier à rendre pour le TP3 d'info502 (théorie des langages)
 *** lexer
 *** étudiant: Traini
 **********************************************************************/

%{
#include "regex.h"
#include "y.tab.h"

void yyerror(char *s);
%}

%option noyywrap

/* des expressions régulières pour repérer certains token */
symbol [a-z]
str \"[a-z]*\"


%%

"+"         return rePLUS;
"."         return reDOT;
"*"         return reSTAR;
"("         return LPAR;
")"         return RPAR;
"0"         return reZERO;
"1"         return reONE;
{symbol}    {yylval.symbol = yytext[0]; return reSYMB;}
{str}       { unsigned n = strlen(yytext);
              char *s = GC_MALLOC(n-1);
              strncpy(s, yytext+1, n-2);
              s[n-1] = '\0';
              yylval.str = s; return STR;}
" " ;       /* on ignore les espaces */
"\n"        return NL;
<<EOF>>     return END;
"?"         return QUESTION;


.           yyerror("caractère inconnu");

