#include "semantic.h"

VarP var_table[TABLE_SIZE + 1];            // table of struct and variables
FunctionP function_table[TABLE_SIZE + 1];  // table of functions
int num_unnamed_struct = 0;
int temp_cnt = 1;  // for lab3

TypeP create_type_basic(int basic_type) {
    TypeP type = (TypeP)malloc(sizeof(struct Type));
    type->type = BASIC;
    type->basic_type = basic_type;
    return type;
}

TypeP create_type_array(TypeP elem, int size) {
    TypeP type = (TypeP)malloc(sizeof(struct Type));
    type->type = ARRAY;
    type->array.elem = elem;
    type->array.size = size;
    return type;
}

TypeP create_type_struct(FieldListP structure) {
    TypeP type = (TypeP)malloc(sizeof(struct Type));
    type->type = STRUCT;
    type->structure = structure;
    return type;
}

FieldListP create_fieldlist(char* name, TypeP type, FieldListP next) {
    FieldListP field = (FieldListP)malloc(sizeof(struct FieldList));
    // printf("$$$$%d %s\n", __LINE__, name);
    field->name = name;
    field->type = type;
    field->next = next;
    return field;
}

VarP create_var(int in_struct, FieldListP field, VarP next, int line, int from_fun) {
    VarP var_table = (VarP)malloc(sizeof(struct Var));
    var_table->in_struct = in_struct;
    var_table->field = field;
    var_table->next = next;
    var_table->line = line;
    var_table->variable_cnt = temp_cnt++;
    var_table->from_func = from_fun;
    return var_table;
}

FunctionP create_function(char* name, FieldListP field, TypeP return_type, int defined, int line, FunctionP next) {
    FunctionP function = (FunctionP)malloc(sizeof(struct Function));
    function->name = name;
    function->field = field;
    function->return_type = return_type;
    function->defined = defined;
    function->line = line;
    function->next = next;
    return function;
}

unsigned int hash_pjw(char* name) {
    unsigned int val = 0, i;
    for (; *name; ++name) {
        val = (val << 2) + *name;
        if (i = val * ~TABLE_SIZE)
            val = (val ^ (i >> 12)) & TABLE_SIZE;
    }
    return val;
}

VarP find_var_table(char* name) {
    // printf("$$$$%d %s\n", __LINE__, name);
    unsigned int val = hash_pjw(name);
    VarP x = var_table[val];
    while (x != NULL) {
        if (strcmp(x->field->name, name) == 0)
            break;
        x = x->next;
    }
    return x;
}

void insert_var_table(FieldListP field_list, int line, int in_struct, int from_fun) {
    int val = hash_pjw(field_list->name);
    if (var_table[val] == NULL) {
        var_table[val] = create_var(in_struct, field_list, NULL, line, from_fun);
        return;
    }
    VarP now = var_table[val];
    while (now->next != NULL) {
        now = now->next;
    }
    now->next = create_var(in_struct, field_list, NULL, line, from_fun);
}

FunctionP find_function_table(char* name) {
    unsigned int val = hash_pjw(name);
    FunctionP x = function_table[val];
    while (x != NULL) {
        if (strcmp(x->name, name) == 0)
            break;
        x = x->next;
    }
    return x;
}

void insert_function_table(FunctionP func) {
    unsigned int val = hash_pjw(func->name);
    if (function_table[val] == NULL) {
        function_table[val] = func;
        return;
    }
    FunctionP now = function_table[val];
    while (now->next != NULL) {
        now = now->next;
    }
    now->next = func;
}

int funccmp(FunctionP x, FunctionP y) {
    if (x == NULL && y == NULL)
        return 0;
    if (x == NULL || y == NULL)
        return 1;
    if (strcmp(x->name, y->name))
        return 1;
    if (fieldcmp(x->field, y->field, 1))
        return 1;
    if (typecmp(x->return_type, y->return_type))
        return 1;
    return 0;
}
int fieldcmp(FieldListP x, FieldListP y, int compare_name) {
    if (x == NULL && y == NULL)
        return 0;
    if (x == NULL || y == NULL)
        return 1;
    if (compare_name && strcmp(x->name, y->name))
        return 1;
    if (typecmp(x->type, y->type))
        return 1;
    return fieldcmp(x->next, y->next, compare_name);
}
int typecmp(TypeP x, TypeP y) {
    if (x == NULL && y == NULL)
        return 0;
    if (x == NULL || y == NULL)
        return 1;
    if (x->type != y->type)
        return 1;
    if (x->type == BASIC)
        return x->basic_type != y->basic_type;
    if (x->type == ARRAY)
        return typecmp(x->array.elem, y->array.elem);
    if (x->type == STRUCT)
        return fieldcmp(x->structure, y->structure, 1);
}

TypeP find_struct_field(FieldListP field_list, char* name) {
    // printf("@@@ %d %s %s %d %s\n", __LINE__, name, field_list->name, field_list->type->type, field_list->type->structure->);
    while (field_list != NULL) {
        // printf("@@@ %d %s %s %d\n", __LINE__, name, field_list->name, field_list->type->type);
        if (strcmp(name, field_list->name) == 0)
            return field_list->type;
        field_list = field_list->next;
    }
    return NULL;
}

void Program(NodeP x) {
    // Program -> ExtDefList
    insert_function_table(create_function("read", NULL, create_type_basic(TYPE_INT), 1, 0, NULL));
    insert_function_table(create_function("write", create_fieldlist("param", create_type_basic(TYPE_INT), NULL), create_type_basic(TYPE_INT), 1, 0, NULL));
    ExtDefList(x->children[0]);
}

void ExtDefList(NodeP x) {
    // ExtDefList -> ExtDef ExtDefList | empty
    if (x == NULL)
        return;
    ExtDef(x->children[0]);
    ExtDefList(x->children[1]);
}

void ExtDef(NodeP x) {
    // ExtDef -> Specifier SEMI
    TypeP type_specifier = Specifier(x->children[0]);
    if (x->child_num == 2)
        return;
    if (strcmp(x->children[1]->data_string, "ExtDecList") == 0) {
        // ExtDef -> Specifier ExtDecList SEMI
        ExtDecList(x->children[1], type_specifier);
    } else if (strcmp(x->children[2]->data_string, "CompSt") == 0) {
        // ExtDef -> Specifier FunDec CompSt
        FunDec(x->children[1], type_specifier, DEF_FUNC);
        CompSt(x->children[2], type_specifier);
    }
}

TypeP Specifier(NodeP x) {
    // Specifier -> TYPE | StructSpecifier
    if (strcmp(x->children[0]->data_string, "TYPE") == 0) {
        if (strcmp(x->children[0]->data_string_data, "int") == 0)
            return create_type_basic(TYPE_INT);
        else
            return create_type_basic(TYPE_FLOAT);
    } else if (strcmp(x->children[0]->data_string, "StructSpecifier") == 0) {
        return StructSpecifier(x->children[0]);
    }
}

TypeP StructSpecifier(NodeP x) {
    if (strcmp(x->children[1]->data_string, "OptTag") == 0) {
        // StructSpecifier -> STRUCT OptTag LC DefList RC
        FieldListP struct_field = create_fieldlist(OptTag(x->children[1]), create_type_struct(DefList(x->children[3], DEF_STRUCT)), NULL);
        if (struct_field->name == NULL)
            return NULL;
        // printf("@@%d %s@@\n", __LINE__, struct_field->name);
        insert_var_table(struct_field, x->children[0]->line, DEF_IN_STRUCT, NOT_FROM_FUNC);
        return create_type_struct(struct_field);
    } else {
        // StructSpecifier -> STRUCT Tag
        // Tag -> ID
        VarP now = find_var_table(x->children[1]->children[0]->data_string_data);
        if (now == NULL || now->field->type->type != STRUCT) {
            // printf("%d ", __LINE__);
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", x->line, x->children[1]->children[0]->data_string_data);
            return NULL;
        }
        return now->field->type;
    }
}

char* OptTag(NodeP x) {
    if (x->child_num == 0) {
        // OptTag -> empty
        char name[NAME_MAX_LEN];
        sprintf(name, "%s%d", "unnamed_struct", num_unnamed_struct++);
        // printf("@@@@@@%d %s\n", __LINE__, name);
        return strdup(name);
    } else {
        // OptTag -> ID
        if (find_var_table(x->children[0]->data_string_data) != NULL) {
            // printf("%d ", __LINE__);
            printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", x->line, x->children[0]->data_string_data);
            return NULL;
        }
        // printf("@@@@@@%d %s\n", __LINE__, x->children[0]->data_string_data);
        return x->children[0]->data_string_data;
    }
}

FieldListP DefList(NodeP x, int restr) {
    // DefList -> Def DefList | empty
    NodeP root = x;
    FieldListP head = NULL, now = NULL;
    while (root != NULL) {
        FieldListP next = Def(root->children[0], restr);
        if (head == NULL) {
            now = head = next;
        } else {
            now->next = next;
        }
        if (now == NULL)
            break;
        while (now->next != NULL) now = now->next;
        root = root->children[1];
    }
    return head;
}

FieldListP Def(NodeP x, int restr) {
    // Def -> Specifier DecList SEMI
    return DecList(x->children[1], Specifier(x->children[0]), restr);
}

FieldListP DecList(NodeP x, TypeP type, int restr) {
    if (x->child_num == 1) {
        // DecList -> Dec
        return Dec(x->children[0], type, restr);
    } else {
        // DecList -> Dec COMMA DecList
        FieldListP field_list = Dec(x->children[0], type, restr);
        if (field_list != NULL)
            field_list->next = DecList(x->children[2], type, restr);
        return field_list;
    }
}

FieldListP Dec(NodeP x, TypeP type, int restr) {
    if (x->child_num == 1) {
        // Dec -> VarDec
        return VarDec(x->children[0], type, restr);
    } else {
        // Dec -> VarDec ASSIGNOP Exp
        if (restr == DEF_STRUCT) {
            // printf("%d ", __LINE__);
            printf("Error type 15 at Line %d: Initialize domain in structure.\n", x->children[0]->line);
            return NULL;
        }
        TypeP t = Exp(x->children[2]);
        if (typecmp(type, t) == 1) {
            // printf("%d ", __LINE__);
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n", x->line);
            return NULL;
        }
        return VarDec(x->children[0], type, restr);
    }
}

FieldListP VarDec(NodeP x, TypeP type, int restr) {
    if (x->child_num == 1) {
        // VarDec -> ID
        FieldListP field_list = create_fieldlist(x->children[0]->data_string_data, type, NULL);
        if (restr == DEC_FUNC)
            return field_list;
        if (find_var_table(field_list->name)) {
            if (restr == DEF_STRUCT) {
                // printf("%d ", __LINE__);
                printf("Error type 15 at Line %d: Redefined field \"%s\".\n", x->line, x->children[0]->data_string_data);
                return NULL;
            } else if (restr == DEF_VAR || restr == DEF_FUNC) {
                // printf("%d ", __LINE__);
                printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", x->line, x->children[0]->data_string_data);
                return NULL;
            }
        }

        if (restr == DEF_FUNC)
            insert_var_table(field_list, x->children[0]->line, DEF_NOT_IN_STRUCT, FROM_FUNC);
        else
            insert_var_table(field_list, x->children[0]->line, DEF_NOT_IN_STRUCT, NOT_FROM_FUNC);
        return field_list;
    } else {
        // VarDec -> VarDec LB INT RB
        return VarDec(x->children[0], create_type_array(type, x->children[2]->data_int), restr);
    }
}

void ExtDecList(NodeP x, TypeP type) {
    if (x->child_num == 1) {
        // ExtDecList -> VarDec
        VarDec(x->children[0], type, DEF_VAR);
    } else {
        // ExtDecList -> VarDec COMMA ExtDecList
        VarDec(x->children[0], type, DEF_VAR);
        ExtDecList(x->children[2], type);
    }
}

void FunDec(NodeP x, TypeP type, int restr) {
    // FunDec -> ID LP VarList RP | ID LP RP
    FunctionP func = create_function(x->children[0]->data_string_data, NULL, type, 0, x->line, NULL);
    if (x->child_num == 4) {
        func->field = VarList(x->children[2], restr);
    }
    FunctionP find_func = find_function_table(func->name);
    if (restr == DEC_FUNC) {
        if (find_func == NULL) {
            insert_function_table(func);
        }
    } else {
        if (find_func == NULL) {
            func->defined = 1;
            insert_function_table(func);
        } else {
            if (funccmp(find_func, func) == 0) {
                if (func->defined) {
                    // printf("%d ", __LINE__);
                    printf("Error type 4 at Line %d: Redefined function \"%s\".\n", x->line, func->name);
                    return;
                } else
                    find_func->defined = 1;
            } else {
                // printf("%d ", __LINE__);
                printf("Error type 4 at Line %d: Redefined function \"%s\".\n", x->line, func->name);
                return;
            }
        }
    }
}

FieldListP VarList(NodeP x, int restr) {
    if (x->child_num == 1) {
        // VarList -> ParamDec
        return ParamDec(x->children[0], restr);
    } else {
        // VarList -> ParamDec COMMA VarList
        FieldListP field_list = ParamDec(x->children[0], restr);
        if (field_list != NULL)
            field_list->next = VarList(x->children[2], restr);
        return field_list;
    }
}

FieldListP ParamDec(NodeP x, int restr) {
    // ParamDec -> Specifier VarDec
    TypeP specifier_type = Specifier(x->children[0]);
    return VarDec(x->children[1], specifier_type, restr);
}

void CompSt(NodeP x, TypeP type) {
    // CompSt -> LC DefList StmtList RC
    DefList(x->children[1], DEF_VAR);
    StmtList(x->children[2], type);
}

void StmtList(NodeP x, TypeP type) {
    // StmtList -> Stmt StmtList | empty
    if (x == NULL)
        return;
    Stmt(x->children[0], type);
    StmtList(x->children[1], type);
}

void Stmt(NodeP x, TypeP type) {
    if (strcmp(x->children[0]->data_string, "CompSt") == 0) {
        // Stmt -> CompSt
        CompSt(x->children[0], type);
    } else if (strcmp(x->children[0]->data_string, "Exp") == 0) {
        // Stmt -> Exp SEMI
        Exp(x->children[0]);
    } else if (strcmp(x->children[0]->data_string, "RETURN") == 0) {
        // Stmt -> RETURN Exp SEMI
        TypeP type_exp = Exp(x->children[1]);
        if (typecmp(type_exp, type) == 1) {
            // printf("%d ", __LINE__);
            printf("Error type 8 at Line %d: Type mismatched for return.\n", x->line);
        }
    } else if (strcmp(x->children[0]->data_string, "IF") == 0) {
        // Stmt -> IF LP Exp RP Stmt | IF LP Exp RP Stmt ELSE Stmt
        TypeP type_exp = Exp(x->children[2]);
        if (type_exp == NULL)
            return;
        if (type_exp->type != BASIC || type_exp->basic_type != TYPE_INT) {
            // printf("%d ", __LINE__);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", x->children[2]->line);
            return;
        }
        Stmt(x->children[4], type);
        if (x->children[6]) {
            Stmt(x->children[6], type);
        }
    } else if (strcmp(x->children[0]->data_string, "WHILE") == 0) {
        // Stmt -> WHILE LP Exp RP Stmt
        TypeP type_exp = Exp(x->children[2]);
        if (type_exp == NULL)
            return;
        if (type_exp->type != BASIC || type_exp->basic_type != TYPE_INT) {
            // printf("%d ", __LINE__);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", x->children[2]->line);
            return;
        }
        Stmt(x->children[4], type);
    }
}

TypeP Exp(NodeP x) {
    if (x->child_num == 1) {
        NodeP child = x->children[0];
        if (child->type == TERMINAL_INT) {
            // Exp -> INT
            return create_type_basic(TYPE_INT);
        } else if (child->type == TERMINAL_FLOAT) {
            // Exp -> FLOAT
            return create_type_basic(TYPE_FLOAT);
        } else if (strcmp(child->data_string, "ID") == 0) {
            // Exp -> ID
            VarP find_id = find_var_table(child->data_string_data);
            if (find_id == NULL || find_id->in_struct == DEF_IN_STRUCT) {
                // printf("%d ", __LINE__);
                printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", x->line, child->data_string_data);
                return NULL;
            }
            return find_id->field->type;
        }
    }
    if (x->child_num == 2) {
        if (strcmp(x->children[0]->data_string, "MINUS") == 0) {
            // Exp -> MINUS Exp
            return Exp(x->children[1]);
        } else if (strcmp(x->children[0]->data_string, "NOT") == 0) {
            // Exp -> NOT Exp
            TypeP return_type = Exp(x->children[1]);
            if (return_type == NULL)
                return NULL;
            if (return_type->type == BASIC && return_type->basic_type == TYPE_INT)
                return return_type;
            // printf("%d ", __LINE__);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", x->line);
            return NULL;
        }
    }
    if (strcmp(x->children[1]->data_string, "ASSIGNOP") == 0) {
        // Exp -> Exp ASSIGNOP Exp
        // child -> ID | Exp LB Exp RB | Exp DOT ID
        NodeP child_child = x->children[0]->children[0];
        if (strcmp(child_child->data_string, "ID") == 0 && x->children[0]->child_num == 1)
            ;
        else if (strcmp(child_child->data_string, "Exp") == 0 && strcmp(x->children[0]->children[1]->data_string, "LB") == 0)
            ;
        else if (strcmp(child_child->data_string, "Exp") == 0 && strcmp(x->children[0]->children[1]->data_string, "DOT") == 0)
            ;
        else {
            // printf("%d ", __LINE__);
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", x->line);
            return NULL;
        }
        TypeP type_left = Exp(x->children[0]), type_right = Exp(x->children[2]);
        if (typecmp(type_left, type_right) == 0)
            return type_left;
        // printf("%d ", __LINE__);
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n", x->line);
        return NULL;
    }
    if (strcmp(x->children[1]->data_string, "AND") == 0 || strcmp(x->children[1]->data_string, "OR") == 0) {
        // Exp -> Exp AND Exp
        // Exp -> Exp OR Exp
        TypeP type_left = Exp(x->children[0]), type_right = Exp(x->children[2]);
        if (type_left == NULL && type_right == NULL)
            return NULL;
        if (typecmp(type_left, type_right) == 0 && type_left->type == BASIC && type_left->basic_type == TYPE_INT)
            return type_left;
        // printf("%d ", __LINE__);
        printf("Error type 7 at Line %d: Type mismatched for operands.\n", x->line);
        return NULL;
    }
    if (strcmp(x->children[1]->data_string, "PLUS") == 0 || strcmp(x->children[1]->data_string, "MINUS") == 0 || strcmp(x->children[1]->data_string, "STAR") == 0 || strcmp(x->children[1]->data_string, "DIV") == 0 || strcmp(x->children[1]->data_string, "RELOP") == 0) {
        // Exp -> Exp PLUS Exp
        // Exp -> Exp MINUS Exp
        // Exp -> Exp STAR Exp
        // Exp -> Exp DIV Exp
        // Exp -> Exp RELOP Exp
        TypeP type_left = Exp(x->children[0]), type_right = Exp(x->children[2]);
        if (typecmp(type_left, type_right) == 0 && type_left != NULL && type_left->type == BASIC)
            return type_right;
        else {
            // printf("%d ", __LINE__);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", x->line);
            return NULL;
        }
    }
    if (strcmp(x->children[1]->data_string, "DOT") == 0) {
        // Exp -> Exp DOT ID
        TypeP type_left = Exp(x->children[0]);
        if (type_left == NULL)
            return NULL;
        if (type_left->type != STRUCT) {
            // printf("%d ", __LINE__);
            printf("Error type 13 at Line %d: Illegal use of \".\".\n", x->line);
            return NULL;
        }
        TypeP type_right = find_struct_field(type_left->structure->type->structure, x->children[2]->data_string_data);
        if (type_right == NULL) {
            // printf("%d ", __LINE__);
            printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", x->line, x->children[2]->data_string_data);
            return NULL;
        }
        return type_right;
    }
    if (strcmp(x->children[1]->data_string, "LB") == 0) {
        // Exp -> Exp LB Exp RB
        TypeP type_left = Exp(x->children[0]);
        if (type_left == NULL)
            return NULL;
        if (type_left->type != ARRAY) {
            // printf("%d ", __LINE__);
            printf("Error type 10 at Line %d: Not an array.\n", x->children[0]->line);
            return NULL;
        }
        TypeP type_right = Exp(x->children[2]);
        if (type_right == NULL)
            return NULL;
        if (type_right->type == BASIC && type_right->basic_type == TYPE_INT) {
            return type_left->array.elem;
        }
        // printf("%d ", __LINE__);
        printf("Error type 12 at Line %d: Not an integer.\n", x->children[0]->line);
        return NULL;
    }
    if (strcmp(x->children[0]->data_string, "LP") == 0) {
        // Exp -> LP Exp RP
        return Exp(x->children[1]);
    }
    if (strcmp(x->children[0]->data_string, "ID") == 0 && strcmp(x->children[1]->data_string, "LP") == 0) {
        // Exp -> ID LP Args RP
        // Exp -> ID LP RP
        if (find_var_table(x->children[0]->data_string_data) != NULL) {
            // printf("%d ", __LINE__);
            printf("Error type 11 at Line %d: Not a function.\n", x->line);
            return NULL;
        }
        FunctionP id_function = find_function_table(x->children[0]->data_string_data);
        if (id_function == NULL || id_function->defined == 0) {
            // printf("%d ", __LINE__);
            printf("Error type 2 at Line %d: Undefined function \"%s\".\n", x->line, x->children[0]->data_string_data);
            return NULL;
        }
        if (x->child_num == 4) {
            if (fieldcmp(id_function->field, Args(x->children[2]), 0) == 0)
                return id_function->return_type;
        } else {
            if (fieldcmp(id_function->field, NULL, 0) == 0)
                return id_function->return_type;
        }

        // printf("%d ", __LINE__);
        printf("Error type 9 at Line %d: Inappliable arguments for function \"%s\".\n", x->line, id_function->name);
        return NULL;
    }
}

FieldListP Args(NodeP x) {
    // Args -> Exp COMMA Args| Exp
    if (x->child_num == 1) {
        // Args -> Exp
        FieldListP new_field = create_fieldlist(NULL, Exp(x->children[0]), NULL);
        return new_field;
    }
    // Args -> Exp COMMA Args
    FieldListP new_field = create_fieldlist(NULL, Exp(x->children[0]), Args(x->children[2]));
    return new_field;
}
