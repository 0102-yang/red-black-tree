## Build red-black-tree.
add_library(red_black_tree OBJECT)
target_sources(red_black_tree PRIVATE "red_black_tree.cpp")
target_compile_options(red_black_tree
    PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            $<$<CONFIG:Debug>:-Wall -Wextra -Werror -Wno-unused-parameter -Wno-attributes -fno-omit-frame-pointer -fno-optimize-sibling-calls>
        >
)
target_compile_definitions(red_black_tree PRIVATE $<$<CONFIG:Debug>:DEBUG>)
target_compile_features(red_black_tree PRIVATE cxx_std_23)

if(LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(red_black_tree PRIVATE DEBUG)
elseif(LOG_LEVEL STREQUAL "TRACE")
    target_compile_definitions(red_black_tree PRIVATE TRACE PRIVATE DEBUG)
endif()

target_include_directories(red_black_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(red_black_tree spdlog::spdlog)