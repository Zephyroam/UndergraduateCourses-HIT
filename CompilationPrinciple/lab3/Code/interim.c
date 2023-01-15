#include "interim.h"

extern int temp_cnt;
int label_cnt = 1;
InterCodeP ic_link_head = NULL, ic_link_now = NULL;

OperandP create_operand(enum KindOperand kind, int id, char* name, TypeP type) {
    OperandP op = (OperandP)malloc(sizeof(struct Operand));
    op->kind = kind;
    op->id = id;
    op->name = name;
    op->type = type;
    return op;
}

InterCodeP create_intercode(enum KindInterCode kind, int intercode_type, ...) {
    InterCodeP ic = (InterCodeP)malloc(sizeof(struct InterCode));
    ic->kind = kind;
    va_list valist;
    va_start(valist, intercode_type % 10);

    if (intercode_type == INTERCODE_1) {
        ic->ulabel.op = va_arg(valist, OperandP);
    } else if (intercode_type == INTERCODE_2) {
        ic->uassign.op1 = va_arg(valist, OperandP);
        ic->uassign.op2 = va_arg(valist, OperandP);
    } else if (intercode_type == INTERCODE_3) {
        ic->ubinop.result = va_arg(valist, OperandP);
        ic->ubinop.op1 = va_arg(valist, OperandP);
        ic->ubinop.op2 = va_arg(valist, OperandP);
    } else if (intercode_type == INTERCODE_IF) {
        ic->uif.x = va_arg(valist, OperandP);
        ic->uif.relop = va_arg(valist, OperandP);
        ic->uif.y = va_arg(valist, OperandP);
        ic->uif.z = va_arg(valist, OperandP);
    } else if (intercode_type == INTERCODE_DEC) {
        ic->udec.op = va_arg(valist, OperandP);
        ic->udec.size = va_arg(valist, int);
    }

    va_end(valist);
    return ic;
}

OperandP creat_temp() {
    return create_operand(TEMP, temp_cnt++, "Temp", NULL);
}

OperandP creat_label() {
    return create_operand(LABEL, label_cnt++, NULL, NULL);
}

void insert_intercode(InterCodeP x) {
    x->prev = x->next = NULL;
    if (ic_link_head == NULL) {
        ic_link_now = ic_link_head = x;
        return;
    }
    ic_link_now->next = x;
    x->prev = ic_link_now;
    ic_link_now = x;
}

void translate_print(FILE* f) {
    InterCodeP x = ic_link_head;
    int judge_is_null = 0;

    while (x != NULL) {
        if (x->kind == ILABEL) {
            fprintf(f, "LABEL label%d :", x->ulabel.op->id);
        } else if (x->kind == IFUNCTION) {
            if (x->prev)
                fprintf(f, "\n");
            fprintf(f, "FUNCTION %s :", x->ulabel.op->name);
        } else if (x->kind == ASSIGN) {
            OperandP t1 = x->uassign.op1;
            OperandP t2 = x->uassign.op2;
            if (t1 == NULL || t2 == NULL) {
                judge_is_null = 1;
            } else if (t1->kind == ADDRESS && t2->kind == ADDRESS) {
                fprintf(f, "*t%d := *t%d", t1->id, t2->id);
            } else if (t2->kind == ADDRESS) {
                fprintf(f, "t%d := *t%d", t1->id, t2->id);
            } else if (t1->kind == ADDRESS) {
                fprintf(f, "&t%d := ", t1->id);
                if (t2->kind == CONSTANT)
                    fprintf(f, "#%d", t2->id);
                else
                    fprintf(f, "t%d", t2->id);
            } else {
                fprintf(f, "t%d := ", t1->id);
                if (t2->kind == CONSTANT)
                    fprintf(f, "#%d", t2->id);
                else
                    fprintf(f, "t%d", t2->id);
            }
        } else if (x->kind == ADD | x->kind == SUB | x->kind == MUL | x->kind == DIV) {
            OperandP t1 = x->ubinop.result, t2 = x->ubinop.op1, t3 = x->ubinop.op2;
            if (t1) {
                fprintf(f, "t%d := ", t1->id);
                if (t2->kind == CONSTANT)
                    fprintf(f, "#%d", t2->id);
                else
                    fprintf(f, "t%d", t2->id);
                if (x->kind == ADD)
                    fprintf(f, " + ");
                else if (x->kind == SUB)
                    fprintf(f, " - ");
                else if (x->kind == MUL)
                    fprintf(f, " * ");
                else if (x->kind == DIV)
                    fprintf(f, " / ");
                if (t3->kind == CONSTANT)
                    fprintf(f, "#%d", t3->id);
                else
                    fprintf(f, "t%d", t3->id);
            }
        } else if (x->kind == ADDRASS1) {
            OperandP t1 = x->uassign.op1, t2 = x->uassign.op2;
            fprintf(f, "t%d := &t%d", t1->id, t2->id);
        } else if (x->kind == ADDRASS2) {
            OperandP t1 = x->uassign.op1, t2 = x->uassign.op2;
            fprintf(f, "t%d := *t%d", t1->id, t2->id);
        } else if (x->kind == ADDRASS3) {
            OperandP t1 = x->uassign.op1, t2 = x->uassign.op2;
            fprintf(f, "*t%d := ", t1->id);
            if (t2->kind == CONSTANT)
                fprintf(f, "#%d", t2->id);
            else
                fprintf(f, "t%d", t2->id);
        } else if (x->kind == GOTO) {
            fprintf(f, "GOTO label%d", x->ulabel.op->id);
        } else if (x->kind == IF) {
            OperandP t1 = x->uif.x, re = x->uif.relop, t2 = x->uif.y, t3 = x->uif.z;
            fprintf(f, "IF ");
            if (t1->kind == CONSTANT)
                fprintf(f, "#%d", t1->id);
            else if (t1->kind == ADDRESS)
                fprintf(f, "*t%d", t1->id);
            else
                fprintf(f, "t%d", t1->id);

            if (re->id == RELOP_LESS)
                fprintf(f, " < ");
            else if (re->id == RELOP_LESSEQUAL)
                fprintf(f, " <= ");
            else if (re->id == RELOP_GREATER)
                fprintf(f, " > ");
            else if (re->id == RELOP_GREATEREQUAL)
                fprintf(f, " >= ");
            else if (re->id == RELOP_EQUAL)
                fprintf(f, " == ");
            else if (re->id == RELOP_NOTEQUAL)
                fprintf(f, " != ");

            if (t2->kind == CONSTANT)
                fprintf(f, "#%d", t2->id);
            else if (t2->kind == ADDRESS)
                fprintf(f, "*t%d", t2->id);
            else
                fprintf(f, "t%d", t2->id);
            fprintf(f, " GOTO label%d", t3->id);
        } else if (x->kind == RETURN) {
            fprintf(f, "RETURN ");
            if (x->ulabel.op->kind == CONSTANT)
                fprintf(f, "#%d", x->ulabel.op->id);
            else
                fprintf(f, "t%d", x->ulabel.op->id);
        } else if (x->kind == DEC) {
            fprintf(f, "DEC t%d %d", x->udec.op->id, x->udec.size);
        } else if (x->kind == ARG) {
            fprintf(f, "ARG ");
            if (x->ulabel.op->kind == CONSTANT)
                fprintf(f, "#%d", x->ulabel.op->id);
            else if (x->ulabel.op->kind == ADDRESS)
                fprintf(f, "&t%d", x->ulabel.op->id);
            else
                fprintf(f, "t%d", x->ulabel.op->id);
        } else if (x->kind == CALL) {
            OperandP t1 = x->uassign.op1, t2 = x->uassign.op2;
            if (t1)
                fprintf(f, "t%d := ", t1->id);
            else {
                OperandP t0 = creat_temp();
                fprintf(f, "t%d := ", t0->id);
            }
            fprintf(f, "CALL %s", t2->name);
        } else if (x->kind == PARAM) {
            fprintf(f, "PARAM ");
            if (x->ulabel.op->kind == CONSTANT)
                fprintf(f, "#%d", x->ulabel.op->id);
            else
                fprintf(f, "t%d", x->ulabel.op->id);
        } else if (x->kind == READ) {
            fprintf(f, "READ ");
            if (x->ulabel.op->kind == CONSTANT)
                fprintf(f, "#%d", x->ulabel.op->id);
            else
                fprintf(f, "t%d", x->ulabel.op->id);
        } else if (x->kind == WRITE) {
            fprintf(f, "WRITE ");
            if (x->ulabel.op->kind == CONSTANT)
                fprintf(f, "#%d", x->ulabel.op->id);
            else
                fprintf(f, "t%d", x->ulabel.op->id);
        }
        if (judge_is_null == 0)
            fprintf(f, "\n");
        else
            judge_is_null = 0;
        x = x->next;
    }
}

void translate_Program(NodeP x, FILE* F) {
    // Program -> ExtDefList
    translate_ExtDefList(x->children[0]);
    translate_print(F);
}

void translate_Cond(NodeP x, OperandP label_true, OperandP label_false) {
    if (x->child_num == 3 && strcmp(x->children[1]->data_string, "RELOP") == 0) {
        // Exp RELOP Exp
        OperandP t1 = creat_temp();
        OperandP t2 = creat_temp();
        translate_Exp(x->children[0], t1);
        translate_Exp(x->children[2], t2);
        OperandP op = create_operand(RELOP, x->children[1]->data_relop, NULL, NULL);
        insert_intercode(create_intercode(IF, INTERCODE_IF, t1, op, t2, label_true));
        insert_intercode(create_intercode(GOTO, INTERCODE_1, label_false));
    } else if (strcmp(x->children[0]->data_string, "NOT") == 0) {
        // NOT Exp
        translate_Cond(x->children[1], label_false, label_true);
    } else if (x->child_num == 3 && strcmp(x->children[1]->data_string, "AND") == 0) {
        // Exp AND Exp
        OperandP l1 = creat_label();
        translate_Cond(x->children[0], l1, label_false);
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
        translate_Cond(x->children[2], label_true, label_false);
    } else if (x->child_num == 3 && strcmp(x->children[1]->data_string, "OR") == 0) {
        // Exp OR Exp
        OperandP l1 = creat_label();
        translate_Cond(x->children[0], label_true, l1);
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
        translate_Cond(x->children[2], label_true, label_false);
    } else {
        // (other cases)
        OperandP t1 = creat_temp();
        OperandP op = create_operand(RELOP, RELOP_NOTEQUAL, NULL, NULL);
        OperandP t2 = create_operand(CONSTANT, 0, NULL, NULL);
        translate_Exp(x, t1);
        insert_intercode(create_intercode(IF, INTERCODE_IF, t1, op, t2, label_true));
        insert_intercode(create_intercode(GOTO, INTERCODE_1, label_false));
    }
}

void translate_ExtDefList(NodeP x) {
    // ExtDefList -> ExtDef ExtDefList | empty
    if (x == NULL)
        return;
    translate_ExtDef(x->children[0]);
    translate_ExtDefList(x->children[1]);
}

void translate_ExtDef(NodeP x) {
    // ExtDef -> Specifier SEMI
    if (x->child_num == 2)
        return;
    if (strcmp(x->children[1]->data_string, "ExtDecList") == 0) {
        // ExtDef -> Specifier ExtDecList SEMI
        return;
    } else if (strcmp(x->children[2]->data_string, "CompSt") == 0) {
        // ExtDef -> Specifier FunDec CompSt
        translate_FunDec(x->children[1]);
        translate_CompSt(x->children[2]);
    }
}

void translate_DefList(NodeP x) {
    // DefList -> Def DefList | empty
    while (x != NULL) {
        translate_Def(x->children[0]);
        x = x->children[1];
    }
}

void translate_Def(NodeP x) {
    // Def -> Specifier DecList SEMI
    return translate_DecList(x->children[1]);
}

void translate_DecList(NodeP x) {
    // DecList -> Dec | Dec COMMA DecList
    translate_Dec(x->children[0]);
    if (x->child_num > 1)
        translate_DecList(x->children[2]);
}

void translate_Dec(NodeP x) {
    // Dec -> VarDec | VarDec ASSIGNOP Exp
    if (x->child_num == 1) {
        OperandP t1 = creat_temp();
        translate_VarDec(x->children[0], t1);
    } else {
        OperandP t1 = creat_temp(), t2 = creat_temp();
        translate_VarDec(x->children[0], t1);
        translate_Exp(x->children[2], t2);
        insert_intercode(create_intercode(ASSIGN, INTERCODE_2, t1, t2));
    }
}

void translate_VarDec(NodeP x, OperandP place) {
    // VarDec -> ID | VarDec LB INT RB
    place->kind = VARIABLE;
    place->name = x->children[0]->data_string_data;
    if (x->child_num == 1) {
        // VarDec -> ID
        VarP var = find_var_table(x->children[0]->data_string_data);
        place->id = var->variable_cnt;
    } else {
        VarP var = find_var_table(x->children[0]->children[0]->data_string_data);
        place->id = var->variable_cnt;
        insert_intercode(create_intercode(DEC, INTERCODE_DEC, place, get_size(var->field->type)));
    }
}

void translate_FunDec(NodeP x) {
    // FunDec -> ID LP VarList RP | ID LP RP
    // function can only be defined, not declared
    OperandP op = create_operand(FUNCTION, OPERAND_NULL, x->children[0]->data_string_data, NULL);
    insert_intercode(create_intercode(IFUNCTION, INTERCODE_1, op));
    if (x->child_num == 3)
        return;
    FunctionP func = find_function_table(x->children[0]->data_string_data);
    FieldListP field_list = func->field;
    while (field_list != NULL) {
        VarP var = find_var_table(field_list->name);
        // there is no array
        OperandP field_list_op = create_operand(VARIABLE, var->variable_cnt, field_list->name, NULL);
        insert_intercode(create_intercode(PARAM, INTERCODE_1, field_list_op));
        field_list = field_list->next;
    }
}

void translate_CompSt(NodeP x) {
    // CompSt -> LC DefList StmtList RC
    translate_DefList(x->children[1]);
    translate_StmtList(x->children[2]);
}

void translate_StmtList(NodeP x) {
    // StmtList -> Stmt StmtList | empty
    if (x == NULL)
        return;
    translate_Stmt(x->children[0]);
    translate_StmtList(x->children[1]);
}

void translate_Stmt(NodeP x) {
    if (strcmp(x->children[0]->data_string, "CompSt") == 0) {
        // Stmt -> CompSt
        translate_CompSt(x->children[0]);
    } else if (strcmp(x->children[0]->data_string, "Exp") == 0) {
        // Stmt -> Exp SEMI
        translate_Exp(x->children[0], NULL);
    } else if (strcmp(x->children[0]->data_string, "RETURN") == 0) {
        // Stmt -> RETURN Exp SEMI
        // there is no array
        OperandP t1 = creat_temp();
        translate_Exp(x->children[1], t1);
        insert_intercode(create_intercode(RETURN, INTERCODE_1, t1));
    } else if (strcmp(x->children[0]->data_string, "IF") == 0) {
        if (x->children[6] == NULL) {
            // Stmt ->IF LP Exp RP Stmt
            OperandP l1 = creat_label(), l2 = creat_label();
            translate_Cond(x->children[2], l1, l2);
            insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
            translate_Stmt(x->children[4]);
            insert_intercode(create_intercode(ILABEL, INTERCODE_1, l2));
        } else {
            // IF LP Exp RP Stmt ELSE Stmt
            OperandP l1 = creat_label(), l2 = creat_label(), l3 = creat_label();
            translate_Cond(x->children[2], l1, l2);
            insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
            translate_Stmt(x->children[4]);
            insert_intercode(create_intercode(GOTO, INTERCODE_1, l3));
            insert_intercode(create_intercode(ILABEL, INTERCODE_1, l2));
            translate_Stmt(x->children[6]);
            insert_intercode(create_intercode(ILABEL, INTERCODE_1, l3));
        }
    } else if (strcmp(x->children[0]->data_string, "WHILE") == 0) {
        // Stmt -> WHILE LP Exp RP Stmt
        OperandP l1 = creat_label(), l2 = creat_label(), l3 = creat_label();
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
        translate_Cond(x->children[2], l2, l3);
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l2));
        translate_Stmt(x->children[4]);
        insert_intercode(create_intercode(GOTO, INTERCODE_1, l1));
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l3));
    }
}

void translate_Exp(NodeP x, OperandP place) {
    if (x->child_num == 1) {
        if (place == NULL)
            return;
        // no Exp -> FLOAT
        if (x->children[0]->type == TERMINAL_INT) {
            // Exp -> INT
            place->kind = CONSTANT;
            place->id = x->children[0]->data_int;
        } else if (strcmp(x->children[0]->data_string, "ID") == 0) {
            // Exp -> ID
            VarP var = find_var_table(x->children[0]->data_string_data);
            if (var->from_func == FROM_FUNC && var->field->type->type != BASIC)
                place->kind = ADDRESS;
            else
                place->kind = VARIABLE;
            place->name = x->children[0]->data_string_data;
            place->id = var->variable_cnt;
            place->type = var->field->type;
        }
    } else if (x->child_num == 2) {
        if (strcmp(x->children[0]->data_string, "MINUS") == 0) {
            // Exp -> MINUS Exp
            if (place == NULL)
                return;
            OperandP t1 = creat_temp();
            translate_Exp(x->children[1], t1);
            insert_intercode(create_intercode(SUB, INTERCODE_3, place, create_operand(CONSTANT, 0, NULL, NULL), t1));
        }
    } else if (strcmp(x->children[1]->data_string, "ASSIGNOP") == 0) {
        // Exp -> Exp ASSIGNOP Exp
        NodeP child_child = x->children[0]->children[0];
        VarP var = find_var_table(child_child->data_string_data);
        OperandP t1 = creat_temp();
        translate_Exp(x->children[2], t1);
        OperandP t2 = create_operand(VARIABLE, var->variable_cnt, child_child->data_string_data, NULL);
        insert_intercode(create_intercode(ASSIGN, INTERCODE_2, t2, t1));
        insert_intercode(create_intercode(ASSIGN, INTERCODE_2, place, t2));

    } else if (strcmp(x->children[1]->data_string, "AND") == 0 || strcmp(x->children[1]->data_string, "OR") == 0 || strcmp(x->children[1]->data_string, "RELOP") == 0 || strcmp(x->children[0]->data_string, "NOT") == 0) {
        // Exp -> Exp AND/OR/RELOP Exp | NOT Exp
        OperandP l1 = creat_label(), l2 = creat_label();
        insert_intercode(create_intercode(ASSIGN, INTERCODE_2, place, create_operand(CONSTANT, 0, NULL, NULL)));
        translate_Cond(x, l1, l2);
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l1));
        insert_intercode(create_intercode(ASSIGN, INTERCODE_2, place, create_operand(CONSTANT, 1, NULL, NULL)));
        insert_intercode(create_intercode(ILABEL, INTERCODE_1, l2));
    } else if (strcmp(x->children[1]->data_string, "PLUS") == 0 || strcmp(x->children[1]->data_string, "MINUS") == 0 || strcmp(x->children[1]->data_string, "STAR") == 0 || strcmp(x->children[1]->data_string, "DIV") == 0) {
        // Exp -> Exp PLUS/MINUS/STAR/DIV Exp
        OperandP t1 = creat_temp(), t2 = creat_temp();
        translate_Exp(x->children[0], t1);
        translate_Exp(x->children[2], t2);
        OperandP t3 = t1, t4 = t2;
        if (t1->kind == CONSTANT && t2->kind == CONSTANT)
            place->kind = VARIABLE;
        if (t1->kind == ADDRESS) {
            t3 = creat_temp();
            insert_intercode(create_intercode(ADDRASS2, INTERCODE_2, t3, t1));
        }
        if (t2->kind == ADDRESS) {
            t4 = creat_temp();
            insert_intercode(create_intercode(ADDRASS2, INTERCODE_2, t4, t2));
        }
        if (strcmp(x->children[1]->data_string, "PLUS") == 0) {
            insert_intercode(create_intercode(ADD, INTERCODE_3, place, t3, t4));
        } else if (strcmp(x->children[1]->data_string, "MINUS") == 0) {
            insert_intercode(create_intercode(SUB, INTERCODE_3, place, t3, t4));
        } else if (strcmp(x->children[1]->data_string, "STAR") == 0) {
            insert_intercode(create_intercode(MUL, INTERCODE_3, place, t3, t4));
        } else if (strcmp(x->children[1]->data_string, "DIV") == 0) {
            insert_intercode(create_intercode(DIV, INTERCODE_3, place, t3, t4));
        }
    } else if (strcmp(x->children[1]->data_string, "LB") == 0) {
        // Exp -> Exp LB Exp RB
        OperandP t0 = creat_temp(), t1 = creat_temp(), t2 = creat_temp(), t3 = creat_temp();
        t0->kind = ADDRESS;
        translate_Exp(x->children[0], t1);
        int offset = get_offset(t1->type);
        insert_intercode(create_intercode(ADDRASS1, INTERCODE_2, t0, t1));

        translate_Exp(x->children[2], t3);
        insert_intercode(create_intercode(MUL, INTERCODE_3, t2, t3, create_operand(CONSTANT, offset, NULL, NULL)));
        if (place == NULL)
            return;
        place->kind = ADDRESS;
        place->type = t1->type->array.elem;
        insert_intercode(create_intercode(ADD, INTERCODE_3, place, t0, t2));

    } else if (strcmp(x->children[0]->data_string, "LP") == 0) {
        // Exp -> LP Exp RP
        translate_Exp(x->children[1], place);
    } else if (strcmp(x->children[2]->data_string, "Args") == 0) {
        // Exp -> ID LP Args RP
        if (strcmp(x->children[0]->data_string_data, "write") == 0) {
            OperandP t1 = creat_temp();
            translate_Exp(x->children[2]->children[0], t1);
            insert_intercode(create_intercode(WRITE, INTERCODE_1, t1));
            return;
        }
        translate_Args(x->children[2], NULL);
        OperandP op = create_operand(FUNCTION, OPERAND_NULL, x->children[0]->data_string_data, NULL);
        insert_intercode(create_intercode(CALL, INTERCODE_2, place, op));
    } else if (strcmp(x->children[2]->data_string, "RP") == 0) {
        // Exp -> ID LP RP
        if (strcmp(x->children[0]->data_string_data, "read") == 0) {
            insert_intercode(create_intercode(READ, INTERCODE_1, place));
            return;
        }
        OperandP op = create_operand(FUNCTION, OPERAND_NULL, x->children[0]->data_string_data, NULL);
        insert_intercode(create_intercode(CALL, INTERCODE_2, place, op));
    }
}

void translate_Args(NodeP x, InterCodeP prev) {
    // Args -> Exp COMMA Args | Exp
    OperandP t1 = creat_temp();
    translate_Exp(x->children[0], t1);
    if (t1->kind == TEMP) {
        t1->kind = VARIABLE;
    }
    InterCodeP intercode = create_intercode(ARG, INTERCODE_1, t1);
    if (prev) {
        prev->next = intercode;
        intercode->prev = prev;
    }
    if (x->child_num > 1)
        translate_Args(x->children[2], intercode);
    insert_intercode(intercode);
}

int get_offset(TypeP return_type) {
    if (return_type->type == BASIC)
        return 4;
    else if (return_type->type == ARRAY)
        return get_size(return_type->array.elem);
}

int get_size(TypeP type) {
    if (type == NULL)
        return 0;
    if (type->type == BASIC)
        return 4;
    else if (type->type == ARRAY) {
        int temp = type->array.size * get_size(type->array.elem);
        return temp;
    }
}
