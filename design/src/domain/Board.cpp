/**
 * @file Board.cpp
 * @brief Implementaçao da entidade Board do sistema Kanban
 * @details Esta classe representa um quadro Kanban, que contém colunas
 *          e coordena a movimentaçao de cards entre elas, mantendo
 *          um histórico de atividades através do ActivityLog.
 */

#include "domain/Board.h"
#include "domain/Column.h"
#include "domain/ActivityLog.h"
#include "domain/Card.h"
#include <algorithm>
#include <stdexcept>

namespace kanban {
namespace domain {

// ============================================================================
// CONSTRUTOR E MÉTODOS BÁSICOS
// ============================================================================

/**
 * @brief Construtor da classe Board
 * @param id Identificador único do board
 * @param name Nome descritivo do board
 * @details Inicializa um novo board com ID e nome fornecidos.
 *          O vetor de columns é inicializado vazio e o activityLog
 *          como nullptr, podendo ser configurado posteriormente.
 */
Board::Board(const Id& id, const std::string& name) noexcept
    : id_(id), name_(name) {
    // columns_ é inicializado automaticamente como vector vazio
    // activityLog_ é inicializado automaticamente como nullptr
}

/**
 * @brief Retorna o ID único do board
 * @return Referência constante para o ID do board
 * @details O ID é imutável durante todo o ciclo de vida do board.
 */
const Board::Id& Board::id() const noexcept {
    return id_;
}

/**
 * @brief Retorna o nome do board
 * @return Referência constante para o nome do board
 * @details O nome pode ser alterado através do método setName().
 */
const std::string& Board::name() const noexcept {
    return name_;
}

/**
 * @brief Define um novo nome para o board
 * @param name Novo nome a ser atribuído ao board
 * @details Permite a alteraçao do nome do board após sua criaçao.
 *          Operaçao que poderia ser registrada no ActivityLog se desejado.
 */
void Board::setName(const std::string& name) {
    name_ = name;
}

// ============================================================================
// GERENCIAMENTO DE COLUNAS
// ============================================================================

/**
 * @brief Adiciona uma coluna ao board
 * @param column Shared pointer para a coluna a ser adicionada
 * @details Verifica se a coluna já existe para evitar duplicatas.
 *          A coluna é adicionada ao final do vetor de columns.
 */
void Board::addColumn(const std::shared_ptr<Column>& column) {
    // Verifica se a coluna já existe para evitar duplicatas
    if (!hasColumn(column->id())) {
        columns_.push_back(column);
    }
}

/**
 * @brief Remove uma coluna do board pelo ID
 * @param columnId ID da coluna a ser removida
 * @return Optional contendo shared_ptr para a coluna removida se encontrada,
 *         ou std::nullopt se a coluna nao existir
 * @details A coluna é removida do vetor interno e retornada para que
 *          o caller possa decidir o que fazer com ela (descarte ou reuso).
 */
std::optional<std::shared_ptr<Column>> Board::removeColumnById(const Id& columnId) {
    auto it = std::find_if(columns_.begin(), columns_.end(),
        [&columnId](const std::shared_ptr<Column>& column) {
            return column->id() == columnId;
        });
    
    if (it != columns_.end()) {
        auto column = *it;
        columns_.erase(it);
        return column;
    }
    return std::nullopt;
}

/**
 * @brief Retorna todas as colunas do board
 * @return Referência constante para o vetor de colunas
 * @details As colunas sao retornadas na ordem em que foram adicionadas.
 */
const std::vector<std::shared_ptr<Column>>& Board::columns() const noexcept {
    return columns_;
}

/**
 * @brief Busca uma coluna específica pelo ID
 * @param columnId ID da coluna a ser encontrada
 * @return Optional contendo shared_ptr para a coluna se encontrada,
 *         ou std::nullopt se nao existir
 * @details Utiliza busca linear no vetor de colunas.
 */
std::optional<std::shared_ptr<Column>> Board::findColumn(const Id& columnId) const noexcept {
    auto it = std::find_if(columns_.begin(), columns_.end(),
        [&columnId](const std::shared_ptr<Column>& column) {
            return column->id() == columnId;
        });
    
    if (it != columns_.end()) {
        return *it;
    }
    return std::nullopt;
}

/**
 * @brief Retorna o número de colunas no board
 * @return Quantidade de colunas presentes no board
 * @details Útil para estatísticas e interface do usuário.
 */
std::size_t Board::columnCount() const noexcept {
    return columns_.size();
}

/**
 * @brief Verifica se uma coluna existe no board
 * @param columnId ID da coluna a ser verificada
 * @return true se a coluna existe, false caso contrário
 * @details Mais eficiente que findColumn() quando apenas a existência importa.
 */
bool Board::hasColumn(const Id& columnId) const noexcept {
    return std::any_of(columns_.begin(), columns_.end(),
        [&columnId](const std::shared_ptr<Column>& column) {
            return column->id() == columnId;
        });
}

// ============================================================================
// MOVIMENTAÇaO DE CARDS E ATIVIDADES
// ============================================================================

/**
 * @brief Move um card entre duas colunas do board
 * @param cardId ID do card a ser movido
 * @param fromColumnId ID da coluna de origem
 * @param toColumnId ID da coluna de destino
 * @throws std::runtime_error Se coluna de origem, coluna de destino ou card nao forem encontrados
 * @details Operaçao atômica que remove o card da coluna de origem e o adiciona
 *          à coluna de destino. Registra a atividade no ActivityLog se configurado.
 */
void Board::moveCard(const std::string& cardId,
                     const Id& fromColumnId,
                     const Id& toColumnId) {
    // Encontrar a coluna de origem
    auto fromColumnOpt = findColumn(fromColumnId);
    if (!fromColumnOpt) {
        throw std::runtime_error("Coluna de origem nao encontrada: " + fromColumnId);
    }
    
    // Encontrar a coluna de destino
    auto toColumnOpt = findColumn(toColumnId);
    if (!toColumnOpt) {
        throw std::runtime_error("Coluna de destino nao encontrada: " + toColumnId);
    }
    
    auto fromColumn = *fromColumnOpt;
    auto toColumn = *toColumnOpt;
    
    // Remover o card da coluna de origem
    auto cardOpt = fromColumn->removeCardById(cardId);
    if (!cardOpt) {
        throw std::runtime_error("Card nao encontrado na coluna de origem: " + cardId);
    }
    
    auto card = *cardOpt;
    
    // Adicionar o card à coluna de destino
    toColumn->addCard(card);
    
    // Registrar a atividade se o ActivityLog estiver configurado
    if (activityLog_) {
        auto now = std::chrono::system_clock::now();
        std::string description = "Card '" + card->title() + "' movido de '" + 
                                 fromColumn->name() + "' para '" + toColumn->name() + "'";
        
        Activity activity(cardId + "_move", description, now);
        activityLog_->add(std::move(activity));
    }
}

// ============================================================================
// GERENCIAMENTO DO ACTIVITY LOG
// ============================================================================

/**
 * @brief Configura o ActivityLog para este board
 * @param log Shared pointer para o ActivityLog a ser associado
 * @details Permite associar um sistema de logging de atividades ao board.
 *          Útil para auditoria e histórico de mudanças.
 */
void Board::setActivityLog(const std::shared_ptr<ActivityLog>& log) noexcept {
    activityLog_ = log;
}

/**
 * @brief Retorna o ActivityLog associado ao board
 * @return Shared pointer para o ActivityLog, pode ser nullptr se nao configurado
 * @details Permite acessar o histórico de atividades do board.
 */
std::shared_ptr<ActivityLog> Board::activityLog() const noexcept {
    return activityLog_;
}

// ============================================================================
// OPERAÇÕES DE LIMPEZA
// ============================================================================

/**
 * @brief Limpa completamente o board
 * @details Remove todas as colunas e desassocia o ActivityLog.
 *          Operaçao destrutiva - use com cuidado.
 */
void Board::clear() noexcept {
    columns_.clear();
    activityLog_ = nullptr;
}

void Board::setColumns(const std::vector<std::shared_ptr<Column>>& columns) {
    columns_ = columns;
}

} // namespace domain
} // namespace kanban