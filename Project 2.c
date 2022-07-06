#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Name:    Ahmaide Al-Awawdah
 ID:      1190823
 Section: 4
 */

typedef struct node* PtrToNode;
typedef PtrToNode Stack;

/// Element is the string (Array of char)
typedef struct node{
char Element [100];
PtrToNode Next;
}node;


void pop( Stack S );
int isEmpty(Stack s);
void MakeEmpty( Stack S );
Stack CreateStack();
void push( char x[], Stack S );
void printAll(PtrToNode s);
void printToFile(PtrToNode s, FILE *out);
Stack s;

int main()
{
    s = CreateStack();
    char cur [100];
    /// This variable will point out when to exit the loop
    int pointer = 0;
    printf("My Command > ");
    gets(cur);
    /// This array of char is to store the undo value
    char removedText [100];
    /// flag will point out if the stack was empty
    int flag =0;
    FILE *out ;
    out = fopen("output.txt","w");

    while(pointer!=1){
            if(cur[0]=='u' && cur[1]=='n' && cur[2]=='d' && cur[3]=='o' && strlen(cur)==4){
                if (isEmpty(s)==1)
                    printf("\nthere is no data to delete");
                else{
                    strcpy(removedText, ((s->Next)->Element));
                    pop(s);
                    flag =1;
                }
            }
            else if(cur[0]=='r' && cur[1]=='e' && cur[2]=='d' && cur[3]=='o' && strlen(cur)==4){
                if(flag != 1){
                    printf("\nthere is no data to restore");
                }
                else{
                    push(removedText,s);
                }
            }
            else if (cur[0]=='p' && cur[1]=='r' && cur[2]=='i' && cur[3]=='n' && cur[4]=='t' && strlen(cur)==5){
                if (isEmpty(s)==1)
                    printf("\n there is no data to print");
                else{
                    printf("\nresult >\n******\n");
                    printAll(s->Next);
                }
            }
            else if (cur[0]=='q' && cur[1]=='u' && cur[2]=='i' && cur[3]=='t' && strlen(cur)==4){
                pointer = 1;
                printf("good bye!\n************************************************************");
                break;
            }
            else if (cur[0]=='s' && cur[1]=='a' && cur[2]=='v' && cur[3]=='e' && strlen(cur)==4){
                fprintf(out,"result >\n******\n");
                printToFile(s->Next,out);
            }
            else {
                push(cur, s);
            }

            printf("\nMy Command > ");
            gets(cur);
    }

    return 0;
}

/// This function returns 1 if the list was empty, and 0 if it was not
int isEmpty(Stack s){
    if(s->Next==NULL)
        return 1;
    else
        return 0;
}

/// This function clears the stack
void MakeEmpty( Stack S ){
    if( S == NULL )
        printf( "Out of space!" );
        else
           while( isEmpty( S ) != 1){
               pop( S );
           }
}

/// This function is to create a new stack
Stack CreateStack(){
    Stack S;
    S =  (node*)malloc(sizeof(node));
    if( S == NULL )
        printf( "Out of space!" );
    S->Next = NULL;
    MakeEmpty( S );
    return S;
}

/// This function deletes the last inserted data to the stack
void pop( Stack S ){
    PtrToNode firstCell;
    if( isEmpty(S)==1)
        printf( "Empty stack" );
    else{
        firstCell = S->Next;
        S->Next = (S->Next)->Next;
        free( firstCell );
    }
}

/// This function adds new data to the stack
void push( char x[], Stack S ){
    PtrToNode temp;
    temp = ( Stack )malloc( sizeof( struct node ) );
    if( temp == NULL)
        printf( "Out of space!" );
    else{
        strcpy(temp->Element, x);
        temp->Next = S->Next;
        S->Next = temp;
    }
}

/// This Function displays all the data stored in the stack using recursion
void printAll(PtrToNode s){
    if(s->Next!=NULL){
        printAll(s->Next);
    }
    for(int i=0 ; i<strlen(s->Element) ; i++){
        printf("%c", s->Element[i]);
    }
    printf("\n");
}

/// This Function prints out all the data in the stack to a file using recursion
void printToFile(PtrToNode s, FILE *out){
    if((s->Next)!=NULL){
        printToFile(s->Next, out);
    }
    for(int i=0 ; i<strlen(s->Element) ; i++){
        fprintf(out,"%c", s->Element[i]);
    }
    fprintf(out,"\n");
}




