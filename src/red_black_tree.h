#pragma once

#include <functional>

namespace rbt
{

#define RED_BLACK_TREE_TEMPLATE_ARGUMENT template <typename KeyType, typename ValueType, class KeyComparator>
#define RED_BLACK_TREE_TYPE RedBlackTree<KeyType, ValueType, KeyComparator>
#define RED_BLACK_TREE_REQUIRES requires std::default_initializable<KeyType> && std::equality_comparable<KeyType> && is_comparator<KeyType, KeyComparator>

template <typename KeyType, typename Comparator>
concept is_comparator = requires(Comparator comparator, KeyType lhs, KeyType rhs) {
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
        red,
        black
    };

    struct RedBlackTreeNode
    {
        KeyType key = {};
        ValueType value = {};
        RedBlackTreeNode* left = nullptr;
        RedBlackTreeNode* right = nullptr;
        ColorType color = ColorType::red;
    };

public:
    RedBlackTree() : key_comparator_() {}

    RedBlackTree(const RedBlackTree&) = delete;

    RedBlackTree(RedBlackTree&&) noexcept = delete;

    auto operator=(const RedBlackTree&) -> RedBlackTree& = delete;

    auto operator=(RedBlackTree&&) noexcept -> RedBlackTree& = delete;

    ~RedBlackTree();

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
    /// <returns>ValueType-bool pair indicates value and success flag.</returns>
    auto GetValue(const KeyType& key) -> std::pair<ValueType, bool>;

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
    [[nodiscard]] size_t Size() const { return size_; }

#ifdef DEBUG
    /**
     * For Debug only.
     */

    /// <summary>
    /// Print red black tree.
    /// </summary>
    void PrintTree();
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
};

} // namespace rbt