/**
 * @file MemoryRepository.h
 * @brief Declaraçao do repositório em memória para persistência de dados
 * @details Este header define a classe MemoryRepository, que implementa a interface
 *          IRepository usando estruturas em memória (std::map) para armazenamento
 *          de entidades do sistema Kanban. Ideal para testes, demonstrações
 *          e cenários onde persistência durável nao é necessária.
 */

#pragma once

#include "../interfaces/IRepository.h"
#include <map>
#include <stdexcept>
#include <algorithm>

namespace kanban {
namespace persistence {

// ============================================================================
// CLASSE MemoryRepositoryException
// ============================================================================

/**
 * @brief Exceçao específica para erros do repositório em memória
 * @details Esta exceçao especializa std::runtime_error para encapsular
 *          falhas específicas das operações do MemoryRepository, como:
 *          - Tentativa de adicionar item com ID duplicado
 *          - Tentativa de remover item inexistente
 *          - Violaçao de regras de integridade do repositório
 */
class MemoryRepositoryException : public std::runtime_error {
public:
    /**
     * @brief Construtor da exceçao MemoryRepositoryException
     * @param what Mensagem descritiva do erro ocorrido
     */
    explicit MemoryRepositoryException(const std::string& what)
        : std::runtime_error(what) {}
};

// ============================================================================
// TEMPLATE MemoryRepository
// ============================================================================

/**
 * @brief Repositório genérico em memória para persistência volátil
 * @tparam T Tipo da entidade armazenada no repositório
 * @tparam Id Tipo do identificador da entidade (padrao: std::string)
 * @details Implementa a interface IRepository usando std::map como
 *          armazenamento interno em memória. Todas as operações sao
 *          realizadas na memória RAM e os dados sao perdidos quando
 *          o repositório é destruído.
 * 
 *          Características principais:
 *          - Armazenamento volátil em memória RAM
 *          - Alta performance (operações O(log n) com std::map)
 *          - Ideal para testes unitários e integraçao
 *          - Útil para demonstrações e protótipos
 *          - Implementaçao completa da interface IRepository
 */
template<typename T, typename Id = std::string>
class MemoryRepository : public interfaces::IRepository<T, Id> {
public:
    /**
     * @brief Construtor do MemoryRepository
     * @details Inicializa um repositório vazio, pronto para armazenar itens.
     *          O mapa interno é inicializado automaticamente.
     */
    MemoryRepository();
    
    /**
     * @brief Destrutor do MemoryRepository
     * @details Destrói o repositório e todos os itens armazenados.
     *          Como o armazenamento é volátil, os dados sao perdidos.
     */
    ~MemoryRepository() = default;

    // ============================================================================
    // IMPLEMENTAÇaO DA INTERFACE IRepository
    // ============================================================================

    /**
     * @brief Adiciona um item ao repositório em memória
     * @param item Shared pointer para o item a ser adicionado
     * @throws MemoryRepositoryException Se já existir item com o mesmo ID
     */
    void add(const std::shared_ptr<T>& item) override;

    /**
     * @brief Remove um item do repositório em memória
     * @param id ID do item a ser removido
     * @throws MemoryRepositoryException Se o item nao existir
     */
    void remove(const Id& id) override;

    /**
     * @brief Retorna todos os itens do repositório em memória
     * @return Vector contendo shared_ptr para todos os itens
     * @details Os itens sao retornados em ordem crescente de ID
     *          (ordenaçao natural do std::map).
     */
    std::vector<std::shared_ptr<T>> getAll() const override;

    /**
     * @brief Busca um item específico pelo ID no repositório
     * @param id ID do item a ser encontrado
     * @return Optional contendo shared_ptr para o item se encontrado,
     *         ou std::nullopt se nao existir
     */
    std::optional<std::shared_ptr<T>> findById(const Id& id) const override;

    // ============================================================================
    // MÉTODOS ADICIONAIS PARA TESTES E UTILITÁRIOS
    // ============================================================================

    /**
     * @brief Remove todos os itens do repositório
     * @details Limpa completamente o mapa interno. Operaçao útil para
     *          redefinir o estado do repositório entre testes.
     */
    void clear();

    /**
     * @brief Retorna o número de itens no repositório
     * @return Quantidade de itens armazenados
     * @details Útil para verificar o estado do repositório em testes
     *          e asserções.
     */
    size_t size() const;

    /**
     * @brief Verifica se um item existe no repositório
     * @param id ID do item a ser verificado
     * @return true se o item existe, false caso contrário
     * @details Mais eficiente que findById() quando apenas a existência
     *          importa, pois evita a construçao do optional.
     */
    bool exists(const Id& id) const;

private:
    std::map<Id, std::shared_ptr<T>> data_; ///< @brief Armazenamento interno usando std::map ordenado por ID
    
    // ============================================================================
    // NOTAS DE IMPLEMENTAÇaO
    // ============================================================================
    // O uso de std::map (em vez de unordered_map) garante ordem consistente
    // dos IDs, o que pode ser útil para testes determinísticos. A performance
    // O(log n) é adequada para o tamanho esperado de repositórios Kanban.
};

} // namespace persistence
} // namespace kanban