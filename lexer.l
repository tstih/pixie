%{
    #include <string.h>

    #include "y.tab.h"
    #include "pipes.h"
    #include "exec.h"
%}

%option nounput
%option noinput

%%

"XOR"           return XOR;
"COPY"          return COPY;
"W"|"WRITE"     return WRITE;
"D"|"DISPLAY"   return DISPLAY;
"PAGE"          return PAGE;
"P"|"PIXEL"     return PIXEL;
"L"|"LINE"      return LINE;
"C"|"CLS"       return CLEAR;
"I"|"INK"       return INK;
"PAPER"         return PAPER;
"B"|"BLIT"      return BLIT;
","             return SEPARATOR;
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