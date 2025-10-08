# README de Design - Sistema Kanban Lite

## 📋 Visão Geral da Arquitetura

### Propósito do Sistema
O **Kanban System Lite** é uma aplicação completa de gerenciamento de tarefas baseada no método Kanban, desenvolvida em C++ moderno. O sistema implementa uma arquitetura em camadas que demonstra princípios avançados de POO, padrões de design e boas práticas de desenvolvimento.

### Princípios de Design Fundamentais
- **Separação de Responsabilidades**: Cada componente tem uma única responsabilidade bem definida
- **Baixo Acoplamento**: Interfaces bem definidas minimizando dependências
- **Alta Coesão**: Componentes relacionados são agrupados logicamente
- **Extensibilidade**: Sistema aberto para extensão, fechado para modificação

## 🏗️ Arquitetura em Camadas

### 1. Camada de Domínio (Domain Layer)

#### Entidades Principais

**Board (Quadro)**
```cpp
class Board {
    // Representa um quadro Kanban completo
    // Responsável por gerenciar colunas e coordenar movimentação
    // Mantém histórico de atividades através do ActivityLog
}
```

**Column (Coluna)**
```cpp
class Column {
    // Container de cards com ordenação preservada
    // Suporta inserção em posições específicas
    // Gerencia a ordem dos cards dentro da coluna
}
```

**Card (Cartão)**
```cpp
class Card {
    // Unidade fundamental de trabalho
    // Possui título, descrição, prioridade e tags
    // Implementa operadores de comparação para ordenação
}
```

**ActivityLog (Registro de Atividades)**
```cpp
class ActivityLog {
    // Sistema de auditoria e histórico
    // Registra timestamp e descrição de todas as ações significativas
    // Mantém ordenação cronológica automática
}
```

#### Padrões no Domínio

**Composition Over Inheritance**
```cpp
// Board COMPÕE Column (relação 1:N)
// Column COMPÕE Card (relação 1:N)  
// Card POSSUI Tag (relação N:M)
// Board ASSOCIA ActivityLog (relação 1:1)
```

**Value Objects**
```cpp
class Tag {
    // Objeto de valor imutável
    // Identificado por ID único
    // Sem identidade própria independente
}
```

### 2. Camada de Aplicação (Application Layer)

#### KanbanService
```cpp
class KanbanService : public IService {
    // Orquestrador principal do sistema
    // Coordena operações complexas entre múltiplas entidades
    // Aplica regras de negócio e validações
    // Gera IDs únicos sequenciais
}
```

**Responsabilidades:**
- Validação de regras de negócio
- Geração de IDs únicos
- Coordenação entre repositórios
- Aplicação de transações lógicas

#### CLIController
```cpp
class CLIController {
    // Controlador do modo interativo CLI
    // Parse de comandos e delegação para serviços
    // Separa lógica de UI da lógica de aplicação
}
```

### 3. Camada de Persistência (Persistence Layer)

#### Interface de Repository
```cpp
template<typename T, typename Id = std::string>
class IRepository {
    // Contrato genérico para operações CRUD
    // Tipo seguro através de templates
    // Suporta diferentes estratégias de persistência
}
```

#### Implementações

**MemoryRepository**
```cpp
template<typename T, typename Id>
class MemoryRepository : public IRepository<T, Id> {
    // Persistência em memória usando std::map
    // Ideal para testes e protótipos
    // Alta performance (O(log n) para operações)
}
```

**FileRepository** 
```cpp
template<typename T, typename Id>
class FileRepository : public IRepository<T, Id> {
    // Persistência em arquivo
    // Usa RAII para gerenciamento de recursos
    // Suporta diferentes formatos (JSON, XML, binário)
}
```

### 4. Camada de Apresentação (Presentation Layer)

#### CLI (Command Line Interface)
```cpp
class CLIView : public IView {
    // Interface textual completa
    // Formatação rica com cores e layouts
    // Suporte a modo interativo e demonstração
}
```

#### GUI (Graphical User Interface)

**MainWindow**
```cpp
class MainWindow : public QMainWindow {
    // Janela principal da aplicação
    // Layout com painéis laterais e área central
    // Sistema completo de filtros e estatísticas
}
```

**ColumnWidget**
```cpp
class ColumnWidget : public QFrame {
    // Representação visual de uma coluna
    // Suporte a drag & drop de cards
    // Área rolável com indicadores de posição
}
```

**CardWidget** 
```cpp
class CardWidget : public QWidget {
    // Cartão visual com informações completas
    // Drag & drop com preview
    // Edição in-place via double-click
}
```

## 🔧 Padrões de Design Implementados

### 1. Repository Pattern
**Problema**: Diferentes estratégias de persistência com mesma interface
**Solução**: Template `IRepository<T>` com implementações específicas
```cpp
// Uso consistente independente da implementação
boardRepository_.add(board);
columnRepository_.findById(columnId);
```

### 2. Service Layer Pattern  
**Problema**: Operações complexas envolvendo múltiplas entidades
**Solução**: `KanbanService` como fachada orquestradora
```cpp
// Operação atômica com validações
service.moveCard(boardId, cardId, fromColumnId, toColumnId);
```

### 3. Strategy Pattern
**Problema**: Diferentes algoritmos de filtragem
**Solução**: Interface `IFilter` com implementações polimórficas
```cpp
class IFilter {
    virtual bool matches(const Card& card) const = 0;
    virtual std::unique_ptr<IFilter> clone() const = 0;
};
```

### 4. Observer Pattern
**Problema**: Atualização automática da UI em resposta a mudanças
**Solução**: Sinais e slots do Qt conectando modelo e vista
```cpp
// GUI reage automaticamente a mudanças
connect(columnWidget, &ColumnWidget::cardMoved, 
        this, &MainWindow::onCardMoved);
```

### 5. Factory Method Pattern
**Problema**: Criação polimórfica de objetos
**Solução**: Método `clone()` em interfaces
```cpp
// Cópia polimórfica de filtros
auto filterCopy = filter->clone();
```

## 🎯 Princípios SOLID Aplicados

### Single Responsibility Principle (SRP)
- `Board`: Gerenciar colunas e movimentação de cards
- `Column`: Manter ordem e posicionamento de cards  
- `Card`: Representar tarefa com atributos
- `KanbanService`: Orquestrar operações do sistema
- `CLIView`: Renderizar saída textual

### Open/Closed Principle (OCP)
```cpp
// Aberto para extensão
class PriorityFilter : public IFilter {
    bool matches(const Card& card) const override;
};

// Fechado para modificação
void applyFilters(const std::vector<std::unique_ptr<IFilter>>& filters);
```

### Liskov Substitution Principle (LSP)
```cpp
// Qualquer IRepository pode substituir outra
IRepository<Board>* repo = new MemoryRepository<Board>();
IRepository<Board>* repo = new FileRepository<Board>(); // Comportamento idêntico
```

### Interface Segregation Principle (ISP)
```cpp
// Interfaces específicas e focadas
class IRepository { /* apenas CRUD */ };
class IService { /* operações de negócio */ };
class IView { /* renderização */ };
```

### Dependency Inversion Principle (DIP)
```cpp
// Domínio não depende de implementações
class KanbanService {
    // Depende de IRepository, não de MemoryRepository/FileRepository
};
```

## 🧠 Decisões de Arquitetura Críticas

### 1. Composição vs Herança

**Composição Preferida**:
```cpp
class Board {
private:
    std::vector<std::shared_ptr<Column>> columns_;  // Composição forte
    std::shared_ptr<ActivityLog> activityLog_;      // Agregação fraca
};
```

**Justificativa**:
- Flexibilidade para alterar relacionamentos
- Evita hierarquias de herança rígidas
- Melhor testabilidade e manutenção

**Herança Aplicada Corretamente**:
```cpp
// Para polimorfismo real de interfaces
class IRepository { /* interface */ };
class MemoryRepository : public IRepository { /* implementação */ };

// Para extensão de framework
class MainWindow : public QMainWindow { /* especialização */ };
```

### 2. Gerenciamento de Memória com Smart Pointers

**std::shared_ptr para Ownership Compartilhado**:
```cpp
// Quando múltiplos donos fazem sentido
auto card = std::make_shared<Card>(id, title);
column->addCard(card);      // Column tem referência
cardRepository_.add(card);  // Repository tem referência
```

**std::unique_ptr para Ownership Exclusivo**:
```cpp
// Quando há um único dono claro
std::unique_ptr<KanbanService> service_;
auto filter = std::make_unique<PriorityFilter>();
```

**Justificativa**:
- Eliminação de memory leaks
- Semântica de ownership clara
- Destruição automática com RAII

### 3. Sistema de Tipos com Templates

**Repository Genérico**:
```cpp
template<typename T, typename Id>
class IRepository {
    // Type-safe para qualquer entidade
    // Compile-time polymorphism
};
```

**Instanciações Explícitas**:
```cpp
template class MemoryRepository<Board>;
template class MemoryRepository<Column>;
template class MemoryRepository<Card>;
```

### 4. Tratamento de Erros com Exceções

**Hierarquia de Exceções**:
```cpp
class MemoryRepositoryException : public std::runtime_error;
class FileRepositoryException : public std::runtime_error;
```

**Estratégia Mista**:
```cpp
// Exceções para condições excepcionais
void add(const std::shared_ptr<T>& item) {
    if (exists(item->id())) {
        throw MemoryRepositoryException("ID duplicado");
    }
}

// std::optional para casos normais de "não encontrado"
std::optional<std::shared_ptr<T>> findById(const Id& id) const;
```

## 🔄 Fluxos de Dados Principais

### 1. Criação de Board
```
Usuário → MainWindow → KanbanService → BoardRepository → Board
                                      → ActivityLog
```

### 2. Movimentação de Card
```
Drag & Drop → ColumnWidget → MainWindow → KanbanService → Board
                                                         → Column (origem)
                                                         → Column (destino)  
                                                         → ActivityLog
```

### 3. Aplicação de Filtros
```
UI Controls → MainWindow → ColumnWidget::refreshCards() → CardWidget::applyFilter()
```

### 4. Persistência em Memória
```
KanbanService → MemoryRepository<T> → std::map<Id, std::shared_ptr<T>>
```

## 🎨 Design da Interface de Usuário

### Arquitetura GUI
```
MainWindow
├── Left Panel (Boards e Controles)
├── Central Area (TabWidget com Columns)
└── Right Panel (Atividades e Estatísticas)
```

### Sistema de Drag & Drop
**MimeData Personalizado**:
```cpp
// Dados ricos para operações complexas
mimeData->setData("application/x-card-id", cardId);
mimeData->setData("application/x-from-column", sourceColumnId);
mimeData->setData("application/x-card-title", cardTitle);
```

**Feedback Visual**:
- Indicadores de posição durante drag
- Highlight de colunas de destino
- Preview do card sendo arrastado

### Filtros em Tempo Real
**Predicate-based Filtering**:
```cpp
columnWidget->refreshCards([this](auto card) {
    return cardMatchesFilter(card);
});
```

**Filtros Combináveis**:
- Por tags (combobox com autocomplete)
- Por prioridade (checkboxes múltiplas)
- Sistema de estado mantido entre navegações

## ⚡ Considerações de Performance

### Otimizações Implementadas

**Move Semantics**:
```cpp
void ActivityLog::add(Activity act) {  // Passagem por valor + move
    activities_.push_back(std::move(act));
}
```

**Lookup Eficiente**:
```cpp
std::map<Id, std::shared_ptr<T>> data_;  // O(log n) para operações
```

**Atualização Seletiva**:
```cpp
void refreshSpecificColumns(const std::string& fromId, const std::string& toId);
// Atualiza apenas colunas envolvidas em movimentação
```

### Trade-offs Aceitáveis

**shared_ptr Overhead**: Aceito em troca de segurança e flexibilidade
**Cópias de Vetores**: Preferido sobre referências em alguns casos para segurança
**Dynamic Dispatch**: Necessário para polimorfismo real de interfaces

## 🔍 Testabilidade

### Design para Testes

**Injeção de Dependências**:
```cpp
class KanbanService {
    // Repositórios podem ser mockados para testes
    MemoryRepository<Board> boardRepository_;
    MemoryRepository<Column> columnRepository_;
};
```

**Interfaces para Mocking**:
```cpp
// Testes podem implementar IRepository com dados controlados
class MockRepository : public IRepository<Board> {
    // Implementação controlada para testes
};
```

**Separação Clara**:
- Domínio testável sem dependências externas
- Lógica de apresentação testável separadamente
- Serviços testáveis com repositórios mockados

## 🚀 Estratégia de Extensibilidade

### Pontos de Extensão

**Novos Repositórios**:
```cpp
class DatabaseRepository : public IRepository<T> {
    // Nova estratégia de persistência sem alterar outras camadas
};
```

**Novos Filtros**:
```cpp
class DateFilter : public IFilter {
    // Novo critério de filtragem
    bool matches(const Card& card) const override;
};
```

**Novas Views**:
```cpp
class WebView : public IView {
    // Nova interface (web) implementando contrato existente
};
```

### Princípios para Evolução

**Open/Closed**: Novas funcionalidades via extensão, não modificação
**Interface Segregation**: Novas interfaces para novas responsabilidades  
**Dependency Injection**: Novas implementações injetáveis sem alterar clientes

## 📊 Métricas de Qualidade de Design

### Acoplamento
- **Baixo acoplamento** entre camadas via interfaces
- **Acoplamento temporal** minimizado com operações atômicas
- **Acoplamento de dados** controlado com DTOs implícitos

### Coesão
- **Alta coesão** dentro de cada camada
- **Coesão funcional** em serviços especializados
- **Coesão sequencial** em fluxos de trabalho

### Complexidade
- **Complexidade ciclomática** reduzida com early returns
- **Complexidade de integração** minimizada com APIs claras
- **Complexidade cognitiva** gerenciada com responsabilidades bem definidas

## 🔮 Roadmap de Evolução Arquitetural

### Melhorias Planejadas

**Persistência Avançada**:
```cpp
class DatabaseRepository : public IRepository<T> {
    // Suporte a bancos de dados relacionais
};

class CloudRepository : public IRepository<T> {
    // Sincronização com serviços em nuvem
};
```

**Sistema de Plugins**:
```cpp
class IPlugin {
    virtual void initialize(KanbanService& service) = 0;
    virtual std::string getName() const = 0;
};
```

**APIs de Integração**:
```cpp
class RESTAPI {
    // Exposição do sistema como serviço web
    // Suporte a clientes externos
};
```

### Considerações para Escala

**Cache de Dados**: Implementação de cache L1/L2 para performance
**Operações Assíncronas**: Suporte a operações não-bloqueantes
**Sharding**: Estratégias para múltiplos boards de grande escala

---

Este design representa uma arquitetura madura e bem fundamentada que equilibra princípios acadêmicos com necessidades práticas de desenvolvimento, resultando em um sistema robusto, maintainable e preparado para evolução futura.