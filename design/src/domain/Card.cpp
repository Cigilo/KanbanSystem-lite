/**
 * @file Card.cpp
 * @brief Implementaçao das entidades Card e Tag do sistema Kanban
 * @details Este arquivo contém a implementaçao das classes Card e Tag,
 *          que representam os cartões (tarefas) e suas etiquetas
 *          no sistema Kanban.
 */

#include "domain/Card.h"
#include <algorithm>
#include <utility>

namespace kanban {
namespace domain {

// ============================================================================
// IMPLEMENTAÇaO DA CLASSE Tag
// ============================================================================

/**
 * @brief Construtor da classe Tag
 * @param id Identificador único da tag
 * @param name Nome descritivo da tag
 * @details Inicializa uma nova tag com ID e nome fornecidos.
 *          Tags sao usadas para categorizar e organizar cards.
 */
Tag::Tag(const std::string& id, const std::string& name) noexcept
    : id_(id), name_(name) {}

/**
 * @brief Retorna o ID único da tag
 * @return Referência constante para o ID da tag
 * @details O ID é imutável durante todo o ciclo de vida da tag.
 */
const std::string& Tag::id() const noexcept { 
    return id_; 
}

/**
 * @brief Retorna o nome da tag
 * @return Referência constante para o nome da tag
 * @details O nome pode ser alterado através do método setName().
 */
const std::string& Tag::name() const noexcept { 
    return name_; 
}

/**
 * @brief Define um novo nome para a tag
 * @param name Novo nome a ser atribuído à tag
 * @details Permite a alteraçao do nome da tag após sua criaçao.
 */
void Tag::setName(const std::string& name) { 
    name_ = name; 
}

/**
 * @brief Sobrecarga do operador de saída para formataçao de Tag
 * @param os Stream de saída onde a tag será formatada
 * @param t Referência para a tag a ser formatada
 * @return Referência para a stream de saída
 * @details Formata a tag em um formato legível incluindo ID e nome.
 */
std::ostream& operator<<(std::ostream& os, const Tag& t) {
    os << "Tag{id=" << t.id() << ", name=" << t.name() << "}";
    return os;
}

// ============================================================================
// IMPLEMENTAÇaO DA CLASSE Card
// ============================================================================

/**
 * @brief Construtor da classe Card
 * @param id Identificador único do card
 * @param title Título do card (tarefa)
 * @details Inicializa um novo card com ID, título, prioridade padrao (0)
 *          e timestamps de criaçao e atualizaçao. A descriçao é opcional
 *          e inicializada como std::nullopt.
 */
Card::Card(const std::string& id, const std::string& title)
    : id_(id),
      title_(title),
      priority_(0),
      createdAt_(Clock::now()),
      updatedAt_(createdAt_) {
    // description_ é inicializado automaticamente como std::nullopt
    // tags_ é inicializado automaticamente como vector vazio
}

/**
 * @brief Retorna o ID único do card
 * @return Referência constante para o ID do card
 * @details O ID é imutável durante todo o ciclo de vida do card.
 */
const std::string& Card::id() const noexcept { 
    return id_; 
}

/**
 * @brief Retorna o título do card
 * @return Referência constante para o título do card
 * @details O título pode ser alterado através do método setTitle().
 */
const std::string& Card::title() const noexcept { 
    return title_; 
}

/**
 * @brief Define um novo título para o card
 * @param title Novo título a ser atribuído ao card
 * @details Atualiza o título e automaticamente atualiza o timestamp
 *          de modificaçao através do método touchUpdated().
 */
void Card::setTitle(const std::string& title) {
    title_ = title;
    touchUpdated();
}

/**
 * @brief Retorna a descriçao do card
 * @return Referência constante para o optional contendo a descriçao
 * @details A descriçao é opcional, podendo ser std::nullopt se nao definida.
 */
const std::optional<std::string>& Card::description() const noexcept {
    return description_;
}

/**
 * @brief Define a descriçao do card
 * @param desc Nova descriçao a ser atribuída ao card
 * @details Atualiza a descriçao e automaticamente atualiza o timestamp
 *          de modificaçao através do método touchUpdated().
 */
void Card::setDescription(const std::string& desc) {
    description_ = desc;
    touchUpdated();
}

/**
 * @brief Define a prioridade do card
 * @param p Valor numérico da prioridade (maior valor = maior prioridade)
 * @details Prioridades mais altas têm valores numéricos maiores.
 *          Atualiza automaticamente o timestamp de modificaçao.
 */
void Card::setPriority(int p) noexcept {
    priority_ = p;
    touchUpdated();
}

/**
 * @brief Retorna a prioridade do card
 * @return Valor numérico da prioridade
 * @details Valor padrao é 0. Valores maiores indicam maior prioridade.
 */
int Card::priority() const noexcept { 
    return priority_; 
}

/**
 * @brief Retorna o timestamp de criaçao do card
 * @return TimePoint representando o momento de criaçao
 * @details O timestamp é capturado no momento da construçao do card.
 */
TimePoint Card::createdAt() const noexcept { 
    return createdAt_; 
}

/**
 * @brief Retorna o timestamp da última atualizaçao do card
 * @return TimePoint representando o momento da última modificaçao
 * @details Atualizado automaticamente sempre que qualquer campo do card é modificado.
 */
TimePoint Card::updatedAt() const noexcept { 
    return updatedAt_; 
}

// ============================================================================
// GERENCIAMENTO DE TAGS
// ============================================================================

/**
 * @brief Adiciona uma tag ao card
 * @param tag Shared pointer para a tag a ser adicionada
 * @details Verifica se a tag já existe para evitar duplicatas.
 *          Atualiza automaticamente o timestamp de modificaçao.
 */
void Card::addTag(const std::shared_ptr<Tag>& tag) {
    // Verifica se a tag já existe para evitar duplicatas
    if (!hasTag(tag->id())) {
        tags_.push_back(tag);
        touchUpdated();
    }
}

/**
 * @brief Remove uma tag do card pelo ID
 * @param tagId ID da tag a ser removida
 * @return true se a tag foi encontrada e removida, false caso contrário
 * @details Atualiza automaticamente o timestamp de modificaçao em caso de sucesso.
 */
bool Card::removeTagById(const std::string& tagId) noexcept {
    auto it = std::find_if(tags_.begin(), tags_.end(),
        [&tagId](const std::shared_ptr<Tag>& tag) {
            return tag->id() == tagId;
        });
    
    if (it != tags_.end()) {
        tags_.erase(it);
        touchUpdated();
        return true;
    }
    return false;
}

/**
 * @brief Verifica se o card possui uma tag específica
 * @param tagId ID da tag a ser verificada
 * @return true se o card possui a tag, false caso contrário
 * @details Utiliza busca linear no vetor de tags.
 */
bool Card::hasTag(const std::string& tagId) const noexcept {
    return std::any_of(tags_.begin(), tags_.end(),
        [&tagId](const std::shared_ptr<Tag>& tag) {
            return tag->id() == tagId;
        });
}

/**
 * @brief Remove todas as tags do card
 * @details Limpa o vetor de tags e atualiza o timestamp de modificaçao.
 *          Nao faz nada se o card já nao tiver tags.
 */
void Card::clearTags() noexcept {
    if (!tags_.empty()) {
        tags_.clear();
        touchUpdated();
    }
}

/**
 * @brief Retorna todas as tags do card
 * @return Referência constante para o vetor de tags
 * @details As tags sao retornadas na ordem em que foram adicionadas.
 */
const std::vector<std::shared_ptr<Tag>>& Card::tags() const noexcept {
    return tags_;
}

// ============================================================================
// OPERADORES E MÉTODOS DE UTILIDADE
// ============================================================================

/**
 * @brief Operador de igualdade entre cards
 * @param other Outro card a ser comparado
 * @return true se os cards têm o mesmo ID, false caso contrário
 * @details Dois cards sao considerados iguais se possuem o mesmo ID.
 */
bool Card::operator==(const Card& other) const noexcept {
    return id_ == other.id_;
}

/**
 * @brief Operador de comparaçao para ordenaçao de cards
 * @param other Outro card a ser comparado
 * @return true se este card deve vir antes do outro na ordenaçao
 * @details Ordena por prioridade (maior prioridade primeiro) e,
 *          em caso de empate, por data de criaçao (mais antigo primeiro).
 */
bool Card::operator<(const Card& other) const noexcept {
    if (priority_ != other.priority_) {
        return priority_ > other.priority_; // Maior prioridade vem primeiro
    }
    // Em caso de empate, o mais antigo vem primeiro
    return createdAt_ < other.createdAt_;
}

/**
 * @brief Atualiza o timestamp de modificaçao do card
 * @details Método interno chamado automaticamente sempre que
 *          qualquer campo do card é modificado.
 */
void Card::touchUpdated() noexcept {
    updatedAt_ = Clock::now();
}

/**
 * @brief Sobrecarga do operador de saída para formataçao de Card
 * @param os Stream de saída onde o card será formatado
 * @param c Referência para o card a ser formatado
 * @return Referência para a stream de saída
 * @details Formata o card em um formato legível incluindo todos os
 *          seus atributos principais e lista de tags.
 */
std::ostream& operator<<(std::ostream& os, const Card& c) {
    os << "Card{id=" << c.id() 
       << ", title=" << c.title()
       << ", priority=" << c.priority()
       << ", description=";
    
    if (c.description().has_value()) {
        os << "\"" << c.description().value() << "\"";
    } else {
        os << "null";
    }
    
    os << ", tags=[";
    for (size_t i = 0; i < c.tags().size(); ++i) {
        os << c.tags()[i]->name();
        if (i < c.tags().size() - 1) {
            os << ", ";
        }
    }
    os << "], created_at=" 
       << std::chrono::duration_cast<std::chrono::seconds>(c.createdAt().time_since_epoch()).count()
       << "s}";
    
    return os;
}

} // namespace domain
} // namespace kanban