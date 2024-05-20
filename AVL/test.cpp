// #include <bits/stdc++.h>
#include <iostream>
#include "src.hpp"
using namespace std;
int main(){
    ESet<int> test;
    for(int i=1;i<=10;i++){
        test.emplace(i);
    }
    test.traverse();
    ESet<int> test2;
    test2 = test;
    test2.traverse();
    for(int i=1;i<=10;i++){
        cout<<"i="<<i<<endl;
        test.erase(i);
        test.traverse();
        // for(int j=1;j<=10;j++){
        //     auto ret = test.find(j);
        //     if(ret.get()) cout<<"found"<<' ';
        //     else cout<<"notfound"<<' ';
        // }
        // cout<<endl;
    }
    cin.get();
    test2.traverse();

    for(int i=1;i<=10;i++){
        cout<<"i="<<i<<endl;
        test2.erase(i);
        test2.traverse();
        // for(int j=1;j<=10;j++){
        //     auto ret = test.find(j);
        //     if(ret.get()) cout<<"found"<<' ';
        //     else cout<<"notfound"<<' ';
        // }
        // cout<<endl;
    }
    // AVL<int> test;
    // for(int i=1;i<=10;i++){
    //     test.insert(&(test.root), i);
    //     test.traverse(test.root);
    //     cout<<"end of this traverse"<<endl;
    //     cin.get();
    // }
    // for(int i=1;i<=10;i++){
    //     test.erase(&(test.root), i);
    //     test.traverse(test.root);
    //     cout<<"end of this traverse"<<endl;
    //     cin.get();
    // }

    // cin.get();
    // if(tree.root == nullptr) cout<<"fuck";
    return 0;
}