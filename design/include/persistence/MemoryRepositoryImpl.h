/**
 * @file MemoryRepository.tpp
 * @brief Implementaçao do template MemoryRepository para persistência em memória
 * @details Este arquivo contém a implementaçao completa do template MemoryRepository,
 *          que fornece armazenamento em memória para entidades do sistema Kanban
 *          usando std::map como container interno. Oferece operações CRUD com
 *          complexidade O(log n) e é ideal para testes e protótipos.
 *
 * @tparam T Tipo da entidade armazenada (deve possuir método id() const)
 * @tparam Id Tipo do identificador único da entidade (padrao: std::string)
 */

#pragma once

#include "MemoryRepository.h"

namespace kanban {
namespace persistence {

// ============================================================================
// CONSTRUTOR
// ============================================================================

/**
 * @brief Construtor padrao do MemoryRepository
 * @details Inicializa um repositório vazio com o mapa interno pronto
 *          para receber itens. Nao requer alocaçao dinâmica adicional.
 */
template<typename T, typename Id>
MemoryRepository<T, Id>::MemoryRepository() = default;

// ============================================================================
// IMPLEMENTAÇaO DA INTERFACE IRepository
// ============================================================================

/**
 * @brief Adiciona um novo item ao repositório
 * @param item Shared pointer para a entidade a ser adicionada
 * @throws MemoryRepositoryException Se já existir um item com o mesmo ID
 * @details A operaçao possui complexidade O(log n) devido à natureza
 *          do std::map. Garante a unicidade dos IDs no repositório.
 */
template<typename T, typename Id>
void MemoryRepository<T, Id>::add(const std::shared_ptr<T>& item) {
    auto id = item->id();
    if (data_.find(id) != data_.end()) {
        throw MemoryRepositoryException("Item com id '" + id + "' já existe");
    }
    data_[id] = item;
}

/**
 * @brief Remove um item do repositório pelo seu ID
 * @param id Identificador único da entidade a ser removida
 * @throws MemoryRepositoryException Se o item nao for encontrado
 * @details Complexidade O(log n). A remoçao é permanente e o item
 *          nao pode ser recuperado após esta operaçao.
 */
template<typename T, typename Id>
void MemoryRepository<T, Id>::remove(const Id& id) {
    auto it = data_.find(id);
    if (it == data_.end()) {
        throw MemoryRepositoryException("Item com id '" + id + "' nao encontrado");
    }
    data_.erase(it);
}

/**
 * @brief Retorna todos os itens armazenados no repositório
 * @return Vetor contendo shared_ptr para todas as entidades
 * @details Os itens sao retornados em ordem crescente de ID
 *          (ordenaçao natural do std::map). Complexidade O(n).
 */
template<typename T, typename Id>
std::vector<std::shared_ptr<T>> MemoryRepository<T, Id>::getAll() const {
    std::vector<std::shared_ptr<T>> result;
    for (const auto& pair : data_) {
        result.push_back(pair.second);
    }
    return result;
}

/**
 * @brief Busca uma entidade específica pelo seu ID
 * @param id Identificador único da entidade
 * @return std::optional contendo shared_ptr para a entidade se encontrada,
 *         ou std::nullopt se nao existir
 * @details Complexidade O(log n). Utiliza busca binária no std::map.
 */
template<typename T, typename Id>
std::optional<std::shared_ptr<T>> MemoryRepository<T, Id>::findById(const Id& id) const {
    auto it = data_.find(id);
    if (it != data_.end()) {
        return it->second;
    }
    return std::nullopt;
}

// ============================================================================
// MÉTODOS ADICIONAIS PARA TESTES E UTILITÁRIOS
// ============================================================================

/**
 * @brief Remove todos os itens do repositório
 * @details Complexidade O(n). Útil para resetar o estado do repositório
 *          entre testes ou reinicializar o sistema.
 */
template<typename T, typename Id>
void MemoryRepository<T, Id>::clear() {
    data_.clear();
}

/**
 * @brief Retorna a quantidade de itens armazenados no repositório
 * @return Número de entidades atualmente armazenadas
 * @details Complexidade O(1). Útil para verificar o estado do repositório
 *          em testes e monitoramento.
 */
template<typename T, typename Id>
size_t MemoryRepository<T, Id>::size() const {
    return data_.size();
}

/**
 * @brief Verifica se existe um item com o ID fornecido no repositório
 * @param id Identificador único a ser verificado
 * @return true se o item existe, false caso contrário
 * @details Complexidade O(log n). Mais eficiente que findById() quando
 *          apenas a existência do item importa.
 */
template<typename T, typename Id>
bool MemoryRepository<T, Id>::exists(const Id& id) const {
    return data_.find(id) != data_.end();
}

} // namespace persistence
} // namespace kanban