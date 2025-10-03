/**
 * @file KanbanService.h
 * @brief Declaraçao do serviço principal do sistema Kanban
 * @details Esta classe implementa a interface IService e atua como a camada de
 *          aplicaçao do sistema, orquestrando todas as operações e coordenando
 *          a interaçao entre domínio, persistência e apresentaçao.
 */

#pragma once

#include "../interfaces/IService.h"
#include "../persistence/MemoryRepository.h"
#include "../domain/Board.h"        // INCLUA ESTES HEADERS COMPLETOS
#include "../domain/Column.h"
#include "../domain/Card.h"
#include "../domain/User.h"         // ESPECIALMENTE ESTE
#include "../domain/ActivityLog.h"
#include <memory>
#include <string>
#include <random>

namespace kanban {
namespace application {

/**
 * @brief Serviço principal do sistema Kanban
 * @details Implementa a interface IService e serve como facade para todas as
 *          operações do sistema. Coordena a interaçao entre as entidades de
 *          domínio, repositórios de persistência e camada de apresentaçao.
 * 
 *          Responsável por:
 *          - Gerenciar o ciclo de vida de boards, columns, cards e usuários
 *          - Aplicar regras de negócio e validações
 *          - Gerar IDs únicos para todas as entidades
 *          - Manter a consistência dos dados entre operações
 *          - Fornecer dados para a camada de apresentaçao
 */
class KanbanService : public interfaces::IService {
public:
    /**
     * @brief Construtor do KanbanService
     * @details Inicializa os repositórios em memória e os contadores de ID
     *          para cada tipo de entidade. Prepara o serviço para operaçao.
     */
    KanbanService();
    
    /**
     * @brief Destrutor do KanbanService
     * @details Realiza limpeza padrao. Os repositórios em memória sao
     *          destruídos automaticamente com seus dados.
     */
    ~KanbanService() = default;

    // ============================================================================
    // OPERAÇÕES PRINCIPAIS DO SISTEMA
    // ============================================================================

    /**
     * @brief Cria dados de exemplo para demonstraçao do sistema
     * @details Cria um board completo com colunas padrao (To Do, Doing, Done)
     *          e cards de exemplo que ilustram o fluxo de trabalho Kanban.
     *          Útil para testes iniciais e demonstrações do sistema.
     */
    void createSampleData() override;

    /**
     * @brief Cria um novo board no sistema
     * @param name Nome do board a ser criado
     * @return ID único do board criado
     * @details Cria a entidade Board, configura seu ActivityLog e persiste
     *          no repositório apropriado. Gera automaticamente um ID único.
     */
    std::string createBoard(const std::string& name) override;

    /**
     * @brief Adiciona uma nova coluna a um board existente
     * @param boardId ID do board onde a coluna será adicionada
     * @param columnName Nome da nova coluna
     * @return ID único da coluna criada
     * @throws std::runtime_error Se o board especificado nao existir
     * @details Valida a existência do board, cria a coluna e a adiciona
     *          tanto ao repositório de columns quanto ao board específico.
     */
    std::string addColumn(const std::string& boardId, const std::string& columnName) override;

    /**
     * @brief Adiciona um novo card a uma coluna existente
     * @param boardId ID do board onde a coluna está localizada
     * @param columnId ID da coluna onde o card será adicionado
     * @param title Título do novo card
     * @return ID único do card criado
     * @throws std::runtime_error Se board ou coluna nao existirem
     * @details Realiza validações em cascata, cria o card e o adiciona
     *          ao repositório de cards e à coluna especificada.
     */
    std::string addCard(const std::string& boardId, const std::string& columnId, const std::string& title) override;

    /**
     * @brief Move um card entre duas colunas do mesmo board
     * @param boardId ID do board onde está o card
     * @param cardId ID do card a ser movido
     * @param fromColumnId ID da coluna de origem
     * @param toColumnId ID da coluna de destino
     * @throws std::runtime_error Se board, colunas ou card nao existirem
     * @details Realiza validações extensivas e delega a operaçao de
     *          movimentaçao para a entidade Board, que também registra
     *          a atividade no ActivityLog se configurado.
     */
    void moveCard(const std::string& boardId, const std::string& cardId, 
                  const std::string& fromColumnId, const std::string& toColumnId) override;

    // ============================================================================
    // CONSULTAS E RELATÓRIOS
    // ============================================================================

    /**
     * @brief Retorna todos os boards do sistema
     * @return Vector contendo shared_ptr para todos os boards
     * @details Consulta simples ao repositório de boards que retorna
     *          todas as entidades existentes, independente de estado.
     */
    std::vector<std::shared_ptr<domain::Board>> listBoards() const override;

    /**
     * @brief Busca um board específico pelo ID
     * @param boardId ID do board a ser encontrado
     * @return std::optional contendo shared_ptr para o board se encontrado,
     *         ou std::nullopt se nao existir
     * @details Usa std::optional para representar claramente a possibilidade
     *          do board nao existir, evitando exceções em casos normais.
     */
    std::optional<std::shared_ptr<domain::Board>> findBoard(const std::string& boardId) const override;

    /**
     * @brief Lista todas as colunas de um board específico
     * @param boardId ID do board cujas colunas serao listadas
     * @return Vector contendo shared_ptr para as colunas do board
     * @throws std::runtime_error Se o board nao existir
     * @details Valida a existência do board antes de acessar suas colunas.
     *          Retorna vector vazio se o board nao tiver colunas.
     */
    std::vector<std::shared_ptr<domain::Column>> listColumns(const std::string& boardId) const override;

    /**
     * @brief Lista todos os cards de uma coluna específica
     * @param columnId ID da coluna cujos cards serao listados
     * @return Vector contendo shared_ptr para os cards da coluna
     * @throws std::runtime_error Se a coluna nao existir
     * @details Valida a existência da coluna antes de acessar seus cards.
     *          Retorna vector vazio se a coluna nao tiver cards.
     */
    std::vector<std::shared_ptr<domain::Card>> listCards(const std::string& columnId) const override;

private:
    // ============================================================================
    // REPOSITÓRIOS DE PERSISTÊNCIA
    // ============================================================================

    /// @brief Repositório para armazenamento de boards em memória
    persistence::MemoryRepository<domain::Board> boardRepository_;
    
    /// @brief Repositório para armazenamento de columns em memória
    persistence::MemoryRepository<domain::Column> columnRepository_;
    
    /// @brief Repositório para armazenamento de cards em memória
    persistence::MemoryRepository<domain::Card> cardRepository_;
    
    /// @brief Repositório para armazenamento de usuários em memória
    persistence::MemoryRepository<domain::User> userRepository_;

    // ============================================================================
    // GERADORES DE ID
    // ============================================================================

    /// @brief Contador sequencial para geraçao de IDs de boards
    int nextBoardId_;
    
    /// @brief Contador sequencial para geraçao de IDs de columns
    int nextColumnId_;
    
    /// @brief Contador sequencial para geraçao de IDs de cards
    int nextCardId_;
    
    /// @brief Contador sequencial para geraçao de IDs de usuários
    int nextUserId_;

    // ============================================================================
    // MÉTODOS AUXILIARES PRIVADOS
    // ============================================================================

    /**
     * @brief Gera um ID único para um novo board
     * @return String no formato "board_X" onde X é um número sequencial
     */
    std::string generateBoardId();
    
    /**
     * @brief Gera um ID único para uma nova coluna
     * @return String no formato "column_X" onde X é um número sequencial
     */
    std::string generateColumnId();
    
    /**
     * @brief Gera um ID único para um novo card
     * @return String no formato "card_X" onde X é um número sequencial
     */
    std::string generateCardId();
    
    /**
     * @brief Gera um ID único para um novo usuário
     * @return String no formato "user_X" onde X é um número sequencial
     */
    std::string generateUserId();
    
    // ============================================================================
    // VALIDAÇÕES DE REGRAS DE NEGÓCIO
    // ============================================================================

    /**
     * @brief Valida se um board existe no sistema
     * @param boardId ID do board a ser validado
     * @throws std::runtime_error Se o board nao for encontrado
     */
    void validateBoardExists(const std::string& boardId) const;
    
    /**
     * @brief Valida se uma coluna existe no sistema
     * @param columnId ID da coluna a ser validada
     * @throws std::runtime_error Se a coluna nao for encontrada
     */
    void validateColumnExists(const std::string& columnId) const;
};

} // namespace application
} // namespace kanban