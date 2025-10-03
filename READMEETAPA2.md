# 🚀 **README COMPLETO - ETAPA 2: KANBAN SYSTEM CLI**

## 📋 **ÍNDICE**
- [Visão Geral](#visão-geral)
- [Requisitos Atendidos](#requisitos-atendidos)
- [Como Executar](#como-executar)
- [Funcionalidades Demonstradas](#funcionalidades-demonstradas)
- [Arquitetura do Sistema](#arquitetura-do-sistema)
- [Tecnologias e Boas Práticas](#tecnologias-e-boas-práticas)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Saída Esperada](#saída-esperada)

---

## 🎯 **VISÃO GERAL**

Este projeto implementa um **Sistema de Gerenciamento de Tarefas Kanban** completo em C++ moderno, desenvolvido como parte da disciplina de Programação Orientada a Objetos. A **Etapa 2** foca na implementação de uma interface de linha de comando (CLI) totalmente funcional que demonstra todas as operações principais do sistema Kanban.

**Tag da Versão:** `v2-cli`  
**Data de Entrega:** 02/10/2025  
**Valor:** 4 pontos

---

## ✅ **REQUISITOS ATENDIDOS**

### 🎯 **Critério Principal: CLI Funcional**
- ✅ **Executável CLI** que demonstra operações-chave do Kanban
- ✅ **Domínio e lógica implementados** independentemente da GUI
- ✅ **Build funcional** com CMake
- ✅ **README com instruções** para rodar os testes CLI

### 🔧 **Critérios Técnicos Específicos**
- ✅ **Smart Pointers**: Uso adequado de `std::shared_ptr` e `std::unique_ptr`
- ✅ **STL Containers**: `std::vector`, `std::map`, `std::optional`, `std::chrono`
- ✅ **Tratamento de Exceções**: Hierarquia própria + `std::exception`
- ✅ **Operadores Sobrecarregados**: `operator<<` para output de objetos
- ✅ **C++ Moderno**: C++17 com move semantics e RAII

---

## 🚀 **COMO EXECUTAR**

### **Pré-requisitos**
- **MSYS2** com MinGW64 (Windows) ou **GCC** (Linux/macOS)
- **CMake** versão 3.15 ou superior
- **Git** (para clone do repositório)

### **Compilação Rápida**

#### **Método 1: Usando Scripts (Recomendado)**
```bash
# Linux/macOS/MSYS2
./scripts/build.sh

# Windows
./scripts/build.bat
```

#### **Método 2: Compilação Manual**
```bash
# Navegar para o diretório do projeto
cd design

# Criar diretório de build e configurar
mkdir -p build
cd build
cmake .. -G "MSYS Makefiles"

# Compilar
cmake --build .

# Alternativa: usar make diretamente
make
```

### **Execução dos Testes CLI**

#### **Execução Rápida com Scripts**
```bash
# Linux/macOS/MSYS2
./scripts/run.sh

# Windows  
./scripts/run.bat
```

#### **Execução Manual**
```bash
cd build
./kanban_cli
```

---

## 🎪 **FUNCIONALIDADES DEMONSTRADAS**

### **Operações Principais do Kanban**
1. **✅ Criar Quadro (Board)** - `createBoard()`
2. **✅ Criar Coluna (List)** - `addColumn()`
3. **✅ Criar Cartão (Card)** - `addCard()`
4. **✅ Mover Cartão entre Colunas** - `moveCard()`
5. **✅ Listar Todos os Elementos** - `listBoards()`, `listColumns()`, `listCards()`
6. **✅ Sistema de Histórico** - `ActivityLog` com registro de atividades

### **Demonstrações Técnicas Explícitas**
- **Smart Pointers**: Contagem de referências com `std::shared_ptr`
- **STL Containers**: Uso de `std::vector`, `std::map`, `std::optional`
- **Tratamento de Exceções**: Cenários de erro e recuperação
- **Operadores Sobrecarregados**: Output formatado de objetos
- **C++ Moderno**: Move semantics, RAII, chrono para timestamps

---

## 🏗️ **ARQUITETURA DO SISTEMA**

### **Diagrama de Camadas**
```
┌─────────────────┐
│    CLIView      │ ← Camada de Apresentação (IView)
├─────────────────┤
│  KanbanService  │ ← Camada de Aplicação (IService)
├─────────────────┤
│     Domínio     │ ← Lógica de Negócio Pura
│  ┌───────────┐  │
│  │   Board   │  │
│  │  Column   │  │
│  │   Card    │  │
│  │   User    │  │
│  │ ActivityLog│ │
│  └───────────┘  │
├─────────────────┤
│   Persistência  │ ← Acesso a Dados (IRepository)
│  ┌───────────┐  │
│  │ MemoryRepo│  │
│  │ FileRepo  │  │
│  └───────────┘  │
└─────────────────┘
```

### **Princípios de Design Aplicados**
- **✅ Separation of Concerns**: Cada camada tem responsabilidade única
- **✅ Dependency Inversion**: Interfaces abstraem implementações
- **✅ Open/Closed Principle**: Fácil extensão para GUI (Etapa 3)
- **✅ RAII**: Gerenciamento automático de recursos
- **✅ Exception Safety**: Operações atômicas e rollback automático

---

## ⚙️ **TECNOLOGIAS E BOAS PRÁTICAS**

### **C++ Moderno (C++17)**
```cpp
// Smart Pointers
std::shared_ptr<Board> board = std::make_shared<Board>("id", "name");
std::unique_ptr<IFilter> filter = someFilter->clone();

// STL Containers
std::vector<std::shared_ptr<Card>> cards;
std::map<std::string, std::shared_ptr<Board>> data_;
std::optional<std::string> description_;

// Modern C++ Features
using TimePoint = std::chrono::system_clock::time_point;
auto now = std::chrono::system_clock::now();
```

### **Tratamento de Exceções Robusto**
```cpp
// Hierarquia de exceções
class MemoryRepositoryException : public std::runtime_error {
    explicit MemoryRepositoryException(const std::string& what);
};

// Tratamento em operações críticas
try {
    service.moveCard(boardId, cardId, fromColId, toColId);
} catch (const MemoryRepositoryException& e) {
    // Exceção específica do repositório
} catch (const std::exception& e) {
    // Exceção genérica
}
```

### **Build System Profissional**
- **CMake** para build multiplataforma
- **Configurações de compiler** otimizadas
- **Dependências gerenciadas** automaticamente
- **Scripts de build** para diferentes ambientes

---

## 📁 **ESTRUTURA DO PROJETO**

```
KanbanSystem-lite/
├── design/
│   ├── include/
│   │   ├── domain/           # Modelo de Domínio
│   │   │   ├── Board.h
│   │   │   ├── Column.h
│   │   │   ├── Card.h
│   │   │   ├── User.h
│   │   │   └── ActivityLog.h
│   │   ├── interfaces/       # Contratos de Interface
│   │   │   ├── IService.h
│   │   │   ├── IView.h
│   │   │   ├── IRepository.h
│   │   │   └── IFilter.h
│   │   └── persistence/      # Camada de Persistência
│   │       ├── MemoryRepository.h
│   │       └── FileRepository.h
│   ├── src/
│   │   ├── domain/           # Implementações do Domínio
│   │   │   ├── Board.cpp
│   │   │   ├── Column.cpp
│   │   │   ├── Card.cpp
│   │   │   ├── User.cpp
│   │   │   └── ActivityLog.cpp
│   │   ├── persistence/      # Implementações de Persistência
│   │   │   └── MemoryRepository.cpp
│   │   └── application/      # Lógica de Aplicação
│   │       ├── KanbanService.cpp
│   │       ├── CLIView.cpp
│   │       └── main.cpp
│   ├── tests/
│   │   └── compile_test.cpp  # Testes de Compilação
│   ├── scripts/              # Scripts de Automação
│   │   ├── build.sh
│   │   ├── build.bat
│   │   ├── run.sh
│   │   └── run.bat
│   ├── uml/                  # Diagramas UML
│   │   ├── diagram.md
│   │   ├── uml.png
│   │   └── uml.svg
│   ├── CMakeLists.txt        # Configuração do Build
│   └── README.md             # Este arquivo
└── .gitignore
```

---

## 📊 **SAÍDA ESPERADA**

### **Execução Completa do CLI**
```
============================================================
KANBAN SYSTEM LITE - ETAPA 2 (CLI)
Data: Oct  1 2025 | Hora: 19:22:15
============================================================

*** DEMONSTRACAO: SMART POINTERS E STL ***
===========================================
1. std::shared_ptr - Shared Ownership:
   Use count: 2 (2 referencias)
2. std::unique_ptr - Exclusive Ownership:
   Ownership exclusivo - nao pode ser copiado
3. STL Containers:
   std::vector com 2 cards
   std::map no MemoryRepository: 1 item
4. std::optional:
   std::optional: valor nao encontrado (has_value() = false)

*** DEMONSTRACAO: TRATAMENTO DE EXCEÇÕES ***
============================================
1. Tentativa de mover card inexistente:
   ✅ Exceção capturada: Card não encontrado na coluna de origem: card_inexistente
2. Tentativa de acessar board inexistente:
   ✅ Exceção capturada: Board não encontrado: board_inexistente
3. Tentativa de ID duplicado no MemoryRepository:
   ✅ MemoryRepositoryException capturada: Item com id 'same_id' já existe

*** DEMONSTRACAO: DETALHES DO DOMINIO ***
=========================================
1. Board com ActivityLog:
   Board criado: Board de Demonstracao (ID: demo_board)
2. Gerenciamento de Column:
   Column adicionada: Coluna Demo
   Numero de colunas no board: 1
3. Card com tags e prioridade:
   Card criado: Card de Demonstracao (Prioridade: 2)
   Tags: 2 tags adicionadas
4. ActivityLog registrando atividades:
   Atividade registrada: Card criado manualmente
   Total de atividades: 1

*** INICIANDO DEMONSTRACAO AUTOMATICA ***
--------------------------------------------------
>> 4. Criando dados de exemplo com KanbanService...
>> 5. Listando todos os elementos do sistema...

*** BOARDS DISPONIVEIS ***
==================================================
> Projeto Kanban de Exemplo (ID: board_1) - 3 colunas
>> 6. Listando colunas do board 'Projeto Kanban de Exemplo'...

*** COLUNAS ***
----------------------------------------
- To Do (2 cards)
- Doing (1 cards)
- Done (1 cards)
>> 7. Detalhando cards por coluna...

>> COLUNA: To Do
  * Configurar ambiente de desenvolvimento [Prioridade: 0]
  * Implementar classes de dominio [Prioridade: 0]

>> COLUNA: Doing
  * Criar KanbanService [Prioridade: 0]

>> COLUNA: Done
  * Definir arquitetura do projeto [Prioridade: 0]
>> 8. Movendo card 'Configurar ambiente de desenvolvimento' de 'To Do' para 'Doing'...
✅ MOVIMENTO REALIZADO COM SUCESSO!
>> 9. Estado atualizado apos movimento:
   To Do: 1 cards
   Doing: 2 cards
   Done: 1 cards
>> 10. Ultima atividade registrada:
   Card 'Configurar ambiente de desenvolvimento' movido de 'To Do' para 'Doing'
>> 11. Criando novo board dinamicamente...
✅ NOVO BOARD CRIADO: 'Projeto CLI Demo' (ID: board_2)
>> 12. RESUMO DO NOVO BOARD:
   Backlog: 2 cards
   Desenvolvimento: 1 cards
   Testes: 0 cards
   Concluido: 0 cards
>> 13. Consultas e verificacoes finais:
   Total de boards no sistema: 2
   Board encontrado: Projeto CLI Demo
--------------------------------------------------
*** DEMONSTRACAO CONCLUIDA COM SUCESSO! ***

============================================================
🎉 ETAPA 2 - CLI CONCLUIDA COM SUCESSO!
✅ TODOS OS REQUISITOS ATENDIDOS:
   ✓ Smart Pointers: std::shared_ptr, std::unique_ptr
   ✓ STL: std::vector, std::map, std::optional, std::chrono
   ✓ Excecoes: Hierarquia propria + std::exception
   ✓ Operacoes Kanban: Criar, listar, mover cards
   ✓ Arquitetura: Dominio independente da GUI
   ✓ CMake: Build funcional e automatizado
📦 Sistema Kanban totalmente funcional em modo texto
👨‍💻 Pronto para a Etapa 3 (GUI)
============================================================
```

---

## 🐛 **SOLUÇÃO DE PROBLEMAS**

### **Erros Comuns e Soluções**

**Problema 1**: CMake não encontra o compilador
```bash
cmake .. -G "MSYS Makefiles" -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
```

**Problema 2**: Erros de linker ou includes
```bash
rm -rf build
mkdir build && cd build
cmake .. -G "MSYS Makefiles"
cmake --build .
```

**Problema 3**: Caracteres especiais no terminal
- Use a versão atualizada do código sem emojis
- Configure o terminal para usar codificação UTF-8

### **Verificação de Integridade**
```bash
# Verificar se todos os arquivos necessários existem
find . -name "*.cpp" -type f | grep -E "(main|Card|Column|Board|ActivityLog|User|MemoryRepository|KanbanService|CLIView)"

# Verificar estrutura de includes
find include -name "*.h" -type f
```

---

## 🔮 **PRÓXIMOS PASSOS**

### **Para Etapa 3 (GUI)**
- O sistema está **perfeitamente preparado** para interface gráfica
- Todas as **interfaces (IService, IView)** já estão definidas
- A **lógica de negócio** está completamente implementada e testada
- A **arquitetura** permite fácil substituição do CLI por GUI

### **Expansões Futuras**
- **Persistência em arquivo** (`FileRepository`)
- **Filtros avançados** (implementações de `IFilter`)
- **Sistema de usuários** e permissões
- **Interface web** ou mobile

---

## 📞 **SUPORTE**

Para questões relacionadas a este projeto:
1. Consulte este README e a documentação no código
2. Verifique a tag `v2-cli` no repositório para a versão exata da entrega
3. Execute os scripts de build fornecidos para verificação rápida

---

**🎉 Desenvolvido como parte da disciplina de Programação Orientada a Objetos em C++**  
**📅 Entrega: Etapa 2 - 02/10/2025**  
**🏷️ Versão: v2-cli**