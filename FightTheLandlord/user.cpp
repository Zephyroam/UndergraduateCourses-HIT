#include <bits/stdc++.h>
#include "general.h"
#include "judger.h"
#include "graph.h"
#include <conio.h>
#include <windows.h>
using std::string;
using std::thread;
using std::cout;
using std::cin;
#define dbg(x) cout<<#x<<" = "<<x<<'\n';

const int CARDNUM=54;
ushort ItPort;
CardInHand Player,BlankPlayer;
Out History[2],Blank;
//角色及像素画初始化
void Init()
{
    GetGraph();
    if(ItPort%10==1)Player.SetName("Lord");
    else if(ItPort%10==2)Player.SetName("Farmer1");
    else if(ItPort%10==3)Player.SetName("Farmer2");
    
}
//开始界面
void StartPage()
{
    system("cls");
    printf(
    "\n\n\n\n\n\n\n          ___________.__       .__     __    ___________.__             .____                       .___.__                   .___\n"
    "          \\_   _____/|__| ____ |  |___/  |_  \\__    ___/|  |__   ____   |    |   _____    ____    __| _/|  |   ___________  __| _/\n"
    "           |    __)  |  |/ ___\\|  |  \\   __\\   |    |   |  |  \\_/ __ \\  |    |   \\__  \\  /    \\  / __ | |  |  /  _ \\_  __ \\/ __ | \n"
    "           |     \\   |  / /_/  >   Y  \\  |     |    |   |   Y  \\  ___/  |    |___ / __ \\|   |  \\/ /_/ | |  |_(  <_> )  | \\/ /_/ | \n"
    "           \\___  /   |__\\___  /|___|  /__|     |____|   |___|  /\\___  > |_______ (____  /___|  /\\____ | |____/\\____/|__|  \\____ | \n"
    "               \\/      /_____/      \\/                       \\/     \\/          \\/    \\/     \\/      \\/                        \\/ \n");
}
//输入端口
ushort GetPort()
{
    ushort Res;
    while(1)
    {
        StartPage();
        printf("\n\n\n\n\n\n\n                            ");
        puts("Please input the server's PORT:(30001~30003)");
        puts("(Lord:30001,Farmer1:30002,Farmer2.30003)");
        cin >> Res;
        if(Res/10==3000&&Res%10<=3&&Res%10>=1)
            break;
        system("cls");
        //puts("Invaild PORT!");
        //PORT = 2000*;
    }
    getchar();
    return Res;
}
DataReceive Rec;
DataSend Sen;
DataBag ReceivedData;
//端口监听
void Listen()
{
    while(1)
        ReceivedData=Rec.ReceiveData();
}
//连接初始化
void ConnectionInit()
{
    char ipadd[16];
    StartPage();
    printf("\n\n\n\n\n\n\n                            ");
    puts("Please input the IP Address");
    scanf("%s", ipadd);
    ItPort=GetPort();
    Sen.Init(IPAd(ipadd), ItPort);
    printf("Connection succeed!\nThe game will begin soon...\n");
    Rec.Init(ItPort - 10000);
    thread l1(Listen);
    l1.detach();
    
    //Sleep(5000);
}
//关闭连接
void Close()
{
    Sen.Close();
}
//Out类输出像素画
void OutGraphOut(Out Now)
{
    string Output[6];
    for (int i = 1; i <= 5;++i)
        Output[i] = "";
    for (int i = 1; i <= TYPENUM; ++i)
        for (int j = 1; j <= Now.GetNum(i); ++j)
        {
            for (int k = 1; k <= 5;++k)
                Output[k] += gra[i].Graph[k],Output[k]+=" ";
        }
    for (int i = 1; i <= 5;++i)
        cout << Output[i] << "\n";
}
//CardInHand类输出像素画
void OutGraphHand(CardInHand Now)
{
    string Output[6];
    for (int i = 1; i <= 5;++i)
        Output[i] = "";
    for (int i = 1; i <= TYPENUM; ++i)
        for (int j = 1; j <= Now.GetNum(i); ++j)
        {
            for (int k = 1; k <= 5;++k)
                Output[k] += gra[i].Graph[k],Output[k]+=" ";
        }
    for (int i = 1; i <= 5;++i)
        cout << Output[i] << "\n";
}
//玩家出牌
void PlayerTurn(bool MustOut)
{
    In Temp;
    int Id[25], Idp = 0;
    int Active[25] = {0}, ActiveNow[25] = {0};
    int NowPos = 1;
    bool Legal = 0;
    for(int i=1;i<=TYPENUM;++i)
        for(int j=1;j<=Player.GetNum(i);++j)
            Id[++Idp] = i;
    while(1)
    {
        NowPos = 1;
        while(1)
        {
            system("cls");
            char Tmp;
            if(History[0].GetSum())
                OutGraphOut(History[0]);
            else puts("PASS");
            puts("");
            if(History[1].GetSum())
                OutGraphOut(History[1]);
            else puts("PASS");
            puts("");
            //cout<<Player.GetName()<<"'s turn:\n";
            OutGraphHand(Player);
            cout<<"\n";
            for (int i = 1; i <= Idp;++i)
                if(ActiveNow[i]&&i!=NowPos)
                    printf("_____ ");
                else if(ActiveNow[i]&&i==NowPos)
                    printf("__*__ ");
                else if(!ActiveNow[i]&&i!=NowPos)
                    printf("      ");
                else
                    printf("  *   ");
            printf("\n");
            while(1)
            {
                Tmp=getch();
                if(Tmp=='D'||Tmp=='d')
                {
                    if(NowPos==Player.GetSum())
                        continue;
                    NowPos++;
                    break;
                        
                }
                else if(Tmp=='A'||Tmp=='a')
                {
                    if(NowPos==1)
                        continue;
                    NowPos--;
                    break;
                }
                else if(Tmp=='w'||Tmp=='W')
                {
                    if(ActiveNow[NowPos])
                        continue;
                    Active[Id[NowPos]]++;
                    ActiveNow[NowPos] = 1;
                    break;
                }
                else if(Tmp=='s'||Tmp=='S')
                {
                    if(!ActiveNow[NowPos])
                        continue;
                    Active[Id[NowPos]]--;
                    ActiveNow[NowPos] = 0;
                    break;
                }
                else if(Tmp==13)
                {
                    Temp=Blank;
                    Legal = 1;
                    for (int i = 1; i <= TYPENUM;++i)
                        for (int j = 1; j <= Active[i];++j)
                            Temp.GetCard(i);
                    Temp.TypeBounds=GetType::Get(Temp);
                    if(!Temp.GetSum()&&MustOut)//处理输入为空的情况
                        Legal=0;
                    for(int i=1;i<=TYPENUM-2&&Legal;++i)if(Temp.GetNum(i)>4)Legal=0;
                    if(Temp.GetNum(TYPENUM)>1||Temp.GetNum(TYPENUM-1)>1)Legal=0;
                    for(int i=1;i<=TYPENUM&&Legal;++i)if(Temp.GetNum(i)>Player.GetNum(i))Legal=0;
                    History[1].TypeBounds=GetType::Get(History[1]);
                    if(Legal&&Temp.GetSum()&&!ValidJudger(History[1],Temp))Legal=0;
                    //cout<<History[1].TypeBounds.Type<<" "<<Temp.TypeBounds.Type<<'\n';
                    ////////判大小
                    if(!Legal)
                    {
                        for (int i = 1; i <= 20;++i)
                            Active[i] = ActiveNow[i] = 0;
                        Temp = Blank;
                    }
                    else
                    {
                        Sen.Send(Card2DataBag(Temp,Blank,Player,4,0).Data);
                        for(int i=1;i<=TYPENUM&&Legal;++i)//从手牌中删除并输出
                            for(int j=1;j<=Temp.GetNum(i);++j)
                                Player.DelCard(i);
                        //History[0]=History[1];
                        //History[1]=Temp;
                        return;
                    }

                    break;
                }
                else
                    continue;
            }
        }
    }
}
//游戏进程
void Play()
{
    while(1)
    {
/*
1,2,3各自胜出 4出牌完毕 5可不出牌 6必须出牌 789其他玩家出的牌
*/
        Sleep(1000);
        if(ReceivedData.Data[45]==0)continue;
/*        for(int i=0;i<=45;++i)
            printf("%c ",ReceivedData.Data[i]-1+'0');
        getchar();
*/
        //cout<<ReceivedData.Data[45]<<"$$$\n";
        if(ReceivedData.Data[45]<=3)
        {
            if(ItPort%10==1&&ItPort%10==ReceivedData.Data[45])
                puts("You Win!");
            else if(ItPort%10==ReceivedData.Data[45]||ItPort%10==(ReceivedData.Data[45]^1))
                puts("You Win!");
            else puts("You Lose!");
            break;
        }
        else if(ReceivedData.Data[45]<=6)
        {
            History[0]=History[1];
            History[1]=Blank;
            for(int i=15;i<=29;++i)
                for(int j=1;j<(int)ReceivedData.Data[i];++j)
                    History[1].GetCard(i-14);
            if(ReceivedData.Data[46])History[0]=History[1]=Blank;
            Player=BlankPlayer;
            for(int i=30;i<=44;++i)
                for(int j=1;j<(int)ReceivedData.Data[i];++j)
                    Player.GetCard(i-29);
            puts("Your turn:");
            PlayerTurn(ReceivedData.Data[45]-5);
        }
        else if(ReceivedData.Data[45]<=9)
        {
            Player=BlankPlayer;
            for(int i=30;i<=44;++i)
                for(int j=1;j<(int)ReceivedData.Data[i];++j)
                    Player.GetCard(i-29);
            History[0]=History[1];
            History[1]=Blank;
            for(int i=0;i<=14;++i)
                for(int j=1;j<(int)ReceivedData.Data[i];++j)
                    History[1].GetCard(i+1);
            if(ReceivedData.Data[46])History[0]=History[1]=Blank;
            system("cls");
            if(History[0].GetSum())
                OutGraphOut(History[0]);
            else puts("PASS");
            puts("");
            if(History[1].GetSum())
                OutGraphOut(History[1]);
            else puts("PASS");
            puts("");
            //cout<<Player.GetName()<<"'s turn:\n";
            OutGraphHand(Player);
            cout<<"\n";
        }
        //cout<<"&&&&&\n";
        ReceivedData.Data[45]=0;
    }
}
//声音播放
void Sound()
{
    PlaySound("welcome.wav",NULL,SND_LOOP);
    PlaySound("exiting.wav",NULL,SND_LOOP);
}
int main()
{
/*
g++ server.cpp -o server.exe -static -lws2_32
g++ user.cpp -o user.exe -static -lws2_32 -lwinmm
*/
//设置为UTF-8编码
    system("chcp 65001");
//打开声音
    thread S(Sound);
    S.detach();
//设置窗口大小
    system("mode con cols=150 lines=60");
//绘制像素画
    GetGraph();
//初始化连接
    ConnectionInit();
    //cout<<"GOOD";
//进行游戏
    Play();
//关闭连接
    Close();
    return 0;
}