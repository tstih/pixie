%{
#include <stdbool.h>
#include <stdint.h>

#include "exec.h"
#include "args.h"
#include "gui.h"

int yylex();
void yyerror(const char *s);

%}

%locations

%start statement
%union {
    uint8_t byte;
    int integer;
    struct point_s {
        int x;
        int y;
    } point;
    struct line_s {
        struct point_s p0;
        struct point_s p1;
    } line;
}
%token <integer> NUMBER 
%token SEPARATOR
%token XOR COPY WRITE DISPLAY
%token PIXEL LINE CLEAR INK BLIT PAPER PAGE

%type <point> pcoords
%type <line> lcoords
%type <byte> pattern

%%

statement   :   PIXEL pcoords           { gui_set_pixel ($2.x, $2.y); }
            |   LINE lcoords pattern    { 
                    gui_draw_line(
                        $2.p0.x,
                        $2.p0.y,
                        $2.p1.x,
                        $2.p1.y,
                        $3              /* Solid pattern. */
                    );
                }
            |   CLEAR                   { gui_cls(); }
            |   INK NUMBER              { gui_set_intensity($2); }
            |   PAPER NUMBER            { gui_set_paper($2); }
            |   BLIT NUMBER             { gui_set_blit($2); }
            |   BLIT XOR                { gui_set_blit(2); }
            |   BLIT COPY               { gui_set_blit(1); }
            |   PAGE DISPLAY NUMBER     { gui_set_dpage($3); }
            |   PAGE WRITE NUMBER       { gui_set_wpage($3);  }
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

pattern     :   /* Empty. */            { $$=0xff; }
                | SEPARATOR NUMBER      { $$ =$2; }
            ;

%%

int yywrap() {
    return 1;
}

