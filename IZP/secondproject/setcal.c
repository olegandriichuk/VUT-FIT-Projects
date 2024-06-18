#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100
#define TRUE (1==1)
#define FALSE (1==0)
#define MIN_ARGC 2
#define MAX_ARGC 3
int errorHandling(int argc, FILE *fp)
{
    if (argc < MIN_ARGC)
    {
        fprintf(stderr, "\nERROR: Not enough arguments.\n");
        return 0;
    }
    if (argc >= MAX_ARGC)
    {
        fprintf(stderr, "\nERROR: Too many arguments.\n");
        return 0;
    }
    if (fp == NULL)
    {
        fprintf(stderr, "\nERROR: Can not open the file.\n");
        return 0;
    }
    return 1;
}

int reflexive(char a[SIZE][SIZE][SIZE], int Radek) //Tiskne true nebo false jestli relaci je reflexivna
{ 
  int n=1;
  for(int i=n+1;a[Radek-1][i][0]!='\0';i+=2)
  {
    if(strcmp(a[Radek-1][n],a[Radek-1][i])==0)
    {
      printf("true\n");
      return 1;
    }
  }
  printf("False\n");
  return 0;
}
int symmetric(char a[SIZE][SIZE][SIZE],int R, int LEN) //Tiskne true nebo false jestli relaci je symmetricka
{
  int yes=0;
  for(int n=1;n<LEN;n+=2)
  {
    for(int b=n+2;b<LEN;b+=2)
    {
      if(strcmp(a[R-1][n+1], a[R-1][b])==0 && strcmp(a[R-1][b+1], a[R-1][n])==0)
      {
        yes=1;
      }
    }
  }
  if(yes==1)
  {
    printf("true\n");
    return 1;
  }
  printf("false\n");
  return 0;
}
int antisymmetric(char a[SIZE][SIZE][SIZE], int R, int LEN) //Tiskne true nebo false jestli relaci je antisymmetricka
{
  int yes=0;
  for(int n=1;n<LEN;n+=2)
  {
    for(int b=n+2;b<LEN;b+=2)
    {
      if(strcmp(a[R-1][n+1], a[R-1][b])==0 && strcmp(a[R-1][b+1], a[R-1][n])==0)
      {
        yes=1;
      }
    }
  }
  if(yes==1)
  {
    printf("false\n");
    return 1;
  }
  printf("true\n");
  return 0;
}

int transitive(char a[SIZE][SIZE][SIZE], int R, int LEN) //Tiskne true nebo false jestli relaci je transitivna
{
    for(int n=2;n<LEN;n+=2)
    {
        for(int b=n-1;b<LEN;b+=2)
        {
            for(int c=n+2;c<LEN;c+=2)
            {
                if(strcmp(a[R-1][n], a[R-1][n+1])==0 && strcmp(a[R-1][b], a[R-1][c+1])==0 && strcmp(a[R-1][c], a[R-1][c+2])==0 && a[R-1][c+2][0]!='\0')
                {
                    printf("true\n");
                    return 1;
                }
            }
        }
    }
    printf("false\n");
    return 0;
}
int function(char a[SIZE][SIZE][SIZE], int R, int LEN)
{
    for(int i=2;i<LEN;i+=2)
    {
        if(strcmp(a[R-1][i], a[R-1][i+1])!=0)
        {
            printf("true\n");
            return 1;
        }
    }
    printf("false\n");
    return 0;
}
void domain(char a[SIZE][SIZE][SIZE], int R, int LEN) //Tiskne 2 Prvki Relace 
{
    printf("S ");
    for(int b=1;b<LEN;b+=2)
    {
        if(strcmp(a[R-1][b], a[R-1][b-2])!=0)
        {
            printf("%s ", a[R-1][b]);
        }
    }
    printf("\n");
}
void codomain(char a[SIZE][SIZE][SIZE], int R, int LEN) //Tiskne 1 Prvki Relace 
{
    printf("S ");

    for(int b=2;b<LEN;b+=2)
    {
        if(strcmp(a[R-1][b], a[R-1][b-2])!=0)
        {
            printf("%s ", a[R-1][b]);
        }
    }
    printf("\n");
}

void surjective(char a[SIZE][SIZE][SIZE], int R)
{
    int no;
    for(int i=1;a[R-1][i][0]!='\0';i++)
    {
        for(int b=i+2; a[R-1][b][0]!='\0';b+=1)
        {
            if(strcmp(a[R-1][i], a[R-1][b])==0)
            {
                no=1;
            }
        }
    }
    if(no==1)
    {
        printf("true\n");
        return;
    }
    printf("false\n");
}
void injective(char a[SIZE][SIZE][SIZE], int R)
{
    int yes;
    for(int i=1;a[R-1][i][0]!='\0';i+=2)
    {
        if(strcmp(a[R-1][i], a[R-1][i+1])==0)
        {
            yes=1;
        }
        if(yes==1)
        {
            printf("false\n");
            return;
        }
    }
    printf("true\n");
}

void bijective(char a[SIZE][SIZE][SIZE], int R)
{
    int yes;
    int no;
    for(int i=1;a[R-1][i][0]!='\0';i++)
    {
        for(int b=i+2; a[R-1][b][0]!='\0';b+=1)
        {
            if(strcmp(a[R-1][i], a[R-1][b])==0)
            {
                no=1;
            }
        }
    }
    for(int i=1;a[R-1][i][0]!='\0';i+=2)
    {
        
        if(strcmp(a[R-1][i], a[R-1][i+1])==0)
        {
            yes=1;
        }
    }
    if(yes!=1 && no ==1)
    {
        printf("true\n");
        return;
    }
    printf("false\n");
}

void scan(char u[SIZE][SIZE][SIZE], FILE *fp)
{
  int a=0;
  int b=0;
  int i=0;
  int n=0;
  for(int c;(c=fgetc(fp))!=EOF;)
  {
    a++;
    if(c != '\n')
    {
      if(c == ' ')
      {
        i++;
        n=0;
      }
      else if(c =='(' || c ==')')
      {

      }
      else
      {
        u[b][i][n]=c;
        n++;
      }
    }
    else
    {
      b++;
      i=0;
      n=0;
    }
  }
}

int countElements (char M[SIZE][SIZE][SIZE], int R) {
    int i=0;
    while (M[R-1][i+1][0]!='\0') {
        i++;
    }
    return i;
}

_Bool ifStringinMnoziny (char _M[SIZE][SIZE][SIZE], int _lenM, int M1,char *_S) {
    int i=1;
    _Bool out=FALSE;

    while (i<=_lenM && !out) {
        int lenS=strlen(_S);
        int lenMi=strlen(_M[M1-1][i]);
        int j=0;
        out = lenS==lenMi;

        while (j<lenS && out) {
            out=_M[M1-1][i][j]==_S[j];
            j++;
        }
        i++;
    }
    return out;
}

_Bool ifMnozinainUniverse (char U[SIZE][SIZE][SIZE], int lenU, int M1,char M[SIZE][SIZE][SIZE], int lenM, int M2) {
    int i=1;
    _Bool out=TRUE;

    while (i<=lenM && out) {
        out = ifStringinMnoziny(U,lenU,M1,M[M2-1][i]);
        i++;
    }
    return out;
}


// check if empty
void Empty(char U[SIZE][SIZE][SIZE], int M1) {
    int lenA=countElements(U,M1);
    if (lenA) {
        printf("false\n");
    }
    else {
        printf("true\n");
    }
    return;
} 

void Card(char U[SIZE][SIZE][SIZE], int M1) {
    int lenA=countElements(U,M1);
    if (lenA) {
        printf("%d\n",lenA);  
    }
    else {
        printf("Mnozina is empty\n");
    }
    return;
}


// complement Mnoziny A
void Complement (char U[SIZE][SIZE][SIZE], int M1) {

    int inA [SIZE], lenU,lenA;
    int k=0;

    lenU=countElements(U,1); // Count of elements
    lenA=countElements(U,M1); // Count of elements in "Mnozina"
 
    for (int i = 1; i <= lenU; i++) {
        char S[SIZE];
        strcpy(S,U[0][i]);
        inA[i]=0; // 0 count of elements
        if (ifStringinMnoziny(U,lenA,M1,S)) { 
            inA[i-1]=1; 
            k++;
        }
    }
    if (lenA==k) {
        int k=0;
        printf("S ");
        for (int i = 1; i <= lenU; i++) {
            if (inA[i-1]==0) {
                if (k++>0) { printf(" "); }
                printf("%s",U[0][i]);
            }
        }
        printf("\n");
    }
}


// union A B
void Union (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int k=0;
    int lenA=countElements(U,M1); // кількість елементів множини А
    int lenB=countElements(U,M2); // кількість елементів множини B

    printf("S ");
    for (int i = 1; i <= lenA; i++) {
        if (k++>0) { printf(" "); }
        printf("%s",U[M1-1][i]);
    }
    for (int i = 1; i <= lenB; i++) {
        if (!ifStringinMnoziny(U,lenA,M1,U[M2-1][i])) { 
            if (k++>0) { printf(" "); }
        printf("%s",U[M2-1][i]);
        }
    }
    printf("\n");
}


//intersect A B
void Intersect (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int k=0;
    int lenA=countElements(U,M1);
    int lenB=countElements(U,M2);

    printf("S ");
    for (int i = 1; i <= lenB; i++) {
        if (ifStringinMnoziny(U,lenA,M1,U[M2-1][i])) {
            if (k++>0) {
                printf(" "); 
            }
            printf("%s",U[M2-1][i]);
        }
    }
    printf("\n"); 
} 

// minus A B
void Minus (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int k=0;
    int lenA=countElements(U,M1);
    int lenB=countElements(U,M2);
        
    printf("S ");
    for (int i = 1; i <=lenA; i++) {
        if (!ifStringinMnoziny(U,lenB,M2,U[M1-1][i])) {
            if (k++>0) {
                printf(" "); 
            }
            printf("%s",U[M1-1][i]);
        }
    }
    printf("\n"); 
}


//subseteq A B
void Subseteq (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int lenA=countElements(U,M1);
    int lenB=countElements(U,M2);
    int out=lenA<=lenB;
        
    out=out && ifMnozinainUniverse(U,lenB,M2,U,lenA,M1);
    if (out) {
        printf("true\n"); 
    }
    else {
        printf("false\n");
    }
}

// subset A B
void Subset (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int lenA=countElements(U,M1);
    int lenB=countElements(U,M2);
    int out=lenA<lenB;
        
    out=out && ifMnozinainUniverse(U,lenB,M2,U,lenA,M1);
    if (out) {
        printf("true\n"); 
    }
    else {
        printf("false\n");
    }
}

// eqals A B
void Eqals (char U[SIZE][SIZE][SIZE], int M1, int M2) {
    int lenA=countElements(U,M1);
    int lenB=countElements(U,M2);
    int out=lenA==lenB;
        
    out=out && ifMnozinainUniverse(U,lenB,M2,U,lenA,M1);
    if (out) {
        printf("true\n"); 
    }
    else {
        printf("false\n");
    }
}

int CekAB(char a[SIZE][SIZE][SIZE], int R)
{
    if(strcmp(a[0][0], "U")==0)
    {
        for(int i=0;a[0][i][0]!='\0';i++)
        {
            int on;
            for(int b=0;a[R][b][0]!='\0';b++)
            {
                if(strcmp(a[R][b], a[0][i])==0)
                {
                    on=1;
                }
                if(on!=1)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}


int main(int argc, char*argv[])
{
  FILE *fp = fopen(argv[1], "r");
  char u[SIZE][SIZE][SIZE]; //TO je massive. kde lezi vsichni radky s R C S U
    if (fp==NULL)
        return 0;

    if(errorHandling(argc, fp)==0)
        return 0;

    if(u!=NULL)
    {
        scan(u, fp);
        int LENR=0;
        for(int n=0;u[n][0][0]!='\0';n++)        //Operaci 1 lezi na 0 a pak dal
        {
            if(strcmp(u[n][0], "C")==0)
            {
                LENR=-1;
                for(int b=0;u[atoi(u[n][2])-1][b][0]!='\0';b++)
                {
                    LENR++;
                }
                if(strcmp(u[n][1], "reflexive")==0) // Ceka na operace reflexive
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        reflexive(u,atoi(u[n][2]));
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "symmetric")==0) // Ceka na operace symmetric
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        symmetric(u,atoi(u[n][2]),LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "antisymmetric")==0) // Ceka na operace antisymmetric
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        antisymmetric(u,atoi(u[n][2]),LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "transitive")==0) // Ceka na operace transitive
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        transitive(u,atoi(u[n][2]),LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "function")==0) // Ceka na operace function
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        function(u,atoi(u[n][2]), LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }   
                }
                if(strcmp(u[n][1], "domain")==0) // Ceka na operace domain
                {
                   if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        domain(u,atoi(u[n][2]), LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "codomain")==0) // Ceka na operace codomain
                {
                    if(u[atoi(u[n][2])-1][0][0]=='R')
                    {
                        codomain(u,atoi(u[n][2]), LENR);
                    }
                    else
                    {
                        printf("This is not Relacie\n");
                    }
                }
                if(strcmp(u[n][1], "injective")==0) // Ceka na operace injective
                {
                   if(CekAB(u,atoi(u[n][2]))==1)
                    {
                        if(u[atoi(u[n][2])-1][0][0]=='R')
                        {
                            injective(u,atoi(u[n][2]));
                        }
                        else
                        {
                            printf("This is not Relacie\n");
                        }
                    } 
                }
                if(strcmp(u[n][1], "surjective")==0) // Ceka na operace surjective
                {
                     if(CekAB(u,atoi(u[n][2]))==1)
                    {
                        if(u[atoi(u[n][2])-1][0][0]=='R')
                        {
                            surjective(u,atoi(u[n][2]));
                        }
                        else
                        {
                            printf("This is not Relacie\n");
                        }
                    }
                }
                if(strcmp(u[n][1], "bijective")==0) // Ceka na operace bijective
                {
                    if(CekAB(u,atoi(u[n][2]))==1)
                    {
                        if(u[atoi(u[n][2])-1][0][0]=='R')
                        {
                            bijective(u,atoi(u[n][2]));
                        }
                        else
                        {
                            printf("This is not Relacie\n");
                        }
                    }
                }
                if (strcmp(u[n][1],"complement")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Complement(u,atoi(u[n][2]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                       
                }
                if(strcmp(u[n][1], "union")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Union(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "empty")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Empty(u,atoi(u[n][2]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "intersect")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Intersect(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "subseteq")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Subseteq(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "subset")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Subset(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "eqals")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Eqals(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
                if(strcmp(u[n][1], "card")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Card(u,atoi(u[n][2]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    } 
                }
                if(strcmp(u[n][1], "minus")==0)
                {
                    if(u[atoi(u[n][2])-1][0][0]=='S')
                    {
                        Minus(u,atoi(u[n][2]),atoi(u[n][3]));
                    }
                    else
                    {
                        printf("This is not množina\n");
                    }
                }
            }
            else
            {
                for(int i=0;u[n][i][0]!='\0';i++)
                {
                    printf("%s ", u[n][i]);
                }
                printf("\n");
            }
        }

    }
    fclose(fp);
    return 0;
}