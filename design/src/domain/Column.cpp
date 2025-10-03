/**
 * @file Column.cpp
 * @brief Implementaçao da entidade Column do sistema Kanban
 * @details Esta classe representa uma coluna em um quadro Kanban,
 *          que contém cards e permite operações de inserçao, remoçao
 *          e gerenciamento dos cards dentro dela.
 */

#include "domain/Column.h"
#include "domain/Card.h"
#include <algorithm>
#include <stdexcept>

namespace kanban {
namespace domain {

// ============================================================================
// CONSTRUTOR E MÉTODOS BÁSICOS
// ============================================================================

/**
 * @brief Construtor da classe Column
 * @param id Identificador único da coluna
 * @param name Nome descritivo da coluna
 * @details Inicializa uma nova coluna com ID e nome fornecidos.
 *          O vetor de cards é inicializado vazio.
 */
Column::Column(const Id& id, const std::string& name) noexcept
    : id_(id), name_(name) {
    // cards_ é inicializado automaticamente como vector vazio
}

/**
 * @brief Retorna o ID único da coluna
 * @return Referência constante para o ID da coluna
 * @details O ID é imutável durante todo o ciclo de vida da coluna.
 */
const Column::Id& Column::id() const noexcept {
    return id_;
}

/**
 * @brief Retorna o nome da coluna
 * @return Referência constante para o nome da coluna
 * @details O nome pode ser alterado através do método setName().
 */
const std::string& Column::name() const noexcept {
    return name_;
}

/**
 * @brief Define um novo nome para a coluna
 * @param name Novo nome a ser atribuído à coluna
 * @details Permite a alteraçao do nome da coluna após sua criaçao.
 */
void Column::setName(const std::string& name) {
    name_ = name;
}

// ============================================================================
// GERENCIAMENTO DE CARDS
// ============================================================================

/**
 * @brief Adiciona um card à coluna
 * @param card Shared pointer para o card a ser adicionado
 * @details Verifica se o card já existe para evitar duplicatas.
 *          Se o card já existir na coluna, a operaçao é ignorada
 *          silenciosamente (comportamento idempotente).
 */
void Column::addCard(const std::shared_ptr<Card>& card) {
    // Verifica se o card já existe para evitar duplicatas
    if (!hasCard(card->id())) {
        cards_.push_back(card);
    }
    // Se já existir, simplesmente ignora (poderia lançar exceçao)
}

/**
 * @brief Insere um card em uma posiçao específica da coluna
 * @param index Posiçao onde o card será inserido (base 0)
 * @param card Shared pointer para o card a ser inserido
 * @details Se o índice for maior ou igual ao tamanho atual,
 *          o card é inserido no final da coluna. Nao verifica
 *          duplicatas, permitindo múltiplas inserções do mesmo card.
 */
void Column::insertCardAt(std::size_t index, const std::shared_ptr<Card>& card) {
    // Se o índice for maior que o tamanho, insere no final
    if (index >= cards_.size()) {
        cards_.push_back(card);
    } else {
        cards_.insert(cards_.begin() + index, card);
    }
}

/**
 * @brief Remove um card da coluna pelo ID
 * @param cardId ID do card a ser removido
 * @return Optional contendo shared_ptr para o card removido se encontrado,
 *         ou std::nullopt se o card nao existir na coluna
 * @details O card é removido do vetor interno e retornado para que
 *          o caller possa decidir o que fazer com ele.
 */
std::optional<std::shared_ptr<Card>> Column::removeCardById(const Id& cardId) {
    auto it = std::find_if(cards_.begin(), cards_.end(),
        [&cardId](const std::shared_ptr<Card>& card) {
            return card->id() == cardId;
        });
    
    if (it != cards_.end()) {
        auto card = *it;
        cards_.erase(it);
        return card;
    }
    return std::nullopt;
}

/**
 * @brief Retorna todos os cards da coluna
 * @return Referência constante para o vetor de cards
 * @details Os cards sao retornados na ordem em que foram adicionados
 *          ou na ordem resultante de operações de inserçao.
 */
const std::vector<std::shared_ptr<Card>>& Column::cards() const noexcept {
    return cards_;
}

/**
 * @brief Busca um card específico na coluna pelo ID
 * @param cardId ID do card a ser encontrado
 * @return Optional contendo shared_ptr para o card se encontrado,
 *         ou std::nullopt se nao existir na coluna
 * @details Utiliza busca linear no vetor de cards.
 */
std::optional<std::shared_ptr<Card>> Column::findCard(const Id& cardId) const noexcept {
    auto it = std::find_if(cards_.begin(), cards_.end(),
        [&cardId](const std::shared_ptr<Card>& card) {
            return card->id() == cardId;
        });
    
    if (it != cards_.end()) {
        return *it;
    }
    return std::nullopt;
}

// ============================================================================
// MÉTODOS DE CONSULTA E ESTADO
// ============================================================================

/**
 * @brief Retorna o número de cards na coluna
 * @return Quantidade de cards presentes na coluna
 * @details Útil para estatísticas e limitações de WIP (Work In Progress).
 */
std::size_t Column::size() const noexcept {
    return cards_.size();
}

/**
 * @brief Verifica se a coluna está vazia
 * @return true se nao há cards na coluna, false caso contrário
 * @details Método de conveniência para verificar a existência de cards
 *          sem precisar verificar o tamanho.
 */
bool Column::empty() const noexcept {
    return cards_.empty();
}

/**
 * @brief Verifica se um card existe na coluna
 * @param cardId ID do card a ser verificado
 * @return true se o card existe na coluna, false caso contrário
 * @details Mais eficiente que findCard() quando apenas a existência importa.
 */
bool Column::hasCard(const Id& cardId) const noexcept {
    return std::any_of(cards_.begin(), cards_.end(),
        [&cardId](const std::shared_ptr<Card>& card) {
            return card->id() == cardId;
        });
}

// ============================================================================
// OPERAÇÕES DE LIMPEZA
// ============================================================================

/**
 * @brief Limpa completamente a coluna
 * @details Remove todos os cards da coluna.
 *          Operaçao destrutiva - use com cuidado.
 */
void Column::clear() noexcept {
    cards_.clear();
}

} // namespace domain
} // namespace kanban