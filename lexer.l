%{
    #include "y.tab.h"
    extern int yyval;
%}

%%

"P"         return PIXEL;
"L"         return LINE;
"C"         return CLEAR;
"I"         return INTENSITY;
[0-9]+      { yyval=atoi(yytext); return NUMBER; }

%%
