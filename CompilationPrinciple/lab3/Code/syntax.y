%{
    # include <stdio.h>
    # include "lex.yy.c"
    # include "node.h"
    struct Node * root;
    extern unsigned synError;
    int yyerror(char* msg);
%}

%union {
    struct Node * node;
}

// terminal symbols
%token <node> TYPE
%token <node> RETURN
%token <node> IF
%token <node> ELSE
%token <node> WHILE
%token <node> STRUCT
%token <node> INT
%token <node> FLOAT
%token <node> ID
%token <node> SEMI
%token <node> COMMA
%token <node> ASSIGNOP
%token <node> RELOP
%token <node> PLUS
%token <node> MINUS
%token <node> STAR
%token <node> DIV
%token <node> AND
%token <node> OR
%token <node> NOT
%token <node> DOT
%token <node> LP
%token <node> RP
%token <node> LB
%token <node> RB
%token <node> LC
%token <node> RC

// non-terminal symbols

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args

%nonassoc error
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LB RB LP RP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

// High-level Definitions
Program : ExtDefList { $$ = get_nonterminal_node(@$.first_line, "Program", 1, $1); root = $$; }
    ;
ExtDefList : ExtDef ExtDefList { $$ = get_nonterminal_node(@$.first_line, "ExtDefList", 2, $1, $2); }
    |   { $$ = NULL; }
    ;
ExtDef : Specifier ExtDecList SEMI { $$ = get_nonterminal_node(@$.first_line, "ExtDef", 3, $1, $2, $3); }
    | Specifier SEMI { $$ = get_nonterminal_node(@$.first_line, "ExtDef", 2, $1, $2); }
    | Specifier FunDec CompSt { $$ = get_nonterminal_node(@$.first_line, "ExtDef", 3, $1, $2, $3); }
    | error SEMI { synError = 1; }
    ;
ExtDecList : VarDec { $$ = get_nonterminal_node(@$.first_line, "ExtDecList", 1, $1); }
    | VarDec COMMA ExtDecList { $$ = get_nonterminal_node(@$.first_line, "ExtDecList", 3, $1, $2, $3); }
    ;

// Specifiers
Specifier : TYPE { $$ = get_nonterminal_node(@$.first_line, "Specifier", 1, $1); }
    | StructSpecifier { $$ = get_nonterminal_node(@$.first_line, "Specifier", 1, $1); }
    ;
StructSpecifier : STRUCT OptTag LC DefList RC { $$ = get_nonterminal_node(@$.first_line, "StructSpecifier", 5, $1, $2, $3, $4, $5); }
    | STRUCT Tag { $$ = get_nonterminal_node(@$.first_line, "StructSpecifier", 2, $1, $2); }
    | STRUCT OptTag LC error RC { synError = 1; }
    ;
OptTag : ID { $$ = get_nonterminal_node(@$.first_line, "OptTag", 1, $1); }
    | { $$ = NULL; }
    ;
Tag : ID { $$ = get_nonterminal_node(@$.first_line, "Tag", 1, $1); }
    ;

// Declarators
VarDec : ID { $$ = get_nonterminal_node(@$.first_line, "VarDec", 1, $1); }
    | VarDec LB INT RB { $$ = get_nonterminal_node(@$.first_line, "VarDec", 4, $1, $2, $3, $4); }
    | error RB { synError = 1; }
    ;
FunDec : ID LP VarList RP { $$ = get_nonterminal_node(@$.first_line, "FunDec", 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = get_nonterminal_node(@$.first_line, "FunDec", 3, $1, $2, $3); }
    | error RP { synError = 1; }
    ;
VarList : ParamDec COMMA VarList { $$ = get_nonterminal_node(@$.first_line, "VarList", 3, $1, $2, $3); }
    | ParamDec { $$ = get_nonterminal_node(@$.first_line, "VarList", 1, $1); }
    ;
ParamDec : Specifier VarDec { $$ = get_nonterminal_node(@$.first_line, "ParamDec", 2, $1, $2); }
    ;

// statements
CompSt : LC DefList StmtList RC { $$ = get_nonterminal_node(@$.first_line, "CompSt", 4, $1, $2, $3, $4); }
    | error RC { synError = 1; }
    ;
StmtList : Stmt StmtList { $$ = get_nonterminal_node(@$.first_line, "StmtList", 2, $1, $2); }
    | { $$ = NULL; }
    ;
Stmt : Exp SEMI { $$ = get_nonterminal_node(@$.first_line, "Stmt", 2, $1, $2); }
    | CompSt { $$ = get_nonterminal_node(@$.first_line, "Stmt", 1, $1); }
    | RETURN Exp SEMI { $$ = get_nonterminal_node(@$.first_line, "Stmt", 3, $1, $2, $3); }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE { $$ = get_nonterminal_node(@$.first_line, "Stmt", 5, $1, $2, $3, $4, $5); }
    | IF LP Exp RP Stmt ELSE Stmt { $$ = get_nonterminal_node(@$.first_line, "Stmt", 7, $1, $2, $3, $4, $5, $6, $7); }
    | WHILE LP Exp RP Stmt { $$ = get_nonterminal_node(@$.first_line, "Stmt", 5, $1, $2, $3, $4, $5); }
    | error SEMI { synError = 1; }
    | WHILE LP error RP Stmt { synError = 1; }
    | IF LP error RP Stmt %prec LOWER_THAN_ELSE { synError = 1; }
    | IF LP error RP Stmt ELSE Stmt { synError = 1; }
    | RETURN error SEMI { synError = 1; }
    ;

// Local Definitions
DefList : Def DefList { $$ = get_nonterminal_node(@$.first_line, "DefList", 2, $1, $2); }
    | { $$ = NULL; }
    ;
Def : Specifier DecList SEMI { $$ = get_nonterminal_node(@$.first_line, "Def", 3, $1, $2, $3); }
    | Specifier error SEMI { synError = 1; }
    | Specifier DecList error { synError = 1; }
    ;
DecList : Dec { $$ = get_nonterminal_node(@$.first_line, "DecList", 1, $1); }
    | Dec COMMA DecList { $$ = get_nonterminal_node(@$.first_line, "DecList", 3, $1, $2, $3); }
    | Dec COMMA error { synError = 1; }
    ;
Dec : VarDec { $$ = get_nonterminal_node(@$.first_line, "Dec", 1, $1); }
    | VarDec ASSIGNOP Exp { $$ = get_nonterminal_node(@$.first_line, "Dec", 3, $1, $2, $3); }
    | VarDec ASSIGNOP error { synError = 1; }
    ;

// Expressions
Exp : Exp ASSIGNOP Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp AND Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp OR Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp RELOP Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp PLUS Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp MINUS Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp STAR Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp DIV Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | LP Exp RP { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | MINUS Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 2, $1, $2); }
    | NOT Exp { $$ = get_nonterminal_node(@$.first_line, "Exp", 2, $1, $2); }
    | ID LP Args RP { $$ = get_nonterminal_node(@$.first_line, "Exp", 4, $1, $2, $3, $4); }
    | ID LP RP { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | Exp LB Exp RB { $$ = get_nonterminal_node(@$.first_line, "Exp", 4, $1, $2, $3, $4); }
    | Exp DOT ID { $$ = get_nonterminal_node(@$.first_line, "Exp", 3, $1, $2, $3); }
    | ID { $$ = get_nonterminal_node(@$.first_line, "Exp", 1, $1); }
    | INT { $$ = get_nonterminal_node(@$.first_line, "Exp", 1, $1); }
    | FLOAT { $$ = get_nonterminal_node(@$.first_line, "Exp", 1, $1); }
    ;
Args : Exp COMMA Args { $$ = get_nonterminal_node(@$.first_line, "Args", 3, $1, $2, $3); }
    | Exp { $$ = get_nonterminal_node(@$.first_line, "Args", 1, $1); }
    | error COMMA Args { synError = 1; }
    ;

%%

int yyerror(char* msg) {
    fprintf(stderr, "Error type B at line %d: %s.\n", yylineno, msg);
    
}