#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#define size 300
/**
DATA STRUCTURES PROJECT NUMBER 1
Tala ALsweiti
1191068
sec 2
**/
struct equation;
typedef struct equation *ptr;
typedef ptr list;
typedef ptr position;
struct equation
{
    double coefficient;
    int exponent;
    //two pointers because it's a doubly linked list
    ptr next;
    ptr previous;
};
int arraySize = 0;

//a flag used to determine if the user read the file before perform the operations
int flg = 0;

//array of linked list for all equations
list arrayOfLinked[size];

//array of linked list to store the results of addition,subtraction,multiplication
list operationsArray[3];

/*
a function used to insert in the linked list
*/
void insertList(position tail, double factor, int pwr)
{
    //create a new node
    position temp=(position) malloc(sizeof(struct equation));
    position last = tail;
    if(temp!=NULL)
    {
        //store the data in the temp node
        temp -> coefficient =  factor;
        temp -> exponent = pwr;

        //to add the node at the end of the list
        temp -> next = NULL ;

        //if it's the first node
        if(tail==NULL)
        {
            temp -> previous = NULL;
            tail  = temp;
            return ;
        }

        //if the tail is not the last node
        while(last->next!=NULL)
            last = last -> next;

        last->next = temp;
        temp -> previous = last;

    }
    else
    {
        printf("sorry the memory is full");
    }

}

/*a function used to delete the list , I used it in the multiplication*/
void deleteList(list L)
{
    position P, temp;
    P = L->next;
    L->next = NULL;
    while(P != NULL)
    {
        temp = P->next;
        free(P);
        P=temp;
    }
}

/* used to read from the file then store the data in an array of linked list */
void read()
{
    char line[300];
    FILE* input ; //file pointer
    input=fopen("equations.txt","r"); //open the file to write
    if(input==NULL)  //if the file is empty
    {

        printf("Unable to open the file.\n");
        exit(1);
    }

    while(!feof(input))
    {

        fgets(line, 300,input); // read a line from the file
        int tall = strlen(line); // to get the line length
        int tmp1 = -1; //used to help in separating the polynomials

        //if it's not an empty line , create a list in the array
        if(line[0]!='\n'&&strcmp(line,"\n"))
        {
            arrayOfLinked[arraySize]=(list) malloc(sizeof(struct equation));
            arrayOfLinked[arraySize] -> next = NULL;

        }
        // a pointer to the head of the list
        position tail = arrayOfLinked[arraySize];

        for(int i=0; i<tall&&line[0]!='\n'&&strcmp(line,"\r\n"); i++)
        {

            double factor ;
            int factorPower;
            if(line[i]!=' ') //to store the coefficient
            {
                if(line[i]!='^')
                {
                    char tempFactor[20];
                    int k = 0;
                    int cnt =0;
                    while(line[i]!='x'&&i<tall)
                    {
                        if(i==0)
                            cnt++;
                        if(line[i]!=' ')
                        {
                            tempFactor[k] = line[i];
                            k++;
                        }
                        i++;

                    }
                    tempFactor[k] = '\0';

                    //C library function used to convert a string to double
                    factor = strtod(tempFactor,NULL);
                    if(factor==0&&line[i]=='x')
                    {
                        factor=1.0;
                    }
                    if((line[i]=='x'&&line[tmp1]=='-'&&factor==1.0) || (line[0]=='-'&&factor==1.0&&line[cnt]=='x'))
                        factor*=-1;

                }

                if(line[i]=='x') // to store the exponent
                {
                    int tmp= i ;
                    i++;
                    int flg = 0;
                    char tempPow[20];
                    int k = 0;
                    int tempCnt = i;
                    while(i<tall)
                    {
                        if(line[i]!= ' ')
                        {

                            if(line[i]!='^')
                            {
                                tempPow[k] = line[i];
                                k++;
                            }

                            if(line[i]=='-')
                            {

                                tmp1=i;
                                i--;
                                break;
                            }
                            if(line[i]=='+')
                            {
                                i--;
                                break;
                            }

                            i++;
                            tempCnt++;
                            if(line[tempCnt]!='^'&&line[tempCnt]!='\0'&&line[tempCnt]!=' ')
                            {
                                flg=1;
                            }
                            tempCnt++;
                        }
                        else
                            i++;
                    }
                    tempPow[k] = '\0';

                    //C library function used to convert a string to integer
                    factorPower = atoi(tempPow);
                    if(!flg&&line[tmp]=='x')
                    {
                        factorPower=1.0;
                    }
                }
                else
                {

                    factorPower = 0;
                }

                //to insert in the linked list
                insertList(tail,factor,factorPower);
                tail = tail->next;
            }
        }
        if(line[0]!='\n')
            arraySize++;
    }

    printf("\nRead Done Successfully !\n\n");
    fclose(input);

}

//print the equations
void printList()
{

//a loop on the array of linked list
    for(int x =0 ; x<arraySize; x++)
    {
        // a pointer for the x'th list
        position toPrint = arrayOfLinked[x];
        printf("\n%d equation :\n",x+1);
        while(toPrint->next!=NULL)
        {
            toPrint=toPrint->next;

            if(toPrint -> exponent!=0)
                printf("%.3lfx^%d ", toPrint -> coefficient,toPrint -> exponent);
            else
                printf("%.3lf ", toPrint -> coefficient);
            if(toPrint->next!=NULL)
            {
                position temp = toPrint -> next;
                if(temp->coefficient>=0)
                {
                    printf("+ ");
                }

            }
        }
        printf("\n\n");
    }

}

/* A function used to find a specific node */
position find(int x, list L)
{
    position p;
    p = L ->next;
    while(p!= NULL &&p->exponent!=x)
    {
        p = p->next;

    }
    return p;
}

/*A function used to delete any zero's results from the linked list after we make the operations */
void deleteZeros(list l)
{
    position p =  l -> next;
    while(p!=NULL)
    {
        position temp = p ;
        if(p->coefficient==0.0)
        {
            if(p->next!=NULL)
            {
                position prev = p ->previous;
                prev->next =p->next;
                p ->next ->previous = prev;
                free(temp);
            }
            else
            {
                position prev = p ->previous;
                prev->next =NULL;
                free(temp);

            }
        }
        p = p->next;
    }
    if(l->next==NULL)
    {
        insertList(l,0,0);
    }

}

/*To perform addition in the equations */
void addition ()
{
    //create a list to store the addition equation in it
    operationsArray[0]=(list) malloc(sizeof(struct equation));
    operationsArray[0] -> next = NULL;
    list L =  operationsArray[0];
    position tail = operationsArray[0];

    //a ptr to the first equation
    position addPtr = arrayOfLinked[0] -> next ;
    for(int i=0; i<arraySize; i++)
    {
        addPtr = arrayOfLinked[i] -> next;
        while(addPtr!=NULL)
        {

            position findExponent = find(addPtr->exponent,L);
            if(findExponent!=NULL)
            {

                findExponent -> coefficient += addPtr -> coefficient;
            }
            else
            {

                insertList(tail, addPtr -> coefficient,addPtr -> exponent);

            }

            addPtr = addPtr->next;
        }
    }
    //to delete any resulting zeros
    deleteZeros(operationsArray[0]);


}


/*To perform subtraction in the equations */
void subtraction()
{
    //operationsArray
    operationsArray[1]=(list) malloc(sizeof(struct equation));
    operationsArray[1] -> next = NULL;
    list L =  operationsArray[1];
    position tail = operationsArray[1];

    position addPtr = arrayOfLinked[0] -> next ;
    //insert the first equation in it
    while(addPtr!=NULL)
    {
        insertList(tail,addPtr -> coefficient,addPtr -> exponent);
        tail = tail -> next;
        addPtr = addPtr->next;

    }
    for(int i=1; i<arraySize; i++)
    {
        addPtr = arrayOfLinked[i] -> next;
        while(addPtr!=NULL)
        {

            position findExponent = find(addPtr->exponent,L);
            if(findExponent!=NULL)
            {

                findExponent -> coefficient -= addPtr -> coefficient;
            }
            else
            {
                insertList(tail,-1*addPtr -> coefficient,addPtr -> exponent);

            }

            addPtr = addPtr->next;
        }

    }
    deleteZeros(operationsArray[1]);

}

/*To perform multiplication in the equations */
void multiplication()
{
    operationsArray[2]=(list) malloc(sizeof(struct equation));
    operationsArray[2] -> next = NULL;
    list L =  operationsArray[2];
    position tail = operationsArray[2];

    position addPtr = arrayOfLinked[0] -> next ;
    while(addPtr!=NULL)
    {
        insertList(tail,addPtr -> coefficient,addPtr -> exponent);
        addPtr = addPtr->next;
    }
    list tempList = (list) malloc(sizeof(struct equation));
    tempList -> next = NULL;
    position tempTail = tempList;
    for(int i=1; i<arraySize; i++)
    {
        addPtr = arrayOfLinked[i] -> next;
        while(addPtr!=NULL)
        {
            position p = operationsArray[2]->next;
            while(p!=NULL)
            {
                insertList(tempTail,p ->coefficient * addPtr ->coefficient,p ->exponent + addPtr ->exponent);
                p = p -> next;

            }
            addPtr = addPtr->next;
        }

        position toSum =  tempList;
        //make addition to the equations with the same power
        while(toSum->next!=NULL)
        {
            toSum=toSum->next;
            int exp = toSum->exponent;
            position tempPtr = toSum->next ;
            while(tempPtr!=NULL)
            {
                if(exp==tempPtr->exponent)
                {
                    toSum ->coefficient +=  tempPtr ->coefficient;
                    tempPtr ->coefficient = 0;
                }
                tempPtr = tempPtr -> next;
            }
        }
        deleteList(L);
        operationsArray[2]=(list) malloc(sizeof(struct equation));
        operationsArray[2] -> next = NULL;
        L = operationsArray[2];
        tail = operationsArray[2];
        position toInsert =  tempList;

        while(toInsert->next!=NULL)
        {
            toInsert = toInsert -> next;

            //to insert in the linked list
            if(toInsert->coefficient!=0)
            {

                insertList(tail,toInsert ->coefficient,toInsert ->exponent );
            }


        }
        deleteList(tempList);
    }
    if(operationsArray[2]->next==NULL)
    {
        insertList(operationsArray[2],0,0);
    }
}

/*To perform substitution in the equations , the user can substitute multiple times*/
void substitute(list L)
{
    printf("\nPlease enter 1 if you want to substitute a value for x  \nOR 0 to back to the menu \n");
    int choice ;
    scanf("%d",&choice);
    if(choice==1)
    {
        double x ;
        printf("Please enter the value of x : \n");
        scanf("%lf",&x);
        position sum = L;
        double result = 0 ;
        while(sum->next!=NULL)
        {
            sum=sum->next;
            double powers = 1 ;
            int tempPowr =sum -> exponent;
            if(tempPowr==0)
            {
                result+= sum -> coefficient;
            }
            else
            {
                while(tempPowr!=0)
                {
                    powers*=x;
                    tempPowr--;
                }
                result+=(sum -> coefficient * powers );
            }
        }
        printf("\nThe result after substitute %lf is :\n%0.3lf \n",x,result);
        substitute(L);
    }
    else if(choice==0)
    {
        return ;
    }
    else {
        printf("Wrong Entry!\n");
        return;
    }
}
/*A function used to print the result in  an ouputfile*/
void printInFile()
{

    FILE *fptr = fopen("results.txt", "w");
    addition();
    subtraction();
    multiplication();
    for(int i=0; i<3; i++)
    {
        position toPrint = operationsArray[i];
        if(i==0)
        {
            fprintf(fptr, "The Addition equation is : \n");
        }
        else if (i==1)
        {
            fprintf(fptr, "The Subtraction equation is : \n");
        }
        else
        {
            fprintf(fptr, "The Multiplication equation is : \n");
        }

        while(toPrint->next!=NULL)
        {
            toPrint=toPrint->next;
            if(toPrint -> exponent!=0 )
                fprintf(fptr,"%.3lfx^%d ", toPrint -> coefficient,toPrint -> exponent);
            else
                fprintf(fptr,"%.3lf ", toPrint -> coefficient);
            if(toPrint->next!=NULL)
            {
                position temp = toPrint -> next;
                if(temp->coefficient>0)
                {
                    fprintf(fptr,"+ ");
                }

            }
        }
        fprintf(fptr,"\n");

    }
    printf("\nThe result has been successfully printed on the output file!\n\n");
    fclose(fptr);
}

/*A function to print the resulting equation*/
void  printEqu(list L)
{

    position toPrint = L;
    while(toPrint->next!=NULL)
    {
        toPrint=toPrint->next;
        if(toPrint -> exponent!=0 )
            printf("%.3lfx^%d ", toPrint -> coefficient,toPrint -> exponent);
        else
            printf("%.3lf ", toPrint -> coefficient);
        if(toPrint->next!=NULL)
        {
            position temp = toPrint -> next;
            if(temp->coefficient>0)
            {
                printf("+ ");
            }

        }
    }
    printf("\n");
}
void menu()
{

    //printing the menu to the user
    printf(">>>>>>>>>>>>>>>>>>>>Welcome to the program<<<<<<<<<<<<<<<<<<<<\n");
    printf("______________________________________________________________\n");
    printf("                            MENU                   \n");
    printf("______________________________________________________________\n");
    printf("Press 1. To read the input file\n");
    printf("______________________________________________________________\n");
    printf("Press 2. To print the equations\n");
    printf("______________________________________________________________\n");
    printf("Press 3. Perform Addition\n");
    printf("______________________________________________________________\n");
    printf("Press 4. Perform Subtraction\n");
    printf("______________________________________________________________\n");
    printf("Press 5. Perform Multiplication\n");
    printf("______________________________________________________________\n");
    printf("Press 6. To Print results in the output file\n");
    printf("______________________________________________________________\n");
    printf("Press 7. Exit\n");
    printf("______________________________________________________________\n");
    int x;
    scanf("%d",&x);

    switch(x)
    {
    case 1 :
        //if the user did not read the file
        if(flg!=1)
        {
            flg =1;
            read();
        }
        else
        {
            printf("\nYou have already read the input file!\n\n");
        }
        menu();
        break;
    case 2 :
        if(flg==0)
        {
            printf("\nPlease read the input file first!\n\n");
        }
        else
        {
            printList();
        }
        menu();
        break;

    case 3 :
        if(flg==0)
        {
            printf("\nPlease read the input file first!\n\n");
        }
        else
        {
            addition();
            printf("\nThe Addition of the equations is : \n");
            printEqu(operationsArray[0]);
            printf("\n");
            substitute(operationsArray[0]);
        }
        menu();
        break;
    case 4 :
        if(flg==0)
        {
            printf("\nPlease read the input file first!\n\n");
        }
        else
        {
            subtraction();
            printf("\nThe Subtraction of the equations is : \n");
            printEqu(operationsArray[1]);
            printf("\n");
            substitute(operationsArray[1]);
        }
        menu();
        break;
    case 5 :
        if(flg==0)
        {
            printf("\nPlease read the input file first!\n\n");
        }
        else
        {
            multiplication();
            printf("\nThe Multiplication of the equations is : \n");
            printEqu(operationsArray[2]);
            printf("\n");
            substitute(operationsArray[2]);
        }
        menu();
        break;
    case 6 :
        if(flg==0)
        {
            printf("\nPlease read the input file first!\n\n");
        }
        else
        {
            printInFile();
        }
        menu();
        break;
    case 7 :
        printf("\nThank you for using our program!\n");
        exit (1) ;
    default :
        printf("Invalid Entry, Please enter a valid number from the menu \n\n");
        menu();
    }
}

int main()
{
    menu();
    return 0;
}
