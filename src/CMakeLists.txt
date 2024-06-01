## Build red-black-tree.
add_library(red_black_tree OBJECT)
target_sources(red_black_tree PRIVATE FILE_SET red_black_tree_module TYPE CXX_MODULES FILES "red_black_tree.ixx")
target_sources(red_black_tree PRIVATE "red_black_tree.ixx" "red_black_tree.cpp")
target_compile_options(red_black_tree
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Debug>:-Wall -Wextra -Werror -Wno-unused-parameter -Wno-attributes -fno-omit-frame-pointer -fno-optimize-sibling-calls>
        >
)
target_compile_definitions(red_black_tree PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(red_black_tree PRIVATE cxx_std_23)

if(LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(red_black_tree PUBLIC -DLOG_LEVEL=DEBUG)
elseif(LOG_LEVEL STREQUAL "TRACE")
    target_compile_definitions(red_black_tree PUBLIC  -DLOG_LEVEL=TRACE)
else()
    target_compile_definitions(red_black_tree PUBLIC -DLOG_LEVEL=INFO)
endif()

target_link_libraries(red_black_tree spdlog::spdlog)