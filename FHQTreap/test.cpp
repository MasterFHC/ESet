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
struct Node{
    int l, r;
    int siz, pri;
    ll dat;
    Node(int l, int r, int siz, int pri, ll dat) : l(l), r(r), siz(siz), pri(pri), dat(dat) {}
};
vector<Node> tr;
int cnt = 0;    //number of nodes in all set
inline void update(int node) {
    tr[node].siz = 1;
    if (tr[node].l) tr[node].siz += tr[tr[node].l].siz;
    if (tr[node].r) tr[node].siz += tr[tr[node].r].siz;
}
int newcnt = 0;
inline int newNode(ll key = 0) {
    tr.emplace_back(0, 0, 1, rand(), key);
    ++cnt;
    return cnt;
}
inline int findMin(int node) {
    while (node) {
        if (tr[node].l) node = tr[node].l;
        else break;
    }
    return tr[node].dat;
}
inline int findMax(int node) {
    while (node) {
        if (tr[node].r) node = tr[node].r;
        else break;
    }
    return tr[node].dat;
}
inline int merge(int x, int y) {
    if (!x or !y) return x + y;
    if (tr[x].pri < tr[y].pri) {
        int rt = newNode();
        tr[rt] = tr[x];
        tr[rt].r = merge(tr[rt].r, y);
        update(rt);
        return rt;
    }
    else {
        int rt = newNode();
        tr[rt] = tr[y];
        tr[rt].l = merge(x, tr[rt].l);
        update(rt);
        return rt;
    }
}
inline void split(int rt, ll key, int& x, int& y) {
    if (!rt) x = y = 0;
    else {
        if (tr[rt].dat <= key) {
            int temp;
            x = newNode();
            tr[x] = tr[rt];
            temp = tr[x].r;
            split(tr[x].r, key, temp, y);
            tr[x].r = temp;
            update(x);
        }
        else {
            int temp;
            y = newNode();
            tr[y] = tr[rt];
            temp = tr[y].l;
            split(tr[y].l, key, x, temp);
            tr[y].l = temp;
            update(y);
        }
    }
}
inline bool find(int node, int key) {
    while (node) {
        if (key == tr[node].dat) return true;
        if (key < tr[node].dat) node = tr[node].l;
        else node = tr[node].r;
    }
    return false;
}
inline int findkth(int node, int key) {
    while (true) {
        if (key <= tr[tr[node].l].siz)
            node = tr[node].l;
        else {
            if (tr[node].l) key -= tr[tr[node].l].siz;
            if (!(--key)) return node;
            node = tr[node].r;
        }
    }
}

void traverse(int node) {
    if (!node) {
        std::cout << "TREE IS EMPTY!" << std::endl;
        return;
    }
    std::cout << node <<' '<<tr[node].dat << ' ' << tr[node].l << ' ' << tr[node].r << ' ' << tr[node].siz << endl;
    if (tr[node].l) traverse(tr[node].l);
    if (tr[node].r) traverse(tr[node].r);
}

int root[300001];   //root[id] stores the current version number of the id-th set

int main() {
    // freopen("data.out", "r", stdin);
    // freopen("1.out", "w", stdout);

   srand(time(NULL));
//    clock_t start, end;
//    start = clock();
    // bool flag = false;
    int op, lst = 0, it_a = -1, it = -1, valid = 0;
    newNode(0);
    tr[0].siz = 0;
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
                auto ret = merge(xx, zz);
                root[a] = ret;
            }
        }
        else if (op == 2) {
            // if(!flag){
            //     end = clock(); //程序结束用时
            //     double endtime = (double) (end - start) / CLOCKS_PER_SEC;
            //     printf("time=%lf s\n", endtime);
            //     flag = true;
            //     start = clock();
            // }
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
                write(tr[zz].siz);
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
                it = tr[findkth(xx, tr[xx].siz)].dat;
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
                    it = tr[findkth(yy, 1)].dat;
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

//    end = clock(); //程序结束用时
//    double endtime = (double) (end - start) / CLOCKS_PER_SEC;
//    printf("time=%lf s\n", endtime);
    return 0;
}