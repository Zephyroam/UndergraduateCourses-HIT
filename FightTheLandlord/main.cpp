#include <bits/stdc++.h>
#include "general.h"
#include "judger.h"
#include "graph.h"
#include <conio.h>
using std::string;
using std::cout;
using std::cin;
#define dbg(x) cout<<#x<<" = "<<x<<'\n';
const int CARDNUM=54;

CardInHand Lord,Farmer1,Farmer2;
Out Pre,Blank,History[2];

void Init()
{
    GetGraph();
    Blank.SetType(1);
    Pre=Blank;
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

    //srand(time(0));
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
/*void OutputCardLeft()
{

    cout<<"Public cards: ";
    for(int i=1;i<=TYPENUM;++i)
        for(int j=1;j<=Pub.GetNum(i);++j)
            cout<<In2Out(i);
    cout<<"\n";

    cout<<"Lord's cards: \n";
    OutGraphHand(Lord);
    cout<<"\n";

    cout<<"Farmer1's cards: \n";
    OutGraphHand(Farmer1);
    cout<<"\n";

    cout<<"Farmer2's cards: \n";
    OutGraphHand(Farmer2);
    cout<<"\n";
}
*/
bool PlayerTurn(CardInHand &Player,bool MustOut)
{
    In Temp;
    int Id[25], Idp = 0;
    int Active[25] = {0}, ActiveNow[25] = {0};
    int Ans, NowPos = 1;
    bool Legal = 0;
    for(int i=1;i<=TYPENUM;++i)
        for(int j=1;j<=Player.GetNum(i);++j)
            Id[++Idp] = i;
    while(1)
    {
        NowPos = 1;
        int Pree = 0;
        while(1)
        {
            system("cls");
            char Tmp;
            cout<<Player.GetName()<<"'s turn:\n";
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
            if(Pree)
                puts("Invalid input!Please select again!");
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
                    if(!Temp.GetSum())//处理输入为空的情况
                    {
                        if(MustOut)Legal=0;
                        else
                        {
                            cout<<Player.GetName()<<"'s output:"<<"Pass\n";
                            return 0;
                        }
                    }
                    else if(!Temp.GetSum())Legal=0;
                    for(int i=1;i<=TYPENUM-2&&Legal;++i)if(Temp.GetNum(i)>4)Legal=0;
                    if(Temp.GetNum(TYPENUM)>1||Temp.GetNum(TYPENUM-1)>1)Legal=0;
                    for(int i=1;i<=TYPENUM&&Legal;++i)if(Temp.GetNum(i)>Player.GetNum(i))Legal=0;
                    if(Legal&&Temp.GetSum()&&!ValidJudger(Pre,Temp))Legal=0;
                    ////////判大小
                    if(!Legal)
                    {
                        for (int i = 1; i <= 20;++i)
                            Active[i] = ActiveNow[i] = 0;
                        Temp = Blank;
                    }
                    else
                    {
                        for(int i=1;i<=TYPENUM&&Legal;++i)//从手牌中删除并输出
                            for(int j=1;j<=Temp.GetNum(i);++j)
                                Player.DelCard(i);
                    }
                    
                    break;
                }
                else
                    continue;
            }
            if(Legal)
                break;
        }
        if(Legal)
            break;
    }
    /*cout<<Player.GetName()<<"'s output:\n";
    for(int i=1;i<=TYPENUM&&Legal;++i)//从手牌中删除并输出
        for(int j=1;j<=Temp.GetNum(i);++j)
            Player.DelCard(i), History.GetCard(i);
    OutGraphOut(Temp);*/
/*
    dbg(Pre.TypeBounds.Type);
    dbg(Pre.TypeBounds.One);
    dbg(Pre.TypeBounds.Two);
    dbg(Temp.TypeBounds.Type);
    dbg(Temp.TypeBounds.One);
    dbg(Temp.TypeBounds.Two);
*/
    Pre=Temp;
    History[0] = History[1];
    History[1] = Temp;
    return 1;
}
void Play()
{
    int PlayerType=-1,PassTimes=0;
    bool PlayerTurnType;
    while(!Lord.end()&&!Farmer1.end()&&!Farmer2.end())
    {
        // system("cls");
        //OutputCardLeft();
        PlayerType++;PlayerType%=3;
        if(PlayerType==0)//Lord's PlayerType
            PlayerTurnType=PlayerTurn(Lord,Pre==Blank);
        else if(PlayerType==1)
            PlayerTurnType=PlayerTurn(Farmer1,Pre==Blank);
        else if(PlayerType==2)
            PlayerTurnType=PlayerTurn(Farmer2,Pre==Blank);
        if(!PlayerTurnType)PassTimes++;
        if(PassTimes==2)Pre=Blank,PassTimes=0;
    }
    if(Lord.end())
        puts("Lord win!");
    else if(Farmer1.end())
        puts("Farmer1 win!");
    else
        puts("Farmer2 win!");
}


void test()
{
    In Temp;
    string Input,Go;
    int Length,Ans;
    Input="";
    Temp=Blank;
    getline(cin,Input);
    Length=Input.length();
    for(int i=0;i<Length;++i)
    {
        while(Input[i]==' '&&i+1<Length)++i;
        Go="";
        while(Input[i]!=' '&&i<Length)Go+=Input[i],++i;
        Ans=Transform::Out2In(Go);
        if(Ans)Temp.GetCard(Ans);
    }
    Temp.TypeBounds=GetType::Get(Temp);
    dbg(Temp.TypeBounds.Type);
    dbg(Temp.TypeBounds.One);
    dbg(Temp.TypeBounds.Two);
}
int StartPage()
{
    const int MenuNum=3;
    system("mode con cols=150 lines=60");
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
            for (int j = 1; j <= 10;++j)
                printf(" ");
            if(NowPos==i)
                printf("*");
            else
                printf(" ");
            for (int j = 1; j <= 3;++j)
                printf(" ");
            if (i == 1)
            {
                printf("1.人人机对战\n");
            }
            else if(i==2)
            {
                printf("2.人机机对战\n");
            }
            else if(i==3)
            {
                printf("3.退出\n");
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
                return NowPos;
            }
            else
                continue;
        }
        
    }
    
}
int main()
{
    system("chcp 65001");
    StartPage();
    Init();
    /*for(int i=1;i<=15;++i)
    {
        for (int j = 1; j <= 5;++j)
            cout << gra[i].Graph[j] << '\n';
        cout << '\n';
    }*/
    //GetGraph();
    //while(1){test();}

    Play();
}
