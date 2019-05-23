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

set(additional_libs profiler) # from gperftools
if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
  set(additional_libs ${additional_libs} pthread)
endif()

set(additional_compiler_flags -std=c++11 -g -O2)

if(NICKITAT_HSORT_WITH_TESTS)
  add_executable(nickitat_hsort_tests test/test.cpp)
  target_link_libraries(nickitat_hsort_tests nickitat_hsort ${additional_libs})
  target_compile_options(nickitat_hsort_tests
                         PRIVATE ${additional_compiler_flags})

  if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    add_custom_command(TARGET nickitat_hsort_tests POST_BUILD
                       COMMAND dsymutil ${CMAKE_BINARY_DIR}/nickitat_hsort_tests
                       WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                       COMMENT "Running dsymutil on nickitat_hsort_tests...")
  endif()
endif()

if(NICKITAT_HSORT_WITH_BENCHMARKS)
  add_executable(nickitat_hsort_gbench_full benchmark/gbench_full.cpp)
  target_link_libraries(nickitat_hsort_gbench_full
                        nickitat_hsort
                        benchmark
                        benchmark_main
                        ${additional_libs})
  target_compile_options(nickitat_hsort_gbench_full
                         PRIVATE ${additional_compiler_flags})

  add_executable(nickitat_hsort_gbench_random benchmark/gbench_random.cpp)
  target_link_libraries(nickitat_hsort_gbench_random
                        nickitat_hsort
                        benchmark
                        benchmark_main
                        ${additional_libs})
  target_compile_options(nickitat_hsort_gbench_random
                         PRIVATE ${additional_compiler_flags})

  if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    add_custom_command(
      TARGET nickitat_hsort_gbench_random POST_BUILD
      COMMAND dsymutil ${CMAKE_BINARY_DIR}/nickitat_hsort_gbench_random
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Running dsymutil on nickitat_hsort_gbench_random...")
  endif()
endif()

# I'm currently using the following script: cmake . -Bbuild
# -DCMAKE_CXX_COMPILER=clang++ -DNICKITAT_HSORT_WITH_TESTS=1 && cmake --build
# build
