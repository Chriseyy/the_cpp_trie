cmake_minimum_required(VERSION 3.20)
project(the_cpp_trie)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true  
)
FetchContent_MakeAvailable(googletest)

add_executable(run_trie_tests test/testTrie.cpp)


target_include_directories(run_trie_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(run_trie_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(run_trie_tests)