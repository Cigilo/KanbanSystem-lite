/**
 * @file User.cpp
 * @brief Implementaçao da entidade User do sistema Kanban
 * @details Esta classe representa um usuário do sistema Kanban,
 *          fornecendo uma estrutura básica para identificaçao e
 *          personalizaçao de usuários no sistema.
 */

#include "domain/User.h"

namespace kanban {
namespace domain {

// ============================================================================
// CONSTRUTOR E MÉTODOS BÁSICOS
// ============================================================================

/**
 * @brief Construtor da classe User
 * @param id Identificador único do usuário
 * @param name Nome completo ou de exibiçao do usuário
 * @details Inicializa um novo usuário com ID e nome fornecidos.
 *          A classe User serve como base para futuras expansões
 *          do sistema de usuários e permissões.
 */
User::User(const Id& id, const std::string& name) noexcept
    : id_(id), name_(name) {}

/**
 * @brief Retorna o ID único do usuário
 * @return Referência constante para o ID do usuário
 * @details O ID é imutável durante todo o ciclo de vida do usuário
 *          e serve como chave primária para identificaçao.
 */
const User::Id& User::id() const noexcept {
    return id_;
}

/**
 * @brief Retorna o nome do usuário
 * @return Referência constante para o nome do usuário
 * @details O nome pode ser alterado através do método setName().
 *          Representa o nome de exibiçao do usuário no sistema.
 */
const std::string& User::name() const noexcept {
    return name_;
}

/**
 * @brief Define um novo nome para o usuário
 * @param name Novo nome a ser atribuído ao usuário
 * @details Permite a alteraçao do nome de exibiçao do usuário
 *          após sua criaçao. Útil para correções ou atualizações.
 */
void User::setName(const std::string& name) {
    name_ = name;
}

// ============================================================================
// OPERADORES DE COMPARAÇaO
// ============================================================================

/**
 * @brief Operador de igualdade entre usuários
 * @param other Outro usuário a ser comparado
 * @return true se os usuários têm o mesmo ID, false caso contrário
 * @details Dois usuários sao considerados iguais se possuem o mesmo ID,
 *          independentemente de outros atributos como nome.
 */
bool User::operator==(const User& other) const noexcept {
    return id_ == other.id_;
}

/**
 * @brief Operador de desigualdade entre usuários
 * @param other Outro usuário a ser comparado
 * @return true se os usuários têm IDs diferentes, false caso contrário
 * @details Implementado em termos do operador de igualdade para
 *          garantir consistência na comparaçao.
 */
bool User::operator!=(const User& other) const noexcept {
    return !(*this == other);
}

// ============================================================================
// OPERADOR DE FORMATAÇaO
// ============================================================================

/**
 * @brief Sobrecarga do operador de saída para formataçao de User
 * @param os Stream de saída onde o usuário será formatado
 * @param u Referência para o usuário a ser formatado
 * @return Referência para a stream de saída
 * @details Formata o usuário em um formato legível para debug,
 *          incluindo ID e nome. Útil para logging e diagnóstico.
 */
std::ostream& operator<<(std::ostream& os, const User& u) {
    os << "User{id=" << u.id() << ", name=" << u.name() << "}";
    return os;
}

} // namespace domain
} // namespace kanban