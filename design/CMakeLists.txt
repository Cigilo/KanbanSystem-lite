cmake_minimum_required(VERSION 3.15)
project(KanbanSystem VERSION 1.0.0 LANGUAGES CXX)

# Configuracão do padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configurar modo de build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

# Incluir diretórios de headers
include_directories(include)

# Diretório de código fonte
set(SRC_DIR src)

# Arquivos fonte - REMOVA A LINHA DUPLICADA DO main.cpp
set(SRC_FILES
    ${SRC_DIR}/domain/Card.cpp
    ${SRC_DIR}/domain/Column.cpp
    ${SRC_DIR}/domain/ActivityLog.cpp 
    ${SRC_DIR}/domain/Board.cpp 
    ${SRC_DIR}/persistence/MemoryRepository.cpp
    ${SRC_DIR}/domain/User.cpp
    ${SRC_DIR}/application/KanbanService.cpp
    ${SRC_DIR}/application/CLIView.cpp 
    # REMOVA ESTA LINHA: ${SRC_DIR}/src/application/main.cpp ← DUPLICADO!
)

# Criar executável - APENAS UMA REFERÊNCIA AO main.cpp
add_executable(kanban_cli
    ${SRC_DIR}/application/main.cpp  # APENAS ESTA REFERÊNCIA
    ${SRC_FILES}
)

# Propriedades do target
set_target_properties(kanban_cli PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Configuracões específicas para Windows/MSYS
if(WIN32)
    if(CMAKE_CXX_COMPILER MATCHES ".*mingw.*")
        target_compile_definitions(kanban_cli PRIVATE _WIN32_WINNT=0x0601)
    endif()
    
    if(MSVC)
        target_compile_options(kanban_cli PRIVATE /W4 /permissive-)
    else()
        target_compile_options(kanban_cli PRIVATE -Wall -Wextra -pedantic)
    endif()
else()
    target_compile_options(kanban_cli PRIVATE -Wall -Wextra -pedantic)
endif()

# Mensagem de sucesso
message(STATUS " ")
message(STATUS "✅ Configuracão CMake concluída!")
message(STATUS "👉 Para compilar: cmake --build .")
message(STATUS "👉 Para executar: ./kanban_cli")
message(STATUS " ")