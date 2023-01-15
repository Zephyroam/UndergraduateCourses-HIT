#include <bits/stdc++.h>
//3 4 5 6 7 8 9 10 J Q  K  A  2  joker JOKER
//1 2 3 4 5 6 7 8  9 10 11 12 13 14    15
struct Graph
{
    std::string Graph[6];
} gra[20];
//牌的像素图
void GetGraph()
{
    gra[1].Graph[1] = "3333 ";
    gra[1].Graph[2] = "    3";
    gra[1].Graph[3] = "33333";
    gra[1].Graph[4] = "    3";
    gra[1].Graph[5] = "3333 ";

    gra[2].Graph[1] = "  44 ";
    gra[2].Graph[2] = " 4 4 ";
    gra[2].Graph[3] = "4  4 ";
    gra[2].Graph[4] = "44444";
    gra[2].Graph[5] = "   4 ";

    gra[3].Graph[1] = "55555";
    gra[3].Graph[2] = "5    ";
    gra[3].Graph[3] = "5555 ";
    gra[3].Graph[4] = "    5";
    gra[3].Graph[5] = "5555 ";

    gra[4].Graph[1] = " 6666";
    gra[4].Graph[2] = "6    ";
    gra[4].Graph[3] = "6666 ";
    gra[4].Graph[4] = "6   6";
    gra[4].Graph[5] = " 666 ";

    gra[5].Graph[1] = "77777";
    gra[5].Graph[2] = "    7";
    gra[5].Graph[3] = "   7 ";
    gra[5].Graph[4] = "  7  ";
    gra[5].Graph[5] = " 7   ";

    gra[6].Graph[1] = " 888 ";
    gra[6].Graph[2] = "8   8";
    gra[6].Graph[3] = " 888 ";
    gra[6].Graph[4] = "8   8";
    gra[6].Graph[5] = " 888 ";

    gra[7].Graph[1] = " 999 ";
    gra[7].Graph[2] = "9   9";
    gra[7].Graph[3] = " 9999";
    gra[7].Graph[4] = "    9";
    gra[7].Graph[5] = " 999 ";

    gra[8].Graph[1] = "1  0 ";
    gra[8].Graph[2] = "1 0 0";
    gra[8].Graph[3] = "1 0 0";
    gra[8].Graph[4] = "1 0 0";
    gra[8].Graph[5] = "1  0 ";

    gra[9].Graph[1] = "   JJ";
    gra[9].Graph[2] = "    J";
    gra[9].Graph[3] = "    J";
    gra[9].Graph[4] = "J   J";
    gra[9].Graph[5] = " JJJ ";

    gra[10].Graph[1] = " QQ  ";
    gra[10].Graph[2] = "Q  Q ";
    gra[10].Graph[3] = "Q  Q ";
    gra[10].Graph[4] = "Q QQ ";
    gra[10].Graph[5] = " QQQQ";

    gra[11].Graph[1] = "K   K";
    gra[11].Graph[2] = "K  K ";
    gra[11].Graph[3] = "KK   ";
    gra[11].Graph[4] = "K  K ";
    gra[11].Graph[5] = "K   K";

    gra[12].Graph[1] = " AAA ";
    gra[12].Graph[2] = "A   A";
    gra[12].Graph[3] = "A   A";
    gra[12].Graph[4] = "AAAAA";
    gra[12].Graph[5] = "A   A";

    gra[13].Graph[1] = "2222 ";
    gra[13].Graph[2] = "    2";
    gra[13].Graph[3] = " 222 ";
    gra[13].Graph[4] = "2    ";
    gra[13].Graph[5] = "22222";

    gra[14].Graph[1] = "  j  ";
    gra[14].Graph[2] = "  o  ";
    gra[14].Graph[3] = "  k  ";
    gra[14].Graph[4] = "  e  ";
    gra[14].Graph[5] = "  r  ";

    gra[15].Graph[1] = "  J  ";
    gra[15].Graph[2] = "  O  ";
    gra[15].Graph[3] = "  K  ";
    gra[15].Graph[4] = "  E  ";
    gra[15].Graph[5] = "  R  ";

}
