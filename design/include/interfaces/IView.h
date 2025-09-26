#pragma once

#include <memory>
#include <string>
#include <vector>

namespace kanban {
namespace domain {
    class Board;
    class Column;
    class Card;
}

namespace interfaces {

// Interface mínima que a camada de apresentação (CLI/GUI) deverá implementar
// para interagir com o domínio.
class IView {
public:
    virtual ~IView() = default;

    // Exibe uma mensagem genérica ao usuário (informação).
    virtual void showMessage(const std::string& msg) const = 0;

    // Exibe uma mensagem de erro ao usuário.
    virtual void showError(const std::string& msg) const = 0;

    // Atualiza a visualização de boards, colunas ou cards.
    virtual void displayBoards(const std::vector<std::shared_ptr<domain::Board>>& boards) const = 0;
    virtual void displayColumns(const std::vector<std::shared_ptr<domain::Column>>& columns) const = 0;
    virtual void displayCards(const std::vector<std::shared_ptr<domain::Card>>& cards) const = 0;
};

} // namespace interfaces
} // namespace kanban
