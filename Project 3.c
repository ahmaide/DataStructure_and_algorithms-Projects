#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Name:    Ahmaide AL-Awawdah
ID:      1190823
Section: 4
*/

/// This will be the AVL node
typedef struct node{
    char name [30];
    char gender;
    int dateOfA;
    int dataofB;
    char city [30];
    char illness [30];
    char blood [3];
    struct node* left;
    struct node* right;
    int Height;
}node;

/// The hash data
typedef struct Patient{
    char name [30];
    char gender;
    int dateOfA;
    int dataofB;
    char city [30];
    char illness [30];
    char blood [3];
}Patient;

/// The Hash table
typedef struct {
    Patient **arrayP;
}hashTable;

/// The AVL tree functions
node * tree = NULL; /// the AVL tree is global
int nOfPatients; /// The number of patients is global
int compareTo(node *a, node*b);
node* insert(node *a, node *T);
node * Construct(char info[]);
int getDate(char str[]);
void printData(node *N);
int Max(int Lhs, int Rhs);
int Height(node *P);
node *SingleRotateWithLeft(node *K2);
node *SingleRotateWithRight(node *K1);
node *DoubleRotateWithLeft(node *K3);
node *DoubleRotateWithRight(node *K1);
void CreateNewPatient();
node *find(char name[], node *T);
void change(node *p);
void printAllData(node *p);
void printForIllness(node *p, char illnesss[]);
node *Delete(node* T,char name[]);
node *FindMin(node *T);
void copyNode(node *a, node*b);
int balance(node *N);
int dateSort(int D, int a);
void printInFile(FILE *out, node *T);

/// The hash functions
int prime; /// The size of the HashTable and it should be a prime number and its global
int getPrimeX2(int a);
int H(char name[], int i);
void insertTreeToHash(node *T, Patient *p[]);
void copyFromNodeToPatient(node *N, Patient *p);
void printHash(Patient *p);
Patient *searchInHashTable(char name[], Patient *p[]);
void addNewPatientToHash(Patient *p[]);
void printInFileForHash(FILE *out, Patient *P[], int size0);
hashTable *rehash(hashTable *H);
int check75(int a, int b);
hashTable *createHashTable();

int main()
{
    /// The AVL menu
    int menu1= 0;
    printf("Enter the following number for the following operation\n******************************************************\n");
    printf("1- To read data from the file\t2- To insert a new patient\n3- Find & update patient data\t4- List all patients\n5- List all patients with same illness\t6- Delete a patient\n7- Print Data in patient_hash file\n");
    printf("**********************************************************\n....:");
    scanf("%d",&menu1);
    char PAinfo [130];
    char checker [130]; /// The checker string (array of char) in order to not have any errors after the last line
    node *p;
    nOfPatients = 0;
    FILE *out;
    out = fopen("patients_hash.data","w");
    int checkIfFileIsRead = 1; /// So the file can be only read once
    while(menu1<8 && menu1>0){
        switch (menu1){
            /// The case to read the file and store the data in the AVL tree
            case 1:{
                if(checkIfFileIsRead%2==1){
                    FILE *in;
                    in = fopen("patients.txt","r");
                    while(!feof(in)){
                        fgets(PAinfo , 130 , in);
                        if(strcmp(PAinfo,checker) && PAinfo[0]!=10){
                            p = Construct(PAinfo);
                            tree = insert(p,tree);
                            nOfPatients++;
                            strcpy(checker,PAinfo);
                        }
                    }
                    checkIfFileIsRead*=2;
                    printf("File Has been read successfully! \n");
                }
                else
                    printf("File already been read\n");

            }
                break;
                /// This case calls a function that creates a new patient and inserts him/her into the tree
            case 2:{
                printf("\n");
                CreateNewPatient();
                nOfPatients++;
            }
                break;
                /// This case searches for patient by name and gives an option to do changes on him\her
            case 3:{
                char name [30];
                printf("Enter the name of the patient: ");
                scanf("%c", &name[0]);
                gets(name);
                p = find(name,tree);
                if(p!=NULL){
                    printData(p);
                    int option;
                    printf("\nEnter 1 if you want to change in this patient's information: ");
                    scanf("%d", &option);
                    if(option==1)
                        change(p);
                    printf("All been fixed");
                }
            }
                break;
                /// This case is to print all patients
            case 4:{
                printf("\n**************************\n");
                printAllData(tree);
            }
                break;
                /// This case is to print all patients with the same illness
            case 5:{
                char illness [30];
                printf("Enter illness: ");
                scanf("%c",&illness[0]);
                gets(illness);
                printf("All the patients with %s\n", illness);
                printf("*****************************************\n");
                printForIllness(tree,illness);
            }
                break;
                /// This case is to delete a patient searched by name
            case 6:{
                char D [30];
                printf("Enter the name of the patient you want to delete: ");
                scanf("%c", &D[0]);
                gets(D);
                tree = Delete(tree, D);
            }
                break;
                /// This case is to save all the patients in the tree to the output file and exit the loop
            case 7:{
                fprintf(out,"The tree data\n**********************************\n\n");
                printInFile(out,tree);
                printf("all data have been printed in the file patients hash \n\n");
            }
                break;
            default:{
                printf("Thank you \n");
            }
        }
        if (menu1 == 7)
            break;
        printf("\nEnter the following number for the following operation\n******************************************************\n");
        printf("1- To read data from the file\t2- To insert a new patient\n3- Find & update patient data\t4- List all patients\n5- List all patients with same illness\t6-Delete a patient\nOther- To exit\n7- Print Data in patient_hash file\n");
        printf("**********************************************************\n....:");
        scanf("%d",&menu1);
    }
    /// prime is the size of the array that should be a prime number (it is global)
    prime = getPrimeX2(((nOfPatients)*4)/3);
    /// The needed size of the hash table is global so no need to have it inserted to the function
    hashTable *patients = createHashTable();
    insertTreeToHash(tree, patients->arrayP);
    /// The Hash menu
    int menu2;
    printf("\n\nNow moving to the hash\n***********************************************\n***********************************************\n\n");
    printf("Enter the following number for the following operation\n******************************************************\n");
    printf("1- To Display the hash table\t2- To get the size of the table\n3- To get the hash function\t4- insert a new record\n5- search for specific patient\t6- Delete a specific record\n7- save the table to the file\tother- To leave\n.\n.\n.............:");
    scanf("%d", &menu2);
    while (menu2>0 && menu2<8){
        switch (menu2){
            /// This case displays the hash table with all empty spots
            case 1:{
                printf("\nThe hash Table:\n******************************************************************************************************************\n");
                for(int i=0; i<prime ; i++){
                    printf("%d- ", i+1);
                    if(patients->arrayP[i]!=NULL)
                        printHash(patients->arrayP[i]);
                    else
                        printf("\n\n");
                }
            }
                break;
                /// This case prints the hash table size with the number of empty spots
            case 2:{
                printf("\nTable size is %d with %d empty spots\n", prime, prime-nOfPatients);
            }
                break;
                /// This case prints the hash function
            case 3:{
                printf("H(x) = [ ( ASCII(1st letter) + ASCII(2nd letter) )  X  ACSII(3rd letter)  X  7 )  +  (i^2) ] % Size");
            }
                break;
                /// This case is to insert a new patient to the hash table or rehash e if it was full
            case 4:{
                nOfPatients++;
                /// No need to rehash here
                if(check75(nOfPatients,prime)){
                    addNewPatientToHash(patients->arrayP);
                }
                /// in this case rehash is implemented
                else {
                    patients = rehash(patients);
                    addNewPatientToHash(patients->arrayP);
                }
            }
                break;
                /// This case is to search for a patient by his/her name and print his/her information
            case 5:{
                char n [30];
                printf("\nEnter the name of the patient you are looking for:");
                scanf("%c",&n[0]);
                gets(n);
                Patient *z = searchInHashTable(n,patients->arrayP);
                printHash(z);
            }
                break;
                /// This case is to delete a patient from the hash table by his/her name
            case 6:{
                char n[30];
                printf("\nEnter the name of the patient you are looking for:");
                scanf("%c",&n[0]);
                gets(n);
                int j;
                for(j=0; strcmp(patients->arrayP[H(n,j)]->name, n) ; j++){
                }
                printf("check\n");
                free(patients->arrayP[H(n,j)]);
                patients->arrayP[H(n,j)] = NULL;
                nOfPatients--;
            }
                break;
                /// This case is to print the hash table into the output file with all empty spots
            case 7: {
                printInFileForHash(out, patients->arrayP, prime);
                printf("\nHash Table is printed in the file :)\n");
            }
        }
        if(menu2==7)
            break;
        printf("\nEnter the following number for the following operation\n******************************************************\n");
        printf("1- To Display the hash table\t2- To get the size of the table\n3- To get the hash function\t4- insert a new record\n5- search for specific patient\t6- Delete a specific record\n7- save the table to the file\tother- To leave\n.\n.\n.............:");
        scanf("%d", &menu2);
    }
    return 0;
}

/// This function is to give which patient goes before the other in the alphabetic order
int compareTo(node *a, node *b){
    int i;
    for(i=0; a->name[i]==b->name[i] && i<strlen(a->name) && i<strlen(b->name) ; i++);
    if(a->name[i] > b->name[i])
        return 1;
    else if(a->name[i] < b->name[i])
        return -1;
    else {
        if(strlen(a->name)>strlen(b->name))
            return 1;
        else if (strlen(b->name) > strlen(a->name))
            return -1;
        else
            return 0;
    }
}

/// This function is to insert a new patient to the tree after making a node full of their information
node *insert( node *a, node *T ){
    if(T == NULL){
        T=a;
    }
    else{
        if(compareTo(a->name,T->name) == -1){
            T->left = insert(a, T->left);
            if(Height(T->left) - Height(T->right) == 2)
                if(compareTo(a->name,T->left->name) == -1)
                    T = SingleRotateWithLeft(T);
                else
                    T = DoubleRotateWithLeft(T);
        }
        else if(compareTo(a->name,T->name) == 1){
            T->right = insert(a, T->right);
            if( Height(T->right) - Height(T->left) == 2)
                if(compareTo(a->name,T->right->name) == 1)
                    T = SingleRotateWithRight(T);
                else
                    T = DoubleRotateWithRight(T);
        }
    }
    T->Height = Max(Height(T->left), Height(T->right) ) + 1;
    return T;
}

/// This function is to fill an AVL node with patient's info from a string (array of char)
node * Construct(char info[]){
    node * N = (node*)malloc(sizeof(node));
    N->left=NULL;
    N->right=NULL;
    N->Height = 0;
    strcpy(N->name, strtok(info , "#"));
    int i = strlen(N->name) +1;
    N->gender = info[i];
    strtok(NULL , "#");
    char date [8];
    strcpy(date, strtok(NULL , "#"));
    N->dateOfA = getDate(date);
    strcpy(date, strtok(NULL , "#"));
    N->dataofB = getDate(date);
    strcpy(N->illness, strtok(NULL , "#"));
    strcpy(N->city, strtok(NULL , "#"));
    strcpy(N->blood, strtok(NULL , "\n"));
    return N;
}

/// This function turns an 8 bit string (array of char) to an integer in order to get the date
int getDate(char str[]){
    int date =0;
    for(int i=0 ; i<8 ; i++){
        date= date*10 + (str[i]-48);
    }
    return date;
}

/// This function to print an AVL node informations
void printData(node *N){
    if(N!=NULL)
        printf("Patient name: %s   Patient gender: %c   Date of admission: %d/%d/%d   Date of birth: %d/%d/%d   illness: %s   address: %s   blood type: %s\n\n",N->name,N->gender,dateSort(N->dateOfA,1),dateSort(N->dateOfA,2),dateSort(N->dateOfA,3),dateSort(N->dataofB,1),dateSort(N->dataofB,2),dateSort(N->dataofB,3),N->illness,N->city,N->blood);
}

/// This function is compare which side is higher in order to get the hight of an AVL node
int Max(int L, int R)
{
    if (L > R)
        return L;
    else
        return R;
}

/// A function to return the height
int Height(node *P){
    if( P == NULL )
        return -1;
    else
        return P->Height;
}

node *SingleRotateWithLeft (node *K2){
    node *K1;
    K1 = K2->left;
    K2->left = K1->right;
    K1->right = K2;
    K2->Height = Max(Height(K2->left), Height(K2->right)) + 1;
    K1->Height = Max(Height(K1->left), K2->Height) + 1;
    return K1;
}

node *SingleRotateWithRight(node *K1){
    node *K2;
    K2 = K1->right;
    K1->right = K2->left;
    K2->left = K1;
    K1->Height = Max(Height(K1->left), Height(K1->right)) + 1;
    K2->Height = Max(Height(K2->right), K1->Height) + 1;
    return K2;
}

node *DoubleRotateWithLeft(node *K3){
    K3->left = SingleRotateWithRight(K3->left);
    return SingleRotateWithLeft(K3);
}

node *DoubleRotateWithRight(node *K1){
    K1->right = SingleRotateWithLeft(K1->right);
    return SingleRotateWithRight(K1);
}

/// This function is to create a new patient and put his/her information into an AVL node and insert it to the tree
void CreateNewPatient(){
    node *p = (node*)malloc(sizeof(node));
    printf("Enter the new ");
    printf("patient's name: ");
    scanf("%c", &p->name[0]);
    gets(p->name);
    printf("Enter the gender of the new patient: ");
    scanf("%c", &p->gender);
    int year;
    int month;
    int day;
    printf("Enter year of admission: ");
    scanf("%d", &year);
    printf("Enter month of admission: ");
    scanf("%d", &month);
    printf("Enter day of admission: ");
    scanf("%d", &day);
    p->dateOfA = (day*1000000 + month*10000 + year);
    printf("Enter year of birth: ");
    scanf("%d", &year);
    printf("Enter month of birth: ");
    scanf("%d", &month);
    printf("Enter day of birth: ");
    scanf("%d", &day);
    p->dataofB = (day*1000000 + month*10000 + year);
    printf("Enter the illness: ");
    scanf("%c", &p->illness[0]);
    gets(p->illness);
    printf("Enter address (Only city name): ");
    //scanf("%c", &p->city[0]);
    gets(p->city);
    printf("Enter blood type: ");
    gets(p->blood);
    p->Height = 0;
    p->right = NULL;
    p->left = NULL;
    insert(p, tree);
    printData(p);
}

/// This function is to search for a patient by his/her name
node *find(char name[], node *T){
    if(T==NULL){
        printf("You have no such patient\n");
        return NULL;
    }
    else{
        if(compareTo(name,T->name)==0){
            return T;
        }
        else if(compareTo(name,T->name)==-1){
            find(name, T->left);
        }
        else if(compareTo(name,T->name)==1){
            find(name, T->right);
        }
    }
}

/// This function is to change a patient's info and gives the user an option change whatever he wants in a loop
/// PS in the gender changing case the user requests to change the gender and the system changes it to the other gender since there is only 2 genders.
void change(node *p){
    /// The selector is a new menu here
    int selector;
    printf("Enter the following number for the following operation\n******************************************************\n");
    printf("1- To change the date of admission\t2- To change the birth date\n3- To change the address\t4- To change the illness\n5- To change the blood type\t6- To change the name\n7- To change gender\tother- To leave\n...:");
    scanf("%d", &selector);
    while(selector>0 && selector<8){
        switch(selector){
            case 1:{
                int year, month, day;
                printf("Enter new admission year: ");
                scanf("%d", &year);
                printf("Enter new admission month: ");
                scanf("%d", &month);
                printf("Enter new admission day: ");
                scanf("%d", &day);
                p->dateOfA = (year + month*10000 + day*1000000);
            }
                break;
            case 2:{
                int year, month, day;
                printf("Enter new birth year: ");
                scanf("%d", &year);
                printf("Enter new birth month: ");
                scanf("%d", &month);
                printf("Enter new birth day: ");
                scanf("%d", &day);
                p->dataofB = (year + month*10000 + day*1000000);
            }
                break;
            case 3:{
                printf("Enter new Address (Only city name): ");
                scanf("%c", &p->city[0]);
                gets(p->city);
            }
                break;
            case 4:{
                printf("Enter new illness: ");
                scanf("%c", &p->illness[0]);
                gets(p->illness);
            }
                break;
            case 5:{
                printf("Enter new Blood type: ");
                scanf("%c", &p->blood[0]);
                gets(p->blood);
            }
                break;
            case 6:{
                node *o = (node*)malloc(sizeof(node));
                copyNode(o,p);
                printf("Enter new name: ");
                scanf("%c", &o->name[0]);
                gets(o->name);
                tree = Delete(tree,p->name);
                nOfPatients++;
                printf("%s it worked******\n", &o->name);
                char returner [30];
                strcpy(returner,o->name);
                tree = insert(o,tree);
                p = find(returner, tree);
                printData(p);
            }
                break;
            case 7:{
                printData(p);
                switch (p->gender){
                    case 'M':{
                        p->gender = 'F';
                    }
                        break;
                    case 'F':{
                        p->gender = 'M';
                    }
                        break;
                }
                printf("done!\n");
            }
                break;
        }
        printf("\nEnter the following number for the following operation\n******************************************************\n");
        printf("1- To change the date of admission\t2- To change the birth date\n3- To change the address\t4- To change the illness\n5- To change the blood type\t6- To change the name\n7- To change gender\tother- To leave\n...:");
        scanf("%d", &selector);

    }


}

/// This function is to print all the patients in a tree
void printAllData(node *p){
    if(p->left!=NULL)
        printAllData(p->left);
    printData(p);
    if(p->right!=NULL)
        printAllData(p->right);
}

/// This function takes an illness and a tree, and prints all the patients in the tree with the illness
void printForIllness(node *p, char illnesss[]){
    if(p->left!=NULL)
        printForIllness(p->left,illnesss);
    if(!strcmp(p->illness,illnesss))
        printData(p);
    if(p->right!=NULL)
        printForIllness(p->right,illnesss);
}

/// This function is to delete a patient from a tree and it has 3 parts
node *Delete(node* T,char name[]){
    /// Part A: look for the needed patient
    if (T == NULL){
        printf("You have no such patient\n");
        return T;
    }
    if(compareTo(name,T->name)==-1)
        T->left = Delete(T->left, name);
    else if(compareTo(name,T->name)==1)
        T->right = Delete(T->right, name);
    else{
        /// Part B: delete the needed patient, and replace it's place in the tree with another patient to keep the tree connected
        nOfPatients--;
        if((T->left == NULL) || (T->right == NULL)){
            node *p;
            if(T->left!=NULL)
                p = T->left;
            else
                p = T->right;
            if (p == NULL){
                p = T;
                T = NULL;
            }
            else{
                copyNode(T,p);
                if(p==T->right)
                    T->right=NULL;
                else if(p==T->left)
                    T->left=NULL;
            }
            free(p);
        }
        else{
            node *p = FindMin(T->right);
            copyNode(T,p);
            T->right = Delete(T->right, p->name);
            nOfPatients++;
        }
    }
    if (T==NULL)
        return NULL;
    /// Part C: make sure that the tree is still balances and do any needed rotates
    T->Height = 1 + Max(Height(T->left),Height(T->right));
    if(balance(T) > 1){
        if(balance(T->left) < 0)
            T->left = SingleRotateWithRight(T->left);
        return SingleRotateWithLeft(T);
    }
    else if(balance(T) < -1){
        if(balance(T->right) > 0)
            T->right = SingleRotateWithLeft(T->right);
        return SingleRotateWithRight(T);
    }
    return T;
}

/// This function returns the first alphabetic patient in the tree after the inserted one
node *FindMin(node *T){
    if( T == NULL)
        return NULL;
    else if(T->left == NULL)
        return T;
    else
        return FindMin(T->left);
}

/// This Function gives an AVL node other node's patient's info
void copyNode(node *a, node*b){
    strcpy(a->name,b->name);
    a->gender = b->gender;
    a->dataofB = b->dataofB;
    a->dateOfA = b->dateOfA;
    strcpy(a->city,b->city);
    strcpy(a->illness,b->illness);
    strcpy(a->blood,b->blood);
}

/// This function gives the balance of an AVL node
int balance(node *N){
    if(N==NULL)
        return 0;
    else
        return Height(N->left) - Height(N->right);
}

/// This function takes a full date (DayMonthYear) and gives the day if a=1, month if a=2, year if a=3
int dateSort(int D, int a){
    switch(a){
        case 1:{
            return (int) D/1000000;
        }
            break;
        case 2:{
            return (((int) D/10000)*10000 - ((int) D/1000000)*1000000)/10000;
        }
            break;
        case 3:{
            return D - ((int) D/10000)*10000;
        }
            break;
        default :{
            return 0;
        }
    }
}

/// This function is to print all tree patients into an output FILE
void printInFile(FILE *out, node *N){
    if(N->left!=NULL)
        printInFile(out,N->left);
    fprintf(out,"Patient name: %s   Patient gender: %c   Date of admission: %d/%d/%d   Date of birth: %d/%d/%d   illness: %s   address: %s   blood type: %s\n",N->name,N->gender,dateSort(N->dateOfA,1),dateSort(N->dateOfA,2),dateSort(N->dateOfA,3),dateSort(N->dataofB,1),dateSort(N->dataofB,2),dateSort(N->dataofB,3),N->illness,N->city,N->blood);
    if(N->right!=NULL)
        printInFile(out,N->right);
}

/// This function gets prime number that is the size of the hash table depending on the number of patients
int getPrimeX2(int a){
    int i = a;
    int j = 2;
    for(i=i ; (i-1)!=j ; i++){
        for(j=2 ; j<i && i%j!=0 ; j++);
    }
    return i-1;
}

/// The hash function = [ ( ASCII(1st letter) + ASCII(2nd letter) )  X  ACSII(3rd letter)  X  7 )  +  (i^2) ] % Size
int H(char name[], int i){
    return ((name[1] + name[2])*name[3]*7 + ((int) pow(i,2)))%prime;
}

/// This function is to insert all the patients in the tree to the hash table
void insertTreeToHash(node *T, Patient *p[]){
    if(T->left!=NULL)
        insertTreeToHash(T->left,p);
    int i;
    for(i =0 ; p[H(T->name,i)]!=NULL ; i++);
    int place = H(T->name,i);
    p[place] = (Patient*)malloc(sizeof(Patient));
    copyFromNodeToPatient(T,p[place]);
    if(T->right!=NULL)
        insertTreeToHash(T->right,p);
}

/// This function gives an AVL node info of a patient to a hash node
void copyFromNodeToPatient(node *N, Patient *p){
    strcpy(p->name,N->name);
    p->gender = N->gender;
    p->dataofB = N->dataofB;
    p->dateOfA = N->dateOfA;
    strcpy(p->blood,N->blood);
    strcpy(p->illness,N->illness);
    strcpy(p->city,N->city);
}

/// This function is to print a hash node information
void printHash(Patient *p){
    if(p!=NULL)
        printf("Patient name: %s   Patient gender: %c   Date of admission: %d/%d/%d   Date of birth: %d/%d/%d   illness: %s   address: %s   blood type: %s\n\n",p->name,p->gender,dateSort(p->dateOfA,1),dateSort(p->dateOfA,2),dateSort(p->dateOfA,3),dateSort(p->dataofB,1),dateSort(p->dataofB,2),dateSort(p->dataofB,3),p->illness,p->city,p->blood);
}

/// This function is to search for a patient in the hash table by his/her name
Patient *searchInHashTable(char name[], Patient *p[]){
    int i=0;
    for(i=0; strcmp(p[H(name,i)]->name, name) ; i++);
    return p[H(name,i)];
}

/// This function is to add a new patient (with unknown data) to the hash table
void addNewPatientToHash(Patient *p[]){
    Patient *m = (Patient*)malloc(sizeof(Patient));
    printf("Enter the new patient's name: ");
    scanf("%c", &m->name[0]);
    gets(m->name);
    printf("Enter the gender of the new patient: ");
    scanf("%c", &m->gender);
    int year;
    int month;
    int day;
    printf("Enter year of admission: ");
    scanf("%d", &year);
    printf("Enter month of admission: ");
    scanf("%d", &month);
    printf("Enter day of admission: ");
    scanf("%d", &day);
    m->dateOfA = (day*1000000 + month*10000 + year);
    printf("Enter year of birth: ");
    scanf("%d", &year);
    printf("Enter month of birth: ");
    scanf("%d", &month);
    printf("Enter day of birth: ");
    scanf("%d", &day);
    m->dataofB = (day*1000000 + month*10000 + year);
    printf("Enter the illness: ");
    scanf("%c", &m->illness[0]);
    gets(m->illness);
    printf("Enter address (Only city name): ");
    gets(m->city);
    printf("Enter blood type: ");
    gets(m->blood);
    int i;
    for(i=0; p[H(m,i)]!=NULL ; i++);
    p[H(m->name,i)] = m;
}

/// This function is to print the hash table to the output FILE with all empty spots
void printInFileForHash(FILE *out, Patient *P[], int size0){
    fprintf(out,"\n\nThe hash Table\n****************************************************\n");
    for(int i=0; i<size0 ; i++){
        fprintf(out,"\n%d- ",i+1);
        if(P[i]!=NULL)
            fprintf(out,"Patient name: %s   Patient gender: %c   Date of admission: %d/%d/%d   Date of birth: %d/%d/%d   illness: %s   address: %s   blood type: %s",P[i]->name,P[i]->gender,dateSort(P[i]->dateOfA,1),dateSort(P[i]->dateOfA,2),dateSort(P[i]->dateOfA,3),dateSort(P[i]->dataofB,1),dateSort(P[i]->dataofB,2),dateSort(P[i]->dataofB,3),P[i]->illness,P[i]->city,P[i]->blood);
        fprintf(out,"\n");
    }
}

/// The function to make a larger hash table with a bigger size and insert the previous elements into it
hashTable *rehash(hashTable *HT){
    int holder = prime;
    prime = getPrimeX2(prime*2);
    hashTable *m = createHashTable();
    for(int i=0 ; i<holder ; i++){
        if(HT->arrayP[i]!=NULL){
            int j=0;
            for(j=0 ; m->arrayP[H(HT->arrayP[i]->name,j)]!=NULL ; j++);
            m->arrayP[H(HT->arrayP[i]->name,j)] = HT->arrayP[i];
        }
    }
    return m;
}

/// This function is to check if a/b > 0.75 or not
int check75(int a, int b){
    double a0 = (double) a;
    double b0 = (double) b;
    if(a0/b0 > 0.75)
        return 0;
    else
        return 1;
}

/// This function is to create a new hash table with the size of the global variable (prime)
hashTable *createHashTable(){
    hashTable *HT = malloc(prime * sizeof(hashTable*));
    HT->arrayP = malloc(prime*sizeof(Patient));
    for(int i=0; i<prime ; i++)
        HT->arrayP[i] = NULL;
    return HT;
}
