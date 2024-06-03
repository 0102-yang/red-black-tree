#include <optional>
#include <stack>

#ifdef DEBUG
#include <iostream>
#include <queue>
#include <sstream>
#endif

#include "red_black_tree.h"

#include <spdlog/spdlog.h>

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
    node = new RedBlackTreeNode{.Key = key, .Value = value, .Color = ColorType::Red};
    if (!root_) [[unlikely]] {
        node->Color = ColorType::Black;
        root_ = node;
        return true;
    }

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
#ifdef TRACE
    spdlog::trace("Before delete {}:", key);
    PrintTree();
#endif

    const auto is_red_node = [](RedBlackTreeNode* n) { return n && n->Color == ColorType::Red; };

    const auto find_max_leaf_node = [](RedBlackTreeNode* root) -> RedBlackTreeNode* {
        while (root->Right) {
            root = root->Right;
        }
        return root;
    };

    const auto get_sibling_node = [](RedBlackTreeNode* parent_node, RedBlackTreeNode* node) {
        return parent_node ? (parent_node->Left == node ? parent_node->Right : parent_node->Left) : nullptr;
    };

    /*
     * Consider whether to recolor the root node to red first.
     * If both left and right child are black, recolor root to red.
     */
    if (IsBlackNode(root_->Left) && IsBlackNode(root_->Right)) {
        root_->Color = ColorType::Red;
    }

#ifdef TRACE
    spdlog::trace("After recolor root to red.");
    PrintTree();
#endif

    RedBlackTreeNode* node = root_;
    RedBlackTreeNode* parent_node = nullptr;
    RedBlackTreeNode* grand_parent_node = nullptr;
    KeyType deleted_key = key;

    while (node) {
        /*
         * Recolor current node to red first.
         */
        if (node->Color == ColorType::Black) {
            RedBlackTreeNode* sibling_node = get_sibling_node(parent_node, node);

            if (IsBlackNode(parent_node) && is_red_node(sibling_node)) {
                HandleRotation(parent_node, sibling_node);
                HandleReconnection(grand_parent_node, sibling_node);
                parent_node->Color = ColorType::Red;
                sibling_node->Color = ColorType::Black;

                // Update sibling node and grand parent node.
                grand_parent_node = sibling_node;
                sibling_node = get_sibling_node(parent_node, node);
            }

            if (IsBlackNode(node->Left) && IsBlackNode(node->Right)) {
                // Sibling_node is black, both node left child and right child are black.
                if (sibling_node && (is_red_node(sibling_node->Left) || is_red_node(sibling_node->Right))) {
                    // Sibling_node is not null and at least one child of sibling_node is red.
                    RedBlackTreeNode* sibling_node_red_child = IsBlackNode(sibling_node->Left) ? sibling_node->Right : sibling_node->Left;
                    bool is_unique_rotate = true;

                    // First rotation.
                    if ((key_comparator_(node->Key, parent_node->Key) || node->Key == parent_node->Key)
                        == (key_comparator_(sibling_node_red_child->Key, sibling_node->Key) || sibling_node_red_child->Key == sibling_node->Key)) {
                        HandleRotation(sibling_node, sibling_node_red_child);
                        HandleReconnection(parent_node, sibling_node_red_child); // NOLINT
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

#ifdef TRACE
        spdlog::trace("After recolor current key {}.", node->Key);
        PrintTree();
#endif

        /*
         * Handle delete.
         * Precondition: node is red.
         */
        if (deleted_key == node->Key) {
            if (node->Left && node->Right) {
                // Node has two children.
                // Replace current node to the predecessor node.
                const auto replaced_node = find_max_leaf_node(node->Left);
                deleted_key = node->Key = replaced_node->Key;
                node->Value = replaced_node->Value;

                grand_parent_node = parent_node;
                parent_node = node;
                node = node->Left;
                continue;
            }

            // Node has zero or one child.
            // 1. If node has one child, node must be a black node, child node must be a red node.
            // 2. If node has zero child, we previously make sure node is red.
            RedBlackTreeNode* child_node = nullptr;
            if (node->Left || node->Right) {
                child_node = node->Left ? node->Left : node->Right;
                child_node->Color = ColorType::Black;
            }

            if (node == root_) [[unlikely]] {
                root_ = child_node;
            } else [[likely]] {
                if (key_comparator_(node->Key, parent_node->Key) || node->Key == parent_node->Key) {
                    parent_node->Left = child_node;
                } else {
                    parent_node->Right = child_node;
                }
            }

            delete node;
            size_--;
            if (root_) {
                root_->Color = ColorType::Black;
            }

#ifdef TRACE
            spdlog::trace("After delete {}:", key);
            PrintTree();
#endif
            return true;
        }

        // Iteration.
        grand_parent_node = parent_node;
        parent_node = node;
        node = NextNode(node, deleted_key);
    }

    if (root_) {
        root_->Color = ColorType::Black;
    }
    return false;
}

RED_BLACK_TREE_TEMPLATE_ARGUMENT RED_BLACK_TREE_REQUIRES std::optional<ValueType> RED_BLACK_TREE_TYPE::GetValue(const KeyType& key) const
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
            goto end; // NOLINT
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
        spdlog::warn("Violate rule 1: Root is not black.");
        return false;
    }

    // 2. If a node is red, its child nodes must be black.
    std::stack<RedBlackTreeNode*> node_stack;
    node_stack.push(root_);

    while (!node_stack.empty()) {
        RedBlackTreeNode* ptr = node_stack.top();
        node_stack.pop();

        if (ptr->Color == ColorType::Red && !(IsBlackNode(ptr->Left) && IsBlackNode(ptr->Right))) {
            spdlog::warn("Violate rule 2: Red node must not have red child.");
            return false;
        }

        if (ptr->Left) {
            node_stack.push(ptr->Left);
        }
        if (ptr->Right) {
            node_stack.push(ptr->Right);
        }
    }

    // 3. Every path from root node to every null node must contain the same number of black nodes.
    std::vector<int> all_black_path_nodes_count;
    all_black_path_nodes_count.reserve(size_);
    ComputeAllBlackPathHeight(root_, 1, all_black_path_nodes_count);

    if (const bool rule3 = std::ranges::adjacent_find(all_black_path_nodes_count, std::not_equal_to()) == all_black_path_nodes_count.end(); !rule3) {
        spdlog::warn("Violate rule 3: Every path from root node to every null node must contain the same number of black nodes.");
        return false;
    }

    return true;
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
        is_unique_rotate ? HandleReconnection(grand_grand_parent_node, parent_node) : HandleReconnection(grand_grand_parent_node, node); // NOLINT

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
void RED_BLACK_TREE_TYPE::HandleReconnection(RedBlackTreeNode* new_parent, RedBlackTreeNode* node)
{
    if (new_parent) {
        // Just reconnect to parent.
        (key_comparator_(node->Key, new_parent->Key) ? new_parent->Left : new_parent->Right) = node;
    } else {
        // Need to reconnect to root_.
        root_ = node;
    }
}

template class RedBlackTree<int, int>;

} // namespace rbt
