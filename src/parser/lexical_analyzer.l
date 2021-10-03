%option noyywrap
%{
/*****************声明和选项设置  begin*****************/
#include <stdio.h>
#include <stdlib.h>

#include "syntax_tree.h"
#include "syntax_analyzer.h"

int lines;
int pos_start;
int pos_end;

void pass_node(char *text){
     yylval.node = new_syntax_tree_node(text);
}

/*****************声明和选项设置  end*****************/

%}


%%
 /* to do for students */
 /* two cases for you, pass_node will send flex's token to bison */
int  {pos_start = pos_end; pos_end += 3; pass_node(yytext); return INT;}
float  {pos_start = pos_end; pos_end += 5; pass_node(yytext); return FLOAT;}
if  {pos_start = pos_end; pos_end += 2; pass_node(yytext); return IF;}
else  {pos_start = pos_end; pos_end += 4; pass_node(yytext); return ELSE;}
return  {pos_start = pos_end; pos_end += 6; pass_node(yytext); return RETURN;}
void  {pos_start = pos_end; pos_end += 4; pass_node(yytext); return VOID;}
while  {pos_start = pos_end; pos_end += 5; pass_node(yytext); return WHILE;}
\+ {pos_start = pos_end; pos_end += 1; pass_node(yytext); return ADD;}
\- {pos_start = pos_end; pos_end += 1; pass_node(yytext); return SUB;}
\* {pos_start = pos_end; pos_end += 1; pass_node(yytext); return MUL;}
\/ {pos_start = pos_end; pos_end += 1; pass_node(yytext); return DIV;}
\< {pos_start = pos_end; pos_end += 1; pass_node(yytext); return LT;}
\<= {pos_start = pos_end; pos_end += 2; pass_node(yytext); return LE;}
\> {pos_start = pos_end; pos_end += 1; pass_node(yytext); return GT;}
\>= {pos_start = pos_end; pos_end += 2; pass_node(yytext); return GE;}
== {pos_start = pos_end; pos_end += 2; pass_node(yytext); return EQ;}
!= {pos_start = pos_end; pos_end += 2; pass_node(yytext); return NEQ;}
= {pos_start = pos_end; pos_end += 1; pass_node(yytext); return ASIGN;}
; {pos_start = pos_end; pos_end += 1; pass_node(yytext); return SEMICO;}
, {pos_start = pos_end; pos_end += 1; pass_node(yytext); return COMM;}
[0-9]+ {pos_start = pos_end; pos_end += strlen(yytext); pass_node(yytext); return INTEGER;}
[0-9]+\.|[0-9]*\.[0-9]+ {pos_start = pos_end; pos_end += strlen(yytext); pass_node(yytext); return FLOATPOINT;}
\{ {pos_start = pos_end; pos_end += 1; pass_node(yytext); return LBRACE;}
\} {pos_start = pos_end; pos_end += 1; pass_node(yytext); return RBRACE;}
\[ {pos_start = pos_end; pos_end += 1; pass_node(yytext); return LBRACK;}
\] {pos_start = pos_end; pos_end += 1; pass_node(yytext); return RBRACK;}
\( {pos_start = pos_end; pos_end += 1; pass_node(yytext); return LPAREN;}
\) {pos_start = pos_end; pos_end += 1; pass_node(yytext); return RPAREN;}
\n {pos_start = 1; pos_end = 1; lines++;}
[ ]|\t|\f|\r|\v {pos_end += 1;}
[a-zA-Z]+ {pos_start = pos_end; pos_end += strlen(yytext); pass_node(yytext); return ID;}
\/\*[^*]*(\*+[^*/][^*]*)*\*+\/|\/\*\/ {pos_end += 1;}
. { pos_start = pos_end; pos_end++; return ERROR; }

 /****请在此补全所有flex的模式与动作  end******/
%%
