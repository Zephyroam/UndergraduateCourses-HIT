#include <bits/stdc++.h>
#include "general.h"
#include "judger.h"
using std::string;
using std::cout;
class AI
{
private:
    Out FinalOut;
    CardInHand Card;
    std::vector<Out>AllOut;
    Out Chosen,Pre;

public:
    AI(){}
    void Dfs(int Position)
    {
        if(Position==TYPENUM+1)
        {
            Chosen.TypeBounds=GetType::Get(Chosen);
            if(!Chosen.TypeBounds.Type)return;
            if(Pre.GetSum()&&Pre.TypeBounds.Type!=Chosen.TypeBounds.Type)return;
            AllOut.push_back(Chosen);
        }
        for(int i=1;i<=Card.GetNum(i);++i)
        {
            Chosen.GetCard(Position);
            Dfs(Position+1);
        }
        for(int i=1;i<=Card.GetNum(i);++i)
            Chosen.DelCard(Position);

    }
    void Work(Out NowPre)
    {
        Pre = NowPre;
        Dfs(1);
        if(Pre.GetSum())
        {
            
        }
    }
};
