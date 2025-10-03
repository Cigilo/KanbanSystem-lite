/**
 * @file ActivityLog.h
 * @brief Declaraçao das classes Activity e ActivityLog para registro de atividades do sistema
 * @details Este header define o sistema de logging de atividades do Kanban,
 *          permitindo rastrear todas as ações significativas realizadas no sistema
 *          como movimentaçao de cards, criaçao de entidades, etc.
 */

#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <utility>
#include <ostream>

namespace kanban {
namespace domain {

/**
 * @brief Tipo de alias para timestamp de atividades
 * @details Representa um ponto no tempo usando a system_clock da std::chrono
 *          para registro preciso do momento em que atividades ocorreram.
 */
using TimePoint = std::chrono::system_clock::time_point;

// ============================================================================
// CLASSE Activity
// ============================================================================

/**
 * @brief Representa uma atividade individual no sistema Kanban
 * @details Registra uma açao específica realizada no sistema (ex.: movimentaçao
 *          de card, criaçao de entidade). Cada atividade possui ID único,
 *          descriçao textual e timestamp preciso.
 * 
 *          A classe é designed para ser imutável após criaçao, garantindo
 *          integridade dos registros históricos.
 */
class Activity {
public:
    /**
     * @brief Construtor explícito da Activity
     * @param id Identificador único da atividade
     * @param description Descriçao textual da açao realizada
     * @param when Timestamp do momento em que a atividade ocorreu
     * @details Construtor explícito previne conversões implícitas indesejadas
     *          e garante que todos os campos essenciais sejam fornecidos.
     */
    explicit Activity(const std::string& id,
                      const std::string& description,
                      TimePoint when) noexcept;

    // ============================================================================
    // REGRA DOS CINCO (FIVE RULE)
    // ============================================================================

    /**
     * @brief Construtor de cópia padrao
     * @details Seguro para std::string e TimePoint - realiza cópia profunda.
     */
    Activity(const Activity&) = default;

    /**
     * @brief Construtor de movimentaçao padrao
     * @details Realiza movimentaçao eficiente dos membros da classe.
     */
    Activity(Activity&&) noexcept = default;

    /**
     * @brief Operador de atribuiçao por cópia padrao
     * @details Mantém consistência com o construtor de cópia.
     */
    Activity& operator=(const Activity&) = default;

    /**
     * @brief Operador de atribuiçao por movimentaçao padrao
     * @details Mantém consistência com o construtor de movimentaçao.
     */
    Activity& operator=(Activity&&) noexcept = default;

    /**
     * @brief Destrutor padrao
     * @details Realiza limpeza automática dos recursos.
     */
    ~Activity() = default;

    // ============================================================================
    // GETTERS - ACESSO AOS DADOS
    // ============================================================================

    /**
     * @brief Retorna o ID único da atividade
     * @return Referência constante para o ID da atividade
     * @details O ID é imutável e serve como identificador único no sistema.
     */
    const std::string& id() const noexcept;

    /**
     * @brief Retorna a descriçao da atividade
     * @return Referência constante para a descriçao textual
     * @details A descriçao explica em linguagem natural qual açao foi executada.
     */
    const std::string& description() const noexcept;

    /**
     * @brief Retorna o timestamp da atividade
     * @return TimePoint representando o momento exato da ocorrência
     * @details Permite ordenaçao cronológica precisa das atividades.
     */
    TimePoint when() const noexcept;

    // ============================================================================
    // OPERADOR DE SAÍDA
    // ============================================================================

    /**
     * @brief Operador de saída para formataçao de Activity
     * @param os Stream de saída onde a atividade será formatada
     * @param a Referência para a atividade a ser formatada
     * @return Referência para a stream de saída
     * @details Formata a atividade em um formato legível para debug,
     *          incluindo ID, descriçao e timestamp formatado.
     */
    friend std::ostream& operator<<(std::ostream& os, const Activity& a);

private:
    std::string id_;           ///< @brief Identificador único da atividade
    std::string description_;  ///< @brief Descriçao textual da açao realizada
    TimePoint when_;          ///< @brief Momento exato em que a atividade ocorreu
};

// ============================================================================
// CLASSE ActivityLog
// ============================================================================

/**
 * @brief Contêiner para registro e gerenciamento de atividades
 * @details Mantém um histórico temporal de todas as atividades realizadas
 *          no sistema. Fornece interface para adiçao, consulta e limpeza
 *          de atividades, garantindo ordenaçao cronológica.
 */
class ActivityLog {
public:
    /**
     * @brief Construtor padrao do ActivityLog
     * @details Inicializa um log vazio, pronto para receber atividades.
     */
    ActivityLog() = default;

    /**
     * @brief Destrutor padrao do ActivityLog
     * @details Realiza limpeza automática de todas as atividades armazenadas.
     */
    ~ActivityLog() = default;

    // ============================================================================
    // MÉTODOS PRINCIPAIS
    // ============================================================================

    /**
     * @brief Adiciona uma nova atividade ao log
     * @param act Atividade a ser adicionada (recebida por valor para permitir move)
     * @details Recebe a atividade por valor para permitir movimentaçao eficiente.
     *          Exemplos de uso:
     *          @code
     *          log.add(Activity("id1", "Card movido", now));
     *          // ou
     *          Activity a("id2", "Card criado", now);
     *          log.add(std::move(a));
     *          @endcode
     */
    void add(Activity act);

    /**
     * @brief Retorna todas as atividades do log
     * @return Referência constante para o vetor interno de atividades
     * @details Expõe o vetor interno apenas para leitura, evitando cópia
     *          desnecessária mas mantendo a imutabilidade do log.
     *          As atividades sao retornadas em ordem cronológica de inserçao.
     */
    const std::vector<Activity>& activities() const noexcept;

    // ============================================================================
    // MÉTODOS UTILITÁRIOS
    // ============================================================================

    /**
     * @brief Retorna o número total de atividades no log
     * @return Quantidade de atividades armazenadas
     * @details Útil para estatísticas e monitoramento do volume de atividades.
     */
    std::size_t size() const noexcept;

    /**
     * @brief Verifica se o log está vazio
     * @return true se nao há atividades no log, false caso contrário
     * @details Método de conveniência para verificar existência de atividades.
     */
    bool empty() const noexcept;

    /**
     * @brief Retorna a última atividade adicionada ao log
     * @return Ponteiro para a última atividade, ou nullptr se o log estiver vazio
     * @details Evita lançamento de exceções quando o log está vazio,
     *          retornando nullptr em vez de acessar elementos inválidos.
     */
    const Activity* last() const noexcept;

    /**
     * @brief Limpa todas as atividades do log
     * @details Remove todas as entradas do histórico de atividades.
     *          Operaçao irreversível - use com cuidado.
     */
    void clear() noexcept;

private:
    std::vector<Activity> activities_;  ///< @brief Armazenamento interno das atividades em ordem cronológica
};

} // namespace domain
} // namespace kanban