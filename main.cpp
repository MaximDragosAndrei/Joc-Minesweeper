#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <ctime>
#include <queue>

using namespace std;
FILE *fin;
FILE *fout;

unsigned char U[20][20];
unsigned char PC[20][20];
bool V[20][20];
int score;
unsigned int n,m,mines,tsteps,nrf;
int dl[]={1,0,-1,0,-1,1,1,-1};//S,V,N,E,NE,SE,SV,NV
int dc[]={0,-1,0,1,1,1,-1,-1};//S,V,N,E,NE,SE,SV,NV
char s[256];
char e[]="exit\0";
char f[]="flag\0";
char c[]="click\0";
char h[]="help\0";
char y[]="yes\0";
char nup[]="no\0";
char rmf[]="rf\0";
struct pct{
    unsigned int l,c;
};
struct player{
    int score;
    char s[256];
};
int conv(char s[])
{
    int i,lg,rez=0;
    lg=strlen(s);
    for(i=0;i<lg;i++){
        if(!(s[i]>='0'&&s[i]<='9'))
        return -1;
        rez=rez*10+s[i]-'0';
    }
    return rez;
}
bool operator <(const player A,const player B){
    if(A.score!=B.score)
        return (A.score<B.score);
    return (strcmp(A.s,B.s)>0);
}

bool operator >(const player A,const player B){
    return !(A<B);
}
bool operator !=(const player A,const player B){
    return ((A.score!=B.score)||(strcmp(A.s,B.s)!=0));
}
bool cmp(const player A,const player B){
    if(A.score!=B.score)
        return A.score>B.score;
    return (strcmp(A.s,B.s)<0);
}
vector <player>P,S;
player F[3];
void first3()
{
    int i;
    char nada[]="----------\0";
    strcpy(F[0].s,nada);
    F[0].score=-500;
    F[2]=F[1]=F[0];
    fin=fopen("score.txt","r");
    i=0;
    while(i<3&&!feof(fin))
    {
       fscanf(fin,"%s\n%d\n",F[i].s,&F[i].score);
       i++;
    }
    fclose(fin);
}
void init ()
{
    unsigned int i,j,nrm,k,nl,nc;
    tsteps=0;
    for(i=1;i<=n;i++)
        for(j=1;j<=m;j++)
        U[i][j]=220,PC[i][j]=176,V[i][j]=false;
    for(i=0;i<=n+1;i++)
    {
        U[i][0]=U[i][m+1]=220;
        V[i][0]=V[i][m+1]=true;
    }
    for(i=0;i<=m+1;i++)
    {
        U[0][i]=U[n+1][i]=220;
        V[0][i]=V[n+1][i]=true;
    }
    for(nrm=1;nrm<=mines;nrm++)
    {
        i=rand()%n+1;
        j=rand()%m+1;
        if(PC[i][j]=='X')
            nrm--;
        else{
            PC[i][j]='X';
            for(k=0;k<8;k++)
            {
                nl=i+dl[k];
                nc=j+dc[k];
                if(PC[nl][nc]==176)
                    PC[nl][nc]='0';
                if(PC[nl][nc]>='0'&&PC[nl][nc]<'8')
                    PC[nl][nc]++;
            }
        }
    }
}
void afis(unsigned char U[20][20])
{
    unsigned int i,j;
    cout<<"   ";
    j=1;
        for(;j<10;j++)
            cout<<j<<"  ";
    for(;j<m;j++)
        cout<<j<<' ';
    cout<<m<<'\n';
    i=1;
    for(;i<10;i++)
    {
        cout<<' '<<i<<' ';
        for(j=1;j<=m;j++)
            cout<<U[i][j]<<"  ";
        cout<<'\n';
    }
    for(;i<=n;i++)
    {
        cout<<i<<' ';
        for(j=1;j<=m;j++)
            cout<<U[i][j]<<"  ";
        cout<<'\n';
    }
    return;
}
void afis_h()
{
    cout<<"click- reveals an area on the map\nexit- close the game\nflag- put a flag on the map\nhelp-show this page\nrf- remove a flag on the map\n";
}
bool rf()
{
    bool ok=false;
    unsigned int l,c;
    if(!nrf)
        return false;
    do{
        cout<<"line of the flag to be remove:";cin.getline(s,255);cout<<'\n';
        l=conv(s);
        if(l>=1&&l<=n)
            ok=true;
        while(!ok){
            cout<<"plese chose a number between 1 and "<<n<<" \n";
            cout<<"line of the flag to be remove:";cin.getline(s,255);cout<<'\n';
            l=conv(s);
            if(l>=1&&l<=n)
            ok=true;
        }
        ok=false;
        cout<<"colomn of the flag to be remove:";cin.getline(s,255);cout<<'\n';
        c=conv(s);
        if(c>=1&&c<=m)
            ok=true;
        while(!ok){
            cout<<"plese chose a number between 1 and "<<m<<" \n";
            cout<<"colomn of the flag to be remove:";cin.getline(s,255);cout<<'\n';
            c=conv(s);
            if(c>=1&&c<=m)
            ok=true;
        }
        ok=false;
        if(U[l][c]=='F'){
            U[l][c]=220;
            ok=true;
        }
        else
            cout<<"Not a valid point\n";
    }while(!ok);
    nrf--;
    return true;
}
void pune_steag()
{
    bool ok=false;
    unsigned int l,c;
    do{
        cout<<"set the flag's line at:";cin.getline(s,255);cout<<'\n';
        l=conv(s);
        if(l>=1&&l<=n)
            ok=true;
        while(!ok){
            cout<<"plese chose a number between 1 and "<<n<<" \n";
            cout<<"set the flag's line at:";cin.getline(s,255);cout<<'\n';
            l=conv(s);
            if(l>=1&&l<=n)
            ok=true;
        }
        ok=false;
        cout<<"set the flag's colomn at:";cin.getline(s,255);cout<<'\n';
        c=conv(s);
        if(c>=1&&c<=m)
            ok=true;
        while(!ok){
            cout<<"plese chose a number between 1 and "<<m<<" \n";
            cout<<"set the flag's colomn at:";cin.getline(s,255);cout<<'\n';
            c=conv(s);
            if(c>=1&&c<=m)
            ok=true;
        }
        ok=false;
        if(U[l][c]==220){
            U[l][c]='F';
            ok=true;
        }
        else
            cout<<"Not a valid point\n";
    }while(!ok);
    nrf++;
}
bool lee(unsigned int l, unsigned int c)
{
    unsigned int k;
    unsigned int nl,nc,steps;
    queue<pct>C;
    pct E;
    if(PC[l][c]=='X'){
        score-=500;
        return false;
    }
    if(PC[l][c]>='1'&&PC[l][c]<='8')
    {
        V[l][c]=true;
        U[l][c]=PC[l][c];
        tsteps++;
        score++;
        return true;
    }
    E.l=l;E.c=c;
    C.push(E);
    steps=0;
    while(!C.empty())
    {
        E=C.front();
        C.pop();
        steps++;
        l=E.l;c=E.c;
        U[l][c]=PC[l][c];
        if(U[l][c]==176){
        for(k=0;k<8;k++)
        {
            nl=l+dl[k];
            nc=c+dc[k];
            if(!V[nl][nc]&&PC[nl][nc]!='X'&&U[nl][nc]!='F')
            {
                E.l=nl;
                E.c=nc;
                C.push(E);
                V[nl][nc]=true;
            }
        }
        }
    }
    tsteps+=steps;
    if(steps>=10&&steps<50){score+=steps<<1;return true;}
    if(steps>=50&&steps<100){score+=steps*3;return true;}
    if(steps>=100&&steps<110){score+=steps<<2;return true;}
    if(steps>=110&&steps<150){score+=steps*5;return true;}
    if(steps>=150&&steps<200){score+=steps*6;return true;}
    if(steps>=200){score+=steps<<3;return true;}
    score+=steps;
    return true;
}
void saveplayer()
{
    char s[256];
    player A;
    cout<<"Type your name : ";
    cin.get();
    cin.getline(s,255);
    cout<<"\n\n";
    if(!s[0]){
            s[0]=' ';s[1]='\0';
    }
    strcpy(A.s,s);
    A.score=score;
    S.push_back(A);
    if(A>F[0]){F[2]=F[1];F[1]=F[0];F[0]=A;}
    else{
        if(A>F[1]){F[2]=F[1];F[1]=A;}
        else{
            if(A>F[2])
                F[2]=A;
        }
    }
}
void interclasare()
{
    player A;
    unsigned int i,lg;
    lg=S.size();
    sort(S.begin(),S.end(),cmp);
    i=0;
    fin=fopen("score.txt","r");
    fscanf(fin,"%s\n%d\n",A.s,&A.score);
    while(i<lg&&(!feof(fin)))
    {
        if(S[i]>A){
            P.push_back(S[i]);
            i++;
        }
        else{
            P.push_back(A);
            fscanf(fin,"%s\n%d\n",A.s,&A.score);
        }
    }
    while(!feof(fin))
    {
        P.push_back(A);
        fscanf(fin,"%s\n%d\n",A.s,&A.score);
    }
    fclose(fin);
    if(i<lg)
    {
        while(A<S[i]&&i<lg)
        {
            P.push_back(S[i]);
            i++;
        }
    }
    if(i==lg)
    {
        P.push_back(A);
        return;
    }
    if(A>S[i])
        P.push_back(A);
    while(i<lg){
        P.push_back(S[i]);
        i++;
    }
    if(A<P[P.size()-1])
        P.push_back(A);
}
void rewrite()
{
    unsigned int i,lg;
    interclasare();
    lg=P.size();
    fout=fopen("score.txt","w");
    for(i=0;i<lg;i++)
        fprintf(fout,"%s\n%d\n",P[i].s,P[i].score);
    fclose(fout);
}
void show3()
{
    system("cls");
    cout<<"\n         1. "<<F[0].s<<": "<<F[0].score<<'\n';
    cout<<"\n         2. "<<F[1].s<<": "<<F[1].score<<'\n';
    cout<<"\n         3. "<<F[2].s<<": "<<F[2].score<<'\n';
    cout<<'\n';
}
bool click()
{
    bool ok;
    unsigned int l,c;
    do{
        ok=false;
        cout<<"set the line at:";
        cin>>l;
        cout<<'\n';
        if(l>=1&&l<=n)
            ok=true;
        while(!ok)
        {
            cout<<"plese chose a number between 1 and "<<n<<" \n";
            cout<<"set the line at:";cin>>l;cout<<'\n';
            if(l>=1&&l<=n)
            ok=true;
        }
        ok=false;
        cout<<"set the colomn at:";cin>>c;cout<<'\n';
        if(c>=1&&c<=m)
            ok=true;
        while(!ok)
        {
            cout<<"plese chose a number between 1 and "<<m<<" \n";
            cout<<"set the colomn at:";cin>>c;cout<<'\n';
            if(c>=1&&c<=m)
            ok=true;
        }
        ok=false;
        if(U[l][c]==220)
            ok=true;
    }while(!ok);
    ok=lee(l,c);
    if(!ok){
        system("cls");
        afis(PC);
        cout<<"      You LOSE!      \n      SCORE: "<<score<<"      \n";
        saveplayer();
        show3();
        return true;
    }
    if((tsteps+mines)==n*m){
        system("cls");
        afis(U);
        score+=500+mines*1000;
        cout<<"      You WIN!      \n      SCORE: "<<score<<"      \n";
        saveplayer();
        show3();
        return true;
    }
    return false;
}
void ent()
{
    do{
        cout<<"\nPress ENTER to quit!!!\n";
        cin.getline(s,255);
    }while(s[0]!='\0');
}
void show()
{
    bool ok;
    char s[12];
    unsigned int i,lg,ls;
    cout<<"Type yes if you want to see the ranking!!!\n";cin.getline(s,255);
    if(strcmp(s,y)){
        ent();
        return;
    }
    system("cls");
    lg=P.size();
    cout<<"The last one is: "<<lg;cout<<'\n';
    ok=false;
    cout<<"First position: ";cin.getline(s,255);cout<<'\n';
    i=conv(s);
    if(i<=lg)
        ok=true;
    while(!ok)
    {
        cout<<"First position (a number between 1 and "<<lg<<" ): ";cin.getline(s,255);cout<<'\n';
        i=conv(s);
        if(i<=lg)
        ok=true;
    }
    ok=false;
    cout<<"Last position: ";cin.getline(s,255);cout<<'\n';
    ls=conv(s);
    if(ls<=lg&&ls>=i)
        ok=true;
    while(!ok)
    {
        cout<<"Last position (a number between "<<i<<"  and "<<lg<<" ): ";cin.getline(s,255);cout<<'\n';
        ls=conv(s);
        if(ls<=lg&&ls>=i)
        ok=true;
    }
    system("cls");
    for(;i<=ls;i++)
        cout<<"     "<<i<<". "<<P[i-1].s<<": "<<P[i-1].score<<'\n';
    ent();
}
int main()
{
    char a;
    bool ok;
    unsigned int minm,maxm;
    srand (time(NULL));
    afis_h();
    first3();
    while(strcmp(s,e))
    {
        score=0;
        //cate linii?
        ok=false;
        cout<<"set number of lines at:";cin.getline(s,255);cout<<'\n';
        n=conv(s);
        if(n>=10&&n<=18)
            ok=true;
        while(!ok)
        {
            cout<<"please chose a number between 10 and 18\n";
            cout<<"set number of lines at:";cin.getline(s,255);cout<<'\n';
            n=conv(s);
            if(n>=10&&n<=18)
                ok=true;
        }
        ok=false;//cate coloane?
        cout<<"set number of colomns at:";cin.getline(s,255);cout<<'\n';
        m=conv(s);
        if(m>=10&&m<=18)
            ok=true;
        while(!ok)
        {
            cout<<"please chose a number between 10 and 18\n";
            cout<<"set number of colomns at:";cin.getline(s,255);cout<<'\n';
            m=conv(s);
            if(m>=10&&m<=18)
                ok=true;
        }
        //intre cat si cat pot sa aleg numarul de mine?
        minm=(n*m)>>2;
        maxm=minm<<1;
        ok=false;//cate mine?
        cout<<"set number of mines at:";cin.getline(s,255);cout<<'\n';
        mines=conv(s);
        if(mines>=minm&&mines<=maxm)
            ok=true;
        while(!ok)
        {
            cout<<"please chose a number between "<<minm<<" and "<<maxm<<" \n";
            cout<<"set number of mines at:";cin.getline(s,255);cout<<'\n';
            mines=conv(s);
            if(mines>=minm&&mines<=maxm)
                ok=true;
        }
        //let the game begin!
        init();
        ok=false;
        while(!ok)
        {
            system("cls");
            afis(U);
            cout<<"What to do? (Type help for help)\n";
            cin.getline(s,255);
            if(!strcmp(s,e))
                ok=true;
            else{
                if(!strcmp(s,h)){
                    afis_h();
                    cout<<"Type help again to quit the help menu\n";
                    s[0]='\0';
                    cin.getline(s,255);
                    while(strcmp(s,h)){
                        cout<<"Type help again to quit the help menu\n";cin.getline(s,255);
                    }
                }
                else{
                    if(!strcmp(s,f))
                        pune_steag();
                    else{
                        if(!strcmp(s,c))
                        ok=click();
                        else
                            if(!strcmp(s,rmf)){
                                ok=rf();
                                if(!ok){
                                    cout<<"There are no flags!!!!\nPress ENTER to continue";
                                    a='\0';
                                    while(a!='\n')
                                        cin.get(a);
                                }
                                ok=false;
                            }
                    }
                }
            }
        }
        if(strcmp(s,e))
        {
            ok=false;
            cout<<"Play again???\n";
            cin.getline(s,255);
            if(!strcmp(s,y))
                ok=true;
            else
            if(!strcmp(s,nup)){
                ok=true;
                strcpy(s,e);
            }
            while(!ok){
                cout<<"Play again"<<"???"<<"(Answer with yes or no)\n";
                cin.getline(s,255);
                if(!strcmp(s,y))
                    ok=true;
                else
                if(!strcmp(s,nup)){
                    ok=true;
                    strcpy(s,e);
                }
            }
        }
    }
    rewrite();
    show();
    return 0;
}
