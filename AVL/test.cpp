// #include <bits/stdc++.h>
#include <iostream>
#include "src.hpp"
using namespace std;
// vector<int> emp = {2000, 1997, 1996, 1993, -2000, 1991, 1987, 1986, 1985, -1995, 1982, 1981, 1978, -1982, 1976, 1975, 1974, 1973, -1991};
int main(){
    // freopen("input.txt","r",stdin);
    ESet<int> test;
    // test.emplace(1288);
    for(int i=1;i<=10;i++){
        test.emplace(i);
    }
    auto it = --(test.end());
    --it;
    test.erase(8);
    cout<<*it<<endl;
    // auto it = test.begin();
    // ++it;++it;++it;++it;++it;
    // for(int i=1;i<=;i++){
    //     test.erase(i);
    //     if(*it != 6) cout<<"fuck"<<endl;
    //     else cout<<"(*it, *begin)=="<<*it<<" "<<*(test.begin())<<endl;
    // }
    // test.emplace(1);
    // test.emplace(2);
    // test.traverse();
    // cin.get();
    // test.upper_bound(24);
    // for(int i=0;i<=22;i++){
    //     for(int j=0;j<=22;j++){
    //         cout<<test.range(i, j)<<' ';
    //     }
    //     cout<<endl;
    // }

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