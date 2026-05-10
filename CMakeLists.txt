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

# Ваша программа
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE sequence)
target_compile_features(my_app PRIVATE cxx_std_17)