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
bool RED_BLACK_TREE_TYPE::Insert(const KeyType& key, const ValueType& value)
{
    // If root is null.
    if (!root_) {
        root_ = new RedBlackTreeNode{.Key = key, .Value = value, .Color = ColorType::Black};
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
        if (node->Key == key) {
            return false;
        }

        // If node's left and right are red, need to reorient.
        if (node->Left && node->Right && node->Left->Color == ColorType::Red && node->Right->Color == ColorType::Red) {
            HandleReorient(grand_grand_parent_node, grand_parent_node, parent_node, node);
        }

        grand_grand_parent_node = grand_parent_node;
        grand_parent_node = parent_node;
        parent_node = node;
        node = NextNode(node, key);
    }

    // Insertion.
    size_++;
    node = new RedBlackTreeNode{.Key = key, .Value = value, .Color = ColorType::
        Red};
    if (key_comparator_(key, parent_node->Key)) {
        parent_node->Left = node;
    } else {
        parent_node->Right = node;
    }

    // Check whether reorient is required.
    HandleReorient(grand_grand_parent_node, grand_parent_node, parent_node, node);

    return true;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
bool RED_BLACK_TREE_TYPE::Erase(const KeyType& key)
{
    auto find_min_leaf_node = [](RedBlackTreeNode* root) -> RedBlackTreeNode* {
        while (root->Left) {
            root = root->Left;
        }
        return root;
    };
    auto find_max_leaf_node = [](RedBlackTreeNode* root) -> RedBlackTreeNode* {
        while (root->Right) {
            root = root->Right;
        }
        return root;
    };
    auto is_red_node = [](RedBlackTreeNode* n) { return n && n->Color == ColorType::Red; };

    RedBlackTreeNode* node = root_;
    RedBlackTreeNode* parent_node = nullptr;
    RedBlackTreeNode* grand_parent_node = nullptr;
    node->Color = ColorType::Red;
    while (node) {
        if (IsBlackNode(node, false)) {
            // Node is black, need to change node's color to red somehow.
            RedBlackTreeNode* sibling_node = parent_node ? (parent_node->Left == node ? parent_node->
                Right : parent_node->Left) : nullptr;
            if (is_red_node(sibling_node)) {
                // Sibling_node are red, node is black,
                // need rotate.
                HandleRotation(parent_node, sibling_node);
                HandleReconnection(grand_parent_node, sibling_node);
                sibling_node->Color = ColorType::Black;
                parent_node->Color = ColorType::Red;
                continue;
            }

            if (IsBlackNode(node->Left) && IsBlackNode(node->Right)) {
                // Sibling_node is black, both node left child and right child are black.
                if (sibling_node && (is_red_node(sibling_node->Left) || is_red_node(sibling_node->Right))) {
                    // Sibling_node is not null and at least one child of sibling_node is red.
                    RedBlackTreeNode* sibling_node_red_child = IsBlackNode(sibling_node->Left) ? sibling_node->Right : sibling_node->Left;
                    bool is_unique_rotate = true;

                    // First rotation.
                    if (key_comparator_(node->Key, parent_node->Key) == key_comparator_(sibling_node_red_child->Key, sibling_node->Key)) {
                        HandleRotation(sibling_node, sibling_node_red_child);
                        HandleReconnection(parent_node, sibling_node_red_child);  // NOLINT
                        is_unique_rotate = false;
                    }

                    // Second rotation.
                    is_unique_rotate ? HandleRotation(parent_node, sibling_node) : HandleRotation(parent_node, sibling_node_red_child);
                    is_unique_rotate ? HandleReconnection(grand_parent_node, sibling_node) : HandleReconnection(grand_parent_node, sibling_node_red_child);

                    // Recolor.
                    node->Color = ColorType::Red;
                    parent_node->Color = ColorType::Black;
                    if (is_unique_rotate) {
                        sibling_node->Color = ColorType::Red;
                        sibling_node_red_child->Color = ColorType::Black;
                    }
                } else {
                    // The sibling_node is black or both child of sibling_node are black.
                    // Flip parent_node, node, sibling_node color.
                    parent_node->Color = ColorType::Black;
                    node->Color = ColorType::Red;
                    if (sibling_node) {
                        sibling_node->Color = ColorType::Red;
                    }
                }
            }
        }

        bool is_replaced = false;
        if (node->Key == key) {
            if (!node->Left && !node->Right) {
                // Node is a leaf node, just delete node.
                if (parent_node) {
                    // Deleted node is not root.
                    (parent_node->Left == node ? parent_node->Left : parent_node->Right) = nullptr;
                } else {
                    // Deleted node is root and node is the unique node.
                    root_ = nullptr;
                }

                // Delete.
                delete node;
                size_--;

                // Tail work.
                if (root_) {
                    root_->Color = ColorType::Black;
                }
                return true;
            }

            // Note: It might be possible to minimize copy cost assumption.
            // Node is an internal node.
            // Need to replace node with its successor or predecessor.
            auto replace_node = node->Right ? find_min_leaf_node(node->Right) : find_max_leaf_node(node->Left);
            std::swap(node->Key, replace_node->Key);
            std::swap(node->Value, replace_node->Value);
            is_replaced = true;
        }

        // Iteration.
        grand_parent_node = parent_node;
        parent_node = node;
        node = key_comparator_(key, node->Key) ^ is_replaced ? node->Left : node->Right;
    }

    root_->Color = ColorType::Black;
    return false;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
auto RED_BLACK_TREE_TYPE::GetValue(const KeyType& key) const -> std::optional<ValueType>
{
    RedBlackTreeNode* ptr = root_;

    while (ptr) {
        if (key == ptr->Key) {
            return std::make_optional(ptr->Value);
        }

        ptr = NextNode(ptr, key);
    }

    return std::nullopt;
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

        if (ptr->Left) {
            node_stack.push(ptr->Left);
        }
        if (ptr->Right) {
            node_stack.push(ptr->Right);
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
            goto end;  // NOLINT
        }

        size_t count = 0;
        for (const auto& node : line) {
            if (node) {
                print_queue.push(node->Left);
                print_queue.push(node->Right);
                os << '[' << node->Key << ' ' << (node->Color == ColorType::Red ? "red" : "black") << ']';
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
    std::cout << os.str() << '\n';
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
bool RED_BLACK_TREE_TYPE::RedBlackTreeRulesCheck()
{
    // 1. Root is always black.
    if (!root_) {
        return true;
    }

    if (root_->Color != ColorType::Black) {
        return false;
    }

    // 2. If a node is red, its child nodes must be black.
    std::stack<RedBlackTreeNode*> node_stack;
    node_stack.push(root_);

    while (!node_stack.empty()) {
        RedBlackTreeNode* ptr = node_stack.top();
        node_stack.pop();

        if (ptr->Color == ColorType::Red && !(IsBlackNode(ptr->Left) && IsBlackNode(ptr->Right))) {
            return false;
        }

        if (ptr->Left) {
            node_stack.push(ptr->Left);
        }
        if (ptr->Right) {
            node_stack.push(ptr->Right);
        }
    }

    // 3. Every path from a node to a null node must contain the same number of black nodes.
    std::vector<int> all_black_path_nodes_count;
    all_black_path_nodes_count.reserve(size_);
    ComputeAllBlackPathHeight(root_, 1, all_black_path_nodes_count);
    return std::ranges::adjacent_find(all_black_path_nodes_count, std::not_equal_to()) == all_black_path_nodes_count.end();
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
    if (node->Left) {
        node->Left->Color = ColorType::Black;
    }
    if (node->Right) {
        node->Right->Color = ColorType::Black;
    }
    if (node == root_) {
        return;
    }

    node->Color = ColorType::Red;
    if (parent_node->Color == ColorType::Red) {
        // Need to rotate.
        grand_parent_node->Color = ColorType::Red;

        // Check if it needs double rotation.
        bool is_unique_rotate = true;
        // First rotation.
        if (key_comparator_(parent_node->Key, grand_parent_node->Key) != key_comparator_(node->Key, parent_node->Key)) {
            HandleRotation(parent_node, node);
            HandleReconnection(grand_parent_node, node);
            is_unique_rotate = false;
        }

        // Second rotation.
        is_unique_rotate ? HandleRotation(grand_parent_node, parent_node) : HandleRotation(grand_parent_node, node);
        is_unique_rotate ? HandleReconnection(grand_grand_parent_node, parent_node) : HandleReconnection(grand_grand_parent_node, node);  // NOLINT

        (is_unique_rotate ? parent_node->Color : node->Color) = ColorType::Black;
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
        RedBlackTreeNode* new_root = is_left_rotation ? r->Left : r->Right;
        if (is_left_rotation) {
            r->Left = new_root->Right;
            new_root->Right = r;
        } else {
            r->Right = new_root->Left;
            new_root->Left = r;
        }
    };

    key_comparator_(sup->Key, root->Key) ? rotate(root, true) : rotate(root, false);
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT
RED_BLACK_TREE_REQUIRES
void RED_BLACK_TREE_TYPE::HandleReconnection(RedBlackTreeNode* parent, RedBlackTreeNode* node)
{
    if (parent) {
        // Just reconnect to parent.
        (key_comparator_(node->Key, parent->Key) ? parent->Left : parent->Right) = node;
    } else {
        // Need to reconnect to root_.
        root_ = node;
    }
}

template class RedBlackTree<int, int>;

} // namespace rbt
