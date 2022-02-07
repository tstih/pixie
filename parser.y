%{
#include <stdbool.h>

#include "exec.h"
#include "args.h"
#include "gui.h"

int yylex();
void yyerror(const char *s);

%}

%start statements
%union {
    int num;
    struct point_s {
        int x;
        int y;
    } point;
    struct line_s {
        struct point_s p0;
        struct point_s p1;
    } line;
}
%token <num> NUMBER 
%token SEPARATOR
%token PIXEL LINE CLEAR INTENSITY

%type <point> pcoords
%type <line> lcoords

%%

statements  :   statement
            |   statement statements
            ;

statement   :   PIXEL pcoords           { gui_set_pixel ($2.x, $2.y); }
            |   LINE lcoords            { 
                    gui_draw_line(
                        $2.p0.x,
                        $2.p0.y,
                        $2.p1.x,
                        $2.p1.y
                    );
                }
            |   CLEAR                   { gui_cls(); }
            |   INTENSITY NUMBER        { gui_set_intensity($2); }
            ;

lcoords     :   pcoords SEPARATOR pcoords {
                    $$.p0=$1;
                    $$.p1=$3;
                }
            ;

pcoords     :   NUMBER SEPARATOR NUMBER {
                    $$.x=$1; $$.y=$3; 
                }
            ;

%%

int yywrap() {return 1;}



