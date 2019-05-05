cmake_minimum_required(VERSION 3.2)

set(nickitat_hsort_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/hsort.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/iterator_wrapper.hpp)

add_library(nickitat_hsort INTERFACE)
target_sources(nickitat_hsort INTERFACE
               "$<BUILD_INTERFACE:${nickitat_mpl_headers}>")
target_include_directories(nickitat_hsort INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/hsort.hpp DESTINATION include/hsort/)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/detail/iterator_wrapper.hpp
        DESTINATION include/hsort/detail)

if(NICKITAT_HSORT_WITH_TESTS)
  add_executable(nickitat_hsort_tests test/test.cpp)
  target_link_libraries(nickitat_hsort_tests nickitat_hsort)
  target_compile_options(nickitat_hsort_tests PRIVATE -std=c++11)
endif()

if(NICKITAT_HSORT_WITH_BENCHMARKS)
  add_executable(nickitat_hsort_benchmarks benchmark/gbench.cpp)
  target_link_libraries(nickitat_hsort_benchmarks
                        nickitat_hsort
                        benchmark
                        benchmark_main)
  target_compile_options(nickitat_hsort_benchmarks PRIVATE -std=c++11 -O2)
endif()

# I'm currently using the following script: cmake . -Bbuild
# -DCMAKE_CXX_COMPILER=clang++ -DNICKITAT_HSORT_WITH_TESTS=1 && cmake --build
# build
