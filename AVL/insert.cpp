// #include <bits/stdc++.h>
#include <iostream>
#include <set>
#include "src.hpp"
using namespace std;
const int n = 256;
int T = 10;
int main(){
    freopen("output.txt","w",stdout);
    srand(time(NULL));
    while(T--){
        vector<int> toerase;
        ESet<int> test;
        ESet<int>::iterator it[n + 1];
        std::set<int> exist;
        for(int i=1;i<=n;i++){
            toerase.push_back(i);
            it[i] = test.emplace(i).first;
            exist.emplace(i);
        }
        random_shuffle(toerase.begin(), toerase.end());
        for(auto ele:toerase) cout<<ele<<" ";
        cout<<endl;
        int beginValue = *(test.begin());
        for(int k=1;k<=n;k++){
            bool beginChanged = false;
            int deleted = *(test.table->root->data);
            if(deleted == *(test.begin())) beginChanged = true;
            // test.emplace(rand()%n);
            test.erase(deleted);
            exist.erase(deleted);
            // cout<<*it<<endl;
            if(!beginChanged){
                if(beginValue != *(test.begin())){
                    cout<<endl<<"wrong begin!"<<endl;
                    return 0;
                }
            }
            for(int i=1;i<=n;i++){
                if(exist.find(i) != exist.end()){
                    ++it[i];
                    it[i]--;
                    it[i]++;
                    --it[i];    
                    if(*(it[i]) != i){
                        cout<<endl<<"wrong it!"<<endl;
                        return 0;
                    }

                }
            }
            if(test.size()) beginValue = *(test.begin());
        }
    }
    cout<<"all correct!"<<endl;
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