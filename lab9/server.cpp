 // server.cpp

#include <winsock2.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

struct Share
{
    long long x;
    long long y;
};

long long mod(long long a,long long p)
{
    long long r=a%p;
    if(r<0) r+=p;
    return r;
}

long long modPow(long long a,long long d,long long n)
{
    long long res=1;
    while(d)
    {
        if(d&1) res=(res*a)%n;
        a=(a*a)%n;
        d>>=1;
    }
    return res;
}

bool millerTest(long long d,long long n)
{
    long long a = 2 + rand() % (n-4);
    long long x = modPow(a,d,n);

    if(x==1 || x==n-1)
        return true;

    while(d != n-1)
    {
        x=(x*x)%n;
        d*=2;

        if(x==1) return false;
        if(x==n-1) return true;
    }

    return false;
}

bool isPrime(long long n)
{
    if(n<=1 || n==4) return false;
    if(n<=3) return true;

    long long d=n-1;
    while(d%2==0)
        d/=2;

    for(int i=0;i<5;i++)
        if(!millerTest(d,n))
            return false;

    return true;
}

long long generatePrime()
{
    while(true)
    {
        long long num = rand()%10000 + 5000;
        if(isPrime(num))
            return num;
    }
}

long long evalPoly(vector<long long>& coeff,long long x,long long p)
{
    long long res=0;
    long long pow=1;

    for(auto c:coeff)
    {
        res = mod(res + c*pow,p);
        pow = mod(pow*x,p);
    }

    return res;
}

int main()
{
    srand(time(0));

    long long prime = generatePrime();

    long long secret = rand()%1000;

    cout<<"Prime: "<<prime<<endl;
    cout<<"Secret: "<<secret<<endl;

    vector<long long> coeff(3);
    coeff[0]=secret;
    coeff[1]=rand()%prime;
    coeff[2]=rand()%prime;

    vector<Share> shares;

    for(int i=1;i<=5;i++)
    {
        Share s;
        s.x=i;
        s.y=evalPoly(coeff,i,prime);
        shares.push_back(s);

        cout<<"Share "<<i<<" : "<<s.x<<" "<<s.y<<endl;
    }

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2),&wsa);

    SOCKET server = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(9000);
    addr.sin_addr.s_addr=INADDR_ANY;

    bind(server,(sockaddr*)&addr,sizeof(addr));
    listen(server,5);

    cout<<"Waiting for clients...\n";

    for(int i=0;i<5;i++)
    {
        SOCKET client=accept(server,NULL,NULL);

        send(client,(char*)&shares[i],sizeof(Share),0);
        send(client,(char*)&prime,sizeof(long long),0);

        cout<<"Sent share "<<i+1<<endl;

        closesocket(client);
    }

    cout<<"Receiving shares back...\n";

    vector<Share> returned;

    for(int i=0;i<3;i++)
    {
        SOCKET client=accept(server,NULL,NULL);

        Share s;
        recv(client,(char*)&s,sizeof(Share),0);

        returned.push_back(s);

        cout<<"Received share "<<s.x<<" "<<s.y<<endl;

        closesocket(client);
    }

    cout<<"Reconstruction phase\n";

    long long secretRec=0;

    for(int i=0;i<returned.size();i++)
    {
        long long xi=returned[i].x;
        long long yi=returned[i].y;

        long long num=1;
        long long den=1;

        for(int j=0;j<returned.size();j++)
        {
            if(i!=j)
            {
                long long xj=returned[j].x;
                num = mod(num * (-xj),prime);
                den = mod(den * (xi-xj),prime);
            }
        }

        long long inv = modPow(den,prime-2,prime);

        long long term = mod(yi * num,prime);
        term = mod(term * inv,prime);

        secretRec = mod(secretRec + term,prime);
    }

    cout<<"Recovered Secret: "<<secretRec<<endl;

    closesocket(server);
    WSACleanup();
}