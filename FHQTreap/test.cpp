#include <bits/stdc++.h>
#define ll long long
using namespace std;
inline ll read() {
    char ch = 0;
    ll w = 1, num = 0;
    while (ch < '0' or ch>'9') {
        if (ch == '-') w = -1;
        ch = getchar();
    }
    while (ch >= '0' and ch <= '9') {
        num = num * 10 + (ch - '0');
        ch = getchar();
    }
    return num * w;
}
inline void write(ll num) {
    ll top = 0;
    int sta[65];
    do {
        sta[top++] = num % 10;
        num /= 10;
    } while (num);
    while (top) putchar(sta[--top] + '0');
}

const int N = (int)3e6;
vector<int> l, r, siz, pri;
vector<ll> dat;
int cnt = 0;    //number of nodes in all set
inline void update(int node) {
    siz[node] = 1;
    if (l[node]) siz[node] += siz[l[node]];
    if (r[node]) siz[node] += siz[r[node]];
}
int newcnt = 0;
inline int newNode(ll key = 0) {
    l.emplace_back(0);
    r.emplace_back(0);
    siz.emplace_back(1);
    pri.emplace_back(rand());
    dat.emplace_back(key);
    ++cnt;
    return cnt;
}
inline void copyTo(int x, int y){
    l[y] = l[x];
    r[y] = r[x];
    siz[y] = siz[x];
    pri[y] = pri[x];
    dat[y] = dat[x];
}
inline int findMin(int node) {
    while (node) {
        if (l[node]) node = l[node];
        else break;
    }
    return dat[node];
}
inline int findMax(int node) {
    while (node) {
        if (r[node]) node = r[node];
        else break;
    }
    return dat[node];
}
inline int merge(int x, int y) {
    if (!x or !y) return x + y;
    if (pri[x] < pri[y]) {
        int rt = newNode(0);
        copyTo(x, rt);
        r[rt] = merge(r[rt], y);
        update(rt);
        return rt;
    }
    else {
        int rt = newNode(0);
        copyTo(y, rt);
        l[rt] = merge(x, l[rt]);
        update(rt);
        return rt;
    }
}
inline void split(int rt, ll key, int& x, int& y) {
    if (!rt) x = y = 0;
    else {
        if (dat[rt] <= key) {
            int temp;
            x = newNode(0);
            copyTo(rt, x);
            temp = r[x];
            split(r[x], key, temp, y);
            r[x] = temp;
            update(x);
        }
        else {
            int temp;
            y = newNode(0);
            copyTo(rt, y);
            temp = l[y];
            split(l[y], key, x, temp);
            l[y] = temp;
            update(y);
        }
    }
}
inline bool find(int node, int key) {
    while (node) {
        if (key == dat[node]) return true;
        if (key < dat[node]) node = l[node];
        else node = r[node];
    }
    return false;
}
inline int findkth(int node, int key) {
    while (true) {
        if (key <= siz[l[node]])
            node = l[node];
        else {
            if (l[node]) key -= siz[l[node]];
            if (!(--key)) return node;
            node = r[node];
        }
    }
}

void traverse(int node) {
    if (!node) {
        std::cout << "TREE IS EMPTY!" << std::endl;
        return;
    }
    std::cout << node <<' '<<dat[node] << ' ' << l[node] << ' ' << r[node] << ' ' << siz[node] << endl;
    if (l[node]) traverse(l[node]);
    if (r[node]) traverse(r[node]);
}

ll root[30001];   //root[id] stores the current version number of the id-th set

int main() {
    freopen("1.in", "r", stdin);
    freopen("1.out", "w", stdout);
    int op, lst = 0, it_a = -1, it = -1, valid = 0;
    newNode(0);
    siz[0] = 0;
    l[0] = 0;
    r[0] = 0;
    dat[0] = 0;
    pri[0] = 0;
    cnt = 0;
    while (scanf("%d", &op) != EOF) {
        long long a, b, c;
        int xx = 0, yy = 0, zz = 0;
        if (op == 0) {
            a = read(), b = read();
            if (!find(root[a], b)) {
                split(root[a], b, xx, yy);
                auto ret = merge(merge(xx, newNode(b)), yy);
                root[a] = ret;
                it_a = a;
                it = b;
                valid = 1;
            }
        }
        else if (op == 1) {
            a = read(), b = read();
            if (valid and it_a == a and it == b) valid = 0;
            if (find(root[a], b)) {
                split(root[a], b, xx, zz);
                split(xx, b - 1, xx, yy);
                yy = merge(l[yy], r[yy]);
                auto ret = merge(merge(xx, yy), zz);
                root[a] = ret;
            }
        }
        else if (op == 2) {
            a = read();
            root[++lst] = root[a];
        }
        else if (op == 3) {
            a = read(), b = read();
            if (find(root[a], b)) {
                puts("true");
                it_a = a;
                it = b;
                valid = 1;
            }
            else {
                puts("false");
            }
        }
        else if (op == 4) {
            a = read(), b = read(), c = read();
            if (c < b) {
                write(0);
                putchar('\n');
            }
            else {
                split(root[a], c, xx, yy);
                split(xx, b - 1, xx, zz);
                write(siz[zz]);
                putchar('\n');
                auto ret = merge(merge(xx, zz), yy);
                root[a] = ret;
            }
        }
        else if (op == 5) {
            if (valid) {
                if (it == findMin(root[it_a])) valid = 0;
            }
            if (valid) {
                split(root[it_a], it - 1, xx, yy);
                it = dat[findkth(xx, siz[xx])];
                write(it);
                putchar('\n');
                root[it_a] = merge(xx, yy);
            }
            else {
                puts("-1");
            }
        }
        else if (op == 6) {
            if (valid) {
                if (it == findMax(root[it_a])) {
                    valid = 0;
                }
                else {
                    split(root[it_a], it, xx, yy);
                    it = dat[findkth(yy, 1)];
                    write(it);
                    putchar('\n');
                    root[it_a] = merge(xx, yy);
                }
            }
            if (!valid) {
                puts("-1");
            }
        }
    }
    return 0;
}