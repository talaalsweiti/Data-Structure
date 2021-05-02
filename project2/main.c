#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
DATA STRUCTURES PROJECT NUMBER 2
Tala ALsweiti
1191068
sec 2
**/

//define the struct
struct node ;
typedef struct node* ptr ;
typedef ptr stack;
struct node
{
    char sentence[100];
    ptr next;
};

//define the stacks we need , the essential one and another stack for the undo and redo commands
stack s ;
stack undoStack ;

//a function to check if the stack is empty
int isEmpty(stack emptyStack )
{
    return (emptyStack->next==NULL);
}

//a function to return a pointer to the top of the stack
ptr topOfStack (stack top)
{
    if(isEmpty(top))
        return NULL;
    return top->next;

}
//a function to delete the top of the stack
void pop (stack popStack)
{
    ptr temp ;
    if(!isEmpty(popStack))
    {
        temp = popStack->next;
        popStack->next = popStack ->next ->next;
        free(temp);
    }
    else
    {
        printf("Can't pop from an empty stack!!\n");
    }

}

// a function to push to the stack
void push (stack pushStack, char addedSentence[100])
{

    ptr temp = (ptr) malloc(sizeof(struct node));
    if(temp!=NULL)
    {
        strcpy( temp -> sentence,addedSentence);
        temp -> next = pushStack -> next;
        pushStack -> next = temp ;
    }
    else
    {
        printf("Out of memory");
        exit(1);
    }

}

//a function to create the stack
stack createStack()
{
    stack create = (stack) malloc(sizeof(struct node));
    if(create==NULL)
    {
        printf("Out of memory");
        exit(1);
    }
    create->next = NULL;
    return create ;

}

//a function to print the stack
void print(stack stackToPrint)
{

    int size =0;
    char temp[200][100];

    while(stackToPrint->next!=NULL)
    {
        stackToPrint =stackToPrint ->next;
        strcpy(temp[size],  stackToPrint->sentence);
        size++;
    }
    if(size!=0)
    {
        printf("result > \n");
        for(int i=size-1; i>-1; i--)
            printf("%s\n",temp[i]);
    }
    else
    {
        printf("The stack is empty! There is nothing to print \n");
    }
    printf("\n");
}

//a function to save the stack in a file
void saveInFile ()
{

    FILE *fptr = fopen("output.txt", "w");

    int size =0;
    char temp[200][100];
    while(s->next!=NULL)
    {
        s =s ->next;
        strcpy(temp[size],  s->sentence);
        size++;
    }
    if(size!=0)
    {
        fprintf(fptr, "results > \n");
        for(int i=size-1; i>-1; i--)
            fprintf(fptr,"%s\n",temp[i]);
    }
    else
    {

        fprintf(fptr, "The stack is empty! There is nothing to print \n");
    }

    fclose(fptr);
}
void choose()
{
    char command[100];
    printf("My Command > ");
    gets(command) ;
    printf("\n");

    if(strcmp(command,"undo")==0)
    {
        //if the stack not empty to do the undo
        if(!isEmpty(s))
        {
            push(undoStack,s->next->sentence);
            pop(s);
            print(s);
        }
        else
        {
            printf("Can't undo from an empty stack!\n\n");
        }
        choose();
    }

    //if the user had not undo anything he cant do redo
    else if (strcmp(command,"redo")==0)
    {

        if(!isEmpty(undoStack))
        {
            push(s,undoStack->next->sentence);
            pop(undoStack);
            print(s);
        }
        else
        {
            printf("You should undo a command to do a redo !\n\n");
        }
        choose();
    }

    else if (strcmp(command,"print")==0)
    {

        print(s);
        choose();
    }

    else if (strcmp(command,"save")==0)
    {
        saveInFile();
        printf("The stack has saved in the output file successfully!\n\n");
        choose();
    }

    else if (strcmp(command,"quit")==0)
    {
        printf("result > Good bye !\n");
        exit(1);
    }
    else
    {
        push(s,command);
        choose();
    }

}
int main()
{
    s = createStack();
    undoStack = createStack();
    printf(">>>>>>>>>>>>>>>>>>>>Welcome to the program<<<<<<<<<<<<<<<<<<<<\n");
    printf("______________________________________________________________\n");
    printf("Please note that you can choose any of the following commands after entering any statement\n\n");
    printf("**The commands in the following menu should entered as a word not as a number**\n\n");
    printf("______________________________________________________________\n");
    printf("1. undo\n");
    printf("______________________________________________________________\n");
    printf("2. redo\n");
    printf("______________________________________________________________\n");
    printf("3. print\n");
    printf("______________________________________________________________\n");
    printf("4. save\n");
    printf("______________________________________________________________\n");
    printf("5. quit\n\n");
    choose();
    return 0;
}
