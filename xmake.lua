set_project("CppTest")
set_version("1.0.0")

add_rules("mode.debug", "mode.release")
set_languages("c++latest")
set_toolchains("llvm")
set_runtimes("c++_shared")

add_requires("spdlog")
add_requires("gtest")

target("red-black-tree")
  if is_mode("release") then
    set_optimize("fastest")
    set_symbols("hidden")
  end
  if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    add_defines("SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG")
  end

  set_kind("static")
  add_includedirs("src", {public = true})
  add_files("src/red_black_tree.cpp")
  add_packages("spdlog")
target_end()

target("insert-test")
  if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
  end

  set_kind("binary")
  add_files("test/test_main.cpp")
  add_files("test/red_black_tree_insert_test.cpp")
  add_deps("red-black-tree")
  add_packages("gtest")
  add_packages("spdlog")
target_end()

target("delete-test")
  if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
  end

  set_kind("binary")
  add_files("test/test_main.cpp")
  add_files("test/red_black_tree_delete_test.cpp")
  add_deps("red-black-tree")
  add_packages("gtest")
  add_packages("spdlog")
target_end()

target("bench-mark")
  set_symbols("hidden")
  set_optimize("fastest")
  set_kind("binary")
  add_files("bench/performance.cpp")
  add_deps("red-black-tree")
  add_packages("spdlog")
target_end()
