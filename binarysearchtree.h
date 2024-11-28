#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <memory>
#include <vector>
#include <functional>

struct BSTNode {
    int value;
    std::shared_ptr<BSTNode> left;
    std::shared_ptr<BSTNode> right;
    
    explicit BSTNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}
    
    bool insert(int value);
    bool remove(int value);
    std::vector<int> search(int value) const;
    void clear();
    
    std::vector<int> inorderTraversal() const;
    std::vector<int> preorderTraversal() const;
    std::vector<int> postorderTraversal() const;
    
    std::shared_ptr<BSTNode> getRoot() const { return root; }
    bool isEmpty() const { return root == nullptr; }
    
    std::vector<int> serialize() const;
    void deserialize(const std::vector<int>& nodes);

private:
    std::shared_ptr<BSTNode> root;
    
    bool searchRecursive(const std::shared_ptr<BSTNode>& node, int value, std::vector<int>& path) const;
    std::shared_ptr<BSTNode> removeRecursive(std::shared_ptr<BSTNode> node, int value, bool& success);
    std::shared_ptr<BSTNode> findMin(const std::shared_ptr<BSTNode> node) const;
    void clearRecursive(std::shared_ptr<BSTNode>& node);
    void traverseInorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const;
    void traversePreorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const;
    void traversePostorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const;
};

#endif // BINARYSEARCHTREE_H
