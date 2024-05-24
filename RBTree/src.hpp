#ifndef SJTU_RBTree
#define SJTU_RBTree
#include <cstddef>
#include <iostream>

template<class T, class Compare = std::less<T>>
class RBTree {
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
        ~RBNode() {
            father = left = right = nullptr;
            delete data;
            data = nullptr;
        }
    };
    RBNode* root;   //The root node
    RBNode* hot;    //The node that is being looked at
    inline int getSize(RBNode* node) {
        if (!node) return 0;
        return node->size;
    }
    bool isEqual(const T& a, const T& b) const {
        return !Compare()(a, b) and !Compare()(b, a);
    }



    /*
    **function for checking correctness and utilities
    */
    void traverse(RBNode* node) {
        if (!node) {
            std::cout << "TREE IS EMPTY!" << std::endl;
            return;
        }
        std::cout << *(node->data) << ' ' << (node->size) << ' ' << (node->color) << ' ';
        if (node->father) std::cout << *(node->father->data) << std::endl;
        else std::cout << "nullptr" << std::endl;
        if (node->left) traverse(node->left);
        if (node->right) traverse(node->right);
    }
    RBNode* findLowerBound(RBNode* node, const T& key) {//key <= data
        bool isValid = false;
        AVLNode* foundNode = nullptr;
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
    RBNode* findUpperBound(RBNode* node, const T& key) {//key < data
        bool isValid = false;
        AVLNode* foundNode = nullptr;
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
        auto ret = table->insert(&(table->root), std::forward<T>(args));
        fixBeginEnd();
        return std::pair<iterator, bool>(iterator(ret.first, this), ret.second);
    }
    template< class... Args >
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto ret = table->insert(&(table->root), std::forward<T>(args)...);
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
    // void traverse() {
    //     if (!table) std::cout << "Tree is empty!" << std::endl;
    //     else table->traverse(table->root);
    // }
};

#endif