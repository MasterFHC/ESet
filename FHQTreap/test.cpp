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
struct Node {
    int id;
    int left, right;
    int size, pri;
    ll data;
    bool isCopy = false;
    Node() {
        cout<<"id="<<id<<" created by default"<<endl;
    }
    ~Node(){
        if(!isCopy) cout<<"id="<<id<<" is freed! fuck!"<<endl;
        else cout<<"id="<<id<<" is freed! THIS IS COPY, it's fine !! "<<endl;
    }
    Node(int id, int left, int right, int size, int pri, ll data) : id(id), left(left), right(right), size(size), pri(pri), data(data) {
        cout<<"id="<<id<<" created by ()()()()"<<endl;
    }
    Node& operator=(const Node& rhs) {
        if (this == &rhs) return (*this);
        id = rhs.id;
        left = rhs.left;
        right = rhs.right;
        size = rhs.size;
        pri = rhs.pri;
        data = rhs.data;
        isCopy = true;
        return *this;
    }
};
// vector<Node> tree(1, Node(0, 0, 0, 0, 0));
vector<Node> tree;
int cnt = 0;    //number of nodes in all set
inline void update(int node) {
    tree[node].size = 1;
    if (tree[node].left) tree[node].size += tree[tree[node].left].size;
    if (tree[node].right) tree[node].size += tree[tree[node].right].size;
}
int newcnt = 0;
inline int newNode(ll key = 0) {
    // cout<<"newcnt = "<<++newcnt<<endl;
    // cout<<"treesize = "<< tree.size()<<endl;
    // tree.push_back(Node(0, 0, 1, rand(), key));
    tree.emplace_back(++cnt,0,0,0,0,0);
    // ++cnt;
    // tree[cnt].id = cnt;
    tree[cnt].data = key;
    // tree[cnt].left = tree[cnt].right = 0;
    tree[cnt].size = 1;
    tree[cnt].pri = rand();
    // cout<<"-------Node List------"<<endl;
    // for(int i=0;i<=cnt;i++){
    //     cout<<"("<<tree[i].id<<","<<tree[i].left<<","<<tree[i].right<<","<<tree[i].data<<")"<<endl;
    // }
    // cout<<"----------------------"<<endl;
    return cnt;
}
inline int findMin(int node) {
    while (node) {
        if (tree[node].left) node = tree[node].left;
        else break;
    }
    return tree[node].data;
}
inline int findMax(int node) {
    while (node) {
        // cout<<node<<' '<<tree[node].left<<' '<<tree[node].right<<endl;
        if (tree[node].right) node = tree[node].right;
        else break;
    }
    return tree[node].data;
}
inline int merge(int x, int y) {
    if (!x or !y) return x + y;
    if (tree[x].pri < tree[y].pri) {
        int rt = newNode();
        tree[rt] = tree[x];
        tree[rt].right = merge(tree[rt].right, y);
        update(rt);
        return rt;
    }
    else {
        int rt = newNode();
        tree[rt] = tree[y];
        tree[rt].left = merge(x, tree[rt].left);
        update(rt);
        return rt;
    }
}
inline void split(int rt, ll key, int& x, int& y) {
    if (!rt) x = y = 0;
    else {
        if (tree[rt].data <= key) {
            x = newNode();
            tree[x] = tree[rt];
            split(tree[x].right, key, tree[x].right, y);
            update(x);
        }
        else {
            y = newNode();
            // cout<<"newed y = "<<y<<endl;
            tree[y] = tree[rt];
            split(tree[y].left, key, x, tree[y].left);
            update(y);
        }
    }
}
inline bool find(int node, int key) {
    while (node) {
        if (key == tree[node].data) return true;
        if (key < tree[node].data) node = tree[node].left;
        else node = tree[node].right;
    }
    return false;
}
inline int findkth(int node, int key) {
    while (true) {
        if (key <= tree[tree[node].left].size)
            node = tree[node].left;
        else {
            if (tree[node].left) key -= tree[tree[node].left].size;
            if (!(--key)) return node;
            node = tree[node].right;
        }
    }
}

void traverse(int node) {
    if (!node) {
        std::cout << "TREE IS EMPTY!" << std::endl;
        return;
    }
    std::cout << tree[node].id<<' '<<tree[node].data << ' ' << tree[node].left << ' ' << tree[node].right << ' ' << tree[node].pri << endl;
    if (tree[node].left) traverse(tree[node].left);
    if (tree[node].right) traverse(tree[node].right);
}

vector<int> root(N/100, 0);   //root[id] stores the current version number of the id-th set

int main() {
    // freopen("1.in", "r", stdin);
    // freopen("1.out", "w", stdout);
    int op, lst = 0, it_a = -1, it = -1, valid = 0;
    tree.emplace_back(114, 0, 0, 0, 0, 0);
    while (scanf("%d", &op) != EOF) {
        long long a, b, c;
        int xx = 0, yy = 0, zz = 0;
        if (op == 0) {
            // a = read(), b = read();
            cin>>a>>b;
            if (!find(root[a], b)) {
                split(root[a], b, xx, yy);
                // cout<<"split done"<<endl;
                auto ret = merge(merge(xx, newNode(b)), yy);
                // cout<<"merged = "<<ret<<endl;
                root[a] = ret;
                it_a = a;
                it = b;
                valid = 1;
            }
        }
        else if (op == 1) {
            // a = read(), b = read();
            cin>>a>>b;
            if (valid and it_a == a and it == b) valid = 0;
            if (find(root[a], b)) {
                split(root[a], b, xx, zz);
                split(xx, b - 1, xx, yy);
                // cout<<"yy before merge= "<< yy << endl;
                // cout<<"left, right = "<<tree[yy].left<<' '<<tree[yy].right<<endl;
                yy = merge(tree[yy].left, tree[yy].right);
                // cout<<"yy after merge= "<< yy << endl;
                auto ret = merge(merge(xx, yy), zz);
                // cout<<"merged = "<<ret<<endl;
                root[a] = ret;
            }
        }
        else if (op == 2) {
            // a = read();
            cin>>a;
            root[++lst] = root[a];
        }
        else if (op == 3) {
            // a = read(), b = read();
            cin>>a>>b;
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
            // a = read(), b = read(), c = read();
            cin>>a>>b>>c;
            if (c < b) {
                write(0);
                putchar('\n');
            }
            else {
                split(root[a], c, xx, yy);
                split(xx, b - 1, xx, zz);
                write(tree[zz].size);
                putchar('\n');
                auto ret = merge(merge(xx, zz), yy);
                // cout<<"merged = "<<ret<<endl;
                root[a] = ret;
            }
        }
        else if (op == 5) {
            if (valid) {
                // cout<<"it, ita="<<it<<' '<<it_a<<endl;
                if (it == findMin(root[it_a])) valid = 0;
            }
            if (valid) {
                split(root[it_a], it - 1, xx, yy);
                it = tree[findkth(xx, tree[xx].size)].data;
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
                // cout<<"root[it_a] = "<<it_a<<' '<<root[it_a]<<endl;
                if (it == findMax(root[it_a])) {
                    // cout<<"it="<<it<<endl;
                    valid = 0;
                }
                else {
                    split(root[it_a], it, xx, yy);
                    it = tree[findkth(yy, 1)].data;
                    write(it);
                    putchar('\n');
                    root[it_a] = merge(xx, yy);
                }
            }
            if (!valid) {
                puts("-1");
            }
        }
        else{
            a = read();
            traverse(root[a]);
            cout<<"cnt="<<cnt<<endl;
        }
    }
    return 0;
}