#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Name :   Ahmaide Al-Awawdah
ID:      1190823
Section: 4
**/


/// A node contains the edge's info
typedef struct edge{
    struct vertex *connectedWith; /// Where the edge will lead to
    int length; /// Edge distance in KM
    struct edge *next; /// Other Edge's in the same vertex
    char name [30]; /// Edge name
}edge;

/// A node contains the vertex's info
typedef struct vertex{
    int id; /// The ID of the vertex
    int nOfConnections; /// The number of the edges this vertex has
    edge *allEdges; /// A linked list contains all the edges of the vertex
}vertex;

/// All functions
int getLastNum(char a[]);
void construct(char info[], vertex *v[]);
void addNewEdge(vertex *v, edge *e);
void newShortPaths(int arrayOfPaths[],edge *arrayOfLastEdge[], int size, int value);
void getTheShortestWay(int dis[],edge *LE[], vertex *ver[], int first, int last, int pastDis);
void printWay(edge *LE[], int last);
void printToFile(edge *LE[], int last, FILE *out);
edge *FindOppesite(edge* e, vertex* v1);

/// This local variable in order to make a new line after printing 5 vertexes of a path
int counter;

int main()
{
    int nOfVertices =0; /// Variable to calculate the number of vertexes for the array size
    int nOfEdges =0; /// Variable for the number of all edges in order to be used in a loop to add the edges to the vertexes
    char info [1000][50]; /// Array of strings to save the data that will be read from the file
    FILE *in;
    in = fopen("segments.txt","r"); /// The file that contains the data
    FILE *out;
    out = fopen("route.txt","w"); /// The file that will have the printed data

    /// This loop is to find the number of vertexes by getting the highest vertex number
    for(int i=0 ; !feof(in) ; i++){
        fgets(info[i], 1000, in);
        if(info[i][0]>=48 && info[i][0]<=57){
            if(getLastNum(info[i]) > nOfVertices)
                nOfVertices = getLastNum(info[i]);
            nOfEdges++;
        }
    }
    vertex *ver [nOfVertices + 1]; /// Array of vertexes
    ver[0] = NULL; /// Since there is no vertex with the id (0)

    /// A loop to make all the vertexes where ver[i] = vertex with the id i
    for(int i=1 ; i < nOfVertices+1 ; i++){
        ver[i] = (vertex*)malloc(sizeof(vertex));
        ver[i]->id = i;
        ver[i]->nOfConnections = 0;
        ver[i]->allEdges = NULL;
    }
    /// This loop is to read the file and store the data in the info array
    for(int i=0 ; i<nOfEdges ; i++){
        construct(info[i], ver);
    }
    int option; /// A variable to store the users option
    printf("If you want to view all the Vertexes enter 1\nEnter any other number if no (optional): ");
    scanf("%d",&option);

    /// This option prints all the vertexes informations and its optional
    if(option==1){
        for(int i=1 ; i<nOfVertices+1 ; i++){
            printf("\nvertex %d\twith %d edges\t and the edges are     ",ver[i]->id, ver[i]->nOfConnections);
            if(ver[i]->allEdges==NULL)
                printf("0");
            else{
                for(edge *j=ver[i]->allEdges ; j!=NULL ; j=j->next){
                    printf("%s (%d) for %d,    ", j->name, j->length, j->connectedWith->id);
                }
            }
        }
    }
    int arrayOfPaths [nOfVertices+1]; /// This array is to store the needed shortest way with the source vertex in order to calculate the shortest path
    edge *arrayOfLastEdge [nOfVertices+1]; /// This array is to store the last leading way to each vertex in order to find the shortest path
    int firstPoint; /// This will be the id of the source vertex
    int lastPoint;/// This will be the id of the destination
    option =1;
    /// This is a loop that the user decided weather to stop it or not
    while(option==1){
        printf("\n\nEnter the first point: ");
        scanf("%d", &firstPoint);
        printf("Enter the last point: ");
        scanf("%d", &lastPoint);

        /// clear both arrays for the new source and the new destination
        newShortPaths(arrayOfPaths, arrayOfLastEdge, nOfVertices, firstPoint);

        /// Give the source a destination of zero
        arrayOfPaths[firstPoint] = 0;

        /// Fill the 2 arrays until they make sure that the shortest path is found
        getTheShortestWay(arrayOfPaths, arrayOfLastEdge, ver, firstPoint, lastPoint, 0);

        /// The case of there is a way
        if(arrayOfPaths[lastPoint] != -1 && ((ver[firstPoint]->nOfConnections!=0  && ver[lastPoint]->nOfConnections!=0) || firstPoint==lastPoint)){
            printf("\n\n         THE DISTANCE BETWEEN %d and %d \n******************************************************\n",firstPoint,lastPoint);
            fprintf(out, "\n\n         THE DISTANCE BETWEEN %d and %d \n******************************************************\n",firstPoint,lastPoint);
            counter =0;
            printWay(arrayOfLastEdge, lastPoint);
            counter =0;
            printToFile(arrayOfLastEdge, lastPoint, out);
            printf("\n.\n.\nDistance is: %d kilometers", arrayOfPaths[lastPoint]);
            fprintf(out, "\n.\nDistance is: %d kilometers", arrayOfPaths[lastPoint]);
        }
        else{
            printf("\n\nThe point %d, can't be reached from %d :(\n", lastPoint, firstPoint);
            fprintf(out, "\n\nThe point %d, can't be reached from %d :(\n", lastPoint, firstPoint);
        }
        fprintf(out, "\n\n");
        printf("\n\n\nEnter 1 if you want to continue, other to leave: ");
        scanf("%d", &option);
    }
    printf("\n\n             ~thank you~\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    return 0;
}

/// This function compares between 2 vertexes in a string in order to find the maximum vertex
int getLastNum(char a[]){
    int first =0;
    int last =0;
    int i=0;
    for(i=0 ; a[i]>=48 && a[i]<=57 ; i++){
        first = first*10 +(a[i] - 48);
    }
    if(a[i] == ' ')
        i++;
    for(i=i ; a[i]>=48 && a[i]<=57 ; i++){
        last = last*10 +(a[i] - 48);
    }
    if(first>last)
        return first;
    else
        return last;
}

/// This function gets the edge from a string and connects it with both of its end
void construct(char info[], vertex* v[]){
    int ver1 =0; /// One end
    int ver2 =0; /// Another end
    int distance =0;
    char edgeName [30];
    int i=0;
    /// This loop to get the id of the first vertex
    for(i=0 ; info[i]>=48 && info[i]<=57 ; i++){
        ver1 = ver1*10 +(info[i] - 48);
    }
    if(info[i] == ' ')
        i++;
    /// This loop to get the id of the second vertex
    for(i=i ; info[i]>=48 && info[i]<=57 ; i++){
        ver2 = ver2*10 +(info[i] - 48);
    }
    if(info[i] == ' ')
        i++;
    int j=0;
    /// This loop is to store the name of the edge
    for(i=i ; info[i]!=' ' || info[i+1] > 57 || info[i+1] < 48 ; i++ , j++){
        edgeName[j] = info[i];
    }
    for(j=j ; j<30 ; j++){
        edgeName[j]=NULL;
    }
    if(info[i] == ' ')
        i++;
    /// This loop is to get the distance
    for(i=i ; i<strlen(info)-1 ; i++){
        distance = distance*10 +(info[i] - 48);
    }
    /// All the rest is to give the edges info to both of its vertexes
    v[ver1]->nOfConnections++;
    v[ver2]->nOfConnections++;
    edge *e1 = (edge*)malloc(sizeof(edge));
    edge *e2 = (edge*)malloc(sizeof(edge));
    e1->connectedWith = v[ver2];
    e2->connectedWith = v[ver1];
    e1->length = distance;
    e2->length = distance;
    strcpy(e1->name, edgeName);
    strcpy(e2->name, edgeName);
    e1->next = NULL;
    e2->next = NULL;
    addNewEdge(v[ver1], e1);
    addNewEdge(v[ver2], e2);
}

/// This function is to add a new edge to a vertex
void addNewEdge(vertex *v, edge *e){
    if(v->allEdges==NULL)
        v->allEdges= e;
    else {
        edge *i = v->allEdges;
        for(i=i ; i->next!=NULL ; i=i->next);
        i->next = e;
    }
}

/// This function clears both arrays (-1 means that there is no path)
void newShortPaths(int arrayOfPaths[],edge *arrayOfLastEdge[], int size, int value){
    int Size = size +1;
    for(int i=0; i<Size ; i++){
        arrayOfPaths[i] = -1;
        arrayOfLastEdge[i]=NULL;
    }
    arrayOfPaths[value] = 0;
    arrayOfLastEdge[value] = (edge*)malloc(sizeof(edge));
    edge *p = arrayOfLastEdge[value];
    p->connectedWith = NULL;
    p->length = 0;
    strcpy(p->name,"");
    p->next = NULL;
}

/// This function takes an edge and it returns the opposite of an edge (the same edge from the other side)
edge *FindOppesite(edge* e, vertex* v1){
    vertex *v2 = e->connectedWith;
    edge *f;
    for(f= v2->allEdges ; f->connectedWith->id != v1->id ; f=f->next);
    return f;
}


/** This function is to Fill both arrays (one for the distance of the shortest path, and the one wit the leading edge to the destination edge)
until it makes sure that the shortest path is found (dis is for the shortest distance found yet array, LE is for the edge leading to the shortest path array,
ver is the array of vertexes, first is the source vertex id, last is the destination vertex id, pastDis is for the distance already passed for the current source)**/

void getTheShortestWay(int dis[],edge *LE[], vertex *ver[], int first, int last, int pastDis){

    vertex *here = ver[first]; /// the current source vertex
    int checker = 0; /// To check weather if it is worth it to go to the connected vertexes

    /// This loop goes on all the vertexes that are connected to the current source
    for(edge *e = here->allEdges ; e!=NULL ; e=e->next){
        vertex *other = e->connectedWith;
        /// This if statement sees if the vertex can find a short path or not
        if( ( (pastDis+e->length < dis[other->id]) || (dis[other->id]==-1) ) && ( (pastDis+e->length < dis[last]) || (dis[last]==-1) ) ){
            dis[other->id] = pastDis + e->length; /// Gives a shorter distance from the main source
            LE[other->id] = FindOppesite(e,ver[first]); /// Gives a shorter path from the main source
            checker++;
        }
    }
    /// If statement to see if it is worthy to get inside the inner vertexes
    if(checker!=0){
        for(edge *e = here->allEdges ; e!=NULL ; e=e->next){
            vertex *other = e->connectedWith;
            /// Only do the following if this vertex shortest path right from the current source path and it should be useful in finding a shorter path to destination
            if(LE[other->id]!=NULL && LE[other->id]->length!=0){
                if(LE[other->id]->connectedWith->id == here->id  &&  other->id != last){
                    getTheShortestWay(dis,LE, ver, other->id, last, pastDis + e->length); /// Recursion with the current vertex as the source
                }
            }
        }
    }
}

/// This function is to print the path from the source to the destination
void printWay(edge *LE[], int last){
    if(LE[last]->length!=0){ /// This if statement means that the function reached the source
        printWay(LE,LE[last]->connectedWith->id);
        printf("   ---(%s,%d)--->   ", LE[last]->name, LE[last]->length);
    }
    if(counter%5==4) /// This if statement to make sure that each line has at most 5 vertexes
        printf("%d\n",last);
    counter++;
    printf("%d",last);
}

/// This function is to print the path from the source to the destination in the output file
void printToFile(edge *LE[], int last, FILE *out){
    if(LE[last]->length!=0){
        printToFile(LE, LE[last]->connectedWith->id, out);
        fprintf(out, "   ---(%s,%d)--->   ", LE[last]->name, LE[last]->length);
    }
    if(counter%4==3)
        fprintf(out, "%d\n",last);
    counter++;
    fprintf(out, "%d", last);
}
