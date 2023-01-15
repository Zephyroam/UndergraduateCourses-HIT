#include <bits/stdc++.h>
#include "general.h"
#include "judger.h"
#include "graph.h"
#include <conio.h>
using std::thread;
using std::string;
using std::cout;
using std::cin;
#define dbg(x) cout<<#x<<" = "<<x<<'\n';
const int CARDNUM=54;
DataBag ReceivedData;
CardInHand Lord,Farmer1,Farmer2;
Out Blank,History[2];
DataReceive Rec1;
DataReceive Rec2;
DataReceive Rec3;
DataSend Sen1;
DataSend Sen2;
DataSend Sen3;
//建立连接
void Init1()
{
    Rec1.Init(30001);
    printf("Connection from Lord\n");
}
void Init2()
{
    Rec2.Init(30002);
    printf("Connection from Farmer1\n");
}
void Init3()
{
    Rec3.Init(30003);
    printf("Connection from Farmer2\n");
}
//开始界面
void StartPage()
{
    const int MenuNum=2;
    system("mode con cols=150 lines=30");
    int NowPos=1;
    while(1)
    {
        system("cls");
        char Tmp;
        printf(
        "\n\n\n\n\n\n\n          ___________.__       .__     __    ___________.__             .____                       .___.__                   .___\n"
        "          \\_   _____/|__| ____ |  |___/  |_  \\__    ___/|  |__   ____   |    |   _____    ____    __| _/|  |   ___________  __| _/\n"
        "           |    __)  |  |/ ___\\|  |  \\   __\\   |    |   |  |  \\_/ __ \\  |    |   \\__  \\  /    \\  / __ | |  |  /  _ \\_  __ \\/ __ | \n"
        "           |     \\   |  / /_/  >   Y  \\  |     |    |   |   Y  \\  ___/  |    |___ / __ \\|   |  \\/ /_/ | |  |_(  <_> )  | \\/ /_/ | \n"
        "           \\___  /   |__\\___  /|___|  /__|     |____|   |___|  /\\___  > |_______ (____  /___|  /\\____ | |____/\\____/|__|  \\____ | \n"
        "               \\/      /_____/      \\/                       \\/     \\/          \\/    \\/     \\/      \\/                        \\/ \n");
        for (int i = 1; i <= MenuNum;++i)
        {
            for (int j = 1; j <= 60;++j)
                printf(" ");
            if(NowPos==i)
                printf("*");
            else
                printf(" ");
            for (int j = 1; j <= 3;++j)
                printf(" ");
            if(i == 1)
            {
                printf("1.三人对战\n");
            }
            else if(i == 2)
            {
                printf("2.退出\n");
            }
        }
        while(1)
        {
            Tmp=getch();
            if(Tmp=='S'||Tmp=='s')
            {
                if(NowPos==MenuNum)
                    continue;
                NowPos++;
                break;
                    
            }
            else if(Tmp=='W'||Tmp=='w')
            {
                if(NowPos==1)
                    continue;
                NowPos--;
                break;
            }
            else if(Tmp==13)
            {
                if(NowPos==MenuNum)
                    exit(0);
                return;
            }
            else
                continue;
        }
        
    }
}
//随机发牌并且初始化必要变量
void Init()
{
    Blank.SetType(1);
    History[0] = History[1] = Blank;
    /*
    cout<<"Hello!This is Fight The Landlord.\n";
    cout<<"Please use \"3,4,5,6,7,8,9,10,J,Q,K,A,2,joker,JOKER\" to play cards\n";
    cout<<"If you want to pass, just press enter without any input.\n\n";
    cout<<"Let's BEGIN in 10 seconds:\n\n";
    */
    //_sleep(10000);
    Farmer1.SetName("Farmer1");
    Farmer2.SetName("Farmer2");
    Lord.SetName("Lord");

    int Cards[50],Cardsp=0;

    srand(time(0));
    for(int i=1;i<=TYPENUM-2;++i)
        for(int j=1;j<=4;++j)
            Cards[++Cardsp]=i;
    Cards[++Cardsp]=TYPENUM-1;
    Cards[++Cardsp]=TYPENUM;
    std::random_shuffle(Cards+1,Cards+Cardsp+1);
    for(int i=1;i<=CARDNUM-3;i+=3)
    {
        Lord.GetCard(Cards[i]);
        Farmer1.GetCard(Cards[i+1]);
        Farmer2.GetCard(Cards[i+2]);
    }
    Lord.GetCard(Cards[CARDNUM]);
    Lord.GetCard(Cards[CARDNUM-1]);
    Lord.GetCard(Cards[CARDNUM-2]);
}
//端口监听
void Listen1()
{
    while(1)
        ReceivedData=Rec1.ReceiveData();
}
void Listen2()
{
    while(1)
        ReceivedData=Rec2.ReceiveData();
}
void Listen3()
{
    while(1)
        ReceivedData=Rec3.ReceiveData();
}
//初始化连接
void InitConnection()
{
    for (int i = 1; i <= 7;++i)
        printf(" ");
    puts("Waiting for connection...");
    thread t1(Init1);
    thread t2(Init2);
    thread t3(Init3);
    t1.join();
    t2.join();
    t3.join();
    Sen1.Init(Rec1.GetIP(), 20001);
    Sen2.Init(Rec2.GetIP(), 20002);
    Sen3.Init(Rec3.GetIP(), 20003);
    thread l1(Listen1);
    thread l2(Listen2);
    thread l3(Listen3);
    l1.detach();
    l2.detach();
    l3.detach();
    printf("Connection succeed!\nThe game will begin in 5 secends...\n");
    Sleep(5000);
    
}
/*bool EmptyJudge(DataBag Now)
{
    for(int i=0;i<=44;++i)
        if(Now.Data[i]!=0)return 0;
    return 1;
}*/
//游戏进程
void Play()
{
    int PlayerType=-1;
    while(!Lord.end()&&!Farmer1.end()&&!Farmer2.end())
    {
        //puts("!!");
        // system("cls");
        //OutputCardLeft();
        PlayerType++;PlayerType%=3;

        Out Temp=Blank;
        if(PlayerType==0)
        {
            //puts("@@");
            //printf("%s\n",Card2DataBag(History[1],Blank,Lord,PlayerType+7).Data);
            Sen2.Send(Card2DataBag(History[1],Blank,Farmer1,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen3.Send(Card2DataBag(History[1],Blank,Farmer2,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen1.Send(Card2DataBag(History[0],History[1],Lord,5+(History[0]==Blank&&History[1]==Blank),History[0]==Blank&&History[1]==Blank).Data);
         /*   for(int i=0;i<=45;++i)
                cout<<(int)Card2DataBag(History[1],Blank,Lord,PlayerType+7).Data[45]-1+'0'<<'\n';
            getchar();*/
        }
        else if(PlayerType==1)
        {
            Sen1.Send(Card2DataBag(History[1],Blank,Lord,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen3.Send(Card2DataBag(History[1],Blank,Farmer2,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen2.Send(Card2DataBag(History[0],History[1],Farmer1,5+(History[0]==Blank&&History[1]==Blank),History[0]==Blank&&History[1]==Blank).Data);
        }
        else if(PlayerType==2)
        {
            Sen1.Send(Card2DataBag(History[1],Blank,Lord,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen2.Send(Card2DataBag(History[1],Blank,Farmer1,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            Sen3.Send(Card2DataBag(History[0],History[1],Farmer2,5+(History[0]==Blank&&History[1]==Blank),History[0]==Blank&&History[1]==Blank).Data);
        }
        while(1)
        {
            if(ReceivedData.Data[45]==4)break;
            Sleep(2000);
        }
        ReceivedData.Data[45]=0;
        for(int i=0;i<=14;++i)
            for(int j=1;j<ReceivedData.Data[i];++j)
            {
                if(PlayerType==0)
                    Lord.DelCard(i+1);
                else if(PlayerType==1)
                    Farmer1.DelCard(i+1);
                else if(PlayerType==2)
                    Farmer2.DelCard(i+1);
                Temp.GetCard(i+1);
            }
        if(PlayerType==0)
            Sen1.Send(Card2DataBag(History[1],Blank,Lord,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
        else if(PlayerType==1)
            Sen2.Send(Card2DataBag(History[1],Blank,Farmer1,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
        else if(PlayerType==2)
            Sen3.Send(Card2DataBag(History[1],Blank,Farmer2,PlayerType+7,History[0]==Blank&&History[1]==Blank).Data);
            
        History[0] = History[1];
        History[1] = Temp;
        
    }
    int Winner;
    //保证先判参数4
    if(Lord.end())
        Winner = 1;
    else if(Farmer1.end())
        Winner = 2;
    else
        Winner = 3;
    Sen1.Send(Card2DataBag(Blank,Blank,Lord,Winner,0).Data);
    Sen2.Send(Card2DataBag(Blank,Blank,Lord,Winner,0).Data);
    Sen3.Send(Card2DataBag(Blank,Blank,Lord,Winner,0).Data);
 
}
//关闭连接
void Close()
{
    Sen1.Close();
    Sen2.Close();
    Sen3.Close();
}
int main()
{
//设置为UTF-8编码
    system("chcp 65001");
//输出开始界面
    StartPage();
//初始化连接
    InitConnection();
//随机发牌并且初始化必要变量
    Init();
//进行游戏
    Play();
//关闭连接
    Close();
    return 0;
}
//加音乐