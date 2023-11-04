#include "red_black_tree.h"

#include <iostream>
#include <queue>
#include <sstream>
#include <stack>

namespace rbt
{

/**
 * #########################################################################
 * #########################################################################
 * ####################  RedBlackTree implementations.  ####################
 * #########################################################################
 * #########################################################################
 */

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
RED_BLACK_TREE_TYPE::~RedBlackTree()
{
    Clear();
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
bool RED_BLACK_TREE_TYPE::Insert(const KeyType& key, const ValueType& value)
{
    // If root is null.
    if (!root_) {
        root_ = new RedBlackTreeNode{.key = key, .value = value, .color = ColorType::black};
        size_++;
        return true;
    }

    // If key is duplicated.
    if (auto [_, success] = GetValue(key); success) {
        return false;
    }

    /**
     * Insert key-value pair into red black tree.
     */
    // Find insertion place.
    node_ = parent_node_ = grand_parent_node_ = grand_grand_parent_node_ = root_;
    while (node_) {
        // If node's left and right are red, need to reorient.
        if (node_->left && node_->right && node_->left->color == ColorType::red && node_->right->color == ColorType::red) {
            HandleReorient(key);
        }

        grand_grand_parent_node_ = grand_parent_node_;
        grand_parent_node_ = parent_node_;
        parent_node_ = node_;
        node_ = key_comparator_(key, node_->key) ? node_->left : node_->right;
    }

    // Insertion.
    size_++;
    node_ = new RedBlackTreeNode{.key = key, .value = value, .color = ColorType::red};
    if (key_comparator_(key, parent_node_->key)) {
        parent_node_->left = node_;
    } else {
        parent_node_->right = node_;
    }

    // Check if needs reorient.
    HandleReorient(key);

    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
bool RED_BLACK_TREE_TYPE::Erase(const KeyType& key)
{
    // TODO: Need to implement.
    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
auto RED_BLACK_TREE_TYPE::GetValue(const KeyType& key) -> std::pair<ValueType, bool>
{
    RedBlackTreeNode* ptr = root_;

    while (ptr) {
        if (key == ptr->key) {
            return {ptr->value, true};
        }

        ptr = key_comparator_(key, ptr->key) ? ptr->left : ptr->right;
    }

    return {{}, false};
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
void RED_BLACK_TREE_TYPE::Clear()
{
    std::stack<RedBlackTreeNode*> node_stack;
    node_stack.push(root_);

    while (!node_stack.empty()) {
        RedBlackTreeNode* ptr = node_stack.top();
        node_stack.pop();

        if (ptr->left) {
            node_stack.push(ptr->left);
        }
        if (ptr->right) {
            node_stack.push(ptr->right);
        }

        delete ptr;
    }

    root_ = nullptr;
    size_ = 0;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
void RED_BLACK_TREE_TYPE::PrintTree()
{
    std::ostringstream os;
    std::queue<RedBlackTreeNode*> print_queue;
    print_queue.push(root_);

    while (!print_queue.empty()) {
        std::vector<RedBlackTreeNode*> line;
        while (!print_queue.empty()) {
            line.push_back(print_queue.front());
            print_queue.pop();
        }

        if (std::all_of(line.begin(), line.end(), [](const auto& node) { return node == nullptr; })) {
            goto end;
        }

        size_t count = 0;
        for (const auto node : line) {
            if (node) {
                print_queue.push(node->left);
                print_queue.push(node->right);
                os << '[' << node->key << ' ' << (node->color == ColorType::red ? "red" : "black") << ']';
            } else {
                print_queue.push(nullptr);
                print_queue.push(nullptr);
                os << "nil";
            }
            if (++count != line.size()) {
                os << ", ";
            }
        }
        os << '\n';
    }

end:
    std::cout << os.str() << std::endl;
}

/**
 * Private methods.
 */

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
void RED_BLACK_TREE_TYPE::HandleReorient(const KeyType& key)
{
    if (node_->left) {
        node_->left->color = ColorType::black;
    }
    if (node_->right) {
        node_->right->color = ColorType::black;
    }
    if (node_ == root_) {
        return;
    }

    node_->color = ColorType::red;
    if (parent_node_->color == ColorType::red) {
        // Need to rotate.
        grand_parent_node_->color = ColorType::red;

        // Check if needs double rotation.
        // First rotation.
        if (key_comparator_(key, grand_parent_node_->key) != key_comparator_(key, parent_node_->key)) {
            HandleRotation(grand_parent_node_, key);
        }

        // Second rotation.
        if (grand_parent_node_ == root_) {
            HandleRotationWithRoot(key);
        } else {
            HandleRotation(grand_grand_parent_node_, key);
        }

        node_->color = ColorType::black;
    }
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
void RED_BLACK_TREE_TYPE::HandleRotation(RedBlackTreeNode* parent_node, const KeyType& key)
{
    if (key_comparator_(key, parent_node->key)) {
        node_ = parent_node->left =
            key_comparator_(key, parent_node->left->key) ? SingleRotationWithLeft(parent_node->left) : SingleRotationWithRight(parent_node->left);
    }
    node_ = parent_node->right =
        key_comparator_(key, parent_node->right->key) ? SingleRotationWithLeft(parent_node->right) : SingleRotationWithRight(parent_node->right);
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
void RED_BLACK_TREE_TYPE::HandleRotationWithRoot(const KeyType& key)
{
    if (key_comparator_(key, root_->key)) {
        node_ = root_ = SingleRotationWithLeft(root_);
    }
    node_ = root_ = SingleRotationWithRight(root_);
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
auto RED_BLACK_TREE_TYPE::SingleRotationWithLeft(RedBlackTreeNode* root) -> RED_BLACK_TREE_TYPE::RedBlackTreeNode*
{
    if (!root->left) {
        return root;
    }

    // Rotate.
    RedBlackTreeNode* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;

    return new_root;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
REQUIRES_COMPARATOR
auto RED_BLACK_TREE_TYPE::SingleRotationWithRight(RedBlackTreeNode* root) -> RED_BLACK_TREE_TYPE::RedBlackTreeNode*
{
    if (!root->right) {
        return root;
    }

    // Rotate.
    RedBlackTreeNode* new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;

    return new_root;
}

template class RedBlackTree<int, int>;

} // namespace rbt
