## Build red-black-tree.
add_library(red_black_tree OBJECT)
target_sources(red_black_tree PRIVATE ${PROJECT_SOURCE_DIR}/src/red_black_tree.cpp)
target_compile_options(red_black_tree
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Debug>:-Wall -Wextra -Werror -Wno-unused-parameter -Wno-attributes -fno-omit-frame-pointer -fno-optimize-sibling-calls>
        >
)
target_compile_definitions(red_black_tree PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_include_directories(red_black_tree PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(red_black_tree spdlog::spdlog)