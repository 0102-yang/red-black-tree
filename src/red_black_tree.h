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
    KeyComparator key_comparator_{};
    size_t size_ = 0;

    /**
     * For handle reorient.
     */

    RedBlackTreeNode* node_ = nullptr;
    RedBlackTreeNode* parent_node_ = nullptr;
    RedBlackTreeNode* grand_parent_node_ = nullptr;
    RedBlackTreeNode* grand_grand_parent_node_ = nullptr;

    /// <summary>
    /// Handle reorient for red-black tree.
    /// </summary>
    /// <param name="key">The key.</param>
    void HandleReorient(const KeyType& key);

    /// <summary>
    /// Handle rotation without changing red-black tree root.
    /// </summary>
    /// <param name="parent_node">Parent node for reconnect nodes.</param>
    /// <param name="key">The key.</param>
    void HandleRotation(RedBlackTreeNode* parent_node, const KeyType& key);

    /// <summary>
    /// Handle rotation without changing red-black tree root.
    /// </summary>
    /// <param name="key">The key.</param>
    void HandleRotationWithRoot(const KeyType& key);

    /// <summary>
    /// Single rotation with root and root->left.
    /// </summary>
    /// <param name="root">Original root.</param>
    /// <returns>The new root.</returns>
    RedBlackTreeNode* SingleRotationWithLeft(RedBlackTreeNode* root);

    /// <summary>
    /// Single rotation with root and root->right.
    /// </summary>
    /// <param name="root">Original root.</param>
    /// <returns>The new root.</returns>
    RedBlackTreeNode* SingleRotationWithRight(RedBlackTreeNode* root);
};

} // namespace rbt