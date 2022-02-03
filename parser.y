%{
extern int yylex(void);
int yyval;
%}

%start statements
%token NUMBER SEPARATOR
%token PIXEL LINE CLEAR INTENSITY

%%

statements  :   statement
            |   statement statements
            ;

statement   :   PIXEL pcoords           { gui_set_pixel ($2); }
            |   LINE lcoords            { }
            |   CLEAR                   { gui_cls(); }
            |   INTENSITY NUMBER        { gui_set_intensity($2); }
            ;

lcoords     :   pcoords SEPARATOR pcoords
            ;

pcoords     :   NUMBER SEPARATOR NUMBER
            ;

%%
yywrap() {return 1;}

void yyerror(char const *s) { }