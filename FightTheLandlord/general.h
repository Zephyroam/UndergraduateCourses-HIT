#pragma once
#include <bits/stdc++.h>
#include <thread>
#include <WINSOCK2.H>


using std::string;
using std::cout;
const int TYPENUMARRY=20;
const int TYPENUM=15;
const string CARDNAME[TYPENUMARRY]={"","3","4","5","6","7","8","9","10","J","Q","K","A","2","joker","JOKER"};

#define MSGSIZE 1024
#define ushort unsigned short

//3 4 5 6 7 8 9 10 J Q  K  A  2  joker JOKER
//1 2 3 4 5 6 7 8  9 10 11 12 13 14    15
//内部与外部数据转化
namespace Transform
{
    string In2Out(int x)//内部转外部
    {
        return CARDNAME[x];
    }
    int Out2In(string x)//外部转内部
    {
        if(x=="3")return 1;
        else if(x=="4")return 2;
        else if(x=="5")return 3;
        else if(x=="6")return 4;
        else if(x=="7")return 5;
        else if(x=="8")return 6;
        else if(x=="9")return 7;
        else if(x=="10")return 8;
        else if(x=="J")return 9;
        else if(x=="Q")return 10;
        else if(x=="K")return 11;
        else if(x=="A")return 12;
        else if(x=="2")return 13;
        else if(x=="joker")return 14;
        else if(x=="JOKER")return 15;
        else return 0;
    }
};
//IP表示
struct IPAd
{
    char IP[20];
    IPAd(){};
    IPAd(char *I)
    {
        for (int i = 0; i < 20;++i)
            IP[i] = I[i];
    }
};
//程序间数据包
struct DataBag
{
/*
0~14
15~29
30~44
45  0,1,2,3各自胜出 4出牌完毕 5可不出牌 6必须出牌 789
*/
    char Data[50];
    int Length;
    DataBag() { Length = 0; memset(Data,0,sizeof(Data));}
    DataBag(char *D,int L)
    {
        for (int i = 0; i < 50;++i)
            Data[i] = D[i];
        Length = L;
    }
};
//接收数据
class DataReceive
{
private:
    WSADATA wsaData;      
    SOCKET sListen;      
    SOCKET sClient;      
    SOCKADDR_IN local;      
    SOCKADDR_IN client;      
    char szMessage[MSGSIZE];
    ushort PORT;
    int iaddrSize = sizeof(SOCKADDR_IN);

public:
    DataReceive() {}
    IPAd GetIP()//获取IP
    {
        return IPAd(inet_ntoa(client.sin_addr));
    }
    ushort GetPort()//获取端口
    {
        return ntohs(client.sin_port);
    }
    DataBag ReceiveData()//接收数据
    {
        memset(szMessage,0,sizeof(szMessage));
        int ret = recv(sClient, szMessage, MSGSIZE, 0);
        szMessage[ret] = '\0';
        return DataBag(szMessage, ret);
    }
    void Init(ushort P)//建立连接
    {
        PORT = P;
        WSAStartup(0x0202, &wsaData);
        sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        local.sin_family = AF_INET;      
        local.sin_port = htons(PORT);
        local.sin_addr.s_addr = htonl(INADDR_ANY);      
        bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));   
        listen(sListen, 1);
        sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);
    }
};
//发送数据
class DataSend
{
private:
    WSADATA wsaData;//连接所用套节字
    SOCKET sClient;//保存远程服务器的地址信息
    SOCKADDR_IN server;//收发缓冲区
    ushort PORT;
    IPAd IPAdd;

public:
    DataSend(){}
    int Send(char *Mess)//发送数据
    {
        return send(sClient, Mess, strlen(Mess), 0);
    }
    void Init(IPAd _IP,ushort _P)//建立连接
    {
        IPAdd=_IP;
        PORT = _P;
        WSAStartup(0x0202, &wsaData);
        sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        memset(&server, 0, sizeof(SOCKADDR_IN));
        server.sin_family = PF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = inet_addr(IPAdd.IP);
        connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN));
    }
    void Close()//关闭连接
    {
        closesocket(sClient);
        WSACleanup();
    }
};
//牌类型
class TypeBound
{
public:
    int One,Two,Type;
    TypeBound(){One=Two=Type=0;}
    TypeBound(int Ty,int O,int Tw){One=O;Two=Tw;Type=Ty;}
};
//打出的牌
class Out
{
private:
    int Card[TYPENUMARRY];
    //int player;
public:
    TypeBound TypeBounds;
    Out(){memset(Card,0,sizeof(Card));}
    inline int GetNum(int x)//获取牌类型为x的牌的数量
    {
        return Card[x];
    }
    inline int GetSum()//获取牌总数
    {
        int Res=0;
        for(int i=1;i<=TYPENUM;++i)
            Res+=Card[i];
        return Res;
    }
    inline void GetCard(int x)//加入牌
    {
        Card[x]++;
        if(Card[x]>4)
        {
            puts("There is something WRONG!The num of a kind of card can NOT more than 4!");
            exit(1);
        }
    }
    bool operator ==(const Out &a)//重定向“==”操作
    {
        for(int i=1;i<=TYPENUM;++i)
            if(Card[i]!=a.Card[i])return 0;
        if(TypeBounds.Type!=a.TypeBounds.Type)return 0;
        return 1;
    }
    inline void SetType(int x)//设置牌的类型
    {
        TypeBounds.Type=x;
    }
    inline int GetType()//获取牌的类型
    {
        return TypeBounds.Type;
    }
    inline void DelCard(int x)
    {
        Card[x]--;
        if(Card[x]<0)
        {
            puts("There is something WRONG!The num of a kind of card can NOT less than 0!");
            exit(1);
        }
    }
};

typedef Out In;
//手里的牌
class CardInHand
{
private:
    int Card[TYPENUMARRY],Robot;
    string Name;
public:
    CardInHand(){memset(Card,0,sizeof(Card));}
    inline string GetName()//获取玩家
    {
        return Name;
    }
    inline int GetSum()
    {
        int res = 0;
        for (int i = 1; i <= TYPENUM;++i)
            res += Card[i];
        return res;
    }
    inline void SetName(string NewName)
    {
        Name=NewName;
    }
    inline int GetNum(int x)
    {
        return Card[x];
    }
    inline void GetCard(int x)
    {
        Card[x]++;
        if(Card[x]>4)
        {
            puts("There is something WRONG!The num of a kind of card can NOT more than 4!");
            exit(1);
        }
    }
    inline void DelCard(int x)
    {
        Card[x]--;
        if(Card[x]<0)
        {
            puts("There is something WRONG!The num of a kind of card can NOT less than 0!");
            exit(1);
        }
    }
    inline void OutputCard()
    {
        for(int i=1;i<=TYPENUM;++i)
            for(int j=1;j<=Card[i];++j)
                std::cout<<Transform::In2Out(i)<<" ";
        std::cout<<'\n';
    }
    inline bool end()
    {
        for(int i=1;i<=TYPENUM;++i)
            if(Card[i])return 0;
        return 1;
    }
};
//数据包生成
DataBag Card2DataBag(Out Now1,Out Now2,CardInHand Now3,int End,int Clear)
{
    DataBag Res;
    for(int i=0;i<=14;++i)
        Res.Data[i]=Now1.GetNum(i+1)+1;
    for(int i=15;i<=29;++i)
        Res.Data[i]=Now2.GetNum(i-14)+1;
    for(int i=30;i<=44;++i)
        Res.Data[i]=Now3.GetNum(i-29)+1;
    Res.Data[45]=End;
    Res.Data[46]=Clear;
    return Res;
}