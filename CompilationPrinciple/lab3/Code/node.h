
#ifndef NODE_H
#define NODE_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILD_NUM 10
#define MAX_STRING_LEN 32
#define NONTERMINAL 0
#define TERMINAL_INT 1
#define TERMINAL_FLOAT 2
#define TERMINAL_STRING 3
#define TERMINAL_TYPE_OR_ID 4
#define RELOP_LESS 5
#define RELOP_LESSEQUAL 6
#define RELOP_GREATER 7
#define RELOP_GREATEREQUAL 8
#define RELOP_EQUAL 9
#define RELOP_NOTEQUAL 10

struct Node {
    int data_int, child_num, line, data_relop;
    short type;
    float data_float;
    char data_string[MAX_STRING_LEN], data_string_data[MAX_STRING_LEN];
    struct Node *children[MAX_CHILD_NUM];
};
typedef struct Node *NodeP;
static NodeP get_nonterminal_node(int line, char *data_string, int child_num, ...) {
    va_list valist;
    NodeP node = (NodeP)malloc(sizeof(struct Node));

    node->type = NONTERMINAL;
    node->child_num = child_num;
    node->line = line;
    strcpy(node->data_string, data_string);

    va_start(valist, child_num);
    for (int i = 0; i < child_num; i++) {
        node->children[i] = va_arg(valist, NodeP);
    }
    va_end(valist);
    return node;
}
static NodeP get_terminal_node_float(float data_float) {
    NodeP node = (NodeP)malloc(sizeof(struct Node));
    node->type = TERMINAL_FLOAT;
    node->child_num = 0;
    node->data_float = data_float;
    return node;
}
static NodeP get_terminal_node_int(int data_int) {
    NodeP node = (NodeP)malloc(sizeof(struct Node));
    node->type = TERMINAL_INT;
    node->child_num = 0;
    node->data_int = data_int;
    return node;
}
static NodeP get_terminal_node(char *data_string) {
    NodeP node = (NodeP)malloc(sizeof(struct Node));
    node->type = TERMINAL_STRING;
    node->child_num = 0;
    strcpy(node->data_string, data_string);
    return node;
}
static NodeP get_terminal_node_relop(char *data_string, char *data_string_data) {
    NodeP node = (NodeP)malloc(sizeof(struct Node));
    node->type = TERMINAL_STRING;
    node->child_num = 0;
    strcpy(node->data_string, data_string);
    if (strcmp(data_string_data, ">") == 0) {
        node->data_relop = RELOP_GREATER;
    } else if (strcmp(data_string_data, ">=") == 0) {
        node->data_relop = RELOP_GREATEREQUAL;
    } else if (strcmp(data_string_data, "<") == 0) {
        node->data_relop = RELOP_LESS;
    } else if (strcmp(data_string_data, "<=") == 0) {
        node->data_relop = RELOP_LESSEQUAL;
    } else if (strcmp(data_string_data, "==") == 0) {
        node->data_relop = RELOP_EQUAL;
    } else if (strcmp(data_string_data, "!=") == 0) {
        node->data_relop = RELOP_NOTEQUAL;
    }
    return node;
}
static NodeP get_terminal_node_string(char *data_string, char *data_string_data) {
    NodeP node = (NodeP)malloc(sizeof(struct Node));
    node->type = TERMINAL_TYPE_OR_ID;
    node->child_num = 0;
    strcpy(node->data_string, data_string);
    strcpy(node->data_string_data, data_string_data);
    return node;
}

static void print_node_tree(NodeP root, int level) {
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
        printf("  ");

    if (root->type == NONTERMINAL) {
        printf("%s (%d)\n", root->data_string, root->line);
        for (int i = 0; i < root->child_num; i++)
            print_node_tree(root->children[i], level + 1);
    } else if (root->type == TERMINAL_INT) {
        printf("INT: %d\n", root->data_int);
    } else if (root->type == TERMINAL_FLOAT) {
        printf("FLOAT: %f\n", root->data_float);
    } else if (root->type == TERMINAL_STRING) {
        printf("%s\n", root->data_string);
    } else if (root->type == TERMINAL_TYPE_OR_ID) {
        printf("%s: %s\n", root->data_string, root->data_string_data);
    }
}

#endif