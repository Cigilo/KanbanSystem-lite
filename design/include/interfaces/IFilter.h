#pragma once

#include <memory>

namespace kanban {
namespace domain { class Card; } // forward declaration correta

namespace interfaces {

// Interface para filtros polimórficos aplicáveis a um Card.
// Observação: a interface aceita uma referência const ao domain::Card para
// desacoplar a API de ownership (shared_ptr). Se preferir, pode mudar para
// receber shared_ptr, mas evitar isso facilita reutilização e testes.
class IFilter {
public:
    virtual ~IFilter() = default;

    // Avalia se o card atende ao critério do filtro.
    // Deve ser const pois não altera o estado do próprio filtro.
    virtual bool matches(const domain::Card& card) const = 0;

    // Permite copiar filtros polimorficamente (útil se for armazená-los em containers).
    virtual std::unique_ptr<IFilter> clone() const = 0;
};

} // namespace interfaces
} // namespace kanban
