# Kanban Lite — Design (Etapa 1)

> **Propósito deste README:** documento técnico de design para a **Etapa 1** do projeto Kanban Lite.
> Contém: visão geral, estrutura de diretórios, descrição das principais classes e interfaces, decisões arquiteturais (justificadas), diagrama UML (Mermaid), instruções mínimas de build/validação e checklist pronto para submissão.
>
> **Observação de estilo:** identificadores/nomes em *English*, comentários e explicações em *Português*.

---

# 1. Visão geral do projeto

**Kanban Lite** é uma aplicação didática escrita em **C++ (C++17)** que modela um quadro Kanban simplificado com foco em demonstrar conceitos de Programação Orientada a Objetos (POO) e boas práticas modernas da linguagem:

* abstração e encapsulamento;
* classes e responsabilidades coerentes (domain-driven design leve);
* polimorfismo (interfaces/contratos);
* composição preferida sobre herança quando aplicável;
* smart pointers (`std::shared_ptr`, `std::unique_ptr`), RAII;
* uso de STL (`std::vector`, `std::optional`, `std::string`) e templates (`IRepository<T>`);
* tratamento de exceções específico para persistência.

A Etapa 1 entrega a **arquitetura** (headers), UML, `CMakeLists.txt` inicial e relatório curto justificando decisões de design. As implementações `.cpp` e a CLI/GUI ficarão para as etapas seguintes.

---

# 2. Estrutura proposta (diretórios e arquivos principais)

```
design/
├─ include/
│  ├─ domain/
│  │  ├─ Board.h
│  │  ├─ Column.h
│  │  ├─ Card.h
│  │  ├─ Tag.h
│  │  ├─ User.h
│  │  ├─ Activity.h
│  │  └─ ActivityLog.h
│  ├─ interfaces/
│  │  ├─ IRepository.h
│  │  ├─ IService.h
│  │  ├─ IFilter.h
│  │  └─ IView.h
│  └─ persistence/
│     └─ FileRepository.h
├─ tests/
│  └─ compile_test.cpp
├─ uml/
│  └─ diagram.md # Mermaid source (ou classes.svg/png)
│  └─ uml.png       
│  └─ uml.svg       
└─ CMakeLists.txt
```

---

# 3. Principais entidades (domain) — responsabilidades detalhadas

> Todos os cabeçalhos devem usar `#pragma once`. Nomes em *English*, comentários/explicações em *Português*. Abaixo cada classe com responsabilidade, invariantes, API pública relevante, semântica de ownership, notas de implementação, mapeamentos POO e sugestões de testes.

---

## `Board`

**Resumo:** representa um quadro Kanban completo — agrega colunas, faz operações de alto nível sobre cards e registra atividades.

**Responsabilidades primárias**

* Gerenciar a coleção de `Column`s (criar, remover, enumerar).
* Orquestrar operações que envolvem múltiplas colunas, p.ex. `moveCard(...)`.
* Registrar ações relevantes em `ActivityLog` (ex.: mover card, renomear coluna).
* Expor consultas de alto nível (buscar coluna por id, contar colunas, etc).

**Invariantes / validações**

* `id` é único e imutável após criação.
* `columns` não deve conter colunas com `id` duplicado.
* `activityLog` pode ser `nullptr` (opcional) ou configurado; se existir, deve receber eventos significativos.

**Ownership / lifetime**

* Mantém `std::vector<std::shared_ptr<Column>>`; a `Board` é *dona* funcional das colunas mas usa `shared_ptr` para permitir referências externas (views, histórico).
* `ActivityLog` mantido por `shared_ptr` (opcional).

**Complexidade / performance**

* Busca linear por coluna/card (aceitável para cargas acadêmicas). Para grande escala, use `unordered_map<Id, index>`.

**Tratamento de erros**

* `moveCard` deve lançar `std::runtime_error` (ou derivada customizada) se IDs inválidos, card não encontrado ou operação inválida. Camadas superiores (IService/CLI) capturam e exibem mensagens amigáveis.

**Mapeamento POO**

* Composição (`Board` *contém* `Column`s).
* Encapsulamento: validações e invariantes mantidos em métodos públicos.

**Casos de uso / sequência típica**

* `createBoard("B1") -> addColumn("ToDo") -> addColumn("Done") -> addCard("ToDo", "C1") -> moveCard("C1", "ToDo", "Done")`.
* Ao mover, `Board` chama `fromColumn->removeCardById`, `toColumn->addCard`, e registra `Activity` no `ActivityLog`.

**Testes recomendados**

* mover card entre colunas (happy path);
* mover card inexistente (espera exceção);
* adicionar coluna com id duplicado (rejeitar).

---

## `Column`

**Resumo:** lista de `Card`s dentro do `Board`. Encapsula operações locais sobre cards.

**Responsabilidades primárias**

* Inserir/Remover/Buscar `Card`s.
* Manter ordem dos cards (inserção ao final por padrão, possibilidade de `insertCardAt`).
* Fornecer operações de consulta (size, empty, hasCard).

**Invariantes / validações**

* Cada `Card` na coluna tem `id` único (dentro da coluna).
* Ordem dos cards deve ser preservada para comportamento de Kanban.

**Ownership / lifetime**

* Guarda `shared_ptr<Card>`: coluna tem posse lógica, mas outros componentes podem manter referências (ex.: histórico/visualização).

**Complexidade**

* `find`/`remove` linear (`O(n)`); ok para casos de uso acadêmicos.

**Tratamento de erros**

* `removeCardById` retorna `std::nullopt` se não existir (evita exceções para fluxo normal). Métodos que não devem falhar são `noexcept`.

**Mapeamento POO**

* Composição: `Column` contém `Card`s.
* Encapsulamento: a coluna controla como e onde os cards são armazenados.

**Testes recomendados**

* inserir/remover card e checar ordem;
* `insertCardAt` com índice fora do intervalo (inserir no final);
* `hasCard` true/false.

---

## `Card`

**Resumo:** unidade de trabalho; contém dados e pequenas operações utilitárias (tags, timestamps, prioridade).

**Responsabilidades primárias**

* Armazenar título, descrição opcional, prioridade, `createdAt/updatedAt`.
* Gerenciar `Tag`s (adicionar, remover, checar).
* Fornecer comparadores (`operator<`, `operator==`) usados para ordenação/igualdade.

**Invariantes / validações**

* `id` imutável; `title` não vazio (validar em `setTitle`/construtor).
* `priority` dentro de faixa aceitável (documentar limites).

**Ownership / lifetime**

* `Card` mantém `shared_ptr<Tag>` para compartilhar etiquetas entre cards.

**Complexidade**

* Operações em tags geralmente `O(n_tags)`.

**Tratamento de erros**

* `setTitle` pode lançar `std::invalid_argument` se título inválido (ou apenas documentar e rejeitar).
* Mutadores atualizam `updatedAt_` via `touchUpdated()`.

**Mapeamento POO**

* Encapsulamento: validação via setters; campos privados.
* Sobreposição de operadores: `operator<` útil para sorting em colunas/filas.

**Testes recomendados**

* comparar cards por prioridade/tempo;
* adicionar/remover tags;
* validação de título vazio.

---

## `Tag`

**Resumo:** rótulo simples reutilizável; permite categorizar cards (cores/labels).

**Responsabilidades**

* Armazenar `id` e `name`.
* Fornecer igualdade / `operator<<` para logging.

**Ownership**

* Normalmente criado por um serviço/gerenciador e compartilhado via `shared_ptr`.

**Uso futuro**

* Expandir com cor/descrição/ordem visual.

**Testes**

* igualdade por `id`, alteração de nome.

---

## `User`

**Resumo:** entidade simples representando um participante local do sistema.

**Responsabilidades**

* Armazenar `id` e `name`.
* (Futuro) ser referenciado por `Card` (atribuição), histórico, filtros.

**Uso futuro**

* adicionar `email`, `avatar`, validação de nomes.
* permitir `assignToCard(cardId, userId)` via `IService`.

**Testes**

* criação, comparação, serialização.

---

## `Activity` / `ActivityLog`

**Resumo:** mecanismo de auditoria local para registrar ações importantes (mover card, criar coluna, editar card).

**`Activity` — responsabilidades**

* Representar evento pontual: `id`, `description`, `when`.
* Fornecer `operator<<` para logs e serialização.

**`ActivityLog` — responsabilidades**

* Armazenar lista ordenada de `Activity`.
* API: `add(Activity)`, `const vector<Activity>& activities()`, `size()`, `empty()`, `last()`, `clear()`.

**Invariantes**

* `Activity` tem `when` não nulo; `id` exclusivo por log (opcional).

**Uso prático**

* `Board::moveCard(...)` cria `Activity` com descrição `"moved card <cardId> from <from> to <to>"` e chama `activityLog->add(activity)`.

**Tratamento de concorrência**

* `ActivityLog` não é thread-safe; se múltiplas threads escreverem, proteger com mutex ou usar fila lock-free.

**Testes**

* adicionar várias activities e verificar ordem/último;
* serialização (JSON) round-trip.

---

## Observações transversais (persistência, serialização, testes)

* **Serialização / persistência:**

  * Planejar `to_json` / `from_json` (ex.: nlohmann::json) para `Card`, `Column`, `Board`, `Activity`.
  * Ao salvar `Board`, gravar `columns` e `cards` com referências por `id` (evitar ciclos).

* **Geração de IDs:**

  * No início usar `std::string` gerada por `uuid` simples ou `counter` em `KanbanService`. Trocar para UUID real se necessário.

* **Validação & exceções:**

  * Preferir `std::invalid_argument` para argumentos inválidos; `std::runtime_error`/custom exceptions para erros de persistência.

* **Thread-safety:**

  * Biblioteca não-thread-safe por padrão; camada de serviço pode encapsular sincronização quando necessário (auto-save em background, por exemplo).

* **Testes unitários sugeridos por classe:**

  * `Board`: criação, adicionar/remover colunas, mover card com ActivityLog, comportamento em erros.
  * `Column`: inserir/ordenar/remover cards, insertAt limites.
  * `Card`: setters/validations/tags/operadores.
  * `ActivityLog`: ordem de inserção, last/clear.
  * `FileRepository` (Etapa 2): salvar/carregar round-trip.

---

# 4. Interfaces (contratos) — por que e como

* **IRepository<T, Id = std::string>** (template)

  * Contrato genérico para persistência (add/remove/getAll/findById).
  * Retornos: `getAll()` → `vector<shared_ptr<T>>`; `findById()` → `optional<shared_ptr<T>>`.
  * Justificativa: desacopla domínio de implementação (in-memory / file / db).

* **IService**

  * Fachada de operações de alto nível usadas por CLI/GUI: criar board/column/card, mover card, listar entidades, createSampleData().
  * Justificativa: centraliza lógica de orquestração (coordenação de repositórios e regras de negócio).

* **IFilter**

  * Interface de filtros polimórficos para `Card` (método `matches(const domain::Card&) const`).
  * Inclui `clone()` (retorna `unique_ptr<IFilter>`) para suportar cópias polimórficas.

* **IView**

  * Contrato mínimo para a camada de apresentação (CLI/GUI): `showMessage`, `showError`, `displayBoards/Columns/Cards`.
  * Justificativa: permite implementar `CliView` e `QtView` sem acoplar serviços à UI concreta.

---

# 5. Persistência (pasta `persistence/`)

* **FileRepository<T, Id = std::string>** (skeleton)

  * Exceção: `FileRepositoryException : public std::runtime_error`.
  * Responsabilidade: salvar/carregar objetos em arquivo (JSON recomendado).
  * Implementação planejada na Etapa 2 (opções: JSON com `nlohmann::json`, ou formato simples).
  * Observação de design: `findById` retorna `optional<shared_ptr<T>>` para clareza de ausência.

---

# 6. Decisões arquiteturais — justificativas detalhadas

### 6.1 Composição vs Herança

* **Composição escolhida** para relações "possui" (Board → Columns, Column → Cards, Card → Tags, ActivityLog → Activity).

  * Motivo: modela propriedade e ciclo de vida; permite implementar facilmente operações de remoção e serialização; evita problemas de substituição Liskov quando não há real relação "é-um".
* **Herança usada apenas para contratos/abstração** (IService, IRepository, IFilter, IView).

  * Motivo: polimorfismo é necessário para trocar implementações (ex.: `FileRepository` vs `InMemoryRepository`) e para estender comportamentos (novos filtros).

### 6.2 Smart pointers & gerenciamento de recursos

* `std::shared_ptr` para objetos que podem ter múltiplas referências (ex.: um `Card` referenciado em histórico e em uma view).
* `std::unique_ptr` para propriedade exclusiva e para `clone()` de filtros (retorna `unique_ptr<IFilter>`).
* RAII para recursos não gerenciados (arquivos em `FileRepository`).

### 6.3 STL e templates

* `std::vector` para coleções ordenadas (boa performance para tamanhos pequenos/medios).
* `std::optional` para valores opcionais e resultados de busca.
* `IRepository<T, Id>` como template para reaproveitamento genérico.

### 6.4 Exceções e erro tratado

* `FileRepositoryException` para erros de I/O/parse.
* Métodos do serviço e domínio documentados para lançar `std::runtime_error` quando apropriado (ex.: mover card inexistente).
* CLI/GUI devem capturar e exibir mensagens amigáveis.

### 6.5 Thread-safety

* Design **não é thread-safe por padrão**. Se houver multi-thread (auto-save, sync), proteger containers com `std::mutex` na camada apropriada.

---

# 7. Diagrama UML (Mermaid) 

> O diagrama abaixo é a versão alinhada ao design atual (observe o uso de `optional` em retornos de busca).

```mermaid
classDiagram
    %% Domain
    class Activity {
        - string id_
        - string description_
        - TimePoint when_
        + Activity(string id, string description, TimePoint when)
        + id() string
        + description() string
        + when() TimePoint
    }

    class ActivityLog {
        - vector<Activity> activities_
        + add(Activity act) void
        + activities() vector<Activity>
        + size() size_t
        + empty() bool
        + last() Activity
        + clear() void
    }

    class Board {
        - string id_
        - string name_
        - vector<shared_ptr<Column>> columns_
        - shared_ptr<ActivityLog> activityLog_
        + Board(string id, string name)
        + id() string
        + name() string
        + setName(string name) void
        + addColumn(shared_ptr<Column>) void
        + removeColumnById(string columnId) optional<shared_ptr<Column>>
        + columns() vector<shared_ptr<Column>>
        + findColumn(string columnId) optional<shared_ptr<Column>>
        + columnCount() size_t
        + moveCard(string cardId, string fromColumnId, string toColumnId) void
        + setActivityLog(shared_ptr<ActivityLog>) void
        + activityLog() shared_ptr<ActivityLog>
        + hasColumn(string columnId) bool
        + clear() void
    }

    class Column {
        - string id_
        - string name_
        - vector<shared_ptr<Card>> cards_
        + Column(string id, string name)
        + id() string
        + name() string
        + setName(string name) void
        + addCard(shared_ptr<Card>) void
        + insertCardAt(size_t index, shared_ptr<Card>) void
        + removeCardById(string cardId) optional<shared_ptr<Card>>
        + cards() vector<shared_ptr<Card>>
        + findCard(string cardId) optional<shared_ptr<Card>>
        + size() size_t
        + empty() bool
        + hasCard(string cardId) bool
        + clear() void
    }

    class Card {
        - string id_
        - string title_
        - optional<string> description_
        - int priority_
        - TimePoint createdAt_
        - TimePoint updatedAt_
        - vector<shared_ptr<Tag>> tags_
        + Card(string id, string title)
        + id() string
        + title() string
        + setTitle(string title) void
        + description() optional<string>
        + setDescription(string desc) void
        + priority() int
        + setPriority(int p) void
        + createdAt() TimePoint
        + updatedAt() TimePoint
        + addTag(shared_ptr<Tag>) void
        + removeTagById(string tagId) bool
        + hasTag(string tagId) bool
        + clearTags() void
        + tags() vector<shared_ptr<Tag>>
        + touchUpdated() void
    }

    class Tag {
        - string id_
        - string name_
        + Tag(string id, string name)
        + id() string
        + name() string
        + setName(string name) void
    }

    class User {
        - string id_
        - string name_
        + User(string id, string name)
        + id() string
        + name() string
        + setName(string name) void
    }

    %% Interfaces
    class IFilter {
        <<interface>>
        + matches(const Card& card) bool
        + clone() unique_ptr<IFilter>
    }

    class IRepository_T_Id {
        <<interface>>
        + add(shared_ptr<Object>) void
        + remove(Id id) void
        + getAll() vector<shared_ptr<Object>>
        + findById(Id id) optional<shared_ptr<Object>>
    }

    class IService {
        <<interface>>
        + createSampleData() void
        + createBoard(string name) string
        + addColumn(string boardId, string columnName) string
        + addCard(string boardId, string columnId, string title) string
        + moveCard(string boardId, string cardId, string fromColumnId, string toColumnId) void
        + listBoards() vector<shared_ptr<Board>>
        + findBoard(string boardId) optional<shared_ptr<Board>>
        + listColumns(string boardId) vector<shared_ptr<Column>>
        + listCards(string columnId) vector<shared_ptr<Card>>
    }

    class IView {
        <<interface>>
        + showMessage(string msg) void
        + showError(string msg) void
        + displayBoards(vector<shared_ptr<Board>>) void
        + displayColumns(vector<shared_ptr<Column>>) void
        + displayCards(vector<shared_ptr<Card>>) void
    }

    %% Persistence
    class FileRepositoryException {
        + FileRepositoryException(string what)
    }

    class FileRepository_T_Id {
        - string path_
        + FileRepository(string path)
        + ~FileRepository()
        + add(shared_ptr<Object>) void
        + remove(Id id) void
        + getAll() vector<shared_ptr<Object>>
        + findById(Id id) optional<shared_ptr<Object>>
    }

    %% Relations
    ActivityLog "1" *-- "0..*" Activity : contains
    Board "1" *-- "0..*" Column : contains
    Column "1" *-- "0..*" Card : contains
    Card "1" *-- "0..*" Tag : tags
    Board "1" *-- "0..1" ActivityLog : owns
    IFilter ..> Card : depends on

    IRepository_T_Id <|.. FileRepository_T_Id : implemented by
    FileRepository_T_Id ..> FileRepositoryException : throws
    FileRepositoryException --|> std::runtime_error
    IService ..> Board : uses
    IService ..> Column : uses
    IService ..> Card : uses
    IView ..> Board : displays
    IView ..> Column : displays
    IView ..> Card : displays
    IService ..> IView : updates
```

> Dica: edite/exporte esse Mermaid em VSCode (extensão Mermaid) ou em [https://mermaid.live](https://mermaid.live) para gerar SVG/PNG.

---

# 8. Instruções de build & validação (Etapa 1)

**Pré-requisitos**

* `cmake` >= 3.16
* compilador com suporte C++17 (g++, clang++, MSVC)

**Passos**

```bash
# na pasta design/
mkdir -p build
cd build
cmake ..
cmake --build .
./compile_test    # programa mínimo que inclui os headers
```

* `compile_test` apenas inclui os headers e imprime uma mensagem de verificação. O objetivo é garantir que `cmake` configure e que os headers compilem sem implementações `.cpp` (pré-check requerido pela Etapa 1).

---

# 9. Checklist de submissão 

* [x] `design/include/domain/*.h` (Board, Column, Card, Tag, User, Activity, ActivityLog)
* [x] `design/include/interfaces/*.h` (IRepository, IService, IFilter, IView)
* [x] `design/include/persistence/FileRepository.h` + `FileRepositoryException`
* [x] `design/uml/classes.mmd` e `design/uml/classes.svg` (exportado)
* [x] `design/CMakeLists.txt` funcional (gera `compile_test`)
* [x] `design/report/etapa1_report.md` (1–2 páginas justificando decisões)
* [x] `design/README.md` (este arquivo, completo e detalhado)

---

# 10. Mapeamento rápido: requisitos POO → onde é atendido (arquivos / exemplos)

1. **Abstração & Encapsulamento**

   * `include/interfaces/*.h` (IRepository, IService, IView) e campos privados com getters/setters em `include/domain/*.h`.

2. **Classes e Objetos**

   * `include/domain/Board.h`, `Column.h`, `Card.h`, `Tag.h`, `User.h`, `ActivityLog.h`.

3. **Herança & Polimorfismo**

   * `IFilter` (e futuras `LabelFilter`, `UserFilter`), `IRepository<T>` e `FileRepository<T>`.

4. **Composição vs Herança**

   * Composição: Board→Columns, Column→Cards, Card→Tags. Herança apenas para interfaces/contratos.

5. **Polimorfismo dinâmico**

   * `std::unique_ptr<IFilter> clone()` e `std::shared_ptr<IRepository<T>>` usadas para ligação tardia.

6. **Gerenciamento de recursos**

   * `FileRepository` com RAII (fstream no .cpp) e smart pointers para ownership.

7. **Templates e STL**

   * `IRepository<T,Id>`, `std::vector`, `std::optional`.

8. **Sobrecarga de operadores**

   * `Card::operator<` e `Card::operator==` (para ordenação e comparação).

9. **Tratamento de exceções**

   * `FileRepositoryException` (persistência) e uso documentado de `std::runtime_error` para erros críticos.

10. **Documentação técnica e UML**

    * Este README + `report/etapa1_report.md` + `uml/classes.mmd`.

11. **Build automatizado**

    * `CMakeLists.txt` inicial; test `compile_test` para pré-check.

---

# 11. Roadmap — próximos passos (Etapa 2 / Etapa 3)

**Etapa 2 — CLI (até a data limite)**

* Implementar `.cpp` para `domain` (construtores, métodos principais).
* Implementar `InMemoryRepository<T>` (simples, rápido) e depois `FileRepository<T>` (JSON).
* Implementar `KanbanService : public interfaces::IService` que usa repositórios.
* Criar `cli_app` com menu (criar board, add column, add card, move card, list) e `CliView`.
* Documentar testes de uso e criar README com instruções para rodar CLI.

**Etapa 3 — GUI**

* Escolher Qt (recomendado) e implementar `QtView` usando API do `IService`.
* Testes manuais + vídeo (≤ 3 min) demonstrando funcionalidades principais.
* Tag no GitHub, instruções cross-platform (Linux/Windows).

---

# 12. Boas práticas e convenções adotadas

* Identificadores em *English*; comentários em *Português*.
* `#pragma once` em headers.
* Nomes de namespaces: `kanban::domain`, `kanban::interfaces`, `kanban::persistence`.
* `using Id = std::string;` nas classes centrais quando conveniente.
* Evitar implementação em headers (salvo templates).
* Testes simples em `tests/` e CI opcional para compilar/rodar testes.

---

# 13. Licença e contribuição

* Sugestão de licença: **MIT** (arquivo `LICENSE`).
* Convenção de commits: `feat:`, `fix:`, `docs:`, `chore:`.
* Pull requests com descrição clara do que muda (arquitetura / implementação / tests).

---

# 14. FAQ rápido

**Q:** Posso rodar `cmake` agora?
**A:** Sim — o `compile_test` verifica inclusão de headers; implementações ficam para a Etapa 2.

**Q:** O design obriga a usar `shared_ptr` sempre?
**A:** Não — usamos `shared_ptr` onde faz sentido. Em locais de propriedade exclusiva `unique_ptr` é preferível. Se trocar ownership, atualize cabeçalhos/diagrama.

**Q:** Como exportar Mermaid para SVG?
**A:** Use VSCode (ext. Mermaid) ou [https://mermaid.live](https://mermaid.live) para colar o bloco e exportar SVG/PNG.

---