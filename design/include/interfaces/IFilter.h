/**
 * @file IFilter.h
 * @brief Declaraçao da interface IFilter para filtros polimórficos aplicáveis a Cards.
 * @details Define uma interface genérica que permite aplicar diferentes critérios de
 *          filtragem sobre instâncias de Card. O design favorece baixo acoplamento
 *          (recebendo referência constante ao Card em vez de ponteiros inteligentes),
 *          o que facilita reuso, testabilidade e extensibilidade de filtros.
 */

#pragma once

#include <memory>

namespace kanban {
namespace domain { class Card; } ///< @brief Declaraçao antecipada da classe Card para evitar dependência circular

namespace interfaces {

/**
 * @class IFilter
 * @brief Interface base para filtros aplicáveis a Cards.
 * @details Permite implementar diferentes critérios de seleçao de Cards em sistemas
 *          Kanban. A interface é abstrata e deve ser implementada por classes concretas
 *          que definem a lógica de filtragem (ex.: filtro por prioridade, por responsável,
 *          por status, etc.).
 *
 *          - Recebe `const Card&` em vez de `shared_ptr<Card>` para evitar dependência
 *            desnecessária de ownership, simplificando testes e promovendo flexibilidade.
 *          - Fornece `clone()` para permitir cópia polimórfica, útil em containers heterogêneos.
 */
class IFilter {
public:
    /**
     * @brief Destrutor virtual padrao.
     * @details Necessário para garantir destruiçao correta em hierarquias polimórficas.
     */
    virtual ~IFilter() = default;

    /**
     * @brief Verifica se um Card atende ao critério definido pelo filtro.
     * @param card Referência constante ao Card a ser avaliado.
     * @return `true` se o Card atende ao critério, `false` caso contrário.
     * @note O método é `const` pois nao altera o estado interno do filtro.
     */
    virtual bool matches(const domain::Card& card) const = 0;

    /**
     * @brief Cria uma cópia polimórfica do filtro.
     * @return `std::unique_ptr<IFilter>` apontando para a nova instância clonada.
     * @details Essencial para armazenar filtros em containers (como `std::vector`)
     *          sem perder polimorfismo.
     */
    virtual std::unique_ptr<IFilter> clone() const = 0;
};

} // namespace interfaces
} // namespace kanban
