#ifndef SJTU_AVLTree
#define SJTU_AVLTree
#include <cstddef>
#include <queue>

template<class T, class Compare = std::less<T>>
class AVL {
private:
    typedef struct Node {
        /*credit to
        https://ivanzz1001.github.io/records/post/data-structure/2018/06/14/ds-avl_tree
        for this clever trick
        */
        int height; //subtree's height
        int size;   //subtree's size(node count)
        T* data;
        struct Node* left;
        struct Node* right;
        Node(int height, int size, T* data, struct Node* left, struct Node* right)
            :height(height), size(size), data(data), left(left), right(right) {}
        Node() {
            left = right = nullptr;
            delete data;
            data = nullptr;
        }
    }AVLNode, * AVLTree;
    int size = 0;
    inline int getHeight(AVLNode* node) {
        if (!node) return 0;
        return node->height;
    }
    inline void refreshHeight(AVLNode* node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }
public:
    AVLNode* root = nullptr;
    AVL(){

    }
    ~AVL(){
        std::queue<AVLNode*> q;
        if(!root) return;
        q.push(root);
        while(!q.empty()){
            auto now = q.front();
            q.pop();
            if(now->left) q.push(now->left);
            if(now->right) q.push(now->right);
            delete now;
        }
    }
    void leftRotate(AVLTree* node) {
        AVLNode* X = *node, * XR = X->right;
        X->right = XR->left;
        XR->left = X;
        *node = XR;
        refreshHeight(X), refreshHeight(XR);
    }
    void rightRotate(AVLTree* node) {
        AVLNode* X = *node, * XL = X->left;
        X->left = XL->right;
        XL->right = X;
        *node = XL;
        refreshHeight(X), refreshHeight(XL);
    }
    AVLNode* find(AVLNode* node, T data) {
        if (!node) return nullptr;
        if (*(node->data) == data) return node;
        if (Compare()(data, *(node->data))) {
            if (node->left) return find(node->left);
            else return nullptr;
        }
        else {
            if (node->right) return find(node->right);
            else return nullptr;
        }
    }
    std::pair<AVLTree*, bool> insert(AVLTree* node, T data) {
        if (!*node) {
            auto created = new AVLNode(1, 1, new T(data), nullptr, nullptr);
            node = &created;
            // root = created;
            // std::cout<<root<<std::endl;
            // std::cout<<node<<std::endl;
            return { node, true };
        }
        if (*((*node)->data) == data) return { node, false };
        else if (Compare()(data, *((*node)->data))) {//insert in left subtree
            auto ret = insert(&((*node)->left), data);
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
            return ret;
        }
        else {//insert in right subtree
            auto ret = insert(&((*node)->right), data);
            if (!ret.second) return { ret.first, false };
            int lheight = getHeight((*node)->left), rheight = getHeight((*node)->right);
            if (rheight - lheight == 2) {
                if (Compare()(data, *((*node)->left->data))) {//RL
                    rightRotate(&((*node)->right));
                    leftRotate(node);
                }
                else {//RR
                    leftRotate(node);
                }
            }
            refreshHeight(*node);
            return ret;
        }
    }
    bool erase(AVLTree* node);
};

template<class T, class Compare = std::less<T>>
class ESet {
    ESet();
    ~ESet();
    class iterator {
        iterator();

        T& operator*() const;
        T* operator->() const noexcept;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
        //it++
        iterator operator++(int);
        //++it
        iterator operator++();
        //it--
        iterator operator--(int);
        //--it
        iterator operator--();
    };
    template< class... Args >
    std::pair<iterator, bool> emplace(Args&&... args);
    size_t erase(const T& key);
    iterator find(const T& key) const;
    ESet(const ESet& other);
    ESet& operator=(const ESet& other);
    ESet(ESet&& other);
    ESet& operator=(ESet&& other) noexcept;
    size_t range(const T& l, const T& r) const;
    size_t size() const noexcept;
    iterator lower_bound(const T& key) const;
    iterator upper_bound(const T& key) const;
    iterator begin() const noexcept;
    iterator end() const noexcept;
};

#endif