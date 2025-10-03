/**
 * @file Column.h
 * @brief Declaraçao da entidade Column do sistema Kanban
 * @details Este header define a classe Column, que representa uma coluna
 *          dentro de um quadro Kanban, contendo cards e permitindo operações
 *          de inserçao, remoçao e gerenciamento de cards em posições específicas.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace kanban {
namespace domain {

// Forward declaration
class Card;

// ============================================================================
// CLASSE Column
// ============================================================================

/**
 * @brief Representa uma coluna (List) dentro de um Board Kanban
 * @details Uma Column é um container de cards que representa um estágio
 *          específico do fluxo de trabalho (ex.: "To Do", "Doing", "Done").
 *          Mantém a ordem dos cards e permite operações de inserçao em
 *          posições específicas para suportar ordenaçao manual.
 */
class Column {
public:
    /**
     * @brief Tipo auxiliar para IDs da coluna
     * @details Facilita futuras alterações no tipo de ID mantendo
     *          a interface consistente.
     */
    using Id = std::string;

    // ============================================================================
    // CONSTRUTOR E REGRA DOS CINCO
    // ============================================================================

    /**
     * @brief Construtor explícito da Column
     * @param id Identificador único da coluna
     * @param name Nome descritivo da coluna
     * @details Construtor explícito previne conversões implícitas indesejadas.
     *          Recebe parâmetros por referência para evitar cópias desnecessárias.
     */
    explicit Column(const Id& id, const std::string& name) noexcept;

    /**
     * @brief Construtor de cópia padrao
     * @details Utiliza semântica de shared_ptr para cópia segura dos cards.
     */
    Column(const Column&) = default;

    /**
     * @brief Construtor de movimentaçao padrao
     * @details Realiza movimentaçao eficiente dos recursos.
     */
    Column(Column&&) noexcept = default;

    /**
     * @brief Operador de atribuiçao por cópia padrao
     */
    Column& operator=(const Column&) = default;

    /**
     * @brief Operador de atribuiçao por movimentaçao padrao
     */
    Column& operator=(Column&&) noexcept = default;

    /**
     * @brief Destrutor padrao
     */
    ~Column() = default;

    // ============================================================================
    // ACESSORES E MODIFICADORES
    // ============================================================================

    /**
     * @brief Retorna o ID único da coluna
     * @return Referência constante para o ID da coluna
     */
    const Id& id() const noexcept;

    /**
     * @brief Retorna o nome da coluna
     * @return Referência constante para o nome da coluna
     */
    const std::string& name() const noexcept;

    /**
     * @brief Define um novo nome para a coluna
     * @param name Novo nome a ser atribuído
     */
    void setName(const std::string& name);

    // ============================================================================
    // GERENCIAMENTO DE CARDS
    // ============================================================================

    /**
     * @brief Adiciona um card ao final da coluna
     * @param card Shared pointer para o card a ser adicionado
     * @details Recebe por referência constante para evitar cópia do controle
     *          de referência do shared_ptr. Verifica duplicatas internamente
     *          e ignora a operaçao se o card já existir na coluna.
     */
    void addCard(const std::shared_ptr<Card>& card);

    /**
     * @brief Insere um card em uma posiçao específica da coluna
     * @param index Posiçao onde o card será inserido (base 0)
     * @param card Shared pointer para o card a ser inserido
     * @details Se o índice for maior ou igual ao tamanho atual, o card é
     *          inserido no final da coluna. Permite múltiplas inserções
     *          do mesmo card em posições diferentes.
     */
    void insertCardAt(std::size_t index, const std::shared_ptr<Card>& card);

    /**
     * @brief Remove um card da coluna pelo ID
     * @param cardId ID do card a ser removido
     * @return Optional contendo shared_ptr para o card removido se encontrado,
     *         ou std::nullopt se o card nao existir na coluna
     * @details O card é removido do vetor interno mas mantido vivo através
     *          do shared_ptr retornado, permitindo que o caller decida seu destino.
     */
    std::optional<std::shared_ptr<Card>> removeCardById(const Id& cardId);

    /**
     * @brief Retorna todos os cards da coluna
     * @return Referência constante para o vetor de cards
     * @details Expõe o container interno apenas para leitura, evitando
     *          modificações nao controladas no estado da coluna.
     *          Os cards sao retornados na ordem atual da coluna.
     */
    const std::vector<std::shared_ptr<Card>>& cards() const noexcept;

    /**
     * @brief Busca um card específico na coluna pelo ID
     * @param cardId ID do card a ser encontrado
     * @return Optional contendo shared_ptr para o card se encontrado,
     *         ou std::nullopt se nao existir na coluna
     * @details Realiza busca linear - adequado para número moderado de cards.
     */
    std::optional<std::shared_ptr<Card>> findCard(const Id& cardId) const noexcept;

    // ============================================================================
    // MÉTODOS UTILITÁRIOS
    // ============================================================================

    /**
     * @brief Retorna o número de cards na coluna
     * @return Quantidade de cards presentes na coluna
     * @details Útil para estatísticas e limitações de WIP (Work In Progress).
     */
    std::size_t size() const noexcept;

    /**
     * @brief Verifica se a coluna está vazia
     * @return true se nao há cards na coluna, false caso contrário
     * @details Método de conveniência para verificar existência de cards.
     */
    bool empty() const noexcept;

    /**
     * @brief Verifica se um card existe na coluna
     * @param cardId ID do card a ser verificado
     * @return true se o card existe na coluna, false caso contrário
     * @details Mais eficiente que findCard() quando apenas a existência importa.
     */
    bool hasCard(const Id& cardId) const noexcept;

    /**
     * @brief Limpa completamente a coluna
     * @details Remove todos os cards da coluna.
     *          Operaçao destrutiva - use com cuidado.
     */
    void clear() noexcept;

private:
    Id id_;                                  ///< @brief Identificador único da coluna
    std::string name_;                       ///< @brief Nome descritivo da coluna
    std::vector<std::shared_ptr<Card>> cards_; ///< @brief Coleçao de cards na coluna (preserva ordem)

    // ============================================================================
    // NOTA SOBRE CONCORRÊNCIA
    // ============================================================================
    // Esta classe NaO é thread-safe por padrao. Se a aplicaçao tiver acesso
    // concorrente à coluna (ex.: GUI thread + background thread), proteja as
    // mutações com std::mutex na camada apropriada.
};

} // namespace domain
} // namespace kanban