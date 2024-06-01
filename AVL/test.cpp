#include <bits/stdc++.h>
// #include <iostream>
#include <set>
#include "src.hpp"
using namespace std;
int n = 100000, m = 1000;
clock_t st, ed;
void myTest(){
    ESet<int> s, copy;
    st = clock();
    for(int i=1;i<=n;i++) s.emplace(i);
    ed = clock(); //程序结束用时
    double endtime = (double) (ed - st) / CLOCKS_PER_SEC;
    printf("time=%lf s\n", endtime);
    st = clock();
    for(int i=1;i<=m;i++) copy = s;
}
void stdTest(){
    set<int> s, copy;
    st = clock();
    for(int i=1;i<=n;i++) s.emplace(i);
    ed = clock(); //程序结束用时
    double endtime = (double) (ed - st) / CLOCKS_PER_SEC;
    printf("time=%lf s\n", endtime);
    st = clock();
    for(int i=1;i<=m;i++) copy = s;
}
int main(){
    // srand(0);

    myTest();
    // stdTest();

    ed = clock(); //程序结束用时
    double endtime = (double) (ed - st) / CLOCKS_PER_SEC;
    printf("time=%lf s\n", endtime);
    return 0;
}