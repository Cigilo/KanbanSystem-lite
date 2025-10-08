# Kanban System Lite - Etapa 3: Entrega Final

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-yellow.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey.svg)

## 📋 Índice
- [Visão Geral](#-visão-geral)
- [🚀 Funcionalidades](#-funcionalidades)
- [🏗️ Arquitetura](#️-arquitetura)
- [📦 Requisitos](#-requisitos)
- [🔧 Build e Instalação](#-build-e-instalação)
- [🎮 Como Usar](#-como-usar)
- [📊 Conceitos POO Implementados](#-conceitos-poo-implementados)
- [🎥 Demonstração](#-demonstração)
- [🏷️ Versão](#️-versão)
- [📁 Estrutura do Projeto](#-estrutura-do-projeto)
- [🤝 Contribuição](#-contribuição)
- [📄 Licença](#-licença)

## 🎯 Visão Geral

O **Kanban System Lite** é uma aplicação completa de gerenciamento de tarefas baseada no método Kanban, desenvolvida em C++ moderno com interface gráfica Qt6. Este projeto representa a **Etapa 3** do trabalho de Programação Orientada a Objetos, demonstrando a aplicação prática de conceitos avançados de POO em um sistema real.

**Versão**: 3.0 Final  
**Data de Entrega**: 06/10/2025  
**Tag no GitHub**: `v3.0-final`

## 🚀 Funcionalidades

### ✨ Funcionalidades Principais
- ✅ **Gerenciamento de Múltiplos Boards** - Crie e organize diferentes quadros Kanban
- ✅ **Colunas Personalizáveis** - Adicione colunas com nomes customizados
- ✅ **Cards Completos** - Tarefas com título, descrição, prioridade e tags
- ✅ **Drag & Drop Avançado** - Movimente cards entre colunas com interface intuitiva
- ✅ **Reordenação Interna** - Mova cards para cima/baixo dentro da mesma coluna
- ✅ **Sistema de Filtros** - Filtre por tags e prioridades em tempo real
- ✅ **Histórico de Atividades** - Registro completo de todas as ações
- ✅ **Estatísticas em Tempo Real** - Métricas de uso e distribuição
- ✅ **Interface Moderna** - Tema escuro com design responsivo

### 🎯 Modos de Operação
- **GUI (Graphical User Interface)**: Interface gráfica completa com Qt6
- **CLI (Command Line Interface)**: Interface de linha de comando para automação

## 🏗️ Arquitetura

### 📐 Diagrama de Arquitetura
```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   APRESENTAÇÃO  │    │    APLICAÇÃO     │    │     DOMÍNIO     │
├─────────────────┤    ├──────────────────┤    ├─────────────────┤
│  MainWindow     │◄───│  KanbanService   │───►│     Board       │
│  ColumnWidget   │    │  CLIController   │    │     Column      │
│  CardWidget     │    │                  │    │      Card       │
│  CardDialog     │    │                  │    │      User       │
│  CLIView        │    │                  │    │  ActivityLog    │
└─────────────────┘    └──────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                     ┌───────────┴───────────┐
                     │      PERSISTÊNCIA     │
                     ├───────────────────────┤
                     │   IRepository<T>      │
                     │  MemoryRepository     │
                     │   FileRepository      │
                     └───────────────────────┘
```

### 🧩 Componentes Principais

**Camada de Domínio** (`domain/`)
- `Board` - Quadro Kanban contendo colunas
- `Column` - Coluna dentro de um board
- `Card` - Tarefa individual com metadados
- `User` - Usuário do sistema
- `ActivityLog` - Histórico de atividades
- `Tag` - Etiquetas para categorização

**Camada de Aplicação** (`application/`)
- `KanbanService` - Orquestrador principal do sistema
- `CLIController` - Controlador da interface CLI
- `CLIView` - Renderizador de saída em terminal

**Camada de Apresentação** (`gui/`)
- `MainWindow` - Janela principal da aplicação
- `ColumnWidget` - Widget visual para colunas
- `CardWidget` - Widget visual para cards
- `CardDialog` - Diálogo de edição de cards

**Camada de Persistência** (`persistence/`)
- `IRepository<T>` - Interface genérica para repositórios
- `MemoryRepository` - Implementação em memória
- `FileRepository` - Implementação em arquivo

## 📦 Requisitos

### 🖥️ Requisitos de Sistema

**Linux (Ubuntu/Debian)**
```bash
# Dependências
sudo apt update
sudo apt install build-essential cmake qt6-base-dev
```

**Linux (Fedora)**
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

**Windows**
- Visual Studio 2019+ **OU** MinGW-w64
- CMake 3.16+
- Qt6 (via installer ou vcpkg)

### 🔧 Requisitos Técnicos
- **Compilador**: GCC 9+, Clang 10+, MSVC 2019+
- **Padrão C++**: C++17
- **CMake**: 3.16+
- **Qt**: 6.0+

## 🔧 Build e Instalação

### 🐧 Linux

#### 1. Clone o repositório
```bash
git clone https://github.com/seu-usuario/KanbanSystem-lite.git
cd KanbanSystem-lite
git checkout v3.0-final
```

#### 2. Configure o build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 3. Compile
```bash
make -j$(nproc)
```

#### 4. Execute
```bash
# Interface Gráfica
./bin/kanban_gui

# Interface de Linha de Comando
./bin/kanban_cli
```

### 🪟 Windows

#### Com Visual Studio
```bash
# No Developer Command Prompt
git clone https://github.com/seu-usuario/KanbanSystem-lite.git
cd KanbanSystem-lite
git checkout v3.0-final

mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release

# Executáveis em build/bin/Release/
./bin/Release/kanban_gui.exe
```

#### Com MSYS2/Mingw64
```bash
# Instale as dependências
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-qt6

# Build
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make

./bin/kanban_gui.exe
```

## 🎮 Como Usar

### 🖥️ Interface Gráfica (GUI)

#### 1. 🏠 Tela Inicial
- A aplicação inicia com dados de exemplo carregados
- Painel lateral esquerdo: lista de boards
- Área central: visualização do board selecionado
- Painel direito: histórico de atividades e estatísticas

#### 2. 📋 Criando um Board
1. Clique em "Criar Novo Board" no painel esquerdo
2. Digite o nome do board
3. Pressione Enter ou clique no botão "Criar Board"

#### 3. 🗂️ Adicionando Colunas
1. Selecione um board na lista
2. Clique em "Adicionar Coluna" 
3. Digite o nome da coluna (ex: "To Do", "Doing", "Done")

#### 4. 🎴 Criando Cards
1. Clique em "+ Adicionar Card" em qualquer coluna
2. Preencha o título do card
3. (Opcional) Adicione descrição, prioridade e tags

#### 5. 🔄 Movimentando Cards
- **Drag & Drop**: Arraste cards entre colunas
- **Reordenação**: Use os botões ⬆️⬇️ nos cards
- **Edição**: Duplo-clique em um card para editar

#### 6. 🔍 Filtros
- **Por Tags**: Selecione tags no combobox de filtros
- **Por Prioridade**: Marque as prioridades desejadas
- **Aplicar**: Clique em "Aplicar Filtros"
- **Limpar**: Clique em "Limpar Filtros"

### 💻 Interface de Linha de Comando (CLI)

#### Comandos Disponíveis
```bash
# Criar um novo board
create-board "Meu Projeto"

# Listar todos os boards  
list-boards

# Mover card entre colunas
move-card board_1 card_1 column_todo column_doing

# Ajuda
help

# Sair
exit
```

#### Exemplo de Sessão CLI
```bash
> create-board "Projeto POO"
>> Board criado: 'Projeto POO' (ID: board_1)

> list-boards
*** BOARDS DISPONIVEIS ***
==================================================
> Projeto POO (ID: board_1) - 0 colunas

> help
Comandos disponiveis:
  create-board <nome do board>    - Cria um novo quadro e imprime o ID
  move-card <boardId> <cardId> <fromColumnId> <toColumnId> - Move um card entre colunas
  list-boards                     - Lista todos os boards
  help                            - Mostra esta ajuda
  exit                            - Sai do programa
```

## 📊 Conceitos POO Implementados

### ✅ 1. Abstração & Encapsulamento
- **Interfaces claras** com separação `.h`/`.cpp`
- **Campos privados** com getters/setters controlados
- **Exemplo**: `Card::setPriority()` com validação implícita

### ✅ 2. Classes e Objetos
- **Modelo coerente**: `Board`, `Column`, `Card`, `User`, `ActivityLog`, `Tag`
- **Controllers**: `KanbanService`, `CLIController` 
- **Views**: `CLIView`, `MainWindow`, `ColumnWidget`, `CardWidget`

### ✅ 3. Herança & Polimorfismo
- **Interfaces abstratas**: `IFilter`, `IRepository<T>`, `IService`, `IView`
- **Métodos virtuais puros**: `IFilter::matches()`, `IFilter::clone()`
- **Herança para polimorfismo**: `CLIView : public IView`

### ✅ 4. Composição vs Herança
- **Composição**: `Board` contém `Column`, `Column` contém `Card`
- **Agregação**: `Card` possui `Tag`
- **Herança apenas para polimorfismo**

### ✅ 5. Polimorfismo Dinâmico
- **Ponteiros polimórficos**: `KanbanService` usa `IRepository<T>`
- **Referências polimórficas**: `CLIController` com `IView&`
- **Interfaces sobre `dynamic_cast`**

### ✅ 6. Gerenciamento de Recursos
- **RAII**: Repositórios gerenciam próprios recursos
- **Smart pointers**: 
  - `std::shared_ptr<Card>` em `Column::cards_`
  - `std::unique_ptr<KanbanService>` em `MainWindow`
  - `std::unique_ptr<IFilter>` para cópia polimórfica

### ✅ 7. Templates e STL
- **Templates**: `MemoryRepository<T, Id>`, `IRepository<T, Id>`
- **STL Containers**: 
  - `std::vector` em `ActivityLog::activities_`
  - `std::map` em `MemoryRepository::data_`
  - `std::optional` em `Card::description_`

### ✅ 8. Sobrecarga de Operadores
- **Comparação**: `Card::operator==`, `Card::operator<`
- **Saída**: `Card::operator<<`, `Activity::operator<<`
- **Igualdade**: `User::operator==`, `User::operator!=`

### ✅ 9. Tratamento de Exceções
- **Exceções customizadas**: `MemoryRepositoryException`
- **Validações**: `KanbanService::validateBoardExists()`
- **Captura adequada**: Try-catch global em `main.cpp`

### ✅ 10. Documentação Técnica
- **Diagrama de Classes UML** completo
- **Documentação Doxygen** em todos os headers
- **README** com instruções detalhadas

### ✅ 11. Build Automatizado
- **CMake** multiplataforma configurado
- **Dependências declaradas**: Qt6, C++17

## 🎥 Demonstração

### 📹 Vídeo de Demonstração
**Duração**: 2 minutos 45 segundos  
**Link**: [Assistir Demonstração](https://github.com/seu-usuario/KanbanSystem-lite/demo/video_demonstracao.mp4)

### 🎯 Conteúdo Demonstrado
1. **Inicialização** da aplicação GUI
2. **Criação** de board e colunas
3. **Gestão** de cards com tags e prioridades
4. **Movimentação** via drag & drop
5. **Reordenação** interna de cards
6. **Aplicação** de filtros avançados
7. **Visualização** do histórico de atividades
8. **Demonstração** da interface CLI
9. **Estatísticas** em tempo real

## 🏷️ Versão

### 📌 Versão Atual
- **Tag**: `v3.0-final`
- **Data**: 06/10/2025
- **Status**: Entrega Final

### 🔗 Acesso à Versão
```bash
git clone https://github.com/seu-usuario/KanbanSystem-lite.git
cd KanbanSystem-lite
git checkout v3.0-final
```

## 📁 Estrutura do Projeto

```
KanbanSystem-lite/
├── design/                          # Código fonte principal
│   ├── include/                     # Headers organizados por camada
│   │   ├── domain/                  # Entidades de domínio
│   │   │   ├── Board.h
│   │   │   ├── Column.h
│   │   │   ├── Card.h
│   │   │   ├── User.h
│   │   │   ├── ActivityLog.h
│   │   │   └── Tag.h
│   │   ├── interfaces/              # Interfaces abstratas
│   │   │   ├── IService.h
│   │   │   ├── IView.h
│   │   │   ├── IRepository.h
│   │   │   └── IFilter.h
│   │   ├── application/             # Camada de aplicação
│   │   │   ├── KanbanService.h
│   │   │   ├── CLIView.h
│   │   │   └── CLIController.h
│   │   ├── persistence/             # Camada de persistência
│   │   │   ├── MemoryRepository.h
│   │   │   ├── MemoryRepositoryImpl.h
│   │   │   └── FileRepository.h
│   │   └── gui/                     # Interface gráfica
│   │       ├── MainWindow.h
│   │       ├── ColumnWidget.h
│   │       ├── CardWidget.h
│   │       └── CardDialog.h
│   ├── src/                         # Implementações
│   │   ├── domain/                  # Implementações do domínio
│   │   │   ├── Board.cpp
│   │   │   ├── Column.cpp
│   │   │   ├── Card.cpp
│   │   │   ├── User.cpp
│   │   │   └── ActivityLog.cpp
│   │   ├── application/             # Implementações da aplicação
│   │   │   ├── KanbanService.cpp
│   │   │   ├── CLIView.cpp
│   │   │   ├── CLIController.cpp
│   │   │   └── main.cpp
│   │   ├── persistence/             # Implementações de persistência
│   │   │   └── MemoryRepository.cpp
│   │   └── gui/                     # Implementações da GUI
│   │       ├── MainWindow.cpp
│   │       ├── ColumnWidget.cpp
│   │       ├── CardWidget.cpp
│   │       ├── CardDialog.cpp
│   │       └── main_gui.cpp
│   └── CMakeLists.txt               # Configuração de build
├── tests/                           # Testes unitários
│   └── compile_test.cpp
├── docs/                            # Documentação adicional
│   ├── uml/                         # Diagramas UML
│   │   ├── diagram.md
│   │   ├── uml.png
│   │   └── uml.svg
│   └── relatorio_tecnico.pdf        # Relatório técnico completo
├── demo/                            # Materiais de demonstração
│   └── video_demonstracao.mp4       # Vídeo de demonstração
└── README.md                        # Este arquivo
```

## 🤝 Contribuição

Contribuições são bem-vindas! Para contribuir:

1. Fork o projeto
2. Crie uma branch para sua feature (`git checkout -b feature/AmazingFeature`)
3. Commit suas mudanças (`git commit -m 'Add some AmazingFeature'`)
4. Push para a branch (`git push origin feature/AmazingFeature`)
5. Abra um Pull Request

## 📄 Licença

Distribuído sob a licença MIT. Veja `LICENSE` para mais informações.

## 👨‍💻 Autor

**Seu Nome**  
- Email: seu.email@example.com  
- GitHub: [seu-usuario](https://github.com/seu-usuario)

## 🔗 Links

- **Repositório**: [https://github.com/seu-usuario/KanbanSystem-lite](https://github.com/seu-usuario/KanbanSystem-lite)
- **Issues**: [https://github.com/seu-usuario/KanbanSystem-lite/issues](https://github.com/seu-usuario/KanbanSystem-lite/issues)
- **Releases**: [https://github.com/seu-usuario/KanbanSystem-lite/releases](https://github.com/seu-usuario/KanbanSystem-lite/releases)

---

<div align="center">

**🎓 Desenvolvido para o Trabalho de Programação Orientada a Objetos - Etapa 3**  
**📅 Entrega: 06/10/2025**  
**⭐ Se este projeto foi útil, deixe uma estrela no repositório!**

</div>