#ifndef SJTU_AVLTree
#define SJTU_AVLTree
#include <cstddef>

template<class T, class Compare = std::less<T>>
class AVL<T> {
public:
    struct AVLNode {
        /*This is a clever trick from
         https://ivanzz1001.github.io/records/post/data-structure/2018/06/14/ds-avl_tree
        */
        int height;
        T data;
        struct AVLNode* left;
        struct AVLNode* right;
    };
private:
    AVLNode* AVLRoot;
    int size = 0;
public:
    void leftRotate(AVLNode* X);
    void rightRotate(AVLNode* X);
    AVLNode* find(AVLNode* X);
    std::pair<AVLNode*, bool> insert(AVLNode* X);
    bool erase(AVLNode* X);
}

template<class T, class Compare = std::less<T>>
class ESet<T> {
    Eet();
    ~Eset();
    template< class... Args >
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
    }
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
}

#endif