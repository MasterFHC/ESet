#include <bits/stdc++.h>
#include "src.hpp"
using namespace std;
vector<int> emp = {1,1,4,5,1,4,1,9,1,9,8,1,0,2,3,4,5,6,7,8,9,10};
int main(){
    ESet<int> s;
    for(auto ele : emp){
        s.emplace(ele);
        // s.traverse();
        // cout<<endl;
    }
    
    // for(auto ele : emp) tr.insert(ele);
    // tr.traverse(tr._root);
    
    return 0;
}