/**
 * @file IRepository.h
 * @brief Declaraçao da interface genérica IRepository para persistência de dados
 * @details Este header define a interface base IRepository que serve como contrato
 *          para implementações de repositórios de persistência (memória, arquivo, banco de dados).
 *          Utiliza templates para fornecer forte tipagem e reutilizaçao para diferentes
 *          entidades do sistema Kanban.
 */

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace kanban {
namespace interfaces {

// ============================================================================
// INTERFACE TEMPLATE IRepository
// ============================================================================

/**
 * @brief Interface genérica para repositórios de persistência
 * @tparam T Tipo da entidade armazenada no repositório
 * @tparam Id Tipo do identificador da entidade (padrao: std::string)
 * @details A interface IRepository define o contrato básico para operações
 *          CRUD (Create, Read, Update, Delete) que todas as implementações
 *          de repositório devem seguir. Isso permite trocar a estratégia
 *          de persistência (memória, arquivo, banco de dados) sem alterar
 *          o código cliente.
 * 
 *          Características de design:
 *          - Template para forte tipagem e reutilizaçao
 *          - Uso de shared_ptr para gerenciamento flexível de ownership
 *          - std::optional para representar claramente a ausência de valores
 *          - Exceções para condições de erro excepcionais
 * 
 * @note Esta interface NaO é thread-safe por padrao. Implementações
 *       concretas devem fornecer sincronizaçao se necessário.
 */
template<typename T, typename Id = std::string>
class IRepository {
public:
    /**
     * @brief Destrutor virtual padrao
     * @details Garante a correta destruiçao de classes derivadas
     *          através de ponteiros da interface base.
     */
    virtual ~IRepository() = default;

    // ============================================================================
    // OPERAÇÕES CRUD (CREATE, READ, UPDATE, DELETE)
    // ============================================================================

    /**
     * @brief Adiciona um item ao repositório
     * @param item Shared pointer para o item a ser adicionado
     * @throws std::runtime_error ou derivada se já existir item com o mesmo ID
     * @details Operaçao de criaçao (CREATE) que adiciona um novo item ao
     *          repositório. A implementaçao deve garantir a unicidade dos IDs.
     */
    virtual void add(const std::shared_ptr<T>& item) = 0;

    /**
     * @brief Remove um item do repositório pelo ID
     * @param id ID do item a ser removido
     * @throws std::runtime_error ou derivada se o item nao existir
     * @details Operaçao de exclusao (DELETE) que remove um item existente.
     *          A remoçao de itens inexistentes é considerada condiçao de erro.
     */
    virtual void remove(const Id& id) = 0;

    /**
     * @brief Retorna todos os itens do repositório
     * @return Vector contendo shared_ptr para todos os itens
     * @details Operaçao de leitura (READ) que retorna todos os itens
     *          armazenados. Retorna uma cópia do vetor interno para
     *          manter o encapsulamento. A ordem dos itens é definida
     *          pela implementaçao concreta.
     */
    virtual std::vector<std::shared_ptr<T>> getAll() const = 0;

    /**
     * @brief Busca um item específico pelo ID
     * @param id ID do item a ser encontrado
     * @return Optional contendo shared_ptr para o item se encontrado,
     *         ou std::nullopt se nao existir
     * @details Operaçao de leitura (READ) que busca um item específico.
     *          Utiliza std::nullopt para representar claramente a
     *          ausência do item, evitando exceções em casos normais.
     */
    virtual std::optional<std::shared_ptr<T>> findById(const Id& id) const = 0;

    // ============================================================================
    // NOTAS DE IMPLEMENTAÇaO
    // ============================================================================
    // Observaçao: esta interface NaO é thread-safe por padrao. Se a aplicaçao
    // requer acesso concorrente ao repositório, a implementaçao concreta deve
    // fornecer sincronizaçao apropriada (ex.: usando std::mutex).
};

} // namespace interfaces
} // namespace kanban