export module red_black_tree;

import <concepts>;
import <functional>;
import <optional>;

#include "red_black_tree.h"

export namespace rbt
{

template <typename KeyType, typename Comparator>
concept IsComparator = requires(Comparator comparator, KeyType lhs, KeyType rhs) {
    {
        comparator(lhs, rhs)
    } -> std::convertible_to<bool>;
};

template <typename KeyType, typename ValueType, class KeyComparator = std::less<KeyType>> RED_BLACK_TREE_REQUIRES class RedBlackTree
{
    /**
     * Red black tree color type.
     * Indicate red node and black node.
     */
    enum class ColorType : bool
    {
        Red,
        Black
    };

    struct RedBlackTreeNode
    {
        KeyType Key = {};
        ValueType Value = {};
        RedBlackTreeNode* Left = nullptr;
        RedBlackTreeNode* Right = nullptr;
        ColorType Color = ColorType::red;
    };

public:
    RedBlackTree() : key_comparator_() {}

    RedBlackTree(const RedBlackTree&) = delete;

    RedBlackTree(RedBlackTree&&) noexcept = delete;

    auto operator=(const RedBlackTree&) -> RedBlackTree& = delete;

    auto operator=(RedBlackTree&&) noexcept -> RedBlackTree& = delete;

    ~RedBlackTree() noexcept { Clear(); }

    /// <summary>
    /// Insert a key-value pair into red-black tree.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="value">The value.</param>
    /// <returns>True for insert successfully.</returns>
    bool Insert(const KeyType& key, const ValueType& value);

    /// <summary>
    /// Erase a key-value pair from red-black tree.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>True for erase successfully.</returns>
    bool Erase(const KeyType& key);

    /// <summary>
    /// Get key-value pair from red-black tree.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>The optional value.</returns>
    [[nodiscard]] auto GetValue(const KeyType& key) const -> std::optional<ValueType>;

    /// <summary>
    /// Clear all elements from red-black tree.
    /// </summary>
    void Clear();

    /// <summary>
    /// Get empty status of red-black tree.
    /// </summary>
    /// <returns>True for tree is empty.</returns>
    [[nodiscard]] bool IsEmpty() const { return root_ == nullptr; }

    /// <summary>
    /// Get size of red-black tree.
    /// </summary>
    /// <returns>The size.</returns>
    [[nodiscard]] auto Size() const -> size_t { return size_; }

#ifdef DEBUG
    /**
     * For Debug only.
     */

    /// <summary>
    /// Print red black tree.
    /// </summary>
    void PrintTree();

    /// <summary>
    /// Check 3(actual and original 4) rules in red-black-tree.
    /// </summary>
    /// <returns>True for check success.</returns>
    bool RedBlackTreeRulesCheck();

#endif

private:
    RedBlackTreeNode* root_ = nullptr;
    size_t size_ = 0;
    KeyComparator key_comparator_{};

    /// <summary>
    /// Handle reorient for red-black tree.
    /// </summary>
    /// <param name="grand_grand_parent_node"></param>
    /// <param name="grand_parent_node"></param>
    /// <param name="parent_node"></param>
    /// <param name="node"></param>
    void HandleReorient(RedBlackTreeNode* grand_grand_parent_node, RedBlackTreeNode* grand_parent_node, RedBlackTreeNode* parent_node, RedBlackTreeNode* node);

    /// <summary>
    /// Handle rotation between root node and sup node,
    /// which sup is one child of root.
    /// </summary>
    /// <param name="root">The root.</param>
    /// <param name="sup">The sup.</param>
    /// <returns>The new root.</returns>
    void HandleRotation(RedBlackTreeNode* root, RedBlackTreeNode* sup);

    /// <summary>
    /// Reconnect node and its parent.
    /// </summary>
    /// <param name="parent">The parent.</param>
    /// <param name="node">The node.</param>
    void HandleReconnection(RedBlackTreeNode* parent, RedBlackTreeNode* node);

    /// <summary>
    /// Check if a node's color is black.
    /// </summary>
    /// <param name="node">The node.</param>
    /// <param name="can_be_null">Whether this node can be a black null node</param>
    /// <returns>True for node is black.</returns>
    bool IsBlackNode(RedBlackTreeNode* node, bool can_be_null = true)
    {
        return can_be_null ? !node || node->Color == ColorType::Black : node && node->Color == ColorType::Black;
    }

    /// <summary>
    /// Compute all black path height of the red-black-tree.
    /// </summary>
    /// <param name="root">The root.</param>
    /// <param name="current_black_height">The current black height.</param>
    /// <param name="all_black_height">All black height array.</param>
    void ComputeAllBlackPathHeight(RedBlackTreeNode* root, const int current_black_height, std::vector<int>& all_black_height)
    {
        if (!root) {
            all_black_height.push_back(current_black_height);
            return;
        }
        ComputeAllBlackPathHeight(root->Left, IsBlackNode(root, false) ? current_black_height + 1 : current_black_height, all_black_height);
        ComputeAllBlackPathHeight(root->Right, IsBlackNode(root, false) ? current_black_height + 1 : current_black_height, all_black_height);
    }

    /**
     * Get the next node in the red-black tree based on the given key.
     * 
     * @param node The current node.
     * @param key The key to compare with.
     * @return The next node in the red-black tree.
     */
    auto NextNode(RedBlackTreeNode* node, const KeyType& key) const -> RedBlackTreeNode* { return key_comparator_(key, node->Key) ? node->Left : node->Right; }
};

} // namespace rbt