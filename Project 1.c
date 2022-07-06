#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 Name:    Ahmaide Al-Awawdah
 ID:      1190823
 Section: 4
 Please make sure that there is no extra lines in the inputs file and that the mouse is at the end of the last polynomial
 */

typedef struct node
{
    int coff;
    int pow;
    struct node * next;
    struct node * previous;
}node;

void clearSpace(char a[]);
node * createList(int a, char b[]);
int iFromc(char a);
node * getSum(node * equ[], int nofPoly);
void printPoly(node * head);
void prepareForSubt(node * equ[], int nofPoly);
node * multiplayList(node * equ[], int nofPoly);
node * copyPoly(node * a);
node * mult(node * eq1, node * eq2);
node * shortcut(node * a);
double substitue(double a, node * list);
void printToFile(node * head, FILE *out);

int main(){
    int menu;
    printf("Please enter the following number for the following operation:\n**************************************************************\n1- To read the file\t2- To print all equations\n3- For adding\t4- For subtracting\n5- for multiplying\t6- To give x a value\n7- to store date to output file    other- to Exit\t:::");
    scanf("%d",&menu);
    /// to save the polynomials as a string
    char arrayOfChar [5000][100];
    int i=0;
    /// nofPoly is the number of polynomials
    int nofPoly=0;
    /// the array that will have all the linked lists for the polynomials
    node * equations [5000];
    /// the linked list that will store the addition equation
    node * summation;
    /// the linked list that will store the subtraction equation
    node * subtraction;
    /// the linked list that will store the multiplication equation
    node * multiplication;
    /// can is to determine which polynomial can be used
    int can =1;
    FILE *out;
    out=fopen("results.txt","w");
    while (1<=menu && menu<=7){
        switch (menu){
            /// In this case the file is read and polynomials are saved in a linked list
            case 1: {
                FILE *in;
                in = fopen("equations.txt","r");
                /// to clear the spaces between the polynomials
                while(!feof(in) && arrayOfChar[i][0]!=10){
                    fgets(arrayOfChar[i] , 100 , in);
                    clearSpace(arrayOfChar[i]);
                    i++;
                }
                nofPoly = i-1;
                /// Just in case there were extra lines
                if(arrayOfChar[nofPoly][0]==10)
                    nofPoly--;
                for (int j=0 ; j<nofPoly ; j++){
                    int counter =1;
                    /// This loop is to count how many variables are in each polynomial
                    for (int k=1;k<(strlen(arrayOfChar[j])-1);k++){
                        if(arrayOfChar[j][k]=='+' || arrayOfChar[j][k]=='-'){
                            counter++;
                            }
                    }
                    /// creating the main array of linkedlists
                    equations[j] = createList(counter, arrayOfChar[j]);
                    }
            printf("\nFile has been read successfully :) \n");
            can*=2;
            }
            break;
            /// In this case all polynomials are displayed with each equations exact variables
            case 2: {
                if(nofPoly==0){
                    printf("you have no data stored yet, please read the data from the file or add data to the file :(");
                }
                else {
                printf("\nAll equations:\n***************\n");
                for(int j=0 ; j<nofPoly ; j++){
                    printf("%d- ",j+1);
                    for(int k=0 ; k<(strlen(arrayOfChar[j])-1) ; k++){
                        printf("%c",arrayOfChar[j][k]);
                    }
                    printf("\n");
                }
                for (int j=0 ; j<nofPoly ; j++){
                        int counter =1;
                        printf("\n\n %d)...\n",j+1);
                        for(node * k = equations[j] ; k!=NULL ; k=k->next){
                            printf("%d- coefficient  is %d\tand power is %d\n",counter,k->coff,k->pow);
                            counter++;
                        }
                }
                }
            }
            break;
            /// This case is to do the addition and display the result of it
            case 3: {
                if(nofPoly==0){
                    printf("you have no data stored yet, please read the data from the file or add data to the file :(");
                }
                else{
                    summation = getSum(equations, nofPoly);
                    printf("\n The addition polynomial is:\n ***************************\n");
                    printPoly(summation);
                }
                printf("\n");
            }
            can*=3;
            break;
            /// This case is to do the subtraction and display the result of it
            case 4: {
                 if(nofPoly==0){
                    printf("you have no data stored yet, please read the data from the file or add data to the file :(");
                }
                else{
                    /// To Multiply all polynomials from second to last with -1, preparing it for subtraction
                    prepareForSubt(equations, nofPoly);
                    /// getting the new list, is the same as subtracting the old one
                    subtraction = getSum(equations, nofPoly);
                    printf("\n The subtraction  polynomial is:\n *******************************\n");
                    printPoly(subtraction);
                    /// To Multiply all polynomials from second to last with -1, to get the old list back
                    prepareForSubt(equations, nofPoly);
                }
                printf("\n");
                can*=5;
            }
            break;
            /// This case is to do the multiplication and display the result of it
            case 5: {
                if(nofPoly==0){
                    printf("you have no data stored yet, please read the data from the file or add data to the file :(");
                }
                else {
                    if(nofPoly==1)
                    multiplication = equations[0];
                    else
                    multiplication = multiplayList(equations, nofPoly);

                    printf("\n The multiplication polynomial is:\n *********************************\n");
                    printPoly(shortcut(multiplication));
               }
                printf("\n");
                can*=7;
            }
            break;
            /// This case is to substitute a doubled value
            case 6: {
                printf("Enter 1 for addition, 2 for subtraction, 3 for multiplication : ");
                int d;
                scanf("%d", &d);
                /// d is to choose which operation will be used
                switch (d) {
                    case 1: {
                        if(can%6==0){
                        printf("enter a value to substitute : ");
                        double s;
                        scanf("%lf", &s);
                        s = substitue(s, summation);
                        printf("the value of the summation = %f", s);
                        }
                        else {
                            printf("Your summation polynomial is empty\n");
                        }
                    }
                    break;
                    case 2: {
                        if(can%10==0){
                        printf("enter a value to substitute : ");
                        double s;
                        scanf("%lf", &s);
                        s = substitue(s, subtraction);
                        printf("the value of the subtraction = %f", s);
                        }
                        else{
                            printf("the polynomial of the subtraction is empty\n");
                        }
                    }
                    break;
                    case 3: {
                        if(can%14==0){
                        printf("enter a value to substitute : ");
                        double s;
                        scanf("%lf", &s);
                        s = substitue(s, multiplication);
                        printf("the value of the multiplication = %f", s);
                        }
                        else {
                            printf("the polynomial of the multiplication is empty\n");
                        }
                    }
                    break;
                    default : {
                        printf("You entered an invalid equation\n");
                    }
                }
            }
            break;
            /// This case will print all the use all the outputs of the previous used equations to the equations.txt file
            case 7 : {
                fprintf(out, "All equations will be stored here\n**************************************************************\n");
                if(can%6==0){
                        fprintf(out,"\nthe summation equation\n**********************\n");
                        printToFile(summation, out);
                }
                        if(can%10==0){
                            fprintf(out,"\n\nthe subtraction equation\n*************************\n");
                            printToFile(subtraction, out);
                        }
                        if(can%14==0){
                            fprintf(out,"\n\nthe multiplication equation\n***************************\n");
                            printToFile(shortcut(multiplication), out);
                        }
                        printf("data have been added to file :)\n");
                    }
                    break;
        }
        printf("\nPlease enter the following number for the following operation:\n**************************************************************\n1- To read the file\t2- To print all equations\n3- For adding\t4- For subtracting\n5- for multiplying\t6- To give x a value\n7- to store date to output file    other- to Exit\t::: ");
        scanf("%d",&menu);
    }
    printf("Thank you for stopping by ********************** :)\n.\n.\n.\n.");
   return 0;
}

/// This functions created a new list with the wanted size and then fills it in with the needed data
node * createList(int a, char arr[]){
    /// Part A) create an empty list with the wanted size
    node * head = NULL;
    node * p = NULL;
    node * o = NULL;
    for(int i=0; i<a;i++){
        p = (node*)malloc(sizeof(node));
        p->next = NULL;
        if(head == NULL){
        head = p;
        head->previous=NULL;}
        else{
            o->next = p;
            p->previous=o;
        }
        o=p;
    }
    /// part B) Fill the list with the data from the string
    o=head;
    int start=0;
    int sign=1;
    int mult =0;
    int pow = 0;
    int switch0 =0;
    if (arr[0]=='-'){
        sign =-1;
        start++;
    }
    for(int i=start; i<(strlen(arr)-1) ; i++){
        int iN= i+1;
        int iP= i-1;
        if(48<=arr[i] && arr[i]<=57 && switch0==0){
            mult = mult*10 + (sign*iFromc(arr[i]));
            if(arr[iN]=='+' || arr[iN]=='-' || iN==(strlen(arr)-1)){
                o->pow=0;
                o->coff=mult;
                }
        }
        else if(arr[i]=='x' || arr[i]=='X'){
            if (i==0 || arr[iP]=='-' || arr[iP]=='+'){
                o->coff=sign;}
            else{
                o->coff=mult;}
            if(arr[iN]!='^'){
                o->pow=1;}
        }
        else if(arr[i]=='^'){
            switch0 = 1;
        }
        else if(48<=arr[i] && arr[i]<=57 && switch0==1){
             pow = pow*10 + (iFromc(arr[i]));
             if ((iN)==(strlen(arr)-1) || arr[iN]=='-' || arr[iN]=='+'){
                o->pow=pow;}
        }
        else if(arr[i]=='+' || arr[i]=='-'){
            pow=0;
            switch0=0;
            mult=0;
            o=o->next;
            if(arr[i]=='+'){
                sign = 1;}
                else{
                    sign = -1;}
        }
        else {
            printf("Error you have an error please check your equations again");
            head = NULL;
            continue;
        }
    }
    /// Part C) Order the list from the largest to the smallest power
    int coff;
    for(o=head ; o!=NULL ; o=o->next){
        for(p=o->next ; p!=NULL ; p=p->next){
            if((o->pow) < (p->pow)){
                coff = o->coff;
                pow = o->pow;
                o->pow = p->pow;
                o->coff = p ->coff;
                p->coff = coff;
                p->pow = pow;
            }
        }
    }
    /// To add the coefficients with the same power to each other
    for(o=head ; o!=NULL ; o=o->next){
        for(p= o->next ; p!=NULL ; p=p->next){
            if(p->pow==o->pow){
                o->coff+=p->coff;
                (p->previous)->next = p->next;
                (p->next)->previous = p->previous;
            }
        }
    }

    return head;
}

/// The function that will clear all the spaces in the polynomial string before reading it
void clearSpace(char a[]){
    int k = 0;
    char b [100];
    for(int j=0 ; j<strlen(a) ; j++){
        if(a[j]!=' '){
            b[k]=a[j];
            k++;
        }
    }
    int i;
    for (i=0 ; i<k ; i++){
        a[i]=b[i];
    }
    for (i=i ; i<strlen(a);i++){
        a[i]=NULL;
    }
}

/// This function turns a number from a string to an integer
int iFromc(char a){
    int b = ((int) a) - 48;
    return b;
 }

/// This function does the summation equation for a whole list of Polynomials
node * getSum(node * equ [], int nofPoly){
    /// Part A) getting the summation into a linked list
    node * head = (node*)malloc(sizeof(node));
    node * p = equ[0];
    node * o = NULL;
    node * c = NULL;
    head->coff = p->coff;
    head->pow = p->pow;
    head->next = NULL;
    head->previous = NULL;
    int i =0;
    int coff;
    int pow;
    if(p->next!=NULL)
        p = p->next;
    else {
        p = equ[1];
        i++;
    }
    while (i< nofPoly){
        coff = p->coff;
        pow = p->pow;
        for(o=head ; o->next!=NULL && o->pow!=pow; o=o->next);
        if(o->pow!=pow){
            c = (node*)malloc(sizeof(node));
            c->coff = coff;
            c->pow = pow;
            c->previous=o;
            c->next=NULL;
            o->next=c;
        }
        else{
            o->coff+= coff;
        }

        if(p->next!=NULL)
        p = p->next;
    else {
        i++;
        p = equ[i];
    }
    }
    /// Part B) getting rid of the zeros coefficients
    for(o=head ; o!=NULL ; o=c){
        p= o->previous;
        c= o->next;
        if(o->coff==0){
            p->next=c;
            if(c!=NULL){
            c->previous=p;
            }
        }
    }
    /// Part C) Order the list from largest to smallest power
    for(o=head ; o!=NULL ; o=o->next){
        for(p=o->next ; p!=NULL ; p=p->next){
            if((o->pow) < (p->pow)){
                coff = o->coff;
                pow = o->pow;
                o->pow = p->pow;
                o->coff = p ->coff;
                p->coff = coff;
                p->pow = pow;
            }
        }
    }
    return head;
}

/// This function displays the polynomial of the linked list on the screen
void printPoly(node * head){
    int i=0;
    for(node * p = head ; p!=NULL ; p=p->next){
        if((p->previous==NULL && p->coff>1) || p->coff<-1){
            printf(" %d",p->coff);
        }
        else if(p->coff == -1){
            printf(" -");
            if(p->pow == 0)
                printf("1");
        }
        else if(p->coff == 1){
            if(p->previous!= NULL)
                printf(" +");
                if (p->pow == 0)
                    printf("1");
        }
        else {
            printf(" +%d",p->coff);
        }
        if(p->pow>0){
            printf("x");
            if(p->pow>1){
                printf("^%d",p->pow);
            }
        }
        i++;
        if(i%7==0){
            printf("\n");
        }
    }
}

/// This function multiply all equations from second to last with -1, in order to use the getsum() function to get the subtraction polynomial
void prepareForSubt(node * equ[], int nofPoly){
    if(nofPoly>1){
        int i=1;
        node * p = equ [1];
        while(i<nofPoly){
            (p->coff)*= -1;
            if(p->next==NULL){
                i++;
                p=equ[i];
            }
            else
                p = p->next;
        }
    }
}

/// This function gets multiplication of a whole list
node * multiplayList(node * equ[], int nofPoly){
    /// Part A) Doing the Multiplication
    if (nofPoly < 2)
        return equ [0];
    else {
        node * exp1 = copyPoly(equ[0]);
        node * exp2 = (node*)malloc(sizeof(node));
        exp2->previous = NULL;
        exp2->next = NULL;
        for(int j=1 ; j<nofPoly ; j++){
            exp2 = mult(exp1, equ[j]);
            free(exp1);
            exp1 = exp2;
        }
    /// Part B) Clearing all zeros
    for(exp2=exp1 ; exp2!=NULL ; exp2=exp2->next){
        node * p = exp2->previous;
        node * c = exp2->next;
        if(exp2->coff==0){
            p->next=c;
            if(c!=NULL){
            c->previous=p;
            }
        }
    }
        return exp1;
    }
}

/// This Function gets the multiplication of two polynomials in order to use in the previous function
node * mult(node * eq1, node * eq2){
    node * M = (node*)malloc(sizeof(node));
    M->previous = NULL;
    M->next = NULL;
    node * m = M;
    for(node * p1 = eq1 ; p1!=NULL ; p1 = p1->next){
        for(node * p2 = eq2 ; p2!=NULL ; p2 = p2 ->next){
            m->coff = (p1->coff)*(p2->coff);
            m->pow = (p1->pow)+(p2->pow);
                if (p1->next!=NULL || p2->next!=NULL ){
                    node * ext = (node*)malloc(sizeof(node));
                    ext->previous = m;
                    m->next=ext;
                    ext->next = NULL;
                    m= m->next;
                }
        }
    }
    return M;
}

/// This function makes a new linked list with save values and size of the inserted list
node * copyPoly(node * a){
    node * head = NULL;
    node * p = NULL;
    node * o = NULL;
    for(node * i = a ; i!=NULL ; i = i->next){
        p = (node*)malloc(sizeof(node));
        if (head == NULL){
            head = p;
            p->previous = NULL;
        }
        else{
            o->next=p;
            p->previous=o;
        }
        p->next=NULL;
        p->coff = i->coff;
        p->pow = i->pow;
        o=p;
    }
    return head;
}

/// This Function adds the coefficients of the same power variables
node * shortcut(node * a){
    node * M = (node*)malloc(sizeof(node));
    M->coff = a->coff;
    M->pow = a->pow;
    M->previous = NULL;
    M->next = NULL;
    node * m;
    for(node * i=a->next ; i!=NULL ; i=i->next){
        for(m=M ; m->next!=NULL && m->pow != i->pow; m=m->next);
        if(m->pow != i->pow){
            node * exp = (node*)malloc(sizeof(node));
            exp->coff = i->coff;
            exp->pow = i->pow;
            exp->previous=m;
            m->next=exp;
            exp->next=NULL;
        }
        else
            m->coff = m->coff + i->coff;
        }
       for(node * i = M ; i!=NULL ; i=i->next){
            for(node * j = i->next ; j!=NULL ; j=j->next){
                if(j->pow > i->pow){
                int coff = i->coff;
                int pow = i->pow;
                i->coff = j->coff;
                i->pow = j->pow;
                j->coff = coff;
                j->pow = pow;
                }
            }

        }
        return M;
    }

/// This function is to get the result of a substituted value of x
double substitue(double a, node * list){

    double m=0;
    for(node * p = list ; p!=NULL ; p=p->next){
        m += (pow(a, p->pow))*(p->coff);
    }
    return m;
}

/// This Function prints a linked list polynomial to the results file
void printToFile(node * head, FILE *out){
    int i =0;
     for(node * p = head ; p!=NULL ; p=p->next){
        if((p->previous==NULL && p->coff>1) || p->coff<-1){
            fprintf(out," %d",p->coff);
        }
        else if(p->coff == -1){
            fprintf(out," -");
            if(p->pow == 0)
                fprintf(out,"1");
        }
        else if(p->coff == 1){
            if(p->previous!= NULL)
                fprintf(out," +");
                if (p->pow == 0)
                    fprintf(out,"1");
        }
        else {
            fprintf(out," +%d",p->coff);
        }
        if(p->pow>0){
            fprintf(out,"x");
            if(p->pow>1){
                fprintf(out,"^%d",p->pow);
            }
        }
        i++;
        if(i%6==0){
            fprintf(out,"\n");
        }
    }
}
