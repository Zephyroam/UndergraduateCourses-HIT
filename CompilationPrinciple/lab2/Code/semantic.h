
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "node.h"

#define TABLE_SIZE 0x3fff
#define TYPE_INT 1
#define TYPE_FLOAT 2
#define NAME_MAX_LEN 32
#define DEF_STRUCT 1
#define DEF_VAR 2
#define DEF_FUNC 3
#define DEC_FUNC 4
#define DEF_IN_STRUCT 5
#define DEF_NOT_IN_STRUCT 6

typedef struct Type* TypeP;
typedef struct FieldList* FieldListP;
typedef struct Function* FunctionP;
typedef struct Var* VarP;
struct Type {
    enum { BASIC,
           ARRAY,
           STRUCT } type;
    int basic_type;
    struct {
        TypeP elem;
        int size;
    } array;
    FieldListP structure;
};
struct FieldList {
    char* name;
    TypeP type;
    FieldListP next;
};
struct Var {
    int in_struct;
    FieldListP field;
    int line;
    VarP next;
};
struct Function {
    char* name;
    FieldListP field;
    TypeP return_type;
    int defined;
    int line;
    FunctionP next;
};

// create a basic type
TypeP create_type_basic(int basic_type);
// create an array type
TypeP create_type_array(TypeP elem, int size);
// create a struct type
TypeP create_type_struct(FieldListP structure);
// create a field list
FieldListP create_fieldlist(char* name, TypeP type, FieldListP next);
// create a table
VarP create_var(int in_struct, FieldListP field, VarP next, int line);
// create a function
FunctionP create_function(char* name, FieldListP field, TypeP return_type, int defined, int line, FunctionP next);

// hash function
unsigned int hash_pjw(char* name);

// check if a name is in the table
VarP find_var_table(char* name);
// add a field to the table
void insert_var_table(FieldListP field_list, int line, int in_struct);

// check if a name is in the function table
FunctionP find_function_table(char* name);
// add a function to the function table
void insert_function_table(FunctionP func);

// compare two functions
int funccmp(FunctionP funca, FunctionP funcb);

// compare two field lists
int fieldcmp(FieldListP x, FieldListP y, int compare_name);

// compare two types
int typecmp(TypeP a, TypeP b);

// find field in a struct
TypeP find_struct_field(FieldListP type, char* name);

// program
void Program(NodeP now);
// list of def of global variables, functions, structs
void ExtDefList(NodeP x);
// def of global variables, functions, structs
void ExtDef(NodeP now);
// def of global variables
void ExtDecList(NodeP now, TypeP type);
// description of type, including TYPE and struct
TypeP Specifier(NodeP now);
// description of struct
TypeP StructSpecifier(NodeP now);
// name of struct
char* OptTag(NodeP x);

// def list of variables, functions, structs (local)
FieldListP DefList(NodeP now, int restr);
// def of variables, functions, structs (local)
FieldListP Def(NodeP now, int restr);
// name list of variables, functions, structs (local)
FieldListP DecList(NodeP now, TypeP type, int restr);
// name of variables, functions, structs (local)
FieldListP Dec(NodeP now, TypeP type, int restr);
// def of variables
FieldListP VarDec(NodeP now, TypeP type, int restr);

// def of header of functions
void FunDec(NodeP now, TypeP type, int restr);
// paramlist of functions
FieldListP VarList(NodeP now, int restr);
// param def of functions
FieldListP ParamDec(NodeP now, int restr);

// staments of functions
void CompSt(NodeP now, TypeP type);
// list of staments
void StmtList(NodeP now, TypeP type);
// stament
void Stmt(NodeP now, TypeP type);
TypeP Exp(NodeP now);
FieldListP Args(NodeP now);

#endif