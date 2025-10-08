cmake_minimum_required(VERSION 3.16)
project(KanbanSystem VERSION 3.0.0 LANGUAGES CXX)

# Configuração do padrão C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Buscar pacotes do Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Ativar MOC automático
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Configurar modo de build
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

# Incluir diretórios de headers
include_directories(include)

# Arquivos fonte comuns (sem main)
set(COMMON_SOURCES
    src/domain/Card.cpp
    src/domain/Column.cpp
    src/domain/ActivityLog.cpp
    src/domain/Board.cpp
    src/domain/User.cpp
    src/persistence/MemoryRepository.cpp
    src/application/KanbanService.cpp
    src/application/CLIView.cpp
    src/application/CLIController.cpp
)

# Arquivos fonte da GUI
set(GUI_SOURCES
    src/gui/main_gui.cpp
    src/gui/MainWindow.cpp
    src/gui/ColumnWidget.cpp    # ADICIONE
    src/gui/CardWidget.cpp      # ADICIONE
    src/gui/CardDialog.cpp
)


# Criar biblioteca comum
add_library(kanban_common STATIC ${COMMON_SOURCES})

# Executável CLI
add_executable(kanban_cli src/application/main.cpp)
target_link_libraries(kanban_cli kanban_common)

# Executável GUI - INCLUA OS HEADERS PARA MOC
add_executable(kanban_gui 
    ${GUI_SOURCES}
    include/gui/MainWindow.h  # INCLUA OS HEADERS PARA MOC
    include/gui/ColumnWidget.h  # ADICIONE
    include/gui/CardWidget.h    # ADICIONE
    include/gui/CardDialog.h
)
target_link_libraries(kanban_gui kanban_common Qt6::Core Qt6::Widgets)

# Configurações de compiler
if(MSVC)
    target_compile_options(kanban_cli PRIVATE /W4 /permissive-)
    target_compile_options(kanban_gui PRIVATE /W4 /permissive-)
else()
    target_compile_options(kanban_cli PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(kanban_gui PRIVATE -Wall -Wextra -pedantic)
endif()

# Propriedades dos targets
set_target_properties(kanban_cli kanban_gui PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Configurar saída dos executáveis
set_target_properties(kanban_cli kanban_gui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Mensagem de sucesso
message(STATUS " ")
message(STATUS "✅ Configuração CMake concluída!")
message(STATUS "👉 Para compilar: cmake --build .")
message(STATUS "👉 CLI: ./bin/kanban_cli")
message(STATUS "👉 GUI: ./bin/kanban_gui")
message(STATUS " ")