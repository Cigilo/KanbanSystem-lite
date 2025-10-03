/**
 * @file MemoryRepositoryInstantiations.cpp
 * @brief Instanciações explícitas dos templates MemoryRepository para todas as entidades
 * @details Este arquivo fornece as instanciações explícitas dos templates de repositório
 *          em memória para todas as entidades do domínio, prevenindo problemas de linker
 *          e garantindo que as especializações estejam disponíveis em todo o sistema.
 */

#include "persistence/MemoryRepository.h"
#include "persistence/MemoryRepositoryImpl.h"
#include "domain/Board.h"
#include "domain/Column.h"
#include "domain/Card.h"
#include "domain/User.h"

namespace kanban {
namespace persistence {

// ============================================================================
// INSTANCIAÇÕES EXPLÍCITAS DOS TEMPLATES
// ============================================================================

/**
 * @brief Instanciaçao explícita do MemoryRepository para entidade Board
 * @details Garante que o template seja instanciado para Board, prevenindo
 *          erros de linker em tempo de compilaçao.
 */
template class MemoryRepository<domain::Board>;

/**
 * @brief Instanciaçao explícita do MemoryRepository para entidade Column
 * @details Especializaçao para gerenciamento de colunas em memória.
 */
template class MemoryRepository<domain::Column>;

/**
 * @brief Instanciaçao explícita do MemoryRepository para entidade Card
 * @details Especializaçao para gerenciamento de cards em memória.
 */
template class MemoryRepository<domain::Card>;

/**
 * @brief Instanciaçao explícita do MemoryRepository para entidade User
 * @details Especializaçao para gerenciamento de usuários em memória.
 */
template class MemoryRepository<domain::User>;

} // namespace persistence
} // namespace kanban