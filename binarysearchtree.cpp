#include "binarysearchtree.h"
#include <stdexcept>

bool BinarySearchTree::insert(int value) {
    if (!root) {
        root = std::make_shared<BSTNode>(value);
        return true;
    }
    
    std::shared_ptr<BSTNode> current = root;
    std::shared_ptr<BSTNode> parent;
    
    while (current) {
        if (value == current->value) {
            return false;  // Value already exists
        }
        
        parent = current;
        if (value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    if (value < parent->value) {
        parent->left = std::make_shared<BSTNode>(value);
    } else {
        parent->right = std::make_shared<BSTNode>(value);
    }
    
    return true;
}

bool BinarySearchTree::remove(int value) {
    bool success = false;
    root = removeRecursive(root, value, success);
    return success;
}

std::shared_ptr<BSTNode> BinarySearchTree::removeRecursive(std::shared_ptr<BSTNode> node, int value, bool& success) {
    if (!node) {
        success = false;
        return nullptr;
    }
    
    if (value < node->value) {
        node->left = removeRecursive(node->left, value, success);
    } else if (value > node->value) {
        node->right = removeRecursive(node->right, value, success);
    } else {
        success = true;
        
        if (!node->left) {
            return node->right;
        }
        if (!node->right) {
            return node->left;
        }
        
        auto minNode = findMin(node->right);
        node->value = minNode->value;
        node->right = removeRecursive(node->right, minNode->value, success);
    }
    
    return node;
}

std::vector<int> BinarySearchTree::search(int value) const {
    std::vector<int> path;
    searchRecursive(root, value, path);
    return path;
}

bool BinarySearchTree::searchRecursive(const std::shared_ptr<BSTNode>& node, int value, std::vector<int>& path) const {
    if (!node) {
        return false;
    }
    
    path.push_back(node->value);
    
    if (value == node->value) {
        return true;
    }
    
    if (value < node->value) {
        return searchRecursive(node->left, value, path);
    } else {
        return searchRecursive(node->right, value, path);
    }
}

void BinarySearchTree::clear() {
    clearRecursive(root);
}

void BinarySearchTree::clearRecursive(std::shared_ptr<BSTNode>& node) {
    if (node) {
        clearRecursive(node->left);
        clearRecursive(node->right);
        node = nullptr;
    }
}

std::shared_ptr<BSTNode> BinarySearchTree::findMin(std::shared_ptr<BSTNode> node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// New traversal implementations
std::vector<int> BinarySearchTree::inorderTraversal() const {
    std::vector<int> result;
    traverseInorder(root, result);
    return result;
}

std::vector<int> BinarySearchTree::preorderTraversal() const {
    std::vector<int> result;
    traversePreorder(root, result);
    return result;
}

std::vector<int> BinarySearchTree::postorderTraversal() const {
    std::vector<int> result;
    traversePostorder(root, result);
    return result;
}

void BinarySearchTree::traverseInorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const {
    if (node) {
        traverseInorder(node->left, result);
        result.push_back(node->value);
        traverseInorder(node->right, result);
    }
}

void BinarySearchTree::traversePreorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const {
    if (node) {
        result.push_back(node->value);
        traversePreorder(node->left, result);
        traversePreorder(node->right, result);
    }
}

void BinarySearchTree::traversePostorder(const std::shared_ptr<BSTNode>& node, std::vector<int>& result) const {
    if (node) {
        traversePostorder(node->left, result);
        traversePostorder(node->right, result);
        result.push_back(node->value);
    }
}

// Serialization methods
std::vector<int> BinarySearchTree::serialize() const {
    return preorderTraversal();  // We use preorder traversal for serialization
}

void BinarySearchTree::deserialize(const std::vector<int>& nodes) {
    clear();
    for (int value : nodes) {
        insert(value);
    }
}
