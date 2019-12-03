#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//global elements
int a = 2, b = 2;
int n = 433;
int order = 19;

//global elements



//prototype

//----->modulus functions
int negmod(int m);
int modInverse(int n, int m);
int gcdExtended(int n, int b, int *x, int *y);

//----->point operations
int pointAddition();
int pointDoubling();

//------->key generation
void KeyGen(int m);

//Encryption and Decryption
int Encrypt(int m,int o);
int Decrypt(int m,int o);

//prottype


struct point
{
    int x;
    int y;
};

struct point G;
struct point Q;

int main()
{
    clock_t begin = clock();
    srand(time(0));

    int an;
    label1:
    //alice key generation
    an = rand() % 100;
    KeyGen(an);
    struct point Ap = Q;
    

    //bob key generation
    int bn;

    label:
    bn = rand() % 100;
    KeyGen(bn);

    struct point Bp = Q;
    if(Bp.x == Ap.x && Bp.y == Ap.y)
        goto label;
    
    if(Ap.x == 0 && Ap.y == 0 && Bp.x == 0 && Bp.y == 0)
        goto label1;

    printf("Private Key of Alice : %d\nPublic Key of Alice : %d ,%d\n",an,Ap.x,Ap.y);
    printf("\nPrivate Key of Bob : %d\nPublic Key of Bob : %d ,%d\n",bn,Bp.x,Bp.y);
   
    //calculating shared secret
    KeyGen(an*bn);
    struct point Skey = Q;
    printf("\nThe shared key calcultated by Alice is : %d %d\n", Skey.x,Skey.y);
    KeyGen(bn*an);
    Skey = Q;
    printf("The shared key calcultated by Bob is : %d %d\n", Skey.x,Skey.y);



    //The algorithm is complete. Now we will send a message

    char string[] = "Sphinx of Black Quartz, judge my vow!";                        // The message
    int size = sizeof(string)/sizeof(string[0]);
    int num[size];

    fputs("The message to be transmitted is : ",stdout);
    for(int i=0; i<size ;i++)
    {
        printf("%c",string[i]);                                     // print the original string
    }

    for(int i=0; i<size ;i++)
    {
        num[i] = string[i];                                         // Set the string array to a number array
    }
    
    for(int i=0; i<size ;i++)
    {
        num[i] = Encrypt(Skey.x,num[i]);                               // Set the cipher number into  the num array
    }

    fputs("\nThe cipher text generated is : ",stdout);
    for(int i=0; i<size ;i++)
    {
        printf("%d",num[i]);                                        // print the cipher generated
    }

    for(int i=0; i<size ;i++)
    {
        num[i] = Decrypt(Skey.x,num[i]);                               // decrypting the message
    }

    fputs("\nThe regenerated message is : ",stdout);

    for(int i=0; i<size ;i++)
    {
        printf("%c",num[i]);                                        // print the generated message
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n Time needed for execution : %lf",time_spent);


}

int pointAddition()
{
    int s;
    int temp1,temp2;

    if(Q.x == G.x && Q.y == G.y)
    {
        pointDoubling();
        return 0;
    }

    if(Q.x == 0 && Q.y == 0)
    {
        Q = G;
        return 0;
    }        

    temp1 = G.y - Q.y;
    if(temp1 < 0)
        temp1 = negmod(temp1);
    else 
        temp1 = temp1 % n;

    temp2 = G.x - Q.x;
    if(temp2 < 0)
        temp2 = negmod(temp2);
    else 
        temp2 = temp2 % n;

    if(temp2 == 0)
    {
        Q.x = 0;
        Q.y = 0;
        return 0;
    }

    temp2 = modInverse(temp2,n);

    temp1 = (temp1 * temp2) % n;

    Q.x = (temp1 * temp1 - (G.x + Q.x)) % n;
    Q.y = (temp1 * (G.x - Q.x) - G.y) % n;

    if(Q.x < 0)
        Q.x = negmod(Q.x);
    if(Q.y < 0)
        Q.y = negmod(Q.y);
    return 1;
}

int pointDoubling()
{
    struct point ptemp = Q;
    int temp1,temp2;   

    if(Q.x == 0 && Q.y == 0)
    {
        Q = G;
        return 0;
    }        

    temp1 = 3 * ptemp.x * ptemp.x + a;
    if(temp1 < 0)
        temp1 = negmod(temp1);
    else
        temp1 = temp1 % n;
    

    temp2 = 2 * ptemp.y; 
    if(temp2 < 0)
        temp2 = negmod(temp2);
    else 
        temp2 = temp2 % n;

    if(temp2 == 0)
    {                                //reached the end case
        Q.x = 0;
        Q.y = 0;
        return 0;
    }

    temp2 = modInverse(temp2, n); 

    temp1 = (temp1*temp2) % n;

    Q.x = (temp1*temp1 - 2*ptemp.x) % n;
    Q.y = (temp1*(ptemp.x - Q.x) - ptemp.y) % n;
    
    if(Q.x < 0)
        Q.x = negmod(Q.x);
    if(Q.y < 0)
        Q.y = negmod(Q.y);

    return 1;   
}

int modInverse(int n, int m) 
{ 
    int x, y; 
    int g = gcdExtended(n, m, &x, &y); 
    if (g != 1) 
        printf("Error!\n");
    else
    { 
        int res = (x%m + m) % m;  
        return res;
    } 
} 
  
int gcdExtended(int n, int b, int *x, int *y) 
{ 
    
    if (n == 0) 
    { 
        *x = 0, *y = 1; 
        return b; 
    } 
  
    int x1, y1; 
    int gcd = gcdExtended(b%n, n, &x1, &y1); 
  
    *x = y1 - (b/n) * x1; 
    *y = x1; 
  
    return gcd; 
}

int negmod(int m)
{
    int k = 0;
    while(k*n > m)
    {
        k--;
    }
    return m - (k)*n;
}

void KeyGen(int m)
{
    int c = 0;
    int d;

    while(2*c < m)
    {
        c++;
    }
    d = m - c;

    G.x = 5;
    G.y = 1;

    Q.x = 5;
    Q.y = 1;

    while(c)
    {
        pointDoubling();
        c--;
    }

    while(d)
    {
        pointAddition();
        d--;
    }
}

int Encrypt(int m, int o)
{
    return m*o;
}

int Decrypt(int m, int o)
{
    return o/m;
}