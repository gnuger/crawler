#include <stdlib.h>

struct node
{
    char *value;
    struct node *next;
};

struct stack
{
    struct node *start;
};

push (struct stack *list, char *string)
{
    struct node *t_node = malloc (sizeof (struct node));
    t_node->value = string;
    t_node->next = list->start;
    list->start = t_node;
}

char *pop (struct stack *list)
{
    char *string = list->start->value;
    struct node *t_node = list->start;
    list->start = list->start->next;
    free (t_node);
    return string;
}

struct stack * newstack()
{
    struct stack *t_list = malloc (sizeof (struct stack));
    t_list->start = NULL;
    return t_list;
}
