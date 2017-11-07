%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help.h"
%}

%union {
        char* string;
}

%token <string> MODULE PROCEDURE
%token <string> VAR CONST
%token <string> IF THEN ELSIF ELSE BEG END
%token <string> DIV MOD AND OR
%token <string> ARRAY RECORD OF TYPE
%token <string> WHILE DO
%token <string> ASSIGN MOREQUAL LESEQUAL
%token <string> INTEGER INT_TYPE
%token <string> IDENT
%token <string> SPACE TAB RETURN

%type <string> integer equal lesequal morequal mod div or colon record_type const
%type <string> plus minus mul less more tilda ampersand sharp field_lists field_list
%type <string> actual_parametres elsif_list array_type ident_list record program module
%type <string> ident dot obr ebr osqbr esqbr selector factor term array of type
%type <string> term_list plus_minus simple_expression expression expression_list end if
%type <string> statement statement_list while_statement if_statement then begin declarations
%type <string> semicolon while do assign comma assignment procedure_call else elsif type_
%type <string> const_declarations const_list formal_params type_declarations type_list
%type <string> procedure_declarations procedure_declaration procedure_heading procedure_body procedure
%type <string> var_declarations ident_def_list var

%%

program                 : module ident semicolon declarations begin statement_list end ident dot {
                                $$ = copy_to(5, $4, $1, $5, $6, $7);
                                freee(9, $1, $2, $3, $4, $5, $6, $7, $8, $9);
                                printf("%s\n", $$);
                        }
                        | module ident semicolon declarations end ident dot {
                                $$ = copy_to(4, $4, $1, "{", $5);
                                freee(7, $1, $2, $3, $4, $5, $6, $7);
                                printf("%s\n", $$);
                        }
                        ;

declarations            : const_declarations type_declarations var_declarations procedure_declarations {
                                $$ = copy_to(4, $2, $1, $3, $4);
                                freee(4, $1, $2, $3, $4);
                        }
                        | { $$ = strdup(""); }
                        ;

type_declarations       : type_ type_list {
                                $$ = copy_to(1, $2);
                                freee(2, $1, $2);
                        }
                        | { $$ = strdup(""); }
                        ;

type_list               : type_list ident colon type semicolon {
                                $$ = copy_to(6, $1, "typedef ", $4, $2, $5, "\n");
                                freee(5, $1, $2, $3, $4, $5);
                        }
                        | { $$ = strdup(""); }
                        ;

const_declarations      : const const_list {
                                $$ = copy_to(1, $2);
                                freee(2, $1, $2);
                        }
                        | { $$ = strdup(""); }
                        ;

const_list              : const_list ident equal expression semicolon {
                                $$ = copy_to(7, $1, "const int ", $2, " = ", $4, $5, "\n");
                                freee(5, $1, $2, $3, $4, $5);
                        }
                        | { $$ = strdup(""); }
                        ;

procedure_declarations  : procedure_declaration semicolon procedure_declarations {
                                $$ = copy_to(3, $3, $1, ";\n");
                                freee(3, $1, $2, $3);
                        }
                        | { $$ = strdup(""); }
                        ;

procedure_declaration   : procedure_heading semicolon procedure_body ident {
                                $$ = copy_to(3, $1, " {\n", $3);
                                freee(4, $1, $2, $3, $4);
                        }
                        ;

procedure_heading       : procedure ident formal_params {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        ;

formal_params           : obr ebr {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        ;

procedure_body          : declarations end {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        | declarations begin statement_list end {
                                $$ = copy_to(4, $1, $3, $4);
                                freee(4, $1, $2, $3, $4);
                        }
                        ;

var_declarations        : var ident_def_list {
                                $$ = copy_to(1, $2);
                                freee(2, $1, $2);
                        }
                        | { $$ = strdup(""); }
                        ;

ident_def_list          : ident_def_list ident_list colon type semicolon {
                                $$ = copy_to(5, $1, $4, $2, $5, "\n");
                                freee(5, $1, $2, $3, $4, $5);
                        }
                        | { $$ = strdup(""); }
                        ;


type                    : ident {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | array_type {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | record_type {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        ;

record_type             : record field_lists end {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        ;

field_lists             : field_lists semicolon field_list {
                                $$ = copy_to(3, $$, $3, ";\n");
                                freee(3, $1,  $2, $3);
                        }
                        | field_list {
                                $$ = copy_to(2, $1, ";\n");
                                free($1);
                        }
                        ;

field_list              : ident_list colon type {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | {
                                $$ = strdup("");
                        }
                        ;

array_type              : array expression of type {
                                $$ = copy_to(4, $4, $1, $2, $3);
                                freee(4, $1, $2, $3, $4);
                        }
                        ;

ident_list              : ident_list comma ident {
                                $$ = copy_to(3, $$, $3, ", ");
                                freee(3, $1, $2, $3);
                        }
                        | ident {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        ;

statement_list          : statement_list semicolon statement {
                                $$ = copy_to(3, $1, $3, ";\n");
                                freee(3, $1,  $2, $3);
                        }
                        | statement {
                                $$ = copy_to(2, $1, ";\n");
                                free($1);
                        }
                        ;

statement               : assignment {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | procedure_call {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | if_statement {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | while_statement {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        ;

while_statement         : while expression do statement_list end {
                                $$ = copy_to(5, $1, $2, $3, $4, $5);
                                freee(5, $1, $2, $3, $4, $5);
                        }
                        ;

if_statement            : if expression then statement_list elsif_list end {
                                $$ = copy_to(6, $1, $2, $3, $4, $5, $6);
                                freee(6, $1, $2, $3, $4, $5, $6);
                        }
                        | if expression then statement_list elsif_list else statement_list end {
                                $$ = copy_to(8, $1, $2, $3, $4, $5, $6, $7, $8);
                                freee(8, $1, $2, $3, $4, $5, $6, $7, $8);
                        }
                        ;

elsif_list              : elsif_list elsif expression then statement_list {
                                $$ = copy_to(4, $2, $3, $4, $5);
                                freee(5, $1, $2, $3, $4, $5);
                        }
                        | {
                                $$ = strdup("");
                        }
                        ;

procedure_call          : ident actual_parametres {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        ;

actual_parametres       : obr expression_list ebr {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | { $$ = copy_to(2, "(", ")"); };

expression_list         : expression {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | expression_list comma expression {
                                $$ = copy_to(3, $$, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | {
                                $$ = strdup("");
                        }
                        ;

assignment              : ident selector assign expression {
                                $$ = copy_to(4, $1, $2, $3, $4);
                                freee(4, $1, $2, $3, $4);
                        }
                        ;

expression              : simple_expression {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | simple_expression equal simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | simple_expression sharp simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | simple_expression less simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | simple_expression lesequal simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | simple_expression more simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | simple_expression morequal simple_expression {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        ;

simple_expression       : plus_minus term_list {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        ;

plus_minus              : plus {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | minus {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | {
                                $$ = strdup("");
                        }
                        ;

term_list               : term {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | term_list plus term {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | term_list minus term {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | term_list or term {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        ;

term                    : factor {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | term mul factor {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | term div factor {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | term mod factor {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | term ampersand factor {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        ;

factor                  : ident selector {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        | integer {
                                $$ = copy_to(1, $1);
                                free($1);
                        }
                        | obr expression ebr {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | tilda factor {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        ;

selector                : dot ident {
                                $$ = copy_to(2, $1, $2);
                                freee(2, $1, $2);
                        }
                        | osqbr expression esqbr {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | selector dot ident {
                                $$ = copy_to(3, $1, $2, $3);
                                freee(3, $1, $2, $3);
                        }
                        | selector osqbr expression esqbr {
                                $$ = copy_to(4, $1, $2, $3, $4);
                                freee(4, $1, $2, $3, $4);
                        }
                        | {
                                $$ = strdup("");
                        }
                        ;

module                  : MODULE { $$ = strdup("int main()"); } ;
procedure               : PROCEDURE { $$ = strdup("void "); }
ident                   : IDENT { $$ = copy_to(1, $1); free($1); }
                        | INT_TYPE { $$ = strdup("int "); } ;
integer                 : INTEGER { $$ = copy_to(1, $1); free($1); } ;
equal			: '=' { $$ = strdup(" == "); } ;
lesequal		: LESEQUAL { $$ = strdup(" <= "); } ;
morequal		: MOREQUAL { $$ = strdup(" >= "); } ;
array                   : ARRAY { $$ = strdup("["); } ;
record                  : RECORD { $$ = strdup("struct {\n"); } ;
of                      : OF { $$ = strdup("]"); } ;
while                   : WHILE { $$ = strdup("while ("); } ;
do                      : DO { $$ = strdup(") {\n"); } ;
begin                   : BEG { $$ = strdup(" {\n"); } ;
end                     : END { $$ = strdup("}"); } ;
type_                   : TYPE { $$ = strdup(""); } ;
var                     : VAR { $$ = strdup(""); }
comma                   : ',' { $$ = strdup(", "); } ;
semicolon               : ';' { $$ = strdup(";"); } ;
colon                   : ':' { $$ = strdup(":"); } ;
if                      : IF { $$ = strdup("if ("); } ;
then                    : THEN { $$ = strdup(") {\n"); } ;
else                    : ELSE { $$ = strdup("} else {\n"); } ;
elsif                   : ELSIF { $$ = strdup("} else\nif ("); } ;
assign                  : ASSIGN { $$ = strdup(" = "); } ;
const                   : CONST { $$ = strdup("const "); } ;
mod			: MOD { $$ = strdup(" % "); } ;
div			: DIV { $$ = strdup(" / "); } ;
or			: OR { $$ = strdup(" || "); } ;
plus			: '+' { $$ = strdup(" + "); } ;
minus			: '-' { $$ = strdup(" - "); } ;
mul			: '*' { $$ = strdup(" * "); } ;
less			: '<' { $$ = strdup(" < "); } ;
more			: '>' { $$ = strdup(" > "); } ;
tilda                   : '~' { $$ = strdup("~"); } ;
ampersand               : '&' { $$ = strdup(" & "); } ;
sharp			: '#' { $$ = strdup(" # "); } ;
dot                     : '.' { $$ = strdup("."); } ;
obr                     : '(' { $$ = strdup("("); } ;
ebr                     : ')' { $$ = strdup(")"); } ;
osqbr                   : '[' { $$ = strdup("["); } ;
esqbr                   : ']' { $$ = strdup("]"); } ;

%%
