#ifndef SJTU_AVLTree
#define SJTU_AVLTree
#include <cstddef>
#include <queue>
#include <iostream>

template<class T, class Compare = std::less<T>>
class AVL {
public:
    typedef struct Node {
        /*credit to
        https://ivanzz1001.github.io/records/post/data-structure/2018/06/14/ds-avl_tree
        for this clever trick
        */
        int height; //subtree's height
        int size;   //subtree's size(node count)
        T* data;
        struct Node* father;
        struct Node* left;
        struct Node* right;
        Node() {
            father = left = right = nullptr;
            data = nullptr;
            height = 1;
            size = 1;
        }
        Node(int height, int size, T* data, struct Node* father, struct Node* left, struct Node* right)
            :height(height), size(size), data(data), father(father), left(left), right(right) {}
        Node(const Node& other) {
            height = other.height;
            size = other.height;
            data = new T(*(other.data));
            father = nullptr;
            left = nullptr;
            right = nullptr;
        }
        ~Node() {
            father = left = right = nullptr;
            delete data;
            data = nullptr;
        }
        Node& operator=(const Node& other) {
            if (&other == this) return (*this);
            height = other.height;
            size = other.size;
            data = new T(*(other.data));
            left = right = nullptr;
            return (*this);
        }
    }AVLNode, * AVLTree;
    // int size = 0;
    inline int getHeight(AVLNode* node) {
        if (!node) return 0;
        return node->height;
    }
    inline int getSize(AVLNode* node) {
        if (!node) return 0;
        return node->size;
    }
    inline void refreshHeight(AVLNode* node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        node->size = 1 + getSize(node->left) + getSize(node->right);
    }
public:
    AVLNode* root = nullptr;
    AVL() {}
    ~AVL() {
        std::queue<AVLNode*> q;
        if (!root) return;
        q.push(root);
        while (!q.empty()) {
            auto now = q.front();
            // std::cout<<now<<std::endl;
            q.pop();
            if (now->left) {
                q.push(now->left);
                // std::cout<<"pushed left="<<now->left<<std::endl;
            }
            if (now->right) {
                q.push(now->right);
                // std::cout<<"pushed right="<<now->right<<std::endl;
            }
            // delete now->data;
            delete now;
            now = nullptr;
        }
        root = nullptr;
        // std::cout << "end of avl" << std::endl;
    }
    AVLNode* findMinNode(AVLNode* node) {
        while (node->left) node = node->left;
        return node;
    }
    AVLNode* findMaxNode(AVLNode* node) {
        while (node->right) node = node->right;
        return node;
    }
    void leftRotate(AVLTree* node) {
        AVLNode* X = *node, * XR = X->right;
        if (XR->left) XR->left->father = X;
        XR->father = X->father;
        X->father = XR;
        X->right = XR->left;
        XR->left = X;
        *node = XR;
        refreshHeight(X), refreshHeight(XR);
    }
    void rightRotate(AVLTree* node) {
        AVLNode* X = *node, * XL = X->left;
        if (XL->right) XL->right->father = X;
        XL->father = X->father;
        X->father = XL;
        X->left = XL->right;
        XL->right = X;
        *node = XL;
        refreshHeight(X), refreshHeight(XL);
    }
    AVLNode* find(AVLNode* node, const T& data) {
        if (!node) return nullptr;
        if (*(node->data) == data) return node;
        if (Compare()(data, *(node->data))) {
            if (node->left) return find(node->left, data);
            else return nullptr;
        }
        else {
            if (node->right) return find(node->right, data);
            else return nullptr;
        }
    }
    std::pair<AVLTree*, bool> insert(AVLTree* node, const T& data) {
        if (!*node) {
            *node = new AVLNode(1, 1, new T(data), nullptr, nullptr, nullptr);
            return { node, true };
        }
        if (*((*node)->data) == data) return { node, false };
        else if (Compare()(data, *((*node)->data))) {//insert in left subtree
            auto ret = insert(&((*node)->left), data);
            auto newnode = *ret.first;
            if (!ret.second) return { ret.first, false };
            int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
            if (lheight - rheight == 2) {
                if (Compare()(data, *((*node)->left->data))) {//LL
                    rightRotate(node);
                }
                else {//LR
                    leftRotate(&((*node)->left));
                    rightRotate(node);
                }
            }
            refreshHeight(*node);
            if((*node)->left) (*node)->left->father = *node;
            return std::make_pair<AVLTree*, bool>(&newnode, true);
        }
        else {//insert in right subtree
            auto ret = insert(&((*node)->right), data);
            auto newnode = *ret.first;
            if (!ret.second) return { ret.first, false };
            int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
            if (rheight - lheight == 2) {
                if (Compare()(data, *((*node)->right->data))) {//RL
                    rightRotate(&((*node)->right));
                    leftRotate(node);
                }
                else {//RR
                    leftRotate(node);
                }
            }
            refreshHeight(*node);
            if((*node)->right) (*node)->right->father = *node;
            return std::make_pair<AVLTree*, bool>(&newnode, true);
        }
    }
    bool erase(AVLTree* node, const T& data) {
        if (!(*node)) return false;
        if (*((*node)->data) == data) {
            if ((*node)->left and (*node)->right) {
                auto key = *(findMinNode((*node)->right))->data;
                *((*node)->data) = key;
                erase(&((*node)->right), key);
                int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
                if (lheight - rheight == 2) {
                    if (getHeight((*node)->left->left) >= getHeight((*node)->left->right)) {//LL
                        rightRotate(node);
                    }
                    else {//LR
                        leftRotate(&((*node)->left));
                        rightRotate(node);
                    }
                }
                refreshHeight(*node);
                if((*node)->left) (*node)->left->father = *node;
                if((*node)->right) (*node)->right->father = *node;
                return true;
            }
            else if (!(*node)->left) {
                auto now = *node;
                if (now->right) now->right->father = now->father;
                *node = now->right;
                // delete now->data;
                delete now;
                now = nullptr;
                return true;
            }
            else {
                auto now = *node;
                if (now->left) now->left->father = now->father;
                *node = now->left;
                // delete now->data;
                delete now;
                now = nullptr;
                return true;
            }
        }
        else if (Compare()(data, *((*node)->data))) {//erase in left subtree
            if (!erase(&((*node)->left), data)) {
                return false;
            }
            int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
            if (rheight - lheight == 2) {
                if (getHeight((*node)->right->right) >= getHeight((*node)->right->left)) {//RR
                    leftRotate(node);
                }
                else {//RL
                    rightRotate(&((*node)->right));
                    leftRotate(node);
                }
            }
            refreshHeight(*node);
            if ((*node)->left) (*node)->left->father = *node;
            return true;
        }
        else {//erase in right subtree
            if (!erase(&((*node)->right), data)) {
                return false;
            }
            int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
            if (lheight - rheight == 2) {
                if (getHeight((*node)->left->left) >= getHeight((*node)->left->right)) {//LL
                    rightRotate(node);
                }
                else {//LR
                    leftRotate(&((*node)->left));
                    rightRotate(node);
                }
            }
            refreshHeight(*node);
            if ((*node)->right) (*node)->right->father = *node;
            return true;
        }
    }

    /*
    **function for checking correctness and utilities
    */
    void traverse(AVLTree node) {
        if (!node) {
            std::cout << "TREE IS EMPTY!" << std::endl;
            return;
        }
        std::cout << *(node->data) << ' ' << (node->size) << ' ';
        if (node->father) std::cout << *(node->father->data) << std::endl;
        else std::cout << "nullptr" << std::endl;
        if (node->left) traverse(node->left);
        if (node->right) traverse(node->right);
    }
    AVLNode* findLowerBound(AVLNode* node, const T& key) {
        if (key == *((*node)->data) or (!(node->left) and !(node->right))) return node;
        else if (Compare()(key, *((*node)->data)))
            return findLowerBound(node->left, key);
        else return findLowerBound(node->right, key);
    }
    AVLNode* findUpperBound(AVLNode* node, const T& key) {
        if (!(node->left) and !(node->right)) return node;
        else if (Compare()(key, *((*node)->data))) {
            auto now = node;
            auto foundKey = *(findMaxNode(node->left)->data);
            if (Compare()(foundKey, key) or foundKey == key) {
                return node;
            }
            else {
                return findUpperBound(node->left, key);
            }
        }
        else {
            return findUpperBound(node->right, key);
        }
    }
    inline bool inRange(const T& data, const T& l, const T& r) const {
        return (Compare()(l, data) or l == data) and
            (Compare()(data, r) or r == data);
    }
    size_t findRange(AVLNode* node, const T& l, const T& r) const {
        int ret = 0;
        if (inRange(*(node->data), l, r)) {
            ++ret;
            if (node->left and *(node->data) != l) ret += findRange(node->left, l, r);
            if (node->right and *(node->data) != r) ret += findRange(node->right, l, r);
        }
        else {
            if (Compare()(*(node->data), l)) {
                if(node->right) ret += findRange(node->right, l, r);
            }
            else {
                if(node->left) ret += findRange(node->left, l, r);
            }
        }
        return ret;
    }
};

template<class T, class Compare = std::less<T>>
class ESet {
public:
    class iterator;
private:
    AVL<T>* table = nullptr;
    inline void fixBeginEnd() {
        beginIt = iterator(table->findMinNode(table->root), const_cast<ESet<T>*>(this));
        endIt = iterator(table->findMaxNode(table->root), const_cast<ESet<T>*>(this));
    }
public:
    iterator beginIt, endIt;//actually, this is a fake endIt(the one before end)
    ESet() {
        table = new AVL<T>();
        beginIt = endIt = iterator(nullptr, const_cast<ESet<T>*>(this));
    }
    ~ESet() {
        delete table;
        table = nullptr;
    }
    class iterator {
    public:
        typename AVL<T>::AVLNode* node;
        const ESet<T>* cont;
        iterator() {}
        iterator(typename AVL<T>::AVLNode* node, const ESet<T>* cont) : node(node), cont(cont) {}
        iterator(const iterator& other){
            node = other.node, cont = other.cont;
        }
        iterator& operator=(const iterator& other){
            if(this == &other) return (*this);
            node = other.node, cont = other.cont;
            return (*this);
        }
        T& operator*() const {
            if (!node) throw("iterator pointing to null");
            return *(node->data);
        }
        T* operator->() const noexcept {
            if (!node) throw("iterator pointing to null");
            return node->data;
        }
        T* get() const noexcept {
            if (!node) return nullptr;
            return node->data;
        }
        bool operator==(const iterator& rhs) const {
            return node == rhs.node;
        }
        bool operator!=(const iterator& rhs) const {
            return node != rhs.node;
        }
        //it++
        iterator operator++(int) {
            if(!node) return (*this);
            auto ret = (*this);
            if ((*this) == cont->endIt) {
                node = nullptr;
                return ret;
            }
            auto now = node;
            typename AVL<T>::AVLNode* last = nullptr;
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
        iterator operator++() {
            if(!node) return (*this);
            if ((*this) == cont->endIt) {
                node = nullptr;
                return (*this);
            }
            auto now = node;
            typename AVL<T>::AVLNode* last = nullptr;
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
            if((*this) == cont->beginIt) return (*this);
            auto ret = (*this);
            if (node == nullptr) {
                (*this) = cont->endIt;
                return ret;
            }
            // if ((*this) == cont->beginIt) throw("begin--");
            auto now = node;
            typename AVL<T>::AVLNode* last = nullptr;
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
        iterator operator--() {
            if((*this) == cont->beginIt) return (*this);
            if (node == nullptr) {
                (*this) = cont->endIt;
                return (*this);
            }
            // if ((*this) == cont->beginIt) throw("begin--");
            auto now = node;
            typename AVL<T>::AVLNode* last = nullptr;
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
        auto ret = table->insert(&(table->root), std::forward<T>(args));
        fixBeginEnd();
        return std::make_pair(iterator(*(ret.first), const_cast<ESet<T>*>(this)), ret.second);
    }
    template< class... Args >
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto ret = table->insert(&(table->root), std::forward<T>(args)...);
        fixBeginEnd();
        return std::make_pair(iterator(*(ret.first), const_cast<ESet<T>*>(this)), ret.second);
    }
    size_t erase(const T& key) {
        auto ret = table->erase(&(table->root), key);
        if (size()) fixBeginEnd();
        return ret;
    }
    iterator find(const T& key) const {
        if(!table) return end();
        auto ret = table->find(table->root, key);
        if (ret) return iterator(ret, const_cast<ESet<T>*>(this));
        else return end();
    }
    ESet(const ESet& other) {
        table = new AVL<T>();
        if (!other.size()) return;
        std::queue<typename AVL<T>::AVLNode*> qthis, qother;
        table->root = new typename AVL<T>::AVLNode();
        // table->root = other->table->root;
        qother.push(other.table->root);
        qthis.push(table->root);
        while (!qother.empty()) {
            auto nowother = qother.front(), nowthis = qthis.front();
            qother.pop(), qthis.pop();
            *(nowthis) = *(nowother);
            if (nowother->left) {
                nowthis->left = new typename AVL<T>::AVLNode();
                nowthis->left->father = nowthis;
                qother.push(nowother->left);
                qthis.push(nowthis->left);
            }
            if (nowother->right) {
                nowthis->right = new typename AVL<T>::AVLNode();
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
        table = new AVL<T>();
        if (!other.size()) return (*this);
        std::queue<typename AVL<T>::AVLNode*> qthis, qother;
        table->root = new typename AVL<T>::AVLNode();
        // table->root = other->table->root;
        qother.push(other.table->root);
        qthis.push(table->root);
        while (!qother.empty()) {
            auto nowother = qother.front(), nowthis = qthis.front();
            qother.pop(), qthis.pop();
            *(nowthis) = *(nowother);
            if (nowother->left) {
                nowthis->left = new typename AVL<T>::AVLNode();
                nowthis->left->father = nowthis;
                qother.push(nowother->left);
                qthis.push(nowthis->left);
            }
            if (nowother->right) {
                nowthis->right = new typename AVL<T>::AVLNode();
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
        delete table;
        table = other.table;
        other.table = nullptr;
        fixBeginEnd();
        return (*this);
    }

    size_t range(const T& l, const T& r) const {
        if (Compare()(r, l)) return 0;
        return table->findRange(table->root, l, r);
    }
    size_t size() const noexcept {
        return table->getSize(table->root);
    }
    iterator lower_bound(const T& key) const {
        return iterator(table->findLowerBound(table->root, key), const_cast<ESet<T>*>(this));
    }
    iterator upper_bound(const T& key) const {
        return iterator(table->findUpperBound(table->root, key), const_cast<ESet<T>*>(this));
    }
    iterator begin() const noexcept {
        return beginIt;
    }
    iterator end() const noexcept {
        return iterator(nullptr, const_cast<ESet<T>*>(this));
    }
    void traverse() {
        if(!table) std::cout<<"Tree is empty!"<<std::endl;
        else table->traverse(table->root);
    }
};

#endif