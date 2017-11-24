%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "oberon.tab.h"
#include "help.h"
%}

IDENT [A-Za-z][A-Za-z0-9]*
INTEGER ([1-9][0-9]*)|0

%option noyywrap

%%

"MODULE"        { return MODULE; }
"PROCEDURE"     { return PROCEDURE; }
"VAR"           { return VAR; }
"IF"            { return IF; }
"THEN"          { return THEN; }
"ELSIF"         { return ELSIF; }
"ELSE"          { return ELSE; }
"BEGIN"         { return BEG; }
"END"           { return END; }
"DIV"           { return DIV; }
"MOD"           { return MOD; }
"AND"           { return AND; }
"OR"            { return OR; }
"ARRAY"         { return ARRAY; }
"RECORD"        { return RECORD; }
"OF"            { return OF; }
"WHILE"         { return WHILE; }
"DO"            { return DO; }
"CONST"         { return CONST; }
"INTEGER"       { return INT_TYPE; }
"TYPE"          { return TYPE; }
{INTEGER}       { yylval.string = copy_to(1, yytext); return INTEGER; }
{IDENT}		{ yylval.string = copy_to(1, yytext); return IDENT; }
":="            { return ASSIGN; }
"="		{ return '='; }
"+"		{ return '+'; }
"-"		{ return '-'; }
"*"		{ return '*'; }
"/"		{ return '/'; }
"#"             { return '#'; }
"&"             { return '&'; }
"~"             { return '~'; }
">="            { return MOREQUAL; }
"<="            { return LESEQUAL; }
">"             { return '>'; }
"<"             { return '<'; }
"("		{ return '('; }
")"		{ return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }
";"             { return ';'; }
":"             { return ':'; }
","             { return ','; }
"."             { return '.'; }
[ \t\n]         { }
.		{ return("Lexical error: symbol '%c'!\n", yytext[0]); exit(1); }

%%
