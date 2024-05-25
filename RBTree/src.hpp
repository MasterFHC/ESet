#ifndef SJTU_RBTree
#define SJTU_RBTree
#include <cstddef>
#include <iostream>

template<class T, class Compare = std::less<T>>
class RBTree {
/*
** credits to this article of RBTree:
** https://www.luogu.com.cn/article/c5vuyipn
*/
public:
    struct RBNode {
        int size;
        bool color;//R = 1, B = 0
        T* data;
        struct RBNode* father;
        struct RBNode* left;
        struct RBNode* right;
        RBNode() {
            father = left = right = nullptr;
            data = nullptr;
            size = 1;
        }
        RBNode(int size, bool color, T* data, struct RBNode* father, struct RBNode* left, struct RBNode* right)
            :size(size), color(color), data(data), father(father), left(left), right(right) {}
        ~RBNode() {
            father = left = right = nullptr;
            delete data;
            data = nullptr;
        }
    };
    RBNode* root = nullptr;   //The root node
    RBNode* hot = nullptr;    //The node that is being looked at

    RBTree(){}
    ~RBTree() {
        std::queue<RBNode*> q;
        if (!root) return;
        q.push(root);
        while (!q.empty()) {
            auto now = q.front();
            q.pop();
            if (now->left) {
                q.push(now->left);
            }
            if (now->right) {
                q.push(now->right);
            }
            delete now;
            now = nullptr;
        }
        root = nullptr;
    }
    RBNode* nextNode(RBNode* node){
        node = node->right;
        while(node->left){
            --(node->size);
            node = node->left;
        }
        return node;
    }
    RBNode* findMinNode(RBNode* node) {
        while (node->left) node = node->left;
        return node;
    }
    RBNode* findMaxNode(RBNode* node) {
        while (node->right) node = node->right;
        return node;
    }
    inline int getSize(RBNode* node) const {
        if (!node) return 0;
        return node->size;
    }
    bool isEqual(const T& a, const T& b) const {
        return !Compare()(a, b) and !Compare()(b, a);
    }
    inline void maintain(RBNode* node){
        node->size = 1 + getSize(node->left) + getSize(node->right);
        // std::cout<<"maintain: data="<<node->data<<" leftsize="<<node->left->size
    }

    RBNode* getBro(RBNode* node) const{
        if(!(node->father)) return nullptr;
        if(node == node->father->left) return node->father->right;
        return node->father->left;
    }
    bool isLeft(RBNode* node) const{
        if(!node) return false;
        if(!(node->father)) return false;
        return node == node->father->left;
    }
    bool isRight(RBNode* node) const{
        if(!node) return false;
        if(!(node->father)) return false;
        return node == node->father->right;
    }

    RBNode* find(RBNode* node, const int& key, int op) {
        if (!node) return nullptr;
        hot = node;
        if (*(node->data) == key) return node;
        if (Compare()(key, *(node->data))) {
            if (node->left){
                auto ret = find(node->left, key, op);
                if(!ret and op == 1) node->size += op;
                if(ret and op == -1) node->size += op;
                return ret;
            }
            else{
                if(op == 1) node->size += op;
                return nullptr;
            }
        }
        else {
            if (node->right){
                auto ret = find(node->right, key, op);
                if(!ret and op == 1) node->size += op;
                if(ret and op == -1) node->size += op;
                return ret;
            }
            else{
                if(op == 1) node->size += op;
                return nullptr;
            }
        }
    }
    std::pair<RBNode*, bool> insert(const T& key) {
        hot = nullptr;
        if(!root){
            root = new RBNode(1, false, new T(key), nullptr, nullptr, nullptr);
            return std::pair<RBNode*, bool>(root, true);
        }
        RBNode* node = find(root, key, 1);
        if(node) return std::pair<RBNode*, bool>(node, false);
        node = new RBNode(1, true, new int(key), hot, nullptr, nullptr);
        if(!Compare()(key, *(hot->data))) hot->right = node;
        else hot->left = node;
        solveRed(node);
        return std::pair<RBNode*, bool>(node, true);
    }
    void balance(RBNode* node, RBNode* lc, RBNode* rc, RBNode* T1, RBNode* T2, RBNode* T3, RBNode* T4){
        node->left = lc;
        if(lc) lc->father = node;
        node->right = rc;
        if(rc) rc->father = node;
        if(lc){
            lc->left = T1;
            if(T1) T1->father = lc;
            lc->right = T2;
            if(T2) T2->father = lc;
        }
        if(rc){
            rc->left = T3;
            if(T3) T3->father = rc;
            rc->right = T4;
            if(T4) T4->father = rc;
        }
        if(lc) maintain(lc);
        if(rc) maintain(rc);
        maintain(node);
    }
    void solveRed(RBNode* node){
        while(!(node->father) or node->father->color){//is root or still double-red
            if(node == root){
                root->color = false;
                return;
            }
            auto fa = node->father;
            if(!(fa->color)) return;        //No double red(fa is black)
            RBNode* uncle = getBro(fa);     //else, fa is red, then fa is not root
            RBNode* gfa = fa->father;       //guaranteed
            if(uncle and uncle->color){     //uncle is red too
                gfa->color = true;
                uncle->color = fa->color = false;
                node = gfa;
            }
            else{//uncle is black, only need rotation
                if(isLeft(fa)){
                    if(isLeft(node)){//LL
                        fa->father = gfa->father;
                        if(gfa == root) root = fa;
                        else if(isLeft(gfa)) gfa->father->left = fa;
                        else gfa->father->right = fa;
                        balance(fa, node, gfa, node->left, node->right, fa->right, gfa->right);
                        fa->color = false;
                        gfa->color = true;
                    }
                    else{//LR
                        node->father = gfa->father;
                        if(gfa == root) root = node;
                        else if(isLeft(gfa)) gfa->father->left = node;
                        else gfa->father->right = node;
                        balance(node, fa, gfa, fa->left, node->left, node->right, gfa->right);
                        node->color = false;
                        gfa->color = true;
                    }
                }
                else{
                    if(isLeft(node)){//RL
                        node->father = gfa->father;
                        if(gfa == root) root = node;
                        else if(isLeft(gfa)) gfa->father->left = node;
                        else gfa->father->right = node;
                        balance(node, gfa, fa, gfa->left, node->left, node->right, fa->right);
                        node->color = false;
                        gfa->color = true;
                    }
                    else{//RR
                        fa->father = gfa->father;
                        if(gfa == root) root = fa;
                        else if(isLeft(gfa)) gfa->father->left = fa;
                        else gfa->father->right = fa;
                        balance(fa, gfa, node, gfa->left, fa->left, node->left, node->right);
                        fa->color = false;
                        gfa->color = true;
                    }
                }
                return;
            }
        }
    }
    
    size_t erase(const T& key){
        RBNode* node = find(root, key, -1);
        if(!node) return 0;
        RBNode* rep;    //replacement node
        while(node->left or node->right){
            // auto nownode = node;
            if(!(node->left)){
                rep = node->right;
            }
            else if(!(node->right)){
                rep = node->left;
            }
            else{
                rep = nextNode(node);
            }
            --(node->size);
            if(!(node->father)) root = rep;
            if(node->father and rep->father){
                RBNode** nodeson, repson;
                if(isLeft(node)) nodeson = &(node->father->left);
                else nodeson = &(node->father->right);
                if(ifLeft(rep)) repson = &(rep->father->left);
                else repson = &(rep->father->right);
                std::swap(*nodeson, *repson);
            }
            else if(node->father){
                if(isLeft(node)) node->father->left = rep;
                else node->father->right = rep;
            }
            else{
                if(isLeft(rep)) rep->father->left = node;
                else rep->father->right = node;
            }
            std::swap(node->size, rep->size);
            std::swap(node->color, rep->color);
            std::swap(node->left, rep->left);
            std::swap(node->right, rep->right);
            std::swap(node->father, rep->father);
            if(node->left) node->left->father = node;
            if(node->right) node->right->father = node;
            node = rep;
            if(node->left) node->left->father = node;
            if(node->right) node->right->father = node;
        }
        if(!(node->color)){//double black possibilities!
            --(node->size);
            solveBlack(node);
        }
        if(node == root){
            node = nullptr;
            delete node;
            return 1;
        }
        if(isLeft(node)) node->father->left = nullptr;
        if(isRight(node)) node->father->right = nullptr;
        delete node;
        return true;
    }
    void solveBlack(RBNode* node){
        while(node != root){
            RBNode* fa = node->father;
            RBNode* bro = getBro(node);
            if(bro->color){//brother is red
                bro->color = false;
                fa->color = true;
                if(root == fa) root = bro;
                if(fa->father){
                    if(isLeft(fa)) fa->father->left = bro;
                    else fa->father->right = bro;
                }
                bro->father = fa->father;
                if(isLeft(node)){
                    balance(bro, fa, bro->right, node, bro->left, ((bro->right)?(bro->right->left):(nullptr)), ((bro->right)?(bro->right->right):(nullptr)));
                }
                else{
                    balance(bro, bro->left, fa, ((bro->left)?(bro->left->left):(nullptr)), ((bro->left)?(bro->left->right):(nullptr)), bro->right, node);
                }
                bro = getBro(node);
                fa = node->father;
            }
            if(bro->left and bro->left->color){//bro leftson is red
                bool faColor = fa->color;
                fa->color = false;
                if(fa->left == node){//bro is right, RL
                    if(fa->father){
                        if(isLeft(fa)) fa->father->left = bro->left;
                        else fa->father->right = bro->left;
                    }
                    bro->left->father = fa->father;
                    if(root == fa) root = bro->left;
                    balance(bro->left, fa, bro, node, bro->left->left, bro->left->right, bro->right);
                }
                else{//bro is left, LL
                    if(fa->father){
                        if(isLeft(fa)) fa->father->left = bro;
                        else fa->father->right = bro;
                    }
                    bro->father = fa->father;
                    if(root == fa) root = bro;
                    balance(bro, bro->left, fa, bro->left->left, bro->left->right, bro->right, node);
                }
                fa->father->color = faColor;
                return;
            }
            else if(bro->right and bro->right->color){//bro rightson is red
                bool faColor = fa->color;
                fa->color = false;
                if(fa->left == node){//bro is right, RR

                }
                else{//bro is left, LR
                    
                }
            }
        }
    }
    /*
    **function for checking correctness and utilities
    */
    void traverse(RBNode* node) {
        if (!node) {
            std::cout << "TREE IS EMPTY!" << std::endl;
            return;
        }
        std::cout << *(node->data) << ' ' << (node->size) << ' ' << ((node->color) ? ("Red") : ("Black")) << ' ';
        if (node->father) std::cout << *(node->father->data) << std::endl;
        else std::cout << "nullptr" << std::endl;
        if (node->left) traverse(node->left);
        if (node->right) traverse(node->right);
    }
    RBNode* findLowerBound(RBNode* node, const T& key) const {//key <= data
        bool isValid = false;
        RBNode* foundNode = nullptr;
        if (!Compare()(*(node->data), key)) isValid = true;
        if (node->left and Compare()(key, *(node->data))) foundNode = findLowerBound(node->left, key);
        else if (node->right and !isValid) foundNode = findLowerBound(node->right, key);
        if (isValid) {
            if (!foundNode) return node;
            else if (Compare()(*(foundNode->data), *(node->data))) return foundNode;
            else return node;
        }
        else return foundNode;
    }
    RBNode* findUpperBound(RBNode* node, const T& key) const {//key < data
        bool isValid = false;
        RBNode* foundNode = nullptr;
        if (Compare()(key, *(node->data))) isValid = true;
        if (node->left and Compare()(key, *(node->data))) foundNode = findUpperBound(node->left, key);
        else if (node->right and !isValid) foundNode = findUpperBound(node->right, key);
        if (isValid) {
            if (!foundNode) return node;
            else if (Compare()(*(foundNode->data), *(node->data))) return foundNode;
            else return node;
        }
        else return foundNode;
    }
    inline bool inRange(const T& data, const T& l, const T& r) const {
        return !Compare()(data, l) and !Compare()(r, data);
    }
    size_t findRange(RBNode* node, const T& l, const T& r) const {
        int ret = 0;
        if (inRange(*(node->data), l, r)) {
            ++ret;
            if (node->left and !isEqual(*(node->data), l)) ret += findRange(node->left, l, r);
            if (node->right and !isEqual(*(node->data), r)) ret += findRange(node->right, l, r);
        }
        else {
            if (Compare()(*(node->data), l)) {
                if (node->right) ret += findRange(node->right, l, r);
            }
            else {
                if (node->left) ret += findRange(node->left, l, r);
            }
        }
        return ret;
    }
};
template<class T, class Compare = std::less<T>>
class ESet {
public:
    class iterator;
    RBTree<T, Compare>* table = nullptr;
private:
    inline void fixBeginEnd() {
        if (!size()) {
            beginIt.node = nullptr;
            endIt.node = nullptr;
        }
        else {
            beginIt = iterator(table->findMinNode(table->root), this);
            endIt = iterator(table->findMaxNode(table->root), this);
        }
    }
public:
    iterator beginIt = end(), endIt = end();//actually, this is a fake endIt(the one before end)
    ESet() {
        table = new RBTree<T, Compare>();
        beginIt = endIt = iterator(nullptr, this);
    }
    ~ESet() {
        delete table;
        table = nullptr;
    }
    class iterator {
    public:
        typename RBTree<T, Compare>::RBNode* node;
        const ESet<T, Compare>* cont;
        iterator() {
            node = nullptr;
            cont = nullptr;
        }
        iterator(typename RBTree<T, Compare>::RBNode* node, const ESet<T, Compare>* cont) : node(node), cont(cont) {}
        iterator(const iterator& other) {
            node = other.node, cont = other.cont;
        }
        iterator& operator=(const iterator& other) {
            if (this == &other) return (*this);
            node = other.node, cont = other.cont;
            return (*this);
        }
        T& operator*() const {
            if (!node) throw("iterator pointing to null");
            return *(node->data);
        }
        const T* operator->() const noexcept {
            if (!node) throw("iterator pointing to null");
            return node->data;
        }
        T* get() const noexcept {
            if (!node) return nullptr;
            return node->data;
        }
        bool operator==(const iterator& rhs) const {
            return node == rhs.node and cont == rhs.cont;
        }
        bool operator!=(const iterator& rhs) const {
            return node != rhs.node or cont != rhs.cont;
        }
        //it++
        iterator operator++(int) {
            if (!node) return (*this);
            auto ret = (*this);
            if ((*this) == cont->endIt) {
                node = nullptr;
                return ret;
            }
            auto now = node;
            typename RBTree<T, Compare>::RBNode* last = nullptr;
            while (true) {
                if (now->right and now->right != last) {
                    now = now->right;
                    while (now->left) now = now->left;
                    node = now;
                    break;
                }
                else {
                    if (now == now->father->left) {
                        now = now->father;
                        node = now;
                        break;
                    }
                    else {
                        last = now;
                        now = now->father;
                    }
                }
            }
            return ret;
        }
        //++it
        iterator& operator++() {
            if (!node) return (*this);
            if ((*this) == cont->endIt) {
                node = nullptr;
                return (*this);
            }
            auto now = node;
            typename RBTree<T, Compare>::RBNode* last = nullptr;
            while (true) {
                if (now->right and now->right != last) {
                    now = now->right;
                    while (now->left) now = now->left;
                    node = now;
                    break;
                }
                else {
                    if (now == now->father->left) {
                        now = now->father;
                        node = now;
                        break;
                    }
                    else {
                        last = now;
                        now = now->father;
                    }
                }
            }
            return (*this);
        }
        //it--
        iterator operator--(int) {
            if (node == cont->beginIt.node or !cont->size()) return (*this);
            auto ret = (*this);
            if (node == nullptr) {
                (*this) = cont->endIt;
                return ret;
            }
            auto now = node;
            typename RBTree<T, Compare>::RBNode* last = nullptr;
            while (true) {
                if (now->left and now->left != last) {
                    now = now->left;
                    while (now->right) now = now->right;
                    node = now;
                    break;
                }
                else {
                    if (now == now->father->right) {
                        now = now->father;
                        node = now;
                        break;
                    }
                    else {
                        last = now;
                        now = now->father;
                    }
                }
            }
            return ret;
        }
        //--it
        iterator& operator--() {
            if (node == cont->beginIt.node or !cont->size()) {
                return (*this);
            }
            if (node == nullptr) {
                (*this) = cont->endIt;
                return (*this);
            }
            auto now = node;
            typename RBTree<T, Compare>::RBNode* last = nullptr;
            while (true) {
                if (now->left and now->left != last) {
                    now = now->left;
                    while (now->right) now = now->right;
                    node = now;
                    break;
                }
                else {
                    if (now == now->father->right) {
                        now = now->father;
                        node = now;
                        break;
                    }
                    else {
                        last = now;
                        now = now->father;
                    }
                }
            }
            return (*this);
        }
    };
    std::pair<iterator, bool> emplace(T&& args) {
        auto ret = table->insert(std::forward<T>(args));
        fixBeginEnd();
        return std::pair<iterator, bool>(iterator(ret.first, this), ret.second);
    }
    template< class... Args >
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto ret = table->insert(std::forward<T>(args)...);
        fixBeginEnd();
        return std::pair<iterator, bool>(iterator(ret.first, this), ret.second);
    }
    size_t erase(const T& key) {
        auto ret = table->erase(&(table->root), key);
        if (size()) fixBeginEnd();
        return ret;
    }
    iterator find(const T& key) const {
        if (!table) return end();
        auto ret = table->find(table->root, key);
        if (ret) return iterator(ret, this);
        else return end();
    }
    ESet(const ESet& other) {
        table = new RBTree<T, Compare>();
        if (!other.size()) return;
        std::queue<typename RBTree<T, Compare>::RBNode*> qthis, qother;
        table->root = new typename RBTree<T, Compare>::RBNode();
        qother.push(other.table->root);
        qthis.push(table->root);
        while (!qother.empty()) {
            auto nowother = qother.front(), nowthis = qthis.front();
            qother.pop(), qthis.pop();
            *(nowthis) = *(nowother);
            if (nowother->left) {
                nowthis->left = new typename RBTree<T, Compare>::RBNode();
                nowthis->left->father = nowthis;
                qother.push(nowother->left);
                qthis.push(nowthis->left);
            }
            if (nowother->right) {
                nowthis->right = new typename RBTree<T, Compare>::RBNode();
                nowthis->right->father = nowthis;
                qother.push(nowother->right);
                qthis.push(nowthis->right);
            }
        }
        fixBeginEnd();
    }
    ESet& operator=(const ESet& other) {
        if (&other == this) return (*this);
        delete table;
        table = nullptr;
        table = new RBTree<T, Compare>();
        if (!other.size()) return (*this);
        std::queue<typename RBTree<T, Compare>::RBNode*> qthis, qother;
        table->root = new typename RBTree<T, Compare>::RBNode();
        qother.push(other.table->root);
        qthis.push(table->root);
        while (!qother.empty()) {
            auto nowother = qother.front(), nowthis = qthis.front();
            qother.pop(), qthis.pop();
            *(nowthis) = *(nowother);
            if (nowother->left) {
                nowthis->left = new typename RBTree<T, Compare>::RBNode();
                nowthis->left->father = nowthis;
                qother.push(nowother->left);
                qthis.push(nowthis->left);
            }
            if (nowother->right) {
                nowthis->right = new typename RBTree<T, Compare>::RBNode();
                nowthis->right->father = nowthis;
                qother.push(nowother->right);
                qthis.push(nowthis->right);
            }
        }
        fixBeginEnd();
        return (*this);
    }
    ESet(ESet&& other) {
        table = other.table;
        other.table = nullptr;
        fixBeginEnd();
    }
    ESet& operator=(ESet&& other) noexcept {
        if (&other == this) return (*this);
        delete table;
        table = other.table;
        other.table = nullptr;
        fixBeginEnd();
        return (*this);
    }

    size_t range(const T& l, const T& r) const {
        if (Compare()(r, l)) return 0;
        if (size()) return table->findRange(table->root, l, r);
        else return 0;
    }
    size_t size() const noexcept {
        return table->getSize(table->root);
    }
    iterator lower_bound(const T& key) const {
        if (size()) return iterator(table->findLowerBound(table->root, key), this);
        else return end();
    }
    iterator upper_bound(const T& key) const {
        if (size()) return iterator(table->findUpperBound(table->root, key), this);
        else return end();
    }
    iterator begin() const noexcept {
        if (size()) return beginIt;
        else return end();
    }
    iterator end() const noexcept {
        return iterator(nullptr, this);
    }
    void traverse() {
        if (!table) std::cout << "Tree is empty!" << std::endl;
        else table->traverse(table->root);
    }
};

#endif