/* Initial C code */
%{
#include "y.tab.h"
#include <variant>
%}
%option noyywrap

ID [a-zA-Z][a-zA-Z0-9_]*


%%
[ \t\n ]+ ;

\-?[0-9]+ |
\-?[0-9]+\. |
\-?[0-9]+\.[0-9]+ |
\-?\.[0-9]+ { yylval.emplace<double>(std::atof(yytext)); return DOUBLE;}

"//".* { return COMMENT;}

"constants" {return CONSTANTS;}

"torus" { return TORUS;}
"sphere" { return SPHERE;}
"box" {return BOX;}
"line" {return LINE;}
"move" {return MOVE;}
"scale" {return SCALE;}
"rotate" {return ROTATE;}
"basename" {return BASENAME;}
"frames" {return FRAMES;}
"vary" {return VARY;}
"push" {return PUSH;}
"pop" {return POP;}
"save" {return SAVE;}
"display" {return DISPLAY;}

[a-zA-Z][\.a-zA-Z0-9_]* { yylval.emplace<std::string>(yytext); return STRING;}
%%
