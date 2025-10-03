/**
 * @file Card.h
 * @brief Declaraçao das entidades Tag e Card do sistema Kanban
 * @details Este header define as classes Tag (etiquetas) e Card (cartões/tarefas)
 *          que compõem o sistema Kanban, incluindo operações de gerenciamento
 *          de tags, prioridades e metadados temporais.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <chrono>
#include <ostream>

namespace kanban {
namespace domain {

/**
 * @brief Tipo de alias para timestamp usando system_clock
 * @details Fornece precisao de tempo para registro de criaçao e modificaçao.
 */
using TimePoint = std::chrono::system_clock::time_point;

/**
 * @brief Tipo de alias para o clock do sistema
 * @details Usado para obtençao do tempo atual de forma consistente.
 */
using Clock = std::chrono::system_clock;

// ============================================================================
// CLASSE Tag
// ============================================================================

/**
 * @brief Representa uma etiqueta (tag) aplicável a cards
 * @details Tags sao usadas para categorizar, filtrar e organizar cards
 *          no sistema Kanban. Cada tag possui ID único e nome descritivo.
 */
class Tag {
public:
    /**
     * @brief Construtor explícito da Tag
     * @param id Identificador único da tag
     * @param name Nome descritivo da tag
     * @details Construtor explícito previne conversões implícitas indesejadas.
     */
    explicit Tag(const std::string& id, const std::string& name) noexcept;

    // ============================================================================
    // REGRA DOS CINCO (FIVE RULE)
    // ============================================================================

    /// @brief Construtor de cópia padrao
    Tag(const Tag&) = default;
    
    /// @brief Construtor de movimentaçao padrao
    Tag(Tag&&) noexcept = default;
    
    /// @brief Operador de atribuiçao por cópia padrao
    Tag& operator=(const Tag&) = default;
    
    /// @brief Operador de atribuiçao por movimentaçao padrao
    Tag& operator=(Tag&&) noexcept = default;
    
    /// @brief Destrutor padrao
    ~Tag() = default;

    // ============================================================================
    // ACESSORES E MODIFICADORES
    // ============================================================================

    /**
     * @brief Retorna o ID único da tag
     * @return Referência constante para o ID da tag
     */
    const std::string& id() const noexcept;

    /**
     * @brief Retorna o nome da tag
     * @return Referência constante para o nome da tag
     */
    const std::string& name() const noexcept;

    /**
     * @brief Define um novo nome para a tag
     * @param name Novo nome a ser atribuído
     */
    void setName(const std::string& name);

    // ============================================================================
    // OPERADOR DE SAÍDA
    // ============================================================================

    /**
     * @brief Operador de saída para formataçao de Tag
     * @param os Stream de saída onde a tag será formatada
     * @param t Referência para a tag a ser formatada
     * @return Referência para a stream de saída
     * @details Formata a tag em formato legível para debug e logs.
     */
    friend std::ostream& operator<<(std::ostream& os, const Tag& t);

private:
    std::string id_;   ///< @brief Identificador único (poderá ser UUID no futuro)
    std::string name_; ///< @brief Nome descritivo da etiqueta
};

// ============================================================================
// CLASSE Card
// ============================================================================

/**
 * @brief Representa um cartao (task) no sistema Kanban
 * @details Um Card representa uma tarefa individual no sistema Kanban,
 *          contendo título, descriçao, prioridade, tags e metadados
 *          temporais para rastreamento de criaçao e modificaçao.
 */
class Card {
public:
    /**
     * @brief Construtor explícito do Card
     * @param id Identificador único do card
     * @param title Título do card (tarefa)
     * @details Recebe strings por referência para evitar cópias desnecessárias.
     *          Inicializa automaticamente os timestamps de criaçao e atualizaçao.
     */
    explicit Card(const std::string& id, const std::string& title);

    // ============================================================================
    // REGRA DOS CINCO (FIVE RULE)
    // ============================================================================

    /// @brief Construtor de cópia padrao (strings e time_point suportam)
    Card(const Card&) = default;
    
    /// @brief Construtor de movimentaçao padrao
    Card(Card&&) noexcept = default;
    
    /// @brief Operador de atribuiçao por cópia padrao
    Card& operator=(const Card&) = default;
    
    /// @brief Operador de atribuiçao por movimentaçao padrao
    Card& operator=(Card&&) noexcept = default;
    
    /// @brief Destrutor padrao
    ~Card() = default;

    // ============================================================================
    // ACESSORES E MODIFICADORES BÁSICOS
    // ============================================================================

    /**
     * @brief Retorna o ID único do card
     * @return Referência constante para o ID do card
     */
    const std::string& id() const noexcept;

    /**
     * @brief Retorna o título do card
     * @return Referência constante para o título do card
     */
    const std::string& title() const noexcept;

    /**
     * @brief Define um novo título para o card
     * @param title Novo título a ser atribuído
     * @details Atualiza automaticamente o timestamp de modificaçao.
     */
    void setTitle(const std::string& title);

    /**
     * @brief Retorna a descriçao do card
     * @return Referência constante para o optional contendo a descriçao
     * @details A descriçao é opcional e pode ser std::nullopt.
     */
    const std::optional<std::string>& description() const noexcept;

    /**
     * @brief Define a descriçao do card
     * @param desc Nova descriçao a ser atribuída
     * @details Atualiza automaticamente o timestamp de modificaçao.
     */
    void setDescription(const std::string& desc);

    // ============================================================================
    // GERENCIAMENTO DE PRIORIDADE E TEMPORAL
    // ============================================================================

    /**
     * @brief Define a prioridade do card
     * @param p Valor numérico da prioridade (maior valor = maior prioridade)
     * @details Atualiza automaticamente o timestamp de modificaçao.
     */
    void setPriority(int p) noexcept;

    /**
     * @brief Retorna a prioridade do card
     * @return Valor numérico da prioridade
     * @details Valor padrao é 0. Valores maiores indicam maior prioridade.
     */
    int priority() const noexcept;

    /**
     * @brief Retorna o timestamp de criaçao do card
     * @return TimePoint representando o momento de criaçao
     */
    TimePoint createdAt() const noexcept;

    /**
     * @brief Retorna o timestamp da última atualizaçao do card
     * @return TimePoint representando o momento da última modificaçao
     */
    TimePoint updatedAt() const noexcept;

    // ============================================================================
    // GERENCIAMENTO DE TAGS
    // ============================================================================

    /**
     * @brief Adiciona uma tag ao card
     * @param tag Shared pointer para a tag a ser adicionada
     * @details Recebe por referência constante para evitar cópia do controle
     *          de referência do shared_ptr. Verifica duplicatas internamente.
     */
    void addTag(const std::shared_ptr<Tag>& tag);

    /**
     * @brief Remove uma tag do card pelo ID
     * @param tagId ID da tag a ser removida
     * @return true se a tag foi encontrada e removida, false caso contrário
     * @details Nao lança exceções - retorna false em caso de tag nao encontrada.
     */
    bool removeTagById(const std::string& tagId) noexcept;

    /**
     * @brief Verifica se o card possui uma tag específica
     * @param tagId ID da tag a ser verificada
     * @return true se o card possui a tag, false caso contrário
     */
    bool hasTag(const std::string& tagId) const noexcept;

    /**
     * @brief Remove todas as tags do card
     * @details Operaçao atômica que limpa todas as tags associadas.
     */
    void clearTags() noexcept;

    /**
     * @brief Retorna todas as tags do card
     * @return Referência constante para o vetor de tags
     * @details Expõe o container interno apenas para leitura, evitando cópia.
     */
    const std::vector<std::shared_ptr<Tag>>& tags() const noexcept;

    // ============================================================================
    // OPERADORES DE COMPARAÇaO E ORDENAÇaO
    // ============================================================================

    /**
     * @brief Operador de igualdade entre cards
     * @param other Outro card a ser comparado
     * @return true se os cards têm o mesmo ID, false caso contrário
     */
    bool operator==(const Card& other) const noexcept;

    /**
     * @brief Operador de comparaçao para ordenaçao
     * @param other Outro card a ser comparado
     * @return true se este card deve vir antes do outro na ordenaçao
     * @details Ordena por prioridade (maior prioridade primeiro) e,
     *          em caso de empate, por data de criaçao (mais antigo primeiro).
     */
    bool operator<(const Card& other) const noexcept;

    // ============================================================================
    // MÉTODOS DE UTILIDADE INTERNOS
    // ============================================================================

    /**
     * @brief Atualiza o timestamp de modificaçao para o momento atual
     * @details Método interno usado pelos setters para manter updatedAt atualizado.
     */
    void touchUpdated() noexcept;

    // ============================================================================
    // OPERADOR DE SAÍDA
    // ============================================================================

    /**
     * @brief Operador de saída para formataçao de Card
     * @param os Stream de saída onde o card será formatado
     * @param c Referência para o card a ser formatado
     * @return Referência para a stream de saída
     * @details Formata o card em formato legível incluindo todos os atributos
     *          principais e lista de tags. Útil para debug e logs.
     */
    friend std::ostream& operator<<(std::ostream& os, const Card& c);

private:
    std::string id_;                          ///< @brief Identificador único do card
    std::string title_;                       ///< @brief Título da tarefa
    std::optional<std::string> description_;  ///< @brief Descriçao opcional da tarefa
    int priority_ = 0;                       ///< @brief Nível de prioridade (0 = padrao)
    TimePoint createdAt_;                    ///< @brief Momento de criaçao do card
    TimePoint updatedAt_;                    ///< @brief Momento da última atualizaçao
    std::vector<std::shared_ptr<Tag>> tags_; ///< @brief Coleçao de tags associadas

    // ============================================================================
    // NOTA SOBRE CONCORRÊNCIA
    // ============================================================================
    // Esta classe NaO é thread-safe por padrao. Se precisar de acesso concorrente,
    // proteja as mutações com mutex na camada superior (ou adicione mutex aqui).
};

} // namespace domain
} // namespace kanban