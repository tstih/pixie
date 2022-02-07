%{
    #include "y.tab.h"
    #include "pipes.h"
%}

%option nounput
%option noinput

%%

"P"         return PIXEL;
"L"         return LINE;
"C"         return CLEAR;
"I"         return INTENSITY;
[0-9]+      { yylval.num=atoi(yytext); return NUMBER; }

%%

void scan_string(char* str)
{
    YY_BUFFER_STATE buffer=yy_scan_buffer(str, MAX_LINE_LEN);
    yyparse();
    yy_delete_buffer(buffer);
}