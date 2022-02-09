%{
    #include <string.h>

    #include "y.tab.h"
    #include "pipes.h"
    #include "exec.h"
%}

%option nounput
%option noinput

%%

"P"         return PIXEL;
"L"         return LINE;
"C"         return CLEAR;
"I"         return INTENSITY;
","         return SEPARATOR;
[0-9]+      { yylval.integer=atoi(yytext); return NUMBER; }

%%

void exec_parse(char* command)
{
    YY_BUFFER_STATE buffer = yy_scan_string(command);
    yyparse();
    yy_delete_buffer(buffer);
}

//void yyerror(char const *s) { 
void yyerror (char const *msg) {
    exec_error(msg, yylloc.last_line, yylloc.last_column);
}