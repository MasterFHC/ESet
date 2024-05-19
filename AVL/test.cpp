// #include <bits/stdc++.h>
#include <iostream>
#include "src.hpp"
using namespace std;
int main(){
    AVL<int> tree;
    tree.insert(&(tree.root), 1);
    cout<<tree.root<<endl;
    // if(tree.root == nullptr) cout<<"fuck";
    return 0;
}