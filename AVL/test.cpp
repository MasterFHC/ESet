// #include <bits/stdc++.h>
#include <iostream>
#include "src.hpp"
using namespace std;
// vector<int> emp = {2000, 1997, 1996, 1993, -2000, 1991, 1987, 1986, 1985, -1995, 1982, 1981, 1978, -1982, 1976, 1975, 1974, 1973, -1991};
int main(){
    // freopen("input.txt","r",stdin);
    ESet<int> test;
    test.emplace(1288);
    test.emplace(1283);
    test.emplace(1321);
    test.emplace(1292);
    auto ret = test.emplace(1303);
    auto it = ret.first;
    bool tof = ret.second;
    if(it.get()) cout<<*it<<endl;
    else cout<<"empty!"<<endl;
    cout<<"tof="<<tof<<endl;
    // int x, y, z;
    // for(int i=1;i<=63;i++){
    //     cin>>x>>y>>z;
    //     if(x==0) test.emplace(z);
    //     if(x==1) test.erase(z);
    // }
    // test.emplace(1102);
    // cout<<test.range(1102, 1268)<<endl;

    // test.traverse();
    // for(auto ele : emp){
    //     if(ele > 0) test.emplace(ele);
    //     else test.erase(-ele);
    // }
    // cout<<test.range(1991, 2225)<<endl;
    // cout<<endl;
    // for(int i=1;i<=10;i++) test.emplace(i);
    // auto it = test.begin();
    // for(int i=1;i<=5;i++) ++it;
    // cout<<"remember it="<<(*it)<<endl;
    // for(int i=11;i<=200;i++){
    //     test.emplace(i);
    //     cout<<"now it="<<(*it)<<endl;
    //     if(*it!=6) break;
    // }
    // cin.get();
    // if(tree.root == nullptr) cout<<"fuck";
    return 0;
}