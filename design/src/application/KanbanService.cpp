/**
 * @file KanbanService.cpp
 * @brief Implementaçao do serviço principal do sistema Kanban
 * @details Esta classe implementa a interface IService e atua como a camada de
 *          aplicaçao, orquestrando todas as operações do sistema e coordenando
 *          a interaçao entre o domínio, persistência e apresentaçao.
 */

#include "application/KanbanService.h"
#include "domain/Board.h"
#include "domain/Column.h"
#include "domain/Card.h"
#include "domain/User.h"
#include "domain/ActivityLog.h"
#include <stdexcept>
#include <sstream>

namespace kanban {
namespace application {

// ============================================================================
// CONSTRUTOR E INICIALIZAÇaO
// ============================================================================

/**
 * @brief Construtor do KanbanService
 * @details Inicializa os contadores de ID para cada tipo de entidade.
 *          Os repositórios sao inicializados automaticamente com seus construtores padrao.
 *          Usa sequências numéricas simples para geraçao de IDs únicos.
 */
KanbanService::KanbanService() 
    : nextBoardId_(1), nextColumnId_(1), nextCardId_(1), nextUserId_(1) {
    // Os repositórios (boardRepository_, columnRepository_, etc.) sao 
    // inicializados automaticamente com seus construtores padrao
}

// ============================================================================
// GERACaO DE IDs ÚNICOS
// ============================================================================

/**
 * @brief Gera um ID único para um novo Board
 * @return String no formato "board_X" onde X é um número sequencial
 * @details Mantém um contador interno que é incrementado a cada chamada.
 *          Garante IDs únicos durante a sessao da aplicaçao.
 */
std::string KanbanService::generateBoardId() {
    return "board_" + std::to_string(nextBoardId_++);
}

/**
 * @brief Gera um ID único para uma nova Column
 * @return String no formato "column_X" onde X é um número sequencial
 * @details Contador separado para columns, independente de outras entidades.
 */
std::string KanbanService::generateColumnId() {
    return "column_" + std::to_string(nextColumnId_++);
}

/**
 * @brief Gera um ID único para um novo Card
 * @return String no formato "card_X" onde X é um número sequencial
 * @details Cada card recebe um ID único, mesmo que esteja em columns diferentes.
 */
std::string KanbanService::generateCardId() {
    return "card_" + std::to_string(nextCardId_++);
}

/**
 * @brief Gera um ID único para um novo User
 * @return String no formato "user_X" onde X é um número sequencial
 * @details Preparado para futura implementaçao de sistema de usuários.
 */
std::string KanbanService::generateUserId() {
    return "user_" + std::to_string(nextUserId_++);
}

// ============================================================================
// VALIDAÇÕES DE REGRAS DE NEGÓCIO
// ============================================================================

/**
 * @brief Valida se um Board existe no sistema
 * @param boardId ID do board a ser validado
 * @throws std::runtime_error Se o board nao for encontrado
 * @details Consulta o repositório de boards para verificar existência.
 *          Lança exceçao com mensagem descritiva em caso de nao existência.
 */
void KanbanService::validateBoardExists(const std::string& boardId) const {
    if (!boardRepository_.exists(boardId)) {
        throw std::runtime_error("Board nao encontrado: " + boardId);
    }
}

/**
 * @brief Valida se uma Column existe no sistema
 * @param columnId ID da coluna a ser validada
 * @throws std::runtime_error Se a coluna nao for encontrada
 * @details Verifica no repositório de columns se a coluna existe.
 *          Importante para operações que dependem de columns válidas.
 */
void KanbanService::validateColumnExists(const std::string& columnId) const {
    if (!columnRepository_.exists(columnId)) {
        throw std::runtime_error("Coluna nao encontrada: " + columnId);
    }
}

// ============================================================================
// IMPLEMENTAÇaO DA INTERFACE IService
// ============================================================================

/**
 * @brief Cria dados de exemplo para demonstraçao e testes
 * @details Cria um board completo com colunas padrao do Kanban e cards
 *          de exemplo que demonstram o fluxo de trabalho típico.
 *          Útil para testes manuais e demonstrações do sistema.
 */
void KanbanService::createSampleData() {
    // Criar um board de exemplo com nome descritivo
    std::string boardId = createBoard("Projeto Kanban de Exemplo");
    
    // Adicionar colunas padrao do fluxo Kanban
    std::string todoId = addColumn(boardId, "To Do");
    std::string doingId = addColumn(boardId, "Doing");
    std::string doneId = addColumn(boardId, "Done");
    
    // Adicionar cards de exemplo que representam tarefas típicas de desenvolvimento
    addCard(boardId, todoId, "Configurar ambiente de desenvolvimento");
    addCard(boardId, todoId, "Implementar classes de domínio");
    addCard(boardId, doingId, "Criar KanbanService");
    addCard(boardId, doneId, "Definir arquitetura do projeto");
}

/**
 * @brief Cria um novo Board no sistema
 * @param name Nome do board a ser criado
 * @return ID único do board criado
 * @details Cria a entidade Board, configura um ActivityLog para ela,
 *          e persiste no repositório apropriado.
 */
std::string KanbanService::createBoard(const std::string& name) {
    // Gerar ID único para o novo board
    std::string boardId = generateBoardId();
    
    // Criar instância do Board usando smart pointer
    auto board = std::make_shared<domain::Board>(boardId, name);
    
    // Criar e configurar ActivityLog para o board (rastreamento de atividades)
    auto activityLog = std::make_shared<domain::ActivityLog>();
    board->setActivityLog(activityLog);
    
    // Persistir o board no repositório
    boardRepository_.add(board);
    
    return boardId;
}

/**
 * @brief Adiciona uma nova Column a um Board existente
 * @param boardId ID do board onde a coluna será adicionada
 * @param columnName Nome da nova coluna
 * @return ID único da coluna criada
 * @throws std::runtime_error Se o board nao existir
 * @details Valida a existência do board, cria a coluna, persiste no repositório
 *          de columns e adiciona ao board especificado.
 */
std::string KanbanService::addColumn(const std::string& boardId, const std::string& columnName) {
    // Validar que o board existe antes de prosseguir
    validateBoardExists(boardId);
    
    // Gerar ID único para a nova coluna
    std::string columnId = generateColumnId();
    auto column = std::make_shared<domain::Column>(columnId, columnName);
    
    // Adicionar ao repositório de colunas (persistência independente)
    columnRepository_.add(column);
    
    // Adicionar a coluna ao board específico
    auto boardOpt = boardRepository_.findById(boardId);
    if (boardOpt.has_value()) {
        auto board = boardOpt.value();
        board->addColumn(column);
    }
    
    return columnId;
}

/**
 * @brief Adiciona um novo Card a uma Column existente
 * @param boardId ID do board onde a coluna está localizada
 * @param columnId ID da coluna onde o card será adicionado
 * @param title Título do novo card
 * @return ID único do card criado
 * @throws std::runtime_error Se board ou coluna nao existirem
 * @details Realiza validações em cascata, cria o card e o adiciona
 *          tanto ao repositório quanto à coluna específica.
 */
std::string KanbanService::addCard(const std::string& boardId, const std::string& columnId, const std::string& title) {
    // Validações em cascata para garantir integridade referencial
    validateBoardExists(boardId);
    validateColumnExists(columnId);
    
    // Gerar ID único para o novo card
    std::string cardId = generateCardId();
    auto card = std::make_shared<domain::Card>(cardId, title);
    
    // Adicionar ao repositório de cards (persistência centralizada)
    cardRepository_.add(card);
    
    // Adicionar o card à coluna específica
    auto columnOpt = columnRepository_.findById(columnId);
    if (columnOpt.has_value()) {
        auto column = columnOpt.value();
        column->addCard(card);
    }
    
    return cardId;
}

/**
 * @brief Move um Card entre duas Columns do mesmo Board
 * @param boardId ID do board onde está o card
 * @param cardId ID do card a ser movido
 * @param fromColumnId ID da coluna de origem
 * @param toColumnId ID da coluna de destino
 * @throws std::runtime_error Se board, colunas ou card nao existirem
 * @details Operaçao complexa que envolve múltiplas validações e delega
 *          a lógica de movimentaçao para o domínio (classe Board).
 *          O ActivityLog do board registra a movimentaçao automaticamente.
 */
void KanbanService::moveCard(const std::string& boardId, const std::string& cardId, 
                            const std::string& fromColumnId, const std::string& toColumnId) {
    // Validações extensivas para garantir que todas as entidades envolvidas existem
    validateBoardExists(boardId);
    validateColumnExists(fromColumnId);
    validateColumnExists(toColumnId);
    
    // Recuperar o board do repositório
    auto boardOpt = boardRepository_.findById(boardId);
    if (!boardOpt.has_value()) {
        throw std::runtime_error("Board nao encontrado: " + boardId);
    }
    
    auto board = boardOpt.value();
    
    // Delegar a operaçao de movimentaçao para a classe Board (domínio)
    // Esta operaçao também acionará o registro no ActivityLog se configurado
    board->moveCard(cardId, fromColumnId, toColumnId);
}

/**
 * @brief Retorna todos os Boards do sistema
 * @return Vector contendo shared_ptr para todos os boards
 * @details Consulta simples ao repositório que retorna todos os boards
 *          existentes, independente de seu estado ou conteúdo.
 */
std::vector<std::shared_ptr<domain::Board>> KanbanService::listBoards() const {
    return boardRepository_.getAll();
}

/**
 * @brief Busca um Board específico pelo ID
 * @param boardId ID do board a ser encontrado
 * @return std::optional contendo shared_ptr para o board se encontrado,
 *         ou std::nullopt se nao existir
 * @details Usa std::optional para representar claramente a possibilidade
 *          de o board nao existir, evitando exceções em casos normais.
 */
std::optional<std::shared_ptr<domain::Board>> KanbanService::findBoard(const std::string& boardId) const {
    return boardRepository_.findById(boardId);
}

/**
 * @brief Lista todas as Columns de um Board específico
 * @param boardId ID do board cujas colunas serao listadas
 * @return Vector contendo shared_ptr para as colunas do board
 * @throws std::runtime_error Se o board nao existir
 * @details Valida a existência do board antes de acessar suas colunas.
 *          Retorna vector vazio se o board nao tiver colunas.
 */
std::vector<std::shared_ptr<domain::Column>> KanbanService::listColumns(const std::string& boardId) const {
    validateBoardExists(boardId);
    
    auto boardOpt = boardRepository_.findById(boardId);
    if (boardOpt.has_value()) {
        auto board = boardOpt.value();
        return board->columns();
    }
    return {}; // Retorna vector vazio se board nao for encontrado (apesar da validaçao)
}

/**
 * @brief Lista todos os Cards de uma Column específica
 * @param columnId ID da coluna cujos cards serao listados
 * @return Vector contendo shared_ptr para os cards da coluna
 * @throws std::runtime_error Se a coluna nao existir
 * @details Valida a existência da coluna antes de acessar seus cards.
 *          Retorna vector vazio se a coluna nao tiver cards.
 */
std::vector<std::shared_ptr<domain::Card>> KanbanService::listCards(const std::string& columnId) const {
    validateColumnExists(columnId);
    
    auto columnOpt = columnRepository_.findById(columnId);
    if (columnOpt.has_value()) {
        auto column = columnOpt.value();
        return column->cards();
    }
    return {}; // Retorna vector vazio se coluna nao for encontrada (apesar da validaçao)
}

} // namespace application
} // namespace kanban