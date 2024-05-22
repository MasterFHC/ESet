// #include <bits/stdc++.h>
#include <iostream>
#include <set>
#include "src.hpp"
using namespace std;
vector<int> emp = {10, 5, 15};
int main(){
    ESet<int> s = ESet<int>();
    for(auto ele : emp) s.emplace(ele);
    auto it2 = s.end();
    auto it = ++s.begin();
    while(it != ++--it2){
        s.erase(*(it++));
        if(it.get()) cout<<*it<<endl;
        else cout<<"nullptr"<<endl;
    }
    return 0;
}