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
[0-9]+      { yylval.num=atoi(yytext); return NUMBER; }

%%

void exec_parse(char* command)
{
    /* Make sure you have enough space. 
    int len=strlen(command);
    char *str=malloc(len+2);
    strcpy(str,command);
    str[len]=0; str[len+1]=0;
    
    YY_BUFFER_STATE yybs = yy_scan_buffer(command, strlen(command)+2);
    yy_switch_to_buffer(yybs);
    yyparse();
    yy_delete_buffer(yybs);

    free(str);*/

    YY_BUFFER_STATE buffer = yy_scan_string(command);
    yyparse();
    yy_delete_buffer(buffer);
}

//void yyerror(char const *s) { 
void yyerror (char const *msg) {
    exec_error(msg, yylloc.last_line, yylloc.last_column);
}