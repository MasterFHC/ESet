#include <bits/stdc++.h>
#include "src.hpp"
using namespace std;
// vector<int> emp = {1,2,3,4,5,6,7,8,9,10};
int main(){
    ESet<int> s;
    for(int i=1;i<=10;i++) s.emplace(i);
    s.traverse();
    cout<<endl;
    for(int i=1;i<=10;i++){
        s.erase(i);
        s.traverse();
        cout<<endl;
    }
    // for(auto ele : emp) tr.insert(ele);
    // tr.traverse(tr._root);
    
    return 0;
}