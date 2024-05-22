// #include <bits/stdc++.h>
#include <iostream>
#include <set>
#include "src.hpp"
using namespace std;
vector<int> emp = {10, 5, 15, 3, 8, 13, 16, 1, 4, 17};
int main(){
    ESet<int> s = ESet<int>();
    for(auto ele : emp) s.emplace(ele);
    s.erase(10);

    return 0;
}