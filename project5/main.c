#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>
#include <ctype.h>
#include <limits.h>
/**
DATA STRUCTURES PROJECT NUMBER 5
Tala ALsweiti
1191068
sec 2
**/
//struct for all data we need to calculate the shortest path
typedef struct
{
    int distance;
    char city[50];
    int shortestPath;
    int boolSpSets;
    int parent;
} cityData;


cityData **graph;
int isRead = 0;
FILE *fptr;

//print the path
void printPath(int src,int current,int dist)
{
    if(src==dist)
    {
        printf("%d -> %d \n", src+1,src+1);
        return;
    }
    if(src==current)
    {
        printf("%d -> ", src+1);
        return;
    }
    printPath(src,graph[src][current].parent,dist);
    printf("%d %s \n",current+1,graph[current][graph[src][current].parent].city);
    if(current!=dist)
    {
        printf("%d -> ",current+1);
    }
}

//to the maximum number for a vertix
int maxNumberOfVertices()
{
    FILE* input ; //file pointer
    input=fopen("segments.txt","r"); //open the file to write
    if(input==NULL)  //if the file is empty
    {

        printf("Unable to open the file.\n");
        exit(1);
    }
    int max = 0;
    char line[300];
    while(!feof(input))
    {
        fgets(line, 300,input);
        char *token = strtok(line," ");
        if(atoi(token)>max) max = atoi(token);
        token = strtok(NULL," ");
        if(atoi(token)>max) max = atoi(token);
    }
    fclose(input);
    return max ;
}
//print the results to the file
void printTofile(int src,int current,int dist)
{
    fptr = fopen("route.txt", "a");
    if(src==dist)
    {
        fprintf(fptr,"%d -> %d \n", src+1,src+1);
        return;
    }
    if(src==current)
    {
        fprintf(fptr,"%d -> ", src+1);
        return;
    }
    printTofile(src,graph[src][current].parent,dist);
    fprintf(fptr,"%d %s \n",current+1,graph[current][graph[src][current].parent].city);
    if(current!=dist)
    {
        fprintf(fptr,"%d -> ",current+1);
    }
}
//to read the data from the file and store in the graph
void  read()
{
    int size=maxNumberOfVertices();
    char line[300];
    FILE* input ; //file pointer
    input=fopen("segments.txt","r"); //open the file to write
    if(input==NULL)  //if the file is empty
    {
        printf("Unable to open the file.\n");
        exit(1);
    }
    //creat a 2d array with size*size
    graph = (cityData**)malloc(size *sizeof(cityData*));

    for(int i=0; i<size; i++)
    {
        graph[i] = (cityData *)malloc(size * sizeof(cityData));
    }
    //0 indexing , initialize the values
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            graph[i][j].distance = 0;
            graph[i][j].shortestPath = INT_MAX;
            graph[i][j].boolSpSets=0;
        }

    }
    while(!feof(input))
    {
        fgets(line, 300,input);
        char temp[50] = "";
        char tempNUM[20] = "";
        int flg = 0;
        int id1, id2, currentDistance;
        for(int i=0; i<strlen(line)&&line[i]!='\0'; i++)
        {
            if(isdigit(line[i]))
            {
                char str1[2] = {line[i], '\0'};
                strcat(tempNUM,str1);
            }
            else
            {
                char str2[2] = {line[i], '\0'};
                strcat(temp,str2);
                if(flg==0)
                {
                    id1= atoi(tempNUM);
                    flg++;
                }
                else if(flg==1)
                {
                    id2 = atoi(tempNUM);

                    flg++;
                }
                else if(flg==2&&atoi(tempNUM)>0)
                {
                    currentDistance= atoi(tempNUM);

                }
                strcpy(tempNUM,"");
            }
        }
        id1--;
        id2--;
        //store the data in the graph
        strcpy(graph[id1][id2].city,temp);
        strcpy(graph[id2][id1].city,temp);
        graph[id1][id2].distance = currentDistance;
        graph[id2][id1].distance = currentDistance;

    }
    fclose(input);
}
void  dijkstra()
{
    int size = maxNumberOfVertices();

    //find the shortest path from i to all j
    //i is the source j is the destination
    for(int i=0; i<size; i++)
    {
        //distance of source to itself is 0
        graph[i][i].shortestPath = 0;

        for(int j=0; j<size-1; j++)
        {
            //find the vertex with minimum distance value, from
            //the set of vertices not yet included in shortest path grpah
            int min = INT_MAX, min_index;
            for(int v=0; v<size; v++)
            {
                if(graph[i][v].boolSpSets==0 && graph[i][v].shortestPath<=min)
                {
                    min = graph[i][v].shortestPath;
                    min_index = v;

                }
            }
            // Mark the picked vertex as processed
            graph[i][min_index].boolSpSets=1;
            // Update dist value of the adjacent vertices of the picked vertex.
            for(int v=0; v<size; v++)
            {
                //update graph[i][v].shortestPath if and only if it's not processed
                //and there's edge from it to the minimum index and the
                // total number from source to dist is smaller than the current value of the shortest path
                //and store the parent for each node
                if (!graph[i][v].boolSpSets && graph[min_index][v].distance && graph[i][min_index].shortestPath != INT_MAX
                        && graph[i][min_index].shortestPath+  graph[min_index][v].distance < graph[i][v].shortestPath)
                {
                    graph[i][v].shortestPath = graph[i][min_index].shortestPath + graph[min_index][v].distance;
                    graph[i][v].parent = min_index;
                }
            }
        }
    }

}

//to calculate the path for a given source and destination
void calculate()
{
    int src,dist ;
    printf("Please enter the source : \n");
    scanf("%d",&src);
    printf("Please enter the destination  : \n");
    scanf("%d",&dist);
    //because the zero indexing
    src--;
    dist--;
    if(graph[src][dist].shortestPath!=INT_MAX)
    {
        printf("\nThe cost from %d to %d = %d\n",src+1,dist+1,graph[src][dist].shortestPath);
        printf("\nThe path is : \n\n");
        printPath(src,dist,dist);
        printf("\n\n");

        fptr = fopen("route.txt", "a");
        fprintf(fptr,"\nThe cost from %d to %d = %d\n\n",src+1,dist+1,graph[src][dist].shortestPath);
        fclose(fptr);
        printTofile(src,dist,dist);
        fprintf(fptr,"\n_____________________\n\n");
        fclose(fptr);
    }
    else
    {
        printf("\nThere is no path from %d to %d \n\n",src+1, dist+1);

    }
    printf("Please enter 1 to enter other destinations \n");
    int x ;
    scanf("%d",&x);
    if(x==1) calculate();
    else return;

}


void menu()
{

    printf("1. Read the file and load the data\n");
    printf("2. Enter 2 points to compute the shortest path between them\n");
    printf("3. Print the routes of the shortest distance to a file called route.txt\n");
    printf("4. Exit\n");
    int choice ;
    scanf("%d",&choice);
    switch(choice)
    {

    case 1:
        read();
        dijkstra();
        isRead =1;
        printf("Read done successfully\n\n");
        menu();
        break;
    case 2:
        if(isRead)
            calculate();
        else
            printf("Please read the data first\n\n");
        menu();
        break;
    case 3:
        printf("Print done successfully\n\n");
        menu();
        break;
    case 4:
        printf("Thank you for using our program!\n");
        exit(1);
        break;
    default:
        printf("Wrong input , Please try again \n");
        menu();
        break;
    }
}
int main()
{
    //to clear the file
    fclose(fopen("route.txt", "w"));
    printf("------------------Welcome to the program----------------------\n\n");
    menu();

    return 0;
}
