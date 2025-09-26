cmake_minimum_required(VERSION 3.15)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclui diretório de headers (agora relativo à pasta design/)
include_directories(include)

# Cria executável de teste
add_executable(compile_test ../tests/compile_test.cpp)