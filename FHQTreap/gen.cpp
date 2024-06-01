#include <bits/stdc++.h>
using namespace std;
int n = 100000;
int m = 1000;
void write(int num){
    int top=0;
    int sta[65];
    do{
        sta[top++]=num%10;
        num/=10;
    }while(num);
    while(top) putchar(sta[--top]+'0');
}
int main(){
    freopen("data.out","w",stdout);
    for(int i=1;i<=n;i++){
        putchar('0');
        putchar(' ');
        putchar('0');
        putchar(' ');
        write(i);
        putchar('\n');
    }
    for(int i=1;i<=m;i++){
        putchar('2');
        putchar(' ');
        putchar('0');
        putchar('\n');
    }
    return 0;
}