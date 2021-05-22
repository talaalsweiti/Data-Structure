#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/**
DATA STRUCTURES PROJECT NUMBER 3
Tala ALsweiti
1191068
sec 2
**/

int flg = 0;
//to get the number of patients in the tree
int pateintsNumber=0;
//to get the number of patients in the hash table
int hashTableSize = 0;

//to get the capacity of the hash table
int tableCapacity =0 ;

//to calculate the load factor for the hash table
double loadFactor ;
int isReadTree = 0 ;


//A STRCUT FOR THE AVL TREE
struct AVLnode;
typedef struct AVLnode *AVL_T;
struct AVLnode
{
    char name[30];
    char gender[2];
    char dateOfAdmission[10];
    char dateOfBirth[10];
    char illness[30];
    char address[30];
    char blood[4];
    AVL_T Left;
    AVL_T Right;
    int Height;
};
//the root of the tree
AVL_T root = NULL;
//////////////////////////
//a struct for the information in the hash table
struct info;
typedef struct info *hashItems ;
struct info
{
    char name[30];
    char gender[2];
    char dateOfAdmission[10];
    char dateOfBirth[10];
    char illness[30];
    char address[30];
    char blood[4];
    /*
     * status= 0 :EMPTY
     * status = 1 : OCCUPIED
     * status = 2 : DELETED
    */
    int status ;

};

//////////////////////////
//a struct for the hash table
struct hash;
typedef struct hash *hashPtr;
struct hash
{
    hashItems array;
//temp pointer to the information used in the rehash
    hashItems tempArray;

};
//main hash table
hashPtr hashTable;
//////////////////////////////////////////////
/*
To check the the date that the user will enter
*/
int checkDate(char date[])
{

    for(int i=0; i<8; i++)
    {
        char c = date[i];
        if(isalpha(c))
        {

            return 0;
        }

    }

    if(strlen(date)>8) return 0;
    return 1;


}

////////////////////////////////////////////////
/*
to get the first prime equal or bigger than the table size
*/
int getPrime(int x)
{
    int flg = 1;
    for(int i=x; i<1e9; i++)
    {
        for(int j=2; j*j<=i; j++)
        {
            if(i%j==0)
            {
                //if not prime
                flg = 0;
                break;
            }
        }
        if(flg)
            return i;
        flg =1;
    }

    return x;
}
////////////////////////////////////////////////
/*
 To get the number of patients in the tree
*/
void numberOfPatients(AVL_T t)
{
    if(t!= NULL)
    {
        pateintsNumber++;
        numberOfPatients(t->Left);
        numberOfPatients(t->Right);
    }


}
////////////////////////////////////////////////
/*
 to create and initialize the table
*/
hashPtr createTable (int tableSize)
{

    hashPtr table = (hashPtr)malloc(sizeof(struct hash)*tableSize) ;
    hashTableSize = 0;
    tableCapacity = tableSize;

    for (int i=0 ; i<tableCapacity ; i++)
    {
        //put all cells to empty
        table->array[i].status = 0;
    }

    return table ;
}
////////////////////////////////////////
/*
 to get the key for the patient name by adding the ascii value for the chars
*/
int key(char name[])
{
    int sum =0;

    for(int i=0; i<strlen(name); i++)
    {
        if(name[i]!=' ')
        {
            sum+=name[i];
        }
    }
    return sum;
}
//////////////////////////////////////
/*
 to print the hash table
*/
void printHash(hashPtr table)
{
    printf("\n\t\t\t                  Hash table for the Patients                     \n");
    printf("_________________________________________________________________________________________________________________________\n");

    printf("Cell status\t Name       \t Gender      Date of admission    Date of birth   Illness   Address (City)     Blood type\n");

    printf("_________________________________________________________________________________________________________________________\n");
    for(int i=0; i<tableCapacity; i++)
    {
        if( table->array[i].status==1)
        {
            printf("OCCUPIED       %s        %s       \t", table->array[i].name,table->array[i].gender);
            int j =0;
            //to print the date in a correct format
            for(; j<4; j+=2)
            {
                printf("%c%c-",table->array[i].dateOfAdmission[j],table->array[i].dateOfAdmission[j+1]);
            }
            for(j=4; j<8; j++)
            {
                printf("%c",table->array[i].dateOfAdmission[j]);
            }
            printf("\t");
            for( j=0; j<4; j+=2)
            {
                printf("%c%c-",table->array[i].dateOfBirth[j],table->array[i].dateOfBirth[j+1]);
            }
            for(j=4; j<8; j++)
            {
                printf("%c",table->array[i].dateOfBirth[j]);
            }
            printf(" \t %s \t %s \t  %s\n",table->array[i].illness,table->array[i].address,table->array[i].blood);

        }
        else  if( table->array[i].status==2)
        {
            printf("DELETED \t NULL       \t NULL \t\t NULL \t\t NULL \t\t NULL \t\t NULL \tNULL \n");
        }
        else
        {
            printf("EMPTY \t\t NULL       \t NULL \t\t NULL \t\t NULL \t\t NULL \t\t NULL \t\t   NULL \n");
        }

        printf("_________________________________________________________________________________________________________________________\n");
    }


}
////////////////////////////////////////////////
/*
if the load factor is more than 0.75 or the insertion failed , will do a rehash
*/
void rehash(hashPtr table)
{
    int size = tableCapacity ;
    int x = getPrime((2*size));
    //create and initialize the new table with double size
    hashPtr newTable = (hashPtr)malloc(sizeof(struct hash)*x) ;
    hashTableSize = 0;
    tableCapacity = x;
    for(int i=0; i<tableCapacity; i++)
    {
        newTable->tempArray[i].status=0;
    }

    for (int i=0; i<size; i++) //moving the old elements of the table to the new one
    {
        if (table->array[i].status==1)
        {
            int j=1;
            int index = (key(table->array[i].name))%tableCapacity;

            while((newTable->array[index].status ==1) && (j < tableCapacity))
            {
                index = ((key(table->array[i].name))+(j*j))%tableCapacity;
                j++;
            }
            strcpy(newTable->tempArray[index].name,  table->array[i].name);
            strcpy(newTable->tempArray[index].blood,  table->array[i].blood);
            strcpy(newTable->tempArray[index].address,  table->array[i].address);
            strcpy(newTable->tempArray[index].dateOfAdmission,   table->array[i].dateOfAdmission);
            strcpy(newTable->tempArray[index].dateOfBirth,   table->array[i].dateOfBirth);
            strcpy(newTable->tempArray[index].gender, table->array[i].gender);
            strcpy(newTable->tempArray[index].illness,  table->array[i].illness);
            newTable->tempArray[index].status = 1;
            hashTableSize++;

        }
    }
//put the new table in the main table
    hashTable = newTable;

}
////////////////////////////////
/*
insert the avl tree in the hash table
*/
void insertTable(AVL_T t)  // create new hash table
{
    if(t!=NULL)
    {
        //calculate the load factor
        loadFactor=(double)hashTableSize/tableCapacity;
        if(loadFactor>=0.75)
        {
            hashPtr temp = createTable(tableCapacity);
            temp=hashTable;
            rehash(temp);
        }

        int i=1;
        int index = key(t->name)%tableCapacity;
        while(hashTable->array[index].status ==1 && i<tableCapacity)
        {
            index = ((key(t->name))+(i*i))%tableCapacity;
            i++;
        }
//if the insertion failed will do a rehash
        if(hashTable->array[index].status ==1)
        {

            rehash(hashTable);
            i=1;
            index = key(t->name)%tableCapacity;
            while(hashTable->array[index].status ==1 && i<tableCapacity)
            {
                index = ((key(t->name))+(i*i))%tableCapacity;
                i++;
            }
        }
        strcpy(hashTable->array[index].name, t->name);
        strcpy(hashTable->array[index].blood, t->blood);
        strcpy(hashTable->array[index].address, t->address);
        strcpy(hashTable->array[index].dateOfAdmission, t->dateOfAdmission);
        strcpy(hashTable->array[index].dateOfBirth, t->dateOfBirth);
        strcpy(hashTable->array[index].gender, t->gender);
        strcpy(hashTable->array[index].illness, t->illness);

        hashTable->array[index].status = 1;
        hashTableSize ++;

        insertTable(t->Left);
        insertTable(t->Right);
    }

}
////////////////////////////////
/*
insert in the hash table from the console
*/
void insertInHash()
{


    fflush(stdin);
    char nameReading[30];
    char genderReading[2];
    char dateOfAdmissionReading[10];
    char dateOfBirthReading[10];
    char illnessReading[30];
    char addressReading[30];
    char bloodReading[4];

    printf("Please Enter the patient name : ");
    gets(nameReading);



    printf("\nPlease Enter the patient gender : ");
    gets(genderReading);
    //check if it's correct gender
    while(strcasecmp(genderReading,"f")!=0 && strcasecmp(genderReading,"m")!=0 )
    {
        printf("\nInvalid gender , Please Enter the patient gender again : ");
        gets(genderReading);
    }

    printf("\nPlease Enter the patient date of admissionReading : ");
    gets(dateOfAdmissionReading);
    //check if it's valid date
    while(!checkDate(dateOfAdmissionReading))
    {
        printf("\nInvalid date of Admission , Please Enter the date again : ");
        gets(dateOfAdmissionReading);
    }
    printf("\nPlease Enter the patient date of birth: ");
    gets(dateOfBirthReading);
    //check if it's valid date
    while(!checkDate(dateOfBirthReading))
    {
        printf("\nInvalid date of Birth , Please Enter the date again : ");
        gets(dateOfBirthReading);
    }
    printf("\nPlease Enter the patient address : ");
    gets(addressReading);


    printf("\nPlease Enter the patient illness : ");
    gets(illnessReading);


    printf("\nPlease Enter the patient blood type : ");
    gets(bloodReading);

    loadFactor=(double)hashTableSize/tableCapacity;

    if(loadFactor>=0.75)
    {
        //calculate the load factor
        rehash(hashTable);
    }

    int i=1;
    int index = key(nameReading)%tableCapacity;

    while(hashTable->array[index].status ==1 && i<tableCapacity)
    {
        index = ((key(nameReading))+(i*i))%tableCapacity;
        i++;
    }
    //if the insertion failed
    if(hashTable->array[index].status ==1)
    {

        rehash(hashTable);
        i=1;
        index = key(nameReading)%tableCapacity;
        while(hashTable->array[index].status ==1 && i<tableCapacity)
        {
            index = ((key(nameReading))+(i*i))%tableCapacity;
            i++;
        }
    }
    strcpy(hashTable->array[index].name,nameReading);
    strcpy(hashTable->array[index].blood, bloodReading);
    strcpy(hashTable->array[index].address, addressReading);
    strcpy(hashTable->array[index].dateOfAdmission, dateOfAdmissionReading);
    strcpy(hashTable->array[index].dateOfBirth,dateOfBirthReading);
    strcpy(hashTable->array[index].gender, genderReading);
    strcpy(hashTable->array[index].illness,illnessReading);
    hashTable->array[index].status = 1;
    hashTableSize ++;
    printf("\nPatient added successfully!\n");
    printf("\n\nPlease enter 1 if you want to enter another patient ,or any number to go back to the menu\n");
    int x ;
    scanf("%d",&x);
    if(x==1)
    {
        insertInHash();
    }
    else
    {
        return;
    }


}
///////////////////////////////
/*
to search for a patient in the hash table
*/
int searchInHash(hashPtr table,char name[])
{
    for(int i=0; i<tableCapacity; i++)
    {
        if( table->array[i].status==1&&strcmp(name,table->array[i].name)==0)
            return i;
    }
    return -1;
}


///////////////////////////////////
/**
A function to print the hash table in a file and exit the program

**NOTE : The function print empty table though the code is correct and simple , and I could not know why .
I asked Dr.Ahmad if should I move the data to an array of strings but he said to leave it as it is.

**/
void printHashInFile( )
{

    FILE *fptr  = fopen("patients_hash.data.txt", "w");
    fprintf(fptr,"\t\t\t                  Hash table for the Patients                     \n");
    fprintf(fptr,"_________________________________________________________________________________________________________________________\n");
    fprintf(fptr,"Cell status\t Name       \t Gender      Date of admission    Date of birth   Illness   Address (City)     Blood type\n");
    fprintf(fptr,"_________________________________________________________________________________________________________________________\n");
    for(int i=0; i<tableCapacity; i++)
    {
        if(hashTable->array[i].status==1)
        {
            fprintf(fptr,"OCCUPIED       %s        %s      \t", hashTable->array[i].name,hashTable->array[i].gender);
            int j =0;
            for(; j<4; j+=2)
            {
                fprintf(fptr,"%c%c-",hashTable->array[i].dateOfAdmission[j],hashTable->array[i].dateOfAdmission[j+1]);
            }
            for(j=4; j<8; j++)
            {
                fprintf(fptr,"%c",hashTable->array[i].dateOfAdmission[j]);
            }
            fprintf(fptr,"\t");
            for( j=0; j<4; j+=2)
            {
                fprintf(fptr,"%c%c-",hashTable->array[i].dateOfBirth[j],hashTable->array[i].dateOfBirth[j+1]);
            }
            for(j=4; j<8; j++)
            {
                fprintf(fptr,"%c",hashTable->array[i].dateOfBirth[j]);
            }
            fprintf(fptr," \t %s \t %s \t   %s \n",hashTable->array[i].illness,hashTable->array[i].address,hashTable->array[i].blood);

        fprintf(fptr,"_________________________________________________________________________________________________________________________\n");
        }
        else  if( hashTable->array[i].status==2)
        {
            fprintf(fptr,"DELETED \t\t NULL       \t NULL \t\t NULL \t\t NULL \t\t NULL \t\t NULL \t\t    NULL \n");

        fprintf(fptr,"_________________________________________________________________________________________________________________________\n");
        }
        else if(hashTable->array[i].status==0)
        {
            fprintf(fptr,"EMPTY \t\t NULL       \t NULL \t\t NULL \t\t NULL \t\t NULL \t\t NULL \t\t   NULL \n");

        fprintf(fptr,"_________________________________________________________________________________________________________________________\n");
        }

    }

    fclose(fptr);
    printf("Thank you for using our program!\n");
    exit(1);
}
//////////////////////////////////////////////
/*
Hash part menu
*/
void hashMenu()
{


    printf("\n                     MENU - Hash Part                   \n");
    printf("______________________________________________________________\n");
    printf("Press 1. To Print hashed table\n");
    printf("______________________________________________________________\n");
    printf("Press 2. To Print out table size\n");
    printf("______________________________________________________________\n");
    printf("Press 3. To Print out the used hash function\n");
    printf("______________________________________________________________\n");
    printf("Press 4. To Insert a new record into the hash table\n");
    printf("______________________________________________________________\n");
    printf("Press 5. To Search for a specific patient\n");
    printf("______________________________________________________________\n");
    printf("Press 6. To Delete a specific record\n");
    printf("______________________________________________________________\n");
    printf("Press 7. To Save hash table back to file and exit.\n");
    printf("______________________________________________________________\n");
    int x;
    scanf("%d",&x);
    switch(x)
    {
    case 1:
        printHash(hashTable);
        hashMenu();
        return ;
        break;
    case 2:
        printf("The table size is : %d\nThe table capacity is : %d\n",hashTableSize, tableCapacity);
        hashMenu();
        break;
    case 3:
        printf("\nThe used hash function is the Quadratic probing (nonlinear search) with the following equation\n");
        printf("hi(key) = [h(key) + i^2] mood tableSize , for i = 0, 1, . . . , tableSize - 1 \n");
        hashMenu();
        break;
    case 4:
        insertInHash();
        printf("Table after adding : \n\n");
        printHash(hashTable);
        hashMenu();
        break;
    case 5:

        printf("Please enter the patient name you want to find\n");
        fflush(stdin);
        char to_find[30];
        gets(to_find);
        int index = searchInHash(hashTable,to_find);
        if(index!=-1)
        {
            printf("\nPatient %s found successfully !\n",to_find);
            printf("\nThe patient information : \n");
            printf("Name : %s\n",hashTable->array[index].name);
            printf("Gender : %s\n",hashTable->array[index].gender);
            printf("Date Of Admission : ");
            int j =0;
            for(; j<4; j+=2)
            {
                printf("%c%c-",hashTable->array[index].dateOfAdmission[j],hashTable->array[index].dateOfAdmission[j+1]);
            }
            for(j=4; j<8; j++)
            {
                printf("%c",hashTable->array[index].dateOfAdmission[j]);
            }
            printf("\nDate of Birth : ");
            for( j=0; j<4; j+=2)
            {
                printf("%c%c-",hashTable->array[index].dateOfBirth[j],hashTable->array[index].dateOfBirth[j+1]);
            }
            for(j=4; j<8; j++)
            {
                printf("%c",hashTable->array[index].dateOfBirth[j]);
            }
            printf("\nAddress : %s\n",hashTable->array[index].address);
            printf("Illness : %s\n",hashTable->array[index].illness);
            printf("Blood type : %s\n",hashTable->array[index].blood);
            printf("____________________________________________\n");
        }
        else
        {
            printf("\nPatient %s not found , please try again\n",to_find);
        }

        hashMenu();
        break;
    case 6:
        printf("\nPlease enter the patient name you want to delete\n");
        fflush(stdin);
        char toDelete[30];
        gets(toDelete);
        int indexDelete = searchInHash(hashTable,toDelete);
        //if the patient is in the table , change status to deleted
        if(indexDelete!=-1)
        {
            hashTable->array[indexDelete].status=2;
            hashTableSize--;
            printf("\nPatient ( %s ) deleted successfully\n",toDelete);
        }
        else
        {
            printf("\nPatient ( %s ) not found , please try again\n",toDelete);
        }
        hashMenu();
        break;
    case 7:
        //print the table in a file and exit
        printHashInFile();
    default :
        printf("\nThe number you enter is not valid , Please try again\n\n");
        hashMenu();
        break;
    }

}

/**





AVL TREE PART



**/

/*
to get the hight of the node
*/
int getHeight (AVL_T t)
{
    if(t==NULL)
        return -1;
    else
        return t->Height;
}
////////////////////////////////////////////////
/*
to get the maximum number
*/
int MAX (int x, int y)
{
    if(x>y)
        return x;
    else
        return y;
}
////////////////////////////////////////////////
/*
To make a single left rotation
*/
AVL_T singleRotateLeft(AVL_T x)
{
    AVL_T y;
    y         = x->Right;
    x->Right  = y->Left;
    y->Left = x;

    x->Height = 1 + MAX(getHeight(x->Left), getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x), getHeight(y->Right));

    return y;
}
////////////////////////////////////////////////
/*
To make a single right rotation
*/
AVL_T singleRotateRight(AVL_T y)
{
    AVL_T x;
    x        = y->Left;
    y->Left  = x->Right;
    x->Right = y;

    y->Height = 1 + MAX(getHeight(y->Right), getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y), getHeight(x->Left));

    return x;
}
////////////////////////////////////////////////
/*
To make a double left rotation
*/
AVL_T doubleRotateLeft(AVL_T K3)
{
    K3->Left  = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}
////////////////////////////////////////////////
/*
To make a double right rotation
*/
AVL_T doubleRotateRight(AVL_T K3)
{
    K3->Right  = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}
//////////////////////////////////////////////
/*
To check the balance for the node
*/
int checkBalance(AVL_T t)
{
    if (t== NULL)
        return 0;
    return getHeight(t->Left) - getHeight(t->Right);
}

////////////////////////////////////////////////
/*
To insert in the tree
*/
AVL_T insert (AVL_T t,char p_name[], char p_gender[],char admissionDate [], char birthDate [], char p_illness [],char p_address [],char p_blood [])
{

    if(t==NULL)
    {

        t=(AVL_T)malloc(sizeof (struct AVLnode));
        strcpy(t->name,p_name);
        strcpy(t->gender,p_gender);
        strcpy(t->dateOfAdmission,admissionDate);
        strcpy(t->dateOfBirth,birthDate);
        strcpy(t->illness,p_illness);
        strcpy(t->address,p_address);
        strcpy(t->blood,p_blood);
        t->Height=0;
        t->Left= t->Right=NULL;
    }

    else if(strcasecmp(p_name,t->name)<0)
    {
        t->Left=insert(t->Left,p_name,p_gender,admissionDate,birthDate,p_illness, p_address, p_blood);
        if((getHeight(t->Left) - getHeight(t->Right))>= 2)
        {
            if(strcasecmp(p_name,t->Left->name)<0)
                t=singleRotateRight(t);
            else
                t=doubleRotateLeft(t);
        }
    }
    else if(strcasecmp(p_name,t->name)>0)
    {
        t->Right = insert(t->Right,p_name,p_gender,admissionDate,birthDate,p_illness, p_address, p_blood);
        if((getHeight(t->Right) - getHeight(t->Left)) >= 2)
        {
            if(strcasecmp(p_name,t-> Right->name)>0)
                t=singleRotateLeft(t);
            else
                t=doubleRotateRight(t);
        }
    }

    t->Height = 1 + MAX(getHeight(t->Right), getHeight(t->Left));

    return t;
}
////////////////////////////////////////////////
/*
To print the a node
*/
void printInformation(AVL_T t)
{
    printf("\nThe patient information :\n");
    printf("Name : %s\n",t->name);
    printf("Gender : %s\n",t->gender);
    printf("Date of Admission :  ");
    int j =0;
    for(; j<4; j+=2)
    {
        printf("%c%c-",t->dateOfAdmission[j],t->dateOfAdmission[j+1]);
    }
    for(j=4; j<8; j++)
    {
        printf("%c",t->dateOfAdmission[j]);
    }
    printf("\nDate of birth : ");
    for( j=0; j<4; j+=2)
    {
        printf("%c%c-",t->dateOfBirth[j],t->dateOfBirth[j+1]);
    }
    for(j=4; j<8; j++)
    {
        printf("%c",t->dateOfBirth[j]);
    }
    printf("\n");
    printf("Address : %s\n",t->address);
    printf("Illness : %s\n",t->illness);
    printf("Blood type : %s\n",t->blood);
    printf("____________________________________________\n");

}
////////////////////////////////////////////////
/*
To print the tree in pre order
*/
void printPreOrder(AVL_T t)
{
    if(t!= NULL)
    {
        printInformation(t);
        printPreOrder(t->Left);
        printPreOrder(t->Right);
    }

}
////////////////////////////////////////////////
/*
to split the line we read from the input file
*/
AVL_T split(AVL_T t,char line[])
{
    char nameReading[30];
    char genderReading[2];
    char dateOfAdmissionReading[10];
    char dateOfBirthReading[10];
    char illnessReading[30];
    char addressReading[30];
    char bloodReading[4];

    char *token;
    char tempName[30];
    token=strtok(line,"#");
    strcpy(nameReading,line);
    strcpy(tempName,nameReading);

    token=strtok(NULL,"#");
    strcpy(genderReading,token);

    token=strtok(NULL,"#");
    strcpy(dateOfAdmissionReading,token);

    token=strtok(NULL,"#");
    strcpy(dateOfBirthReading,token);

    token=strtok(NULL,"#");
    strcpy(illnessReading,token);

    token=strtok(NULL,"#");
    strcpy(addressReading,token);

    token=strtok(NULL," \n");
    strcpy(bloodReading,token);
//inserting the information after splitting the line in the tree
    t=insert(t,tempName,genderReading,dateOfAdmissionReading,dateOfBirthReading,illnessReading,addressReading,bloodReading);

    return t;
}

////////////////////////////////////////////////
/*
to read the data from the input file
*/
AVL_T read(AVL_T t)
{

    FILE* input ; //file pointer
    input=fopen("patients.txt","r"); //open the file to write
    if(input==NULL)  //if the file is empty
    {

        printf("Unable to open the file.\n");
        exit(1);
    }
    char line[120] ;
    while(!feof(input))
    {
        if (fgets(line, 120,input)!= NULL)//for reading each line
        {
            if (( line[strlen(line)-1] ) == '\n')//deleting new lines that should not exist (fgets reads until the new line)
                line[strlen(line)-1] = '\0';
            if(line[strlen(line)-1]!='\0')
                t = split(t,line);
        }

    }
    fclose(input);
    return t ;

}
////////////////////////////////////////////////
/*
to insert a node from the user
*/
AVL_T insertFromConsole(AVL_T t)
{
    char nameReading[30];
    char genderReading[2];
    char dateOfAdmissionReading[10];
    char dateOfBirthReading[10];
    char illnessReading[30];
    char addressReading[30];
    char bloodReading[4];

    fflush(stdin);
    printf("Please Enter the patient name : ");
    gets(nameReading);



    printf("\nPlease Enter the patient gender : ");

    gets(genderReading);
    //check for the gender
    while(strcasecmp(genderReading,"f")!=0 && strcasecmp(genderReading,"m")!=0 )
    {
        printf("\nInvalid gender , Please Enter the patient gender again : ");
        gets(genderReading);
    }

    printf("\nPlease Enter the patient date of admissionReading : ");
    gets(dateOfAdmissionReading);

    while(!checkDate(dateOfAdmissionReading))
    {
        printf("\nInvalid date of Admission , Please Enter the date again : ");
        gets(dateOfAdmissionReading);
    }

    printf("\nPlease Enter the patient date of birth: ");
    gets(dateOfBirthReading);

    while(!checkDate(dateOfBirthReading))
    {
        printf("\nInvalid date of Birth , Please Enter the date again : ");
        gets(dateOfBirthReading);
    }

    printf("\nPlease Enter the patient address : ");
    gets(addressReading);


    printf("\nPlease Enter the patient illness : ");
    gets(illnessReading);


    printf("\nPlease Enter the patient blood type : ");
    gets(bloodReading);
    t=insert(t,nameReading,genderReading,dateOfAdmissionReading,dateOfBirthReading,illnessReading,addressReading,bloodReading);//inserting the information of the line in the tree
    printf("\nPatient added successfully!\n");
    //ask the user to if he want to enter a new patient
    printf("\n\nPlease enter 1 if you want to enter another patient ,or any number to go back to the menu\n");
    int x ;
    scanf("%d",&x);
    if(x==1)
    {
        t = insertFromConsole(t);
    }

    return t;


}
//////////////////////////////////////////
/*
find the minimum key (node)
*/
AVL_T findMin( AVL_T t )
{
    if( t == NULL)
        return NULL;
    else if( t -> Left == NULL)
        return t;
    else
        return findMin( t->Left );
}
/////////////////////////////////////////
/*
This function is used to delete a book based on it's name
*/
AVL_T deletePatient( char toDelete[], AVL_T t )
{
    // if the tree has no nodes to delete, return NULL
    if (t == NULL)
    {
        return NULL;
    }

    if (strcmp(toDelete, t -> name) < 0)  // if the value we want to delete is less than the node we are standing at, go left
    {
        t -> Left = deletePatient(toDelete, t -> Left);
    }
    else if(strcmp(toDelete, t -> name) > 0)    // if the value we want to delete is less than the node we are standing at, go right
    {
        t -> Right = deletePatient(toDelete, t -> Right);
    }
    else
    {
        if(t -> Left == NULL || t -> Right == NULL)   // if we reached the node that we want and it has one or zero children
        {
            // if the left is NULL, then the child is the right one (even if it is also NULL)
            // if the right is NULL, then the child is the left one (even if it is also NULL)
            AVL_T temp = t -> Left ? t -> Left : t -> Right;
            if (temp == NULL)
            {
                temp = t;
                t = NULL;
            }
            else
            {
                *t = *temp;
            }
            free(temp);
        }
        else   // if we reached the node that we want and it has two children
        {
            AVL_T temp = findMin(t -> Right);  // find the minimum in the right subtree
            strcpy(t -> name, temp -> name); // move the data in temp  to the node we want to delete
            strcpy(t -> gender, temp -> gender);
            strcpy(t -> illness, temp -> illness);
            strcpy(t ->blood, temp -> blood);
            strcpy(t -> address, temp -> address);
            strcpy(t ->dateOfAdmission, temp -> dateOfAdmission);
            strcpy(t ->dateOfBirth, temp -> dateOfBirth);
            t -> Right = deletePatient(temp -> name, t -> Right);// delete temp from the tree
        }
    }

    if (t == NULL)
    {
        return t;
    }
    // update the height
    t -> Height = MAX(getHeight(t -> Left), getHeight(root -> Right));
    // Checking the balance factor after the deletion.
    int balance = checkBalance(t);

    if (balance > 1 && checkBalance(t -> Left) >= 0)
    {
        return singleRotateRight(t);
    }

    if (balance > 1 && checkBalance(t -> Left) < 0)
    {
        return doubleRotateLeft(t);
    }

    if (balance < -1 && checkBalance(t -> Right) <= 0)
    {
        return singleRotateLeft(t);
    }

    if (balance < -1 && checkBalance(t -> Right) > 0)
    {
        return doubleRotateRight(t);
    }
    return t;


}
///////////////////////////////////////////////
/*
to search for a patient by it's name in the tree
*/
//a pointer to the searched node
AVL_T searchNode = NULL;
void searchInAVL(AVL_T t,char find[])
{

    if(t!= NULL)
    {
        if(strcmp(find,t->name)==0)
        {
            searchNode =t;
            return ;
        }
        searchInAVL(t->Left,find);
        searchInAVL(t->Right,find);
    }
}

////////////////////////////////////////////////
void findAndUpdate (AVL_T t)
{

    printf("Please enter the patient name you want to find\n");
    fflush(stdin);
    char to_find[30];
    gets(to_find);

    searchInAVL(root,to_find);

    if (searchNode==NULL)//if the patient was not found
    {
        printf("\nPatient not ( %s ) found,Please try again\n\n",to_find);
        findAndUpdate(root);

    }
    //if the patient is found
    else
    {
        printf("\n\nThe patient ( %s ) is found\nPlease enter 1 if you want to update the information ,or any number to go back to the menu\n",searchNode->name);
        int  x ;
        scanf("%d",&x);
        if(x==1)
        {
            char updated_name[30];
            char updated_gender[2];
            char updated_dateOfAdmission[10];
            char updated_dateOfBirth[10];
            char updated_illness[30];
            char updated_address[30];
            char updated_blood[4];

            printf("Press 1. to update patient's name\n");
            printf("Press 2. to update patient's gender\n");
            printf("Press 3. to update patient's date of admission\n");
            printf("Press 4. to update patient's date of birth\n");
            printf("Press 5. to update patient's illness\n");
            printf("Press 6. to update patient's address\n");
            printf("Press 7. to update patient's blood type\n");
            int answer ;
            scanf("%d",&answer);

            switch(answer)
            {

            case 1:
                printf("\nPlease enter the updated name : ");
                fflush(stdin);
                gets(updated_name);
                printf("%s\n",updated_name);
                //insert the patient again so it can be in the right place then delete the original node
                t=insert(t,updated_name,searchNode->gender,searchNode->dateOfAdmission,searchNode->dateOfBirth,searchNode->illness,searchNode->illness,searchNode->blood);
                root = deletePatient(searchNode->name,t);
                break;


            case 2 :
                printf("\nPlease enter the updated gender : ");
                fflush(stdin);
                gets(updated_gender);
                while(strcasecmp(updated_gender,"f")!=0 && strcasecmp(updated_gender,"m")!=0 )
                {
                    printf("\nInvalid gender , Please Enter the patient gender again : ");
                    gets(updated_gender);
                }
                strcpy(searchNode->gender,updated_gender);
                break;




            case 3:
                printf("\nPlease enter the updated date of admission : ");
                fflush(stdin);
                gets(updated_dateOfAdmission);
                while(!checkDate(updated_dateOfAdmission))
                {
                    printf("\nInvalid date of Admission , Please Enter the date again : ");
                    gets(updated_dateOfAdmission);
                }
                strcpy(searchNode->dateOfAdmission,updated_dateOfAdmission);
                break;


            case 4 :
                printf("\nPlease enter the updated date of birth : ");
                fflush(stdin);
                gets(updated_dateOfBirth);
                while(!checkDate(updated_dateOfBirth))
                {
                    printf("\nInvalid date of Birth , Please Enter the date again : ");
                    gets(updated_dateOfBirth);
                }
                strcpy(searchNode->dateOfBirth,updated_dateOfBirth);
                break;

            case 5 :
                printf("\nPlease enter the updated illness : ");
                fflush(stdin);
                gets(updated_illness);
                strcpy(searchNode->illness,updated_illness);
                break;

            case 6 :
                printf("\nPlease enter the updated address : ");
                fflush(stdin);
                gets(updated_address);
                strcpy(searchNode->address,updated_address);
                break;

            case 7:
                printf("\nPlease enter the updated blood type : ");
                fflush(stdin);
                gets(updated_blood);
                strcpy(searchNode->blood,updated_blood);
                break;

            default :
                printf("Wrong entry\n");
                return ;
            }


            printf("\nInformation updated successfully\n");
            //set the search node back to null so we can reuse it
            searchNode = NULL;

            printf("\nPlease enter 1 if you want to search for another patient\nor any number to go back to the menu\n");
            int y ;
            scanf("%d",&y);
            if(y==1)
            {
                searchNode = NULL;
                findAndUpdate(t);
            }
            else
            {
                searchNode = NULL;
                return;
            }


        }
        else
        {
            searchNode = NULL;
            return;
        }

    }
    searchNode = NULL;
}
////////////////////////////////////////////////
/*
print the tree nodes in lexicographic order based in the patients' names using in-order print method
*/
void lexicographicOrder(AVL_T t)
{
    if(t!= NULL)
    {

        lexicographicOrder(t->Left);
        printInformation(t);
        lexicographicOrder(t->Right);
    }

}
////////////////////////////////////////////////
/*
search for the patients having the same illness
*/
void sameIllness(AVL_T t, char illnessSearch[])
{
    int cnt = 1 ;
    if(t!= NULL)
    {
        if(strcmp(illnessSearch,t->illness)==0)
        {
            if(cnt==1)
            {
                printf("\nPatients having ( %s ) are : \n",illnessSearch);
                cnt--;
            }
            flg = 1;
            printInformation(t);
        }
        sameIllness(t->Left,illnessSearch);
        sameIllness(t->Right,illnessSearch);
    }

}
////////////////////////////////////////////////
/*
save the tree in a file
*/
void saveInFile(FILE* fptr,AVL_T t)
{
    if(t!= NULL)
    {

        saveInFile(fptr,t->Left);
        fprintf(fptr,"Name : %s \n",t->name);
        fprintf(fptr,"Gender : %s \n",t->gender);
        fprintf(fptr,"Date Of Admission : %s \n",t->dateOfAdmission);
        fprintf(fptr,"Date Of Birth : %s \n",t->dateOfBirth);
        fprintf(fptr,"Address : %s \n",t->address);
        fprintf(fptr,"Illness : %s \n",t->illness);
        fprintf(fptr,"Blood type : %s\n",t->blood);
        fprintf(fptr,"_______________________________________________\n");
        saveInFile(fptr,t->Right);
    }

}
////////////////////////////////////////////////
void menu()
{

    //printing the tree menu to the user
    printf(">>>>>>>>>>>>>>>>>>>>Welcome to the program<<<<<<<<<<<<<<<<<<<<\n");
    printf("______________________________________________________________\n");
    printf("                            MENU                   \n");
    printf("______________________________________________________________\n");
    printf("Press 1. To read the input file and store the data in an AVL Tree\n");
    printf("______________________________________________________________\n");
    printf("Press 2. To insert a new patient from the console\n");
    printf("______________________________________________________________\n");
    printf("Press 3. To Find a patient and update it's information if needed\n");
    printf("______________________________________________________________\n");
    printf("Press 4. To List all patients in lexicographic order\n");
    printf("______________________________________________________________\n");
    printf("Press 5. To List all patients that have the same illness.\n");
    printf("______________________________________________________________\n");
    printf("Press 6. To Delete a patient from the system.\n");
    printf("______________________________________________________________\n");
    printf("Press 7. To Save all patients in file patients_hash.data and exit\n");
    printf("______________________________________________________________\n");
    printf("Press 8. To exit\n");
    printf("______________________________________________________________\n");
    int x;
    scanf("%d",&x);
    switch(x)
    {
    case 1 :
        root = read(root);
        printf("Read has done successfully ! \n");
        isReadTree = 1;
        printf("\nPatients in Pre Order : \n");
        printf("____________________________________________\n");
        printPreOrder(root);
        menu();
        break;
    case 2 :
        isReadTree = 1;
        root = insertFromConsole(root);
        menu();
        break;
    case 3 :
        if(isReadTree==0)
        {
            printf("Please read the patients information before performing any operation\n");
        }
        else
        {
            findAndUpdate(root);
            printf("\nPatients after the update : \n");
            printf("____________________________________________\n");
            printPreOrder(root);
        }
        menu();
        break;
    case 4 :
        if(isReadTree==0)
        {
            printf("Please read the patients information before performing any operation\n");
        }
        else
        {
            lexicographicOrder(root);
        }
        menu();
        break;
    case 5 :
        if(isReadTree==0)
        {
            printf("Please read the patients information before performing any operation\n");
        }
        else
        {
            printf("Please enter the illness you want to search for : ");
            fflush(stdin);
            char illnessToSearch[30];
            gets(illnessToSearch);
            sameIllness(root,illnessToSearch);
            if(flg==0)
            {
                printf("\nThere is no patient having the illness %s \n",illnessToSearch);
            }
            flg=0;
        }
        menu();
        break;
    case 6 :
        if(isReadTree==0)
        {
            printf("Please read the patients information before performing any operation\n");
        }
        else
        {
            printf("Please enter the patient name you want to delete from the system : \n");
            fflush(stdin);
            char patientToDelete[30];
            gets(patientToDelete);
            searchInAVL(root,patientToDelete);

            if (searchNode==NULL)//if the patient was not found
            {
                printf("\nPatient not found,Please try again\n");

            }
            else
            {
                root =deletePatient(patientToDelete,root);
                printf("\nDeletion has done successfully\n");
                printf("\nPatients after the deletion : \n");
                printf("____________________________________________\n");
                printPreOrder(root);
            }
            searchNode =NULL;
        }
        menu();
        break;
    case 7:
        ;
        FILE *fptr  = fopen("patients_hash.data.txt", "w");
        fprintf(fptr,"Patients  informations : \n ");
        saveInFile(fptr,root);
        fclose(fptr);
        //create the hash table
        numberOfPatients(root);
        int primeNum= pateintsNumber/0.6;
        int tableSize = getPrime(primeNum+1);
        hashTable=createTable(tableSize);
        insertTable(root);
        printf("Data has been successfully moved to the hash table \n\nHash part : \n\n");
        printHash(hashTable);
        printf("\n");
        //go to the hash menu
        hashMenu();
        break;
    case 8:
        exit(1);
    default :
        printf("\nThe number you enter is not valid , Please try again\n");
        menu();
        break;
    }
}
int main()
{
    menu();
    return 0;
}
