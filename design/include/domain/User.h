/**
 * @file User.h
 * @brief Declaraçao da entidade User do sistema Kanban
 * @details Este header define a classe User, que representa um usuário
 *          local do sistema Kanban. Fornece estrutura básica para
 *          identificaçao e personalizaçao de usuários no sistema.
 */

#pragma once

#include <string>
#include <ostream>

namespace kanban {
namespace domain {

// ============================================================================
// CLASSE User
// ============================================================================

/**
 * @brief Representa um usuário local do sistema Kanban
 * @details A classe User fornece uma estrutura básica para identificaçao
 *          de usuários no sistema. Atualmente nao inclui sistema de
 *          autenticaçao remota, focando em identificaçao local simples.
 * 
 *          Pode ser estendida no futuro para incluir permissões, preferências
 *          e outras informações de perfil do usuário.
 */
class User {
public:
    /**
     * @brief Tipo auxiliar para IDs do usuário
     * @details Facilita futuras alterações no tipo de ID mantendo
     *          a interface consistente.
     */
    using Id = std::string;

    // ============================================================================
    // CONSTRUTOR E REGRA DOS CINCO
    // ============================================================================

    /**
     * @brief Construtor explícito do User
     * @param id Identificador único do usuário
     * @param name Nome completo ou de exibiçao do usuário
     * @details Construtor explícito previne conversões implícitas indesejadas.
     *          Recebe parâmetros por referência para evitar cópias desnecessárias.
     */
    explicit User(const Id& id, const std::string& name) noexcept;

    /**
     * @brief Construtor de cópia padrao
     * @details std::string suporta cópia e movimentaçao de forma segura.
     */
    User(const User&) = default;

    /**
     * @brief Construtor de movimentaçao padrao
     */
    User(User&&) noexcept = default;

    /**
     * @brief Operador de atribuiçao por cópia padrao
     */
    User& operator=(const User&) = default;

    /**
     * @brief Operador de atribuiçao por movimentaçao padrao
     */
    User& operator=(User&&) noexcept = default;

    /**
     * @brief Destrutor padrao
     */
    ~User() = default;

    // ============================================================================
    // ACESSORES E MODIFICADORES
    // ============================================================================

    /**
     * @brief Retorna o ID único do usuário
     * @return Referência constante para o ID do usuário
     * @details O ID é imutável durante todo o ciclo de vida do usuário
     *          e serve como chave primária para identificaçao.
     */
    const Id& id() const noexcept;

    /**
     * @brief Retorna o nome do usuário
     * @return Referência constante para o nome do usuário
     * @details Representa o nome de exibiçao do usuário no sistema.
     */
    const std::string& name() const noexcept;

    /**
     * @brief Define um novo nome para o usuário
     * @param name Novo nome a ser atribuído
     * @details Permite a alteraçao do nome de exibiçao do usuário
     *          após sua criaçao. Útil para correções ou atualizações.
     */
    void setName(const std::string& name);

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
    bool operator==(const User& other) const noexcept;

    /**
     * @brief Operador de desigualdade entre usuários
     * @param other Outro usuário a ser comparado
     * @return true se os usuários têm IDs diferentes, false caso contrário
     * @details Implementado em termos do operador de igualdade para
     *          garantir consistência na comparaçao.
     */
    bool operator!=(const User& other) const noexcept;

    // ============================================================================
    // OPERADOR DE SAÍDA
    // ============================================================================

    /**
     * @brief Operador de saída para formataçao de User
     * @param os Stream de saída onde o usuário será formatado
     * @param u Referência para o usuário a ser formatado
     * @return Referência para a stream de saída
     * @details Formata o usuário em formato legível para debug e logs,
     *          incluindo ID e nome.
     */
    friend std::ostream& operator<<(std::ostream& os, const User& u);

private:
    Id id_;           ///< @brief Identificador único do usuário
    std::string name_; ///< @brief Nome de exibiçao do usuário

    // ============================================================================
    // NOTA SOBRE CONCORRÊNCIA
    // ============================================================================
    // Esta classe NaO é thread-safe por padrao. Se houver concorrência entre
    // threads, proteja leituras/escritas a 'name_' com mutex na camada superior.
};

} // namespace domain
} // namespace kanban