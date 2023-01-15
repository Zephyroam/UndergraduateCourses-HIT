#include <bits/stdc++.h>
#include "general.h"
using std::string;
using std::cout;

/*======================================
非法牌型 0
过 1
单张 2
对子 3
三个 4
纯炸 5
三带一 6
三带一对 7
顺子（5张-12张） 8-15
联对（3对-10对）16-23
飞机（2-6连）24-28
四带两单张 29
四带一对子 30
四带两对子 31
飞机带单张（2-5连）32-35
飞机带对子（2-4连）36-38
王炸 39
======================================*/
namespace GetType
{
    TypeBound Get(Out Card)
    {
        int One=0,Two=0,Sum=Card.GetSum(),Num;
        bool can;
        if(Sum==0)return TypeBound(1,0,0);
        //单张 2
        if(Sum==1)
        {
            One=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=1){can=0;break;}
                    if(!One)One=i;
                    else {can=0;break;}
                }
            if(can)return TypeBound(2,One,Two);
        }

        //对子 3
        if(Sum==2)
        {
            One=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=2){can=0;break;}
                    if(!One)One=i;
                    else {can=0;break;}
                }
            if(can)return TypeBound(3,One,Two);
        }

        //三个 4
        if(Sum==3)
        {
            One=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=3){can=0;break;}
                    if(!One)One=i;
                    else {can=0;break;}
                }
            if(can)return TypeBound(4,One,Two);
        }

        //纯炸 5
        if(Sum==4)
        {
            One=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=4){can=0;break;}
                    if(!One)One=i;
                    else {can=0;break;}
                }
            if(can)return TypeBound(5,One,Two);
        }

        //三带一 6
        if(Sum==4)
        {
            One=0;Two=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=3&&Card.GetNum(i)!=1){can=0;break;}
                    if(Card.GetNum(i)==3&&!One)One=i;
                    else if(Card.GetNum(i)==3){can=0;break;}
                    if(Card.GetNum(i)==1&&!Two)Two=i;
                    else if(Card.GetNum(i)==1){can=0;break;}
                }
            if(can&&One&&Two)return TypeBound(6,One,Two);
        }

        //三带一对 7
        if(Sum==5)
        {
            One=0;Two=0;can=1;Num=0;
            for(int i=1;i<=TYPENUM-2;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=3&&Card.GetNum(i)!=2){can=0;break;}
                    if(Card.GetNum(i)==3&&!One)One=i;
                    else if(Card.GetNum(i)==3){can=0;break;}
                    if(Card.GetNum(i)==2&&!Two)Two=i,Num=1;
                    else if(Card.GetNum(i)==2){can=0;break;}
                }
            if((Card.GetNum(TYPENUM)^Card.GetNum(TYPENUM-1)))can=0;
            if(Card.GetNum(TYPENUM)&&Card.GetNum(TYPENUM-1))Num++,Two=TYPENUM-1;
            if(can&&One&&Num==1)return TypeBound(7,One,Two);
        }

        //顺子（5张-12张） 8-15
        if(Sum>=5&&Sum<=12)
        {
            One=0;Two=0;can=1;
            for(int i=1;i<=TYPENUM-2;++i)
                if(Card.GetNum(i)>1){can=0;break;}
            if(Card.GetNum(TYPENUM-1))can=0;
            if(Card.GetNum(TYPENUM))can=0;
            for(int i=1;i<=TYPENUM-2&&can;++i)
                if(Card.GetNum(i))
                {
                    Two=One=i;
                    break;
                }
            for(int i=One+1;i<=TYPENUM-2&&can;++i)
                if(Card.GetNum(i))Two=i;
                else break;
            for(int i=Two+1;i<=TYPENUM-2&&can;++i)
                if(Card.GetNum(i)){can=0;break;}
            if(can&&Two-One+1>=5)
                return TypeBound(Two-One+1+3,One,Two);
        }

        //连对（3对-10对） 16-23
        if(Sum>=6)
        {
            One=0;Two=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i)&&Card.GetNum(i)!=2){can=0;break;}
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i))
                {
                    Two=One=i;
                    break;
                }
            for(int i=One+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i))Two=i;
                else break;
            for(int i=Two+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)){can=0;break;}
            if(can&&Two-One+1>=3)
                return TypeBound(Two-One+1+13,One,Two);
        }

        //飞机（2-6连） 24-28
        if(Sum>=6)
        {
            One=0;Two=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i)&&Card.GetNum(i)!=3){can=0;break;}
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i))
                {
                    Two=One=i;
                    break;
                }
            for(int i=One+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i))Two=i;
                else break;
            for(int i=Two+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)){can=0;break;}
            if(can&&Two-One+1>=2)
                return TypeBound(Two-One+1+22,One,Two);
        }

        //四带两单张 29
        if(Sum==6)
        {
            One=0;Two=0;can=1;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=4&&Card.GetNum(i)!=1){can=0;break;}
                    if(Card.GetNum(i)==4&&!One)One=i;
                    else if(Card.GetNum(i)==4){can=0;break;}
                    if(Card.GetNum(i)==1)Two++;
                }
            if(can&&One&&Two==2)return TypeBound(29,One,Two);
        }

        //四带一对子 30
        if(Sum==6)
        {
            One=0;Two=0;can=1;Num=0;
            for(int i=1;i<=TYPENUM-2;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=4&&Card.GetNum(i)!=2){can=0;break;}
                    if(Card.GetNum(i)==4&&!One)One=i;
                    else if(Card.GetNum(i)==4){can=0;break;}
                    if(Card.GetNum(i)==2&&!Two)Two=i,Num++;
                    else if(Card.GetNum(i)==2){can=0;break;}
                }
            if((Card.GetNum(TYPENUM)^Card.GetNum(TYPENUM-1)))can=0;
            if(Card.GetNum(TYPENUM)&&Card.GetNum(TYPENUM-1))Num++;
            if(can&&One&&Num==1)return TypeBound(30,One,Two);
        }

        //四带两对子 31
        if(Sum==8)
        {
            One=0;Two=0;can=1;Num=0;
            for(int i=1;i<=TYPENUM-2;++i)
                if(Card.GetNum(i))
                {
                    if(Card.GetNum(i)!=4&&Card.GetNum(i)!=2){can=0;break;}
                    if(Card.GetNum(i)==4&&!One)One=i;
                    else if(Card.GetNum(i)==4){can=0;break;}
                    if(Card.GetNum(i)==2&&!Two)Two=i,Num++;
                    else if(Card.GetNum(i)==2){can=0;break;}
                }
            if((Card.GetNum(TYPENUM)^Card.GetNum(TYPENUM-1)))can=0;
            if(Card.GetNum(TYPENUM)&&Card.GetNum(TYPENUM-1))Num++;
            if(can&&One&&Num==2)return TypeBound(31,One,Two);
        }

        //飞机带单张（2-5连）32-35
        if(Sum>=8)
        {
            One=0;Two=0;can=1;Num=0;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i)&&Card.GetNum(i)>3){can=0;break;}
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)!=3)Num+=Card.GetNum(i);
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3)
                {
                    Two=One=i;
                    break;
                }
            for(int i=One+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3)Two=i;
                else break;
            for(int i=Two+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3){can=0;break;}
            if(can&&Two-One+1>=2&&Num==Two-One+1)
                return TypeBound(Two-One+1+30,One,Two);
        }

        //飞机带对子（2-4连）36-38
        if(Sum>=10)
        {
            One=0;Two=0;can=1;Num=0;
            for(int i=1;i<=TYPENUM;++i)
                if(Card.GetNum(i)&&Card.GetNum(i)!=3&&Card.GetNum(i)!=2){can=0;break;}
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==2)Num++;
            for(int i=1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3)
                {
                    Two=One=i;
                    break;
                }
            for(int i=One+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3)Two=i;
                else break;
            for(int i=Two+1;i<=TYPENUM&&can;++i)
                if(Card.GetNum(i)==3){can=0;break;}
            if((Card.GetNum(TYPENUM)^Card.GetNum(TYPENUM-1)))can=0;
            if(Card.GetNum(TYPENUM)&&Card.GetNum(TYPENUM-1))Num++;
            if(can&&Two-One+1>=2&&Num==Two-One+1)
                return TypeBound(Two-One+1+34,One,Two);
        }

        //王炸 39
        if(Sum==2)
        {
            if(Card.GetNum(TYPENUM)&&Card.GetNum(TYPENUM-1))
                return TypeBound(39,One,Two);
        }

        return TypeBound(0,0,0);
    }


};

//牌大小判断
bool ValidJudger(Out Pre,Out Lat)
{
    if(Pre.TypeBounds.Type==Lat.TypeBounds.Type&&Pre.TypeBounds.One<Lat.TypeBounds.One)return 1;
    if(Pre.TypeBounds.Type==1&&Lat.TypeBounds.Type>1)return 1;
    if(Lat.TypeBounds.Type==39)return 1;
    if(Pre.TypeBounds.Type!=39&&Pre.TypeBounds.Type!=5&&Lat.TypeBounds.Type==5)return 1;
    return 0;
}
