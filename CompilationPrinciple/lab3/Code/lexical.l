%{
    # include "syntax.tab.h"
    # include "node.h"

    int yycolumn = 1;
    extern unsigned lexError;

    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
    
%}

%option yylineno

ws [ \t]+
digit [0-9]
letter [_a-zA-Z]

INT 0|[1-9]{digit}*
FLOAT {digit}+\.{digit}+|{digit}*\.{digit}+[eE][+-]?{digit}+
ID {letter}({letter}|{digit})*
SEMI ";"
COMMA ","
ASSIGNOP "="
RELOP "<"|"<="|">"|">="|"=="|"!="
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
NOT "!"
DOT "."
TYPE int|float
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while

%%

{ws} {}
\n|\r { yycolumn = 1; }

{IF} { yylval.node = get_terminal_node("IF"); return IF; }
{ELSE} { yylval.node = get_terminal_node("ELSE"); return ELSE; }
{WHILE} { yylval.node = get_terminal_node("WHILE"); return WHILE; }
{TYPE} { yylval.node = get_terminal_node_string("TYPE", yytext); return TYPE; }
{STRUCT} { yylval.node = get_terminal_node("STRUCT"); return STRUCT; }
{RETURN} { yylval.node = get_terminal_node("RETURN"); return RETURN; }
{RELOP} { yylval.node = get_terminal_node_relop("RELOP", yytext); return RELOP; }

{PLUS} { yylval.node = get_terminal_node("PLUS"); return PLUS; }
{MINUS} { yylval.node = get_terminal_node("MINUS"); return MINUS; }
{STAR} { yylval.node = get_terminal_node("STAR"); return STAR; }
{DIV} { yylval.node = get_terminal_node("DIV"); return DIV; }
{AND} { yylval.node = get_terminal_node("AND"); return AND; }
{OR} { yylval.node = get_terminal_node("OR"); return OR; }
{NOT} { yylval.node = get_terminal_node("NOT"); return NOT; }

{DOT} { yylval.node = get_terminal_node("DOT"); return DOT; }
{SEMI} { yylval.node = get_terminal_node("SEMI"); return SEMI; }
{COMMA} { yylval.node = get_terminal_node("COMMA"); return COMMA; }
{ASSIGNOP} { yylval.node = get_terminal_node("ASSIGNOP"); return ASSIGNOP; }

{LP} { yylval.node = get_terminal_node("LP"); return LP; }
{RP} { yylval.node = get_terminal_node("RP"); return RP; }
{LB} { yylval.node = get_terminal_node("LB"); return LB; }
{RB} { yylval.node = get_terminal_node("RB"); return RB; }
{LC} { yylval.node = get_terminal_node("LC"); return LC; }
{RC} { yylval.node = get_terminal_node("RC"); return RC; }

{ID} { yylval.node = get_terminal_node_string("ID", yytext); return ID; }
{INT} { yylval.node = get_terminal_node_int(atoi(yytext)); return INT; }
{FLOAT} { yylval.node = get_terminal_node_float(atof(yytext)); return FLOAT; }


. { lexError = 1; printf("Error type A at Line %d: Mysterious characters \"%s\".\n", yylineno, yytext); }

%%

//{user subroutines}otes 