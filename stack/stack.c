#include <stdio.h>
#include <stdlib.h>

struct listnode{
        int value;
        struct listnode *next;
};

struct Stack{
        struct listnode *top;
        int size;
};

struct Stack *stack_create(){
        struct Stack *s = malloc(sizeof(*s));

        if(s != NULL){
        s->size = 0;
        s->top = NULL;
        }
return s;
}

struct listnode *list_createnode(int value){
        struct listnode *p = malloc(sizeof(*p));

        if(p != NULL){
        p->value = value;
        p->next = NULL;
        }
return p;
}

struct listnode *add_front(struct listnode *top, int value){
        struct listnode *newnode;

        newnode = list_createnode(value);
        if(newnode != NULL){
        newnode->value = value;
        newnode->next = top;
        return newnode;
        }
return top;
}

int stack_push(struct Stack *s, int value){
        s->top = add_front(s->top, value);
        if(s->top == NULL){
        puts("Stack overflow.\n");
        return -1;
        }
        s->size++;
return 0;
}

int main(void){
        struct Stack *s;

        s = stack_create();
        for(int i = 1; i <= 10; i++){
        stack_push(s, i);
        printf("%d\n", s->top->value);
        }
        //stack_free(s);
return 0;
}
