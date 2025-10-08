# Relatório Técnico - Sistema Kanban Lite
## Arquitetura e Decisões de Design em C++ Moderno

### 1. Introdução e Visão Geral

O **Sistema Kanban Lite** é uma aplicação completa de gerenciamento de tarefas desenvolvida em C++ moderno, implementando os princípios do método Kanban para organização de fluxo de trabalho. Este documento detalha as decisões de arquitetura e design que orientaram o desenvolvimento, justificando a aplicação de conceitos avançados de Programação Orientada a Objetos e padrões de projeto modernos.

A arquitetura foi concebida para demonstrar domínio dos requisitos acadêmicos enquanto entrega uma aplicação robusta e escalável, seguindo as melhores práticas da indústria para desenvolvimento em C++.

### 2. Arquitetura em Camadas e Separação de Responsabilidades

#### 2.1. Estrutura em Camadas

O sistema adota uma arquitetura em **quatro camadas principais**, cada uma com responsabilidades bem definidas:

**Camada de Domínio (Model)**
- **Entidades**: `Board`, `Column`, `Card`, `User`, `Tag`, `ActivityLog`
- **Responsabilidade**: Representar o coração do negócio e encapsular as regras fundamentais
- **Justificativa**: Isolamento completo da lógica de negócio das preocupações técnicas

**Camada de Aplicação (Service Layer)**
- **Componentes**: `KanbanService`, `CLIController`
- **Responsabilidade**: Orquestrar operações complexas e coordenar o fluxo entre camadas
- **Justificativa**: Implementação do padrão **Facade** para simplificar a API externa

**Camada de Persistência (Repository)**
- **Componentes**: `MemoryRepository`, `FileRepository`, interfaces `IRepository`
- **Responsabilidade**: Abstrair o armazenamento de dados
- **Justificativa**: **Dependency Inversion Principle** - o domínio não depende de implementações concretas

**Camada de Apresentação (View)**
- **Componentes**: `CLIView`, `MainWindow`, `ColumnWidget`, `CardWidget`
- **Responsabilidade**: Interface com o usuário final
- **Justificativa**: Separação radical entre lógica de apresentação e lógica de negócio

#### 2.2. Vantagens da Abordagem em Camadas

- **Testabilidade**: Cada camada pode ser testada independentemente
- **Manutenibilidade**: Mudanças em uma camada não afetam as outras
- **Substituibilidade**: É possível trocar implementações (ex: GUI por CLI) sem modificar o domínio
- **Evolução Independente**: Cada camada pode evoluir em ritmos diferentes

### 3. Princípios SOLID e Padrões de Projeto Aplicados

#### 3.1. Single Responsibility Principle (SRP)

Cada classe possui uma única responsabilidade bem definida:

```cpp
// Board: Gerenciar colunas e movimentação de cards
// Column: Gerenciar ordem e posicionamento de cards  
// Card: Representar uma tarefa com seus atributos
// KanbanService: Orquestrar operações do sistema
```

#### 3.2. Open/Closed Principle (OCP)

O sistema é aberto para extensão, mas fechado para modificação:

```cpp
// Interface IFilter permite novos filtros sem modificar Card
class IFilter {
public:
    virtual bool matches(const domain::Card& card) const = 0;
    virtual std::unique_ptr<IFilter> clone() const = 0;
};
```

#### 3.3. Liskov Substitution Principle (LSP)

Todas as implementações de interface são completamente substituíveis:

```cpp
// CLIView e GUI são substituíveis via IView
// MemoryRepository e FileRepository são substituíveis via IRepository
```

#### 3.4. Interface Segregation Principle (ISP)

Interfaces são específicas e coesas:

```cpp
// IRepository foca apenas em operações CRUD
// IService foca em operações de negócio
// IView foca em apresentação
```

#### 3.5. Dependency Inversion Principle (DIP)

Módulos de alto nível não dependem de módulos de baixo nível:

```cpp
// KanbanService depende de IRepository, não de implementações concretas
class KanbanService : public interfaces::IService {
private:
    persistence::MemoryRepository<domain::Board> boardRepository_;
    // Depende da abstração, não de implementações específicas
};
```

### 4. Composição vs Herança: Análise das Decisões

#### 4.1. Uso de Composição

**Composição como Relacionamento Primário**:

```cpp
class Board {
private:
    std::vector<std::shared_ptr<Column>> columns_;  // COMPOSIÇÃO
    std::shared_ptr<ActivityLog> activityLog_;      // AGREGAÇÃO
};

class Column {
private:
    std::vector<std::shared_ptr<Card>> cards_;      // COMPOSIÇÃO
};

class Card {
private:
    std::vector<std::shared_ptr<Tag>> tags_;        // AGREGAÇÃO
};
```

**Justificativa para Composição**:
- **Flexibilidade**: Cards podem existir independentemente de Tags
- **Reuso**: Tags podem ser compartilhadas entre múltiplos Cards
- **Vida Útil Desacoplada**: ActivityLog pode sobreviver ao Board
- **Manutenibilidade**: Mais fácil modificar relacionamentos

#### 4.2. Uso de Herança

**Herança Aplicada Corretamente**:

```cpp
// Herança para polimorfismo de interfaces
class IRepository { /* interface pura */ };
class MemoryRepository : public IRepository { /* implementação */ };

// Herança para especialização de framework
class MainWindow : public QMainWindow { /* especialização Qt */ };
class CardWidget : public QWidget { /* especialização Qt */ };
```

**Justificativa para Herança**:
- **Polimorfismo Real**: Interfaces definem contratos claros
- **Extensão Framework**: Herança necessária para integração Qt
- **Não Violação LSP**: Todas as substituições são válidas

### 5. Gerenciamento de Recursos e Smart Pointers

#### 5.1. Estratégia de Memory Management

**std::shared_ptr para Ownership Compartilhado**:

```cpp
// Quando múltiplos owners fazem sentido
std::shared_ptr<Board> board = std::make_shared<Board>(id, name);
board->addColumn(std::make_shared<Column>(columnId, "To Do"));

// Cards compartilhados entre Column e repositório
auto card = std::make_shared<Card>(cardId, title);
column->addCard(card);           // Column tem referência
cardRepository_.add(card);       // Repository tem referência
```

**Justificativa para shared_ptr**:
- **Ciclo de Vida Complexo**: Entidades são referenciadas em múltiplos lugares
- **Segurança**: Eliminação de dangling pointers
- **RAII Automático**: Destruição automática quando último owner é destruído

**std::unique_ptr para Ownership Exclusivo**:

```cpp
// KanbanService é dono exclusivo dos repositórios
std::unique_ptr<application::KanbanService> service_;

// Clone polimórfico de IFilter
std::unique_ptr<IFilter> clone() const = 0;
```

**Justificativa para unique_ptr**:
- **Sem Overhead**: Mais eficiente que shared_ptr
- **Intenção Clara**: Ownership exclusivo e não-transferível
- **Segurança**: Impossibilidade de acidentais shared ownership

#### 5.2. RAII para Recursos Não-Memória

```cpp
// QObject hierarchy gerencia recursos Qt automaticamente
// std::fstream em FileRepository gerencia arquivos
// std::chrono gerencia timestamps
```

### 6. Templates e STL: Uso Avançado

#### 6.1. Template de Repository

```cpp
template<typename T, typename Id = std::string>
class IRepository {
public:
    virtual void add(const std::shared_ptr<T>& item) = 0;
    virtual std::vector<std::shared_ptr<T>> getAll() const = 0;
    virtual std::optional<std::shared_ptr<T>> findById(const Id& id) const = 0;
};
```

**Vantagens do Template**:
- **Type Safety**: Compilador verifica tipos em tempo de compilação
- **Reuso**: Mesma interface para Board, Column, Card, User
- **Performance**: Sem overhead de runtime vs abordagens baseadas em void*

#### 6.2. Uso de Containers STL

```cpp
// std::vector para coleções ordenadas
std::vector<std::shared_ptr<Column>> columns_;

// std::map para lookup eficiente por ID  
std::map<Id, std::shared_ptr<T>> data_;

// std::optional para valores que podem não existir
std::optional<std::shared_ptr<Board>> findBoard(const std::string& boardId) const;

// std::chrono para timestamps precisos
using TimePoint = std::chrono::system_clock::time_point;
```

### 7. Sistema de Exceções e Tratamento de Erros

#### 7.1. Hierarquia de Exceções

```cpp
// Exceções específicas por camada
class MemoryRepositoryException : public std::runtime_error {
    explicit MemoryRepositoryException(const std::string& what);
};

class FileRepositoryException : public std::runtime_error {
    explicit FileRepositoryException(const std::string& what);
};
```

#### 7.2. Estratégia de Tratamento

**Exceções para Condições Excepcionais**:
- Arquivo não encontrado
- ID duplicado em repositório
- Entidade não encontrada

**Códigos de Retorno para Casos Normais**:
- `std::optional` para entidades que podem não existir
- `bool` de sucesso/falha para operações não-críticas

### 8. Polimorfismo e Ligação Tardia

#### 8.1. Polimorfismo de Interface

```cpp
// Diferentes implementações de View
CLIView : public IView    // Texto no terminal
MainWindow : public QObject // GUI gráfica

// Diferentes implementações de Repository  
MemoryRepository : public IRepository  // Memória volátil
FileRepository : public IRepository    // Persistência em arquivo
```

#### 8.2. Uso de dynamic_cast

```cpp
// Uso judicioso apenas quando necessário
ColumnWidget* columnWidget = qobject_cast<ColumnWidget*>(parent);
if (columnWidget) {
    // Operações específicas de ColumnWidget
}
```

### 9. Padrões de Design Implementados

#### 9.1. Repository Pattern
Abstrai a camada de dados, permitindo trocar entre memória, arquivo ou banco sem afetar o domínio.

#### 9.2. Service Layer Pattern
`KanbanService` atua como fachada para operações complexas, simplificando a API para clientes.

#### 9.3. MVC/MVP Pattern
Separação clara entre Model (domínio), View (GUI/CLI) e Controller/Presenter.

#### 9.4. Strategy Pattern
`IFilter` permite diferentes algoritmos de filtragem serem aplicados a Cards.

#### 9.5. Factory Method Pattern
`IFilter::clone()` permite criação polimórfica de objetos.

### 10. Decisões de API e Design de Interfaces

#### 10.1. Design por Contrato

```cpp
// Interfaces explícitas com pré/pós-condições
std::string addColumn(const std::string& boardId, const std::string& columnName);
// Pré-condição: boardId existe
// Pós-condição: coluna criada e associada ao board
```

#### 10.2. Princípio do Menor Surpresa

```cpp
// Nomes consistentes e comportamentos previsíveis
createBoard(), addColumn(), addCard(), moveCard()
// Sempre retornam ID da entidade criada
```

### 11. Considerações de Performance

#### 11.1. Otimizações Implementadas

- **Move Semantics**: Transferência eficiente de objetos
- **Passagem por Referência**: Evitar cópias desnecessárias
- **Reserva de Capacidade**: `std::vector::reserve()` quando possível
- **Lookup Eficiente**: `std::map` para busca por ID (O(log n))

#### 11.2. Trade-offs Aceitáveis

- **shared_ptr Overhead**: Aceito em troca de segurança
- **Cópias de vetores**: Em alguns casos, preferido sobre referências para segurança
- **Dynamic Dispatch**: Necessário para polimorfismo real

### 12. Conclusão

O Sistema Kanban Lite demonstra uma aplicação madura dos princípios de POO em C++ moderno. As decisões de arquitetura refletem um equilíbrio cuidadoso entre:

- **Abstração** vs **Performance**
- **Flexibilidade** vs **Simplicidade**
- **Segurança** vs **Eficiência**

A escolha por **composição sobre herança** sempre que possível, o uso adequado de **smart pointers** para gerenciamento automático de recursos, e a aplicação consistente dos **princípios SOLID** resultaram em um sistema que é ao mesmo tempo robusto, maintainable e extensível.

A arquitetura em camadas, combinada com interfaces bem definidas, permite que o sistema evolua naturalmente para incluir novas funcionalidades como persistência em banco de dados, integração com sistemas externos, ou diferentes interfaces de usuário, sempre mantendo o núcleo de domínio estável e independente.

Este projeto serve como exemplar de como os conceitos acadêmicos de POO podem ser aplicados em cenários do mundo real, produzindo código de qualidade profissional que atende tanto aos requisitos funcionais quanto aos não-funcionais de um sistema de software moderno.