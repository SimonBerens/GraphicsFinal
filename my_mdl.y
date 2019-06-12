%require "3"
%language "c++"

%{
  /* C declarations */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #include <variant>
    #include "parsing/symt.h"
    #include "parsing/cmd.h"
    #include "parsing/sym.h"
    #include "matrices/m_matri.h"
    #include "matrices/rm.h"
    #include "matrices/sm.h"
    #include "matrices/tm.h"

    extern int yylex();
    extern int yyparse();
    extern FILE *yyin;
    void yyerror(char const *s);

    #define YYERROR_VERBOSE 1

    Parser p;
    int lineno=0;
%}


/* Bison Declarations */
%define api.value.type {std::variant<double, std::string>}

%token COMMENT
%token <double> DOUBLE
%token <std::string> STRING
%token <std::string> CONSTANTS SPHERE TORUS BOX LINE MOVE SCALE ROTATE BASENAME FRAMES VARY PUSH POP SAVE DISPLAY
%%
/* Grammar rules */

input:
| input command
;

command:

COMMENT {}|

SPHERE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<1>, DRAW_SPHERE( "", {$2, $3, $4}, $5)}); // todo change types
}|
SPHERE STRING DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<1>, DRAW_SPHERE( $2, {$3, $4, $5}, $6)});
}|
TORUS DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<2>, DRAW_TORUS( "", {$2, $3, $4}, $5, $6)});
}|
TORUS STRING DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<2>, DRAW_TORUS( $2, {$3, $4, $5}, $6, $7)});

}|
BOX DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<3>, DRAW_BOX( "", {$2, $3, $4}, $5, $6, $7)});
}|
BOX STRING DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<3>, DRAW_BOX( $2, {$3, $4, $5}, $6, $7, $8)});
}|
LINE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    Command c;
    p.add_command(Command{in_place_index<4>, LINE( {$2, $3, $4}, {$5, $6, $7})});
}|
MOVE DOUBLE DOUBLE DOUBLE STRING
{
    lineno++;
    p.add_command(Command{in_place_index<5>, MOVE( {{$2, $3, $4}}, $5)});
}|
MOVE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<5>, MOVE( {{$2, $3, $4}}, "")});
}|

SCALE DOUBLE DOUBLE DOUBLE STRING
{
    lineno++;
    p.add_command(Command{in_place_index<6>, SCALE( {{$2, $3, $4}}, $5)});

}|
SCALE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<6>, SCALE( {{$2, $3, $4}}, "")});
}|
ROTATE STRING DOUBLE STRING
{
    lineno++;
    RM::Axis axis;
    switch (*$2) {
        case 'x':
        case 'X':
            axis = RM::X;
            break;
        case 'y':
        case 'Y':
            axis = RM::Y;
            break;
        case 'z':
        case 'Z':
            axis = RM::Z;
            break;
    }
    p.add_command(Command{in_place_index<7>, ROTATE( axis, $3, $4)});
}|
ROTATE STRING DOUBLE
{
    lineno++;
    RM::Axis axis;
    switch (*$2) {
        case 'x':
        case 'X':
            axis = RM::X;
            break;
        case 'y':
        case 'Y':
            axis = RM::Y;
            break;
        case 'z':
        case 'Z':
            axis = RM::Z;
            break;
    }
    p.add_command(Command{in_place_index<7>, ROTATE( axis, $3, "")});
}|
PUSH
{
    lineno++;
    p.add_command(Command{in_place_index<8>, PUSH()});
}|
POP
{
    lineno++;
    p.add_command(Command{in_place_index<9>, POP()});

}|
DISPLAY
{
    lineno++;
    p.add_command(Command{in_place_index<10>, DISPLAY()});

}|

SAVE STRING
{
    lineno++;
    p.add_command(Command{in_place_index<14>, SAVE( $2)});
}|
CONSTANTS STRING DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE DOUBLE
{
    lineno++;
    Command c{in_place_index<0>, Surface{{{$3, $4, $5}}, {{$6, $7, $8}}, {{$9, $10, $11}}}};
    p.add_command(c);
    p.add_symbol($2, SymData{in_place_index<0>, c});
}|
BASENAME STRING
{
    lineno++;
    p.add_command(Command{in_place_index<11>, SET_BASENAME( $2)});
}|
FRAMES DOUBLE
{
    lineno++;
    p.add_command(Command{in_place_index<12>, SET_FRAMES( $2)});
}|
VARY STRING DOUBLE DOUBLE DOUBLE DOUBLE
{

    lineno++;
    Command c{in_place_index<13>, VARY{$2, $3, $4, $5, $6}};
    p.add_command(c);
//    SymData sd;
//    sd.t = SymData::SURFACE;
//    sd.surface = {c};
//    p.add_symbol($2, sd);
// todo does this matter?
};
%%

/* Other C stuff */
void yyerror(char const *s)
{
  printf("Error in line %d:%s\n",lineno,s);
}


int main(int argc, char **argv) {

  yyin = fopen(argv[1],"r");

  yyparse();


  p.parse();

  return 0;
}
