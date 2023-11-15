#include "red_black_tree.h"

#ifdef DEBUG
#include <iostream>
#include <queue>
#include <sstream>
#endif

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
RED_BLACK_TREE_REQUIRES
RED_BLACK_TREE_TYPE::~RedBlackTree()
{
    Clear();
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
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
    RedBlackTreeNode* node = root_;
    RedBlackTreeNode* parent_node = nullptr;
    RedBlackTreeNode* grand_parent_node = nullptr;
    RedBlackTreeNode* grand_grand_parent_node = nullptr;
    // node_ = parent_node_ = grand_parent_node_ = grand_grand_parent_node_ = root_;
    while (node) {
        // If node's left and right are red, need to reorient.
        if (node->left && node->right && node->left->color == ColorType::red && node->right->color == ColorType::red) {
            HandleReorient(grand_grand_parent_node, grand_parent_node, parent_node, node);
        }

        grand_grand_parent_node = grand_parent_node;
        grand_parent_node = parent_node;
        parent_node = node;
        node = key_comparator_(key, node->key) ? node->left : node->right;
    }

    // Insertion.
    size_++;
    node = new RedBlackTreeNode{.key = key, .value = value, .color = ColorType::red};
    if (key_comparator_(key, parent_node->key)) {
        parent_node->left = node;
    } else {
        parent_node->right = node;
    }

    // Check if needs reorient.
    HandleReorient(grand_grand_parent_node, grand_parent_node, parent_node, node);

#ifdef DEBUG
    PrintTree();
#endif

    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
bool RED_BLACK_TREE_TYPE::Erase(const KeyType& key)
{
    // TODO: Need to implement.
    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
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
RED_BLACK_TREE_REQUIRES
void RED_BLACK_TREE_TYPE::Clear()
{
    if (!root_) {
        return;
    }

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

#ifdef DEBUG

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
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
        for (const auto& node : line) {
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

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
bool RED_BLACK_TREE_TYPE::RedBlackTreeRulesCheck()
{
    // Todo: Need to implement.
    // 1. Root is always black.
    if (!root_) {
        return true;
    }

    if (root_->color != ColorType::black) {
        return false;
    }

    // 2. If a node is red, its child nodes must be black.
    std::stack<RedBlackTreeNode*> node_stack;
    node_stack.push(root_);

    while (!node_stack.empty()) {
        RedBlackTreeNode* ptr = node_stack.top();
        node_stack.pop();

        if (ptr->color == ColorType::red && !(IsBlackNode(ptr->left) && IsBlackNode(ptr->right))) {
            return false;
        }

        if (ptr->left) {
            node_stack.push(ptr->left);
        }
        if (ptr->right) {
            node_stack.push(ptr->right);
        }
    }

    // 3. Every path from a node to a null node must contain the same number of black nodes.
    std::vector<int> all_black_path_nodes_count;
    all_black_path_nodes_count.reserve(size_);
    ComputeAllBlackPathHeight(root_, 1, all_black_path_nodes_count);
    return std::adjacent_find(all_black_path_nodes_count.begin(), all_black_path_nodes_count.end(), std::not_equal_to()) == all_black_path_nodes_count.end();
}

#endif

/**
 * Private methods.
 */

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
void RED_BLACK_TREE_TYPE::HandleReorient(RedBlackTreeNode* grand_grand_parent_node, RedBlackTreeNode* grand_parent_node, RedBlackTreeNode* parent_node,
                                         RedBlackTreeNode* node)
{
    if (node->left) {
        node->left->color = ColorType::black;
    }
    if (node->right) {
        node->right->color = ColorType::black;
    }
    if (node == root_) {
        return;
    }

    node->color = ColorType::red;
    if (parent_node->color == ColorType::red) {
        // Need to rotate.
        grand_parent_node->color = ColorType::red;

        // Check if needs double rotation.
        bool is_unique_rotate = true;
        // First rotation.
        if (key_comparator_(parent_node->key, grand_parent_node->key) != key_comparator_(node->key, parent_node->key)) {
            HandleRotation(parent_node, node);
            HandleReconnection(grand_parent_node, node);
            is_unique_rotate = false;
        }

        // Second rotation.
        is_unique_rotate ? HandleRotation(grand_parent_node, parent_node) : HandleRotation(grand_parent_node, node);
            is_unique_rotate ? HandleReconnection(grand_grand_parent_node, parent_node) : HandleReconnection(grand_grand_parent_node, node);

        (is_unique_rotate ? parent_node->color : node->color) = ColorType::black;
    }
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
void RED_BLACK_TREE_TYPE::HandleRotation(RedBlackTreeNode* root, RedBlackTreeNode* sup)
{
    if (!sup) {
        return;
}

    auto rotate = [](RedBlackTreeNode* r, bool is_left_rotation) {
        RedBlackTreeNode* new_root = is_left_rotation ? r->left : r->right;
        if (is_left_rotation) {
            r->left = new_root->right;
            new_root->right = r;
        } else {
            r->right = new_root->left;
            new_root->left = r;
    }
    };

    key_comparator_(sup->key, root->key) ? rotate(root, true) : rotate(root, false);
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
void RED_BLACK_TREE_TYPE::HandleReconnection(RedBlackTreeNode* parent, RedBlackTreeNode* node)
{
    if (parent) {
        // Just reconnect to parent.
    (key_comparator_(node->key, parent->key) ? parent->left : parent->right) = node;
    } else {
        // Need to reconnect to root_.
        root_ = node;
    }
}

template class RedBlackTree<int, int>;

} // namespace rbt
