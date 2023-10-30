#pragma once

#include <functional>

namespace rbt
{

#define RED_BLACK_TREE_TEMPLATE_ARGUMENT template <typename KeyType, typename ValueType, class KeyComparator>
#define RED_BLACK_TREE_TYPE RedBlackTree<KeyType, ValueType, KeyComparator>
#define REQUIRES_COMPARATOR requires is_comparator<KeyType, KeyComparator>

template <typename KeyType, typename Comparator>
concept is_comparator = requires(Comparator comparator, KeyType lhs, KeyType rhs) {
    {
        comparator(lhs, rhs)
    } -> std::convertible_to<bool>;
};

template <typename KeyType, typename ValueType, class KeyComparator = std::less<KeyType>> REQUIRES_COMPARATOR class RedBlackTree
{
    /**
     * Red black tree color type.
     * Indicate red node and black node.
     */
    enum class ColorType
    {
        red,
        black
    };

    class RedBlackTreeNode
    {
    public:
        KeyType key;
        ValueType value;
        RedBlackTreeNode* left = nullptr;
        RedBlackTreeNode* right = nullptr;
        ColorType color = ColorType::black;
    };

public:
    RedBlackTree() = delete;

    explicit RedBlackTree(const KeyComparator& key_comparator = KeyComparator()) : key_comparator_(key_comparator) {}

    RedBlackTree(const RedBlackTree& tree) = delete;

    explicit RedBlackTree(RedBlackTreeNode&& tree) = delete;

    auto operator=(const RedBlackTree& tree) noexcept -> RedBlackTree& = delete;

    auto operator=(RedBlackTree&& tree) noexcept -> RedBlackTree& = delete;

    ~RedBlackTree();

    bool Insert(const KeyType& key, const ValueType& value);

    bool Delete(const KeyType& key);

    auto GetValue(const KeyType& key) -> std::pair<ValueType, bool>;

    [[nodiscard]] bool IsEmpty() const;

private:
    RedBlackTreeNode* root_ = nullptr;
    KeyComparator key_comparator_;
};

} // namespace rbt