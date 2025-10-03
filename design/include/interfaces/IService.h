/**
 * @file IService.h
 * @brief Declaraçao da interface IService para o sistema Kanban
 * @details Este header define a interface principal do sistema Kanban,
 *          atuando como uma fachada (Facade Pattern) que abstrai a complexidade
 *          interna e fornece uma API simplificada para clientes (CLI, GUI, etc.).
 *          Esta interface representa a camada de aplicaçao do sistema.
 */

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace kanban {
namespace domain {
    // Forward declarations para reduzir acoplamento
    class Board;
    class Column;
    class Card;
}

namespace interfaces {

// ============================================================================
// INTERFACE IService
// ============================================================================

/**
 * @brief Interface de serviço de alto nível do sistema Kanban
 * @details A interface IService atua como a fachada principal do sistema,
 *          fornecendo uma API coesa e simplificada para todas as operações
 *          do Kanban. Esconde a complexidade das interações entre domínio,
 *          persistência e regras de negócio, apresentando uma interface
 *          unificada para clientes.
 * 
 *          Responsabilidades principais:
 *          - Orquestrar operações complexas envolvendo múltiplas entidades
 *          - Aplicar regras de negócio e validações
 *          - Coordenar a persistência de dados
 *          - Fornecer dados para a camada de apresentaçao
 * 
 *          Padrões de design aplicados:
 *          - Facade: simplifica a API complexa do sistema
 *          - Service Layer: encapsula a lógica de aplicaçao
 *          - Dependency Inversion: depende de abstrações (domínio)
 */
class IService {
public:
    /**
     * @brief Destrutor virtual padrao
     * @details Garante a correta destruiçao de implementações concretas
     *          através de ponteiros da interface base.
     */
    virtual ~IService() = default;

    // ============================================================================
    // MÉTODOS UTILITÁRIOS
    // ============================================================================

    /**
     * @brief Cria dados de exemplo para testes e demonstraçao
     * @details Inicializa o sistema com um board completo contendo
     *          colunas padrao (To Do, Doing, Done) e cards de exemplo
     *          que ilustram o fluxo de trabalho típico do Kanban.
     *          Útil para:
     *          - Testes manuais da aplicaçao
     *          - Demonstrações do sistema
     *          - Desenvolvimento inicial
     */
    virtual void createSampleData() = 0;

    // ============================================================================
    // OPERAÇÕES PRINCIPAIS (COMANDOS)
    // ============================================================================

    /**
     * @brief Cria um novo board no sistema
     * @param name Nome do board a ser criado
     * @return ID único do board criado
     * @details Operaçao de criaçao que instancia um novo board,
     *          configura seu ActivityLog e persiste no repositório.
     *          Retorna o ID gerado para referência futura.
     */
    virtual std::string createBoard(const std::string& name) = 0;

    /**
     * @brief Cria uma nova coluna em um board existente
     * @param boardId ID do board onde a coluna será criada
     * @param columnName Nome da nova coluna
     * @return ID único da coluna criada
     * @throws std::runtime_error Se o board especificado nao existir
     * @details Adiciona uma coluna ao board especificado e a persiste
     *          no repositório de colunas. Valida a existência do board.
     */
    virtual std::string addColumn(const std::string& boardId,
                                  const std::string& columnName) = 0;

    /**
     * @brief Cria um novo card em uma coluna específica
     * @param boardId ID do board onde a coluna está localizada
     * @param columnId ID da coluna onde o card será criado
     * @param title Título do novo card
     * @return ID único do card criado
     * @throws std::runtime_error Se board ou coluna nao existirem
     * @details Cria um card com o título especificado e o adiciona
     *          à coluna indicada. Realiza validações em cascata.
     */
    virtual std::string addCard(const std::string& boardId,
                                const std::string& columnId,
                                const std::string& title) = 0;

    /**
     * @brief Move um card entre duas colunas do mesmo board
     * @param boardId ID do board onde está o card
     * @param cardId ID do card a ser movido
     * @param fromColumnId ID da coluna de origem
     * @param toColumnId ID da coluna de destino
     * @throws std::runtime_error Se:
     *         - Board nao existir
     *         - Coluna de origem/destino nao existir
     *         - Card nao existir na coluna de origem
     * @details Operaçao atômica que coordena a remoçao do card da
     *          coluna de origem e sua inserçao na coluna de destino.
     *          Registra a atividade no ActivityLog se configurado.
     */
    virtual void moveCard(const std::string& boardId,
                          const std::string& cardId,
                          const std::string& fromColumnId,
                          const std::string& toColumnId) = 0;

    // ============================================================================
    // OPERAÇÕES DE CONSULTA (QUERIES)
    // ============================================================================

    /**
     * @brief Retorna todos os boards do sistema
     * @return Vector contendo shared_ptr para todos os boards
     * @details Consulta completa do repositório de boards.
     *          Retorna todos os boards independente de estado ou conteúdo.
     */
    virtual std::vector<std::shared_ptr<domain::Board>> listBoards() const = 0;

    /**
     * @brief Busca um board específico pelo ID
     * @param boardId ID do board a ser encontrado
     * @return Optional contendo shared_ptr para o board se encontrado,
     *         ou std::nullopt se nao existir
     * @details Consulta específica que retorna um board individual.
     *          Utiliza std::optional para representar claramente a
     *          possibilidade do board nao existir.
     */
    virtual std::optional<std::shared_ptr<domain::Board>> findBoard(const std::string& boardId) const = 0;

    /**
     * @brief Lista todas as colunas de um board específico
     * @param boardId ID do board cujas colunas serao listadas
     * @return Vector contendo shared_ptr para as colunas do board
     * @throws std::runtime_error Se o board nao existir
     * @details Retorna as colunas na ordem em que foram adicionadas
     *          ao board. Valida a existência do board antes do acesso.
     */
    virtual std::vector<std::shared_ptr<domain::Column>> listColumns(const std::string& boardId) const = 0;

    /**
     * @brief Lista todos os cards de uma coluna específica
     * @param columnId ID da coluna cujos cards serao listados
     * @return Vector contendo shared_ptr para os cards da coluna
     * @throws std::runtime_error Se a coluna nao existir
     * @details Retorna os cards na ordem atual da coluna.
     *          Valida a existência da coluna antes do acesso.
     */
    virtual std::vector<std::shared_ptr<domain::Card>> listCards(const std::string& columnId) const = 0;
};

} // namespace interfaces
} // namespace kanban