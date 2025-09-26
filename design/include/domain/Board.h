#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace kanban {
namespace domain {

// Forward declarations
class Column;
class ActivityLog;

// Representa o quadro Kanban (Board)
class Board {
public:
    // Tipo auxiliar para ids — facilita alteração futura (ex.: UUID type)
    using Id = std::string;

    // Construtor explícito, passa por referência para evitar cópia.
    explicit Board(const Id& id, const std::string& name) noexcept;

    // Defaulted special members (copiable/movable usando shared_ptr semantics).
    Board(const Board&) = default;
    Board(Board&&) noexcept = default;
    Board& operator=(const Board&) = default;
    Board& operator=(Board&&) noexcept = default;
    ~Board() = default;

    // Accessors
    const Id& id() const noexcept;
    const std::string& name() const noexcept;
    void setName(const std::string& name);

    // Columns management
    // Recebe por const-ref para evitar cópia desnecessária do shared_ptr.
    void addColumn(const std::shared_ptr<Column>& column);

    // Remove a coluna e retorna a instância removida.
    // Retorna std::nullopt se a coluna não existir.
    std::optional<std::shared_ptr<Column>> removeColumnById(const Id& columnId);

    // Retorna referência const ao container interno (somente leitura).
    const std::vector<std::shared_ptr<Column>>& columns() const noexcept;

    // Busca sem remover — retorna std::nullopt se não encontrada.
    std::optional<std::shared_ptr<Column>> findColumn(const Id& columnId) const noexcept;

    std::size_t columnCount() const noexcept;

    // Operação que move um card entre colunas (registra no ActivityLog).
    // Lança std::runtime_error (ou derivada) em casos de erro (ex.: coluna/cartId não encontrado,
    // card não existente). Documente e trate na camada superior (CLI/GUI).
    void moveCard(const std::string& cardId,
                  const Id& fromColumnId,
                  const Id& toColumnId);

    // ActivityLog (opcional). Usamos shared_ptr para permitir referência externa.
    void setActivityLog(const std::shared_ptr<ActivityLog>& log) noexcept;
    std::shared_ptr<ActivityLog> activityLog() const noexcept;

    // Utilitários
    bool hasColumn(const Id& columnId) const noexcept;
    void clear() noexcept;

private:
    Id id_;
    std::string name_;
    // Composição: Board possui as Column instances. Mantemos shared_ptr por flexibilidade.
    std::vector<std::shared_ptr<Column>> columns_;
    std::shared_ptr<ActivityLog> activityLog_; // pode ser nullopt se não configurado

    // Nota: esta classe **não** é thread-safe por padrão. Se a aplicação tiver acesso
    // concorrente (GUI thread + background), proteja 'columns_' e 'activityLog_' com mutex.
};

} // namespace domain
} // namespace kanban
