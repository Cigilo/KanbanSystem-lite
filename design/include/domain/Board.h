/**
 * @file Board.h
 * @brief Declaraçao da entidade Board do sistema Kanban
 * @details Este header define a classe Board, que representa um quadro Kanban
 *          contendo colunas e cards, com operações para gerenciar o fluxo
 *          de trabalho e registrar atividades através do ActivityLog.
 */

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

// ============================================================================
// CLASSE Board
// ============================================================================

/**
 * @brief Representa um quadro Kanban no sistema
 * @details Um Board é o container principal do sistema Kanban, contendo
 *          múltiplas colunas que organizam cards (tarefas) em diferentes
 *          estágios do fluxo de trabalho. Oferece operações para gerenciar
 *          colunas, mover cards entre colunas e registrar atividades.
 * 
 *          A classe utiliza shared_ptr para flexibilidade no gerenciamento
 *          de memória e permite composiçao complexa entre entidades.
 */
class Board {
public:
    /**
     * @brief Tipo auxiliar para IDs do board
     * @details Facilita futuras alterações no tipo de ID (ex.: para UUID)
     *          mantendo a interface consistente.
     */
    using Id = std::string;

    // ============================================================================
    // CONSTRUTOR E REGRA DOS CINCO
    // ============================================================================

    /**
     * @brief Construtor explícito do Board
     * @param id Identificador único do board
     * @param name Nome descritivo do board
     * @details Recebe parâmetros por referência constante para evitar
     *          cópias desnecessárias de strings.
     */
    explicit Board(const Id& id, const std::string& name) noexcept;

    /**
     * @brief Construtor de cópia padrao
     * @details Utiliza semântica de shared_ptr para cópia segura das colunas.
     */
    Board(const Board&) = default;

    /**
     * @brief Construtor de movimentaçao padrao
     * @details Realiza movimentaçao eficiente dos recursos.
     */
    Board(Board&&) noexcept = default;

    /**
     * @brief Operador de atribuiçao por cópia padrao
     */
    Board& operator=(const Board&) = default;

    /**
     * @brief Operador de atribuiçao por movimentaçao padrao
     */
    Board& operator=(Board&&) noexcept = default;

    /**
     * @brief Destrutor padrao
     */
    ~Board() = default;

    // ============================================================================
    // ACESSORES BÁSICOS
    // ============================================================================

    /**
     * @brief Retorna o ID único do board
     * @return Referência constante para o ID do board
     */
    const Id& id() const noexcept;

    /**
     * @brief Retorna o nome do board
     * @return Referência constante para o nome do board
     */
    const std::string& name() const noexcept;

    /**
     * @brief Define um novo nome para o board
     * @param name Novo nome a ser atribuído
     */
    void setName(const std::string& name);

    // ============================================================================
    // GERENCIAMENTO DE COLUNAS
    // ============================================================================

    /**
     * @brief Adiciona uma coluna ao board
     * @param column Shared pointer para a coluna a ser adicionada
     * @details Recebe por referência constante para evitar cópia desnecessária
     *          do shared_ptr. Verifica internamente por duplicatas.
     */
    void addColumn(const std::shared_ptr<Column>& column);

    /**
     * @brief Remove uma coluna do board pelo ID
     * @param columnId ID da coluna a ser removida
     * @return Optional contendo shared_ptr para a coluna removida se encontrada,
     *         ou std::nullopt se a coluna nao existir
     * @details A coluna é removida do vetor interno mas mantida viva através
     *          do shared_ptr retornado, permitindo que o caller decida seu destino.
     */
    std::optional<std::shared_ptr<Column>> removeColumnById(const Id& columnId);

    /**
     * @brief Retorna todas as colunas do board
     * @return Referência constante para o vetor de colunas
     * @details Expõe o container interno apenas para leitura, evitando
     *          modificações nao controladas no estado do board.
     */
    const std::vector<std::shared_ptr<Column>>& columns() const noexcept;

    /**
     * @brief Busca uma coluna específica pelo ID
     * @param columnId ID da coluna a ser encontrada
     * @return Optional contendo shared_ptr para a coluna se encontrada,
     *         ou std::nullopt se nao existir
     * @details Realiza busca linear - adequado para número moderado de colunas.
     */
    std::optional<std::shared_ptr<Column>> findColumn(const Id& columnId) const noexcept;

    /**
     * @brief Retorna o número de colunas no board
     * @return Quantidade de colunas presentes no board
     */
    std::size_t columnCount() const noexcept;

    // ============================================================================
    // OPERAÇÕES DE MOVIMENTAÇaO DE CARDS
    // ============================================================================

    /**
     * @brief Move um card entre duas colunas do board
     * @param cardId ID do card a ser movido
     * @param fromColumnId ID da coluna de origem
     * @param toColumnId ID da coluna de destino
     * @throws std::runtime_error ou derivada em casos de erro:
     *         - Coluna de origem nao encontrada
     *         - Coluna de destino nao encontrada  
     *         - Card nao encontrado na coluna de origem
     * @details Operaçao atômica que remove o card da coluna de origem e
     *          o adiciona à coluna de destino. Registra a atividade no
     *          ActivityLog se configurado. O tratamento de exceções deve
     *          ser feito na camada superior (CLI/GUI).
     */
    void moveCard(const std::string& cardId,
                  const Id& fromColumnId,
                  const Id& toColumnId);

    // ============================================================================
    // GERENCIAMENTO DO ACTIVITY LOG
    // ============================================================================

    /**
     * @brief Configura o ActivityLog para este board
     * @param log Shared pointer para o ActivityLog a ser associado
     * @details Utiliza shared_ptr para permitir referência externa e
     *          compartilhamento do mesmo log entre diferentes componentes.
     */
    void setActivityLog(const std::shared_ptr<ActivityLog>& log) noexcept;

    /**
     * @brief Retorna o ActivityLog associado ao board
     * @return Shared pointer para o ActivityLog (pode ser nullptr)
     */
    std::shared_ptr<ActivityLog> activityLog() const noexcept;

    // ============================================================================
    // MÉTODOS UTILITÁRIOS
    // ============================================================================

    /**
     * @brief Verifica se uma coluna existe no board
     * @param columnId ID da coluna a ser verificada
     * @return true se a coluna existe, false caso contrário
     */
    bool hasColumn(const Id& columnId) const noexcept;

    /**
     * @brief Limpa completamente o board
     * @details Remove todas as colunas e desassocia o ActivityLog.
     *          Operaçao destrutiva - use com cuidado.
     */
    void clear() noexcept;

    /**
     * @brief Define uma nova ordem para as colunas
     * @param columns Novo vetor de colunas na ordem desejada
     */
    void setColumns(const std::vector<std::shared_ptr<Column>>& columns);

private:
    Id id_;                                  ///< @brief Identificador único do board
    std::string name_;                       ///< @brief Nome descritivo do board
    std::vector<std::shared_ptr<Column>> columns_;      ///< @brief Coleçao de colunas do board (composiçao)
    std::shared_ptr<ActivityLog> activityLog_;          ///< @brief Log de atividades (opcional - pode ser nullptr)

    // ============================================================================
    // NOTA SOBRE CONCORRÊNCIA
    // ============================================================================
    // Esta classe **nao** é thread-safe por padrao. Se a aplicaçao tiver acesso
    // concorrente (ex.: GUI thread + background thread), proteja 'columns_' e 
    // 'activityLog_' com mutex apropriado.
};

} // namespace domain
} // namespace kanban