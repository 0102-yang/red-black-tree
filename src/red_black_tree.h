#pragma once

namespace rbt
{

#define RED_BLACK_TREE_TEMPLATE_ARGUMENT template <typename KeyType, typename ValueType, class KeyComparator>
#define RED_BLACK_TREE_TYPE RedBlackTree<KeyType, ValueType, KeyComparator>
#define RED_BLACK_TREE_REQUIRES requires std::default_initializable<KeyType> && std::equality_comparable<KeyType> && IsComparator<KeyType, KeyComparator>

} // namespace rbt