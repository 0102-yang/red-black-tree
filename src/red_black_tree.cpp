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

    /**
     * Insert key-value pair into red black tree.
     */
    // Find insertion place.
    RedBlackTreeNode* node = root_;
    RedBlackTreeNode* parent_node = nullptr;
    RedBlackTreeNode* grand_parent_node = nullptr;
    RedBlackTreeNode* grand_grand_parent_node = nullptr;
    while (node) {
        if (node->key == key) {
            return false;
        }

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

    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
bool RED_BLACK_TREE_TYPE::Erase(const KeyType& key)
{
    auto find_min_leaf_node = [](RedBlackTreeNode* root) -> RedBlackTreeNode* {
        while (root->left) {
            root = root->left;
        }
        return root;
    };
    auto find_max_leaf_node = [](RedBlackTreeNode* root) -> RedBlackTreeNode* {
        while (root->right) {
            root = root->right;
        }
        return root;
    };
    auto is_red_node = [](RedBlackTreeNode* n) { return n && n->color == ColorType::red; };

    RedBlackTreeNode* node = root_;
    RedBlackTreeNode* parent_node = nullptr;
    RedBlackTreeNode* grand_parent_node = nullptr;
    node->color = ColorType::red;
    while (node) {
        if (IsBlackNode(node, false)) {
            // Node is black, need to change node's color to red somehow.
            RedBlackTreeNode* sibling_node = parent_node ? (parent_node->left == node ? parent_node->right : parent_node->left) : nullptr;
            if (is_red_node(sibling_node)) {
                // Sibling_node are red, node is black,
                // need rotate.
                HandleRotation(parent_node, sibling_node);
                HandleReconnection(grand_parent_node, sibling_node);
                sibling_node->color = ColorType::black;
                parent_node->color = ColorType::red;
                continue;
            }

            if (IsBlackNode(node->left) && IsBlackNode(node->right)) {
                // Sibling_node is black, both node left child and right child are black.
                if (sibling_node && (is_red_node(sibling_node->left) || is_red_node(sibling_node->right))) {
                    // Sibling_node is not null and at least one child of sibling_node is red.
                    RedBlackTreeNode* sibling_node_red_child = IsBlackNode(sibling_node->left) ? sibling_node->right : sibling_node->left;
                    bool is_unique_rotate = true;

                    // First rotation.
                    if (key_comparator_(node->key, parent_node->key) == key_comparator_(sibling_node_red_child->key, sibling_node->key)) {
                        HandleRotation(sibling_node, sibling_node_red_child);
                        HandleReconnection(parent_node, sibling_node_red_child);
                        is_unique_rotate = false;
                    }

                    // Second rotation.
                    is_unique_rotate ? HandleRotation(parent_node, sibling_node) : HandleRotation(parent_node, sibling_node_red_child);
                    is_unique_rotate ? HandleReconnection(grand_parent_node, sibling_node) : HandleReconnection(grand_parent_node, sibling_node_red_child);

                    // Recolor.
                    node->color = ColorType::red;
                    parent_node->color = ColorType::black;
                    if (is_unique_rotate) {
                        sibling_node->color = ColorType::red;
                        sibling_node_red_child->color = ColorType::black;
                    }
                } else {
                    // The sibling_node is black or both child of sibling_node are black.
                    // Flip parent_node, node, sibling_node color.
                    parent_node->color = ColorType::black;
                    node->color = ColorType::red;
                    if (sibling_node) {
                        sibling_node->color = ColorType::red;
                    }
                }
            }
        }

        bool is_replaced = false;
        if (node->key == key) {
            if (!node->left && !node->right) {
                // Node is a leaf node, just delete node.
                if (parent_node) {
                    // Deleted node is not root.
                    (parent_node->left == node ? parent_node->left : parent_node->right) = nullptr;
                } else {
                    // Deleted node is root and node is the unique node.
                    root_ = nullptr;
                }

                // Delete.
                delete node;
                size_--;

                // Tail work.
                if (root_) {
                    root_->color = ColorType::black;
                }
                return true;
            }

            // Note: It might be possible to minimize copy cost assumption.
            // Node is a internal node.
            // Need to replace node with its successor or predecessor.
            auto replace_node = node->right ? find_min_leaf_node(node->right) : find_max_leaf_node(node->left);
            std::swap(node->key, replace_node->key);
            std::swap(node->value, replace_node->value);
            is_replaced = true;
        }

        // Iteration.
        grand_parent_node = parent_node;
        parent_node = node;
        node = key_comparator_(key, node->key) ^ is_replaced ? node->left : node->right;
    }

    root_->color = ColorType::black;
    return false;
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
