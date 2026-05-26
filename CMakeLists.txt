cmake_minimum_required(VERSION 3.14)
project(MyNewProject VERSION 1.0.0 LANGUAGES CXX)

# -------------------- Подключаем библиотеку структур данных --------------------
include(FetchContent)
FetchContent_Declare(
    data_structures
    GIT_REPOSITORY  https://github.com/TibytenFox/sequence
    GIT_TAG         main
)
FetchContent_MakeAvailable(data_structures)
# ----------------------------------------------------------------------------

# Программа
add_executable(app_set main_set.cpp)
target_link_libraries(app_set PRIVATE sequence)
target_compile_features(app_set PRIVATE cxx_std_17)

add_executable(app_matrix main_matrix.cpp)
target_link_libraries(app_matrix PRIVATE sequence)
target_compile_features(app_matrix PRIVATE cxx_std_17)

add_executable(app_queue main_queue.cpp)
target_link_libraries(app_queue PRIVATE sequence)
target_compile_features(app_queue PRIVATE cxx_std_17)

# Тесты
add_executable(run_tests_set test_set.cpp)
target_link_libraries(run_tests_set PRIVATE sequence)

enable_testing()
add_test(NAME SetTests COMMAND run_tests_set)

add_executable(run_tests_matrix test_matrix.cpp)
target_link_libraries(run_tests_matrix PRIVATE sequence)

enable_testing()
add_test(NAME MatrixTests COMMAND run_tests_matrix)

add_executable(run_tests_queue test_queue.cpp)
target_link_libraries(run_tests_queue PRIVATE sequence)

enable_testing()
add_test(NAME QueueTests COMMAND run_tests_queue)