#pragma once

#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace kanban {
namespace domain {
    class Board;
    class Column;
    class Card;
}

namespace interfaces {

// Interface de serviço de alto nível.
// É a fachada que o CLI/GUI utilizarão para interagir com o sistema Kanban.
class IService {
public:
    virtual ~IService() = default;

    // --- Utilitários ---
    // Cria dados de exemplo para testes/demonstração.
    virtual void createSampleData() = 0;

    // --- Operações principais ---
    // Cria um novo board e retorna seu id.
    virtual std::string createBoard(const std::string& name) = 0;

    // Cria uma nova coluna em um board existente.
    virtual std::string addColumn(const std::string& boardId,
                                  const std::string& columnName) = 0;

    // Cria um novo card em uma coluna.
    virtual std::string addCard(const std::string& boardId,
                                const std::string& columnId,
                                const std::string& title) = 0;

    // Move um card entre colunas.
    virtual void moveCard(const std::string& boardId,
                          const std::string& cardId,
                          const std::string& fromColumnId,
                          const std::string& toColumnId) = 0;

    // --- Consultas ---
    virtual std::vector<std::shared_ptr<domain::Board>> listBoards() const = 0;
    virtual std::optional<std::shared_ptr<domain::Board>> findBoard(const std::string& boardId) const = 0;

    virtual std::vector<std::shared_ptr<domain::Column>> listColumns(const std::string& boardId) const = 0;
    virtual std::vector<std::shared_ptr<domain::Card>> listCards(const std::string& columnId) const = 0;
};

} // namespace interfaces
} // namespace kanban
