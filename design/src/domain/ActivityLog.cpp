/**
 * @file ActivityLog.cpp
 * @brief Implementaçao do sistema de registro de atividades do Kanban
 * @details Este arquivo contém a implementaçao das classes Activity e ActivityLog,
 *          responsáveis pelo rastreamento e histórico de todas as ações realizadas
 *          no sistema Kanban.
 */

#include "domain/ActivityLog.h"
#include <algorithm>

namespace kanban {
namespace domain {

// ============================================================================
// IMPLEMENTAÇaO DA CLASSE Activity
// ============================================================================

/**
 * @brief Construtor da classe Activity
 * @param id Identificador único da atividade
 * @param description Descriçao textual da açao realizada
 * @param when Timestamp do momento em que a atividade ocorreu
 * @details Inicializa uma nova atividade com todos os atributos necessários
 *          para rastreamento completo das ações do sistema.
 */
Activity::Activity(const std::string& id, const std::string& description, TimePoint when) noexcept
    : id_(id), description_(description), when_(when) {}

/**
 * @brief Retorna o ID único da atividade
 * @return String contendo o identificador único da atividade
 * @details Cada atividade possui um ID único que permite sua identificaçao
 *          específica no log de atividades.
 */
const std::string& Activity::id() const noexcept { 
    return id_; 
}

/**
 * @brief Retorna a descriçao da atividade
 * @return String contendo a descriçao textual da açao realizada
 * @details A descriçao explica em linguagem natural qual açao foi
 *          executada no sistema (ex: "Card movido de To Do para Doing").
 */
const std::string& Activity::description() const noexcept { 
    return description_; 
}

/**
 * @brief Retorna o timestamp da atividade
 * @return TimePoint representando o momento exato da ocorrência
 * @details O timestamp é capturado no momento da criaçao da atividade
 *          e permite ordenaçao cronológica precisa.
 */
TimePoint Activity::when() const noexcept { 
    return when_; 
}

/**
 * @brief Sobrecarga do operador de saída para formataçao de Activity
 * @param os Stream de saída onde a atividade será formatada
 * @param a Referência para a atividade a ser formatada
 * @return Referência para a stream de saída
 * @details Formata a atividade em um formato legível incluindo ID,
 *          descriçao e timestamp formatado como string de data/hora.
 */
std::ostream& operator<<(std::ostream& os, const Activity& a) {
    auto time_t = std::chrono::system_clock::to_time_t(a.when());
    os << "Activity{id=" << a.id() 
       << ", description=\"" << a.description() << "\""
       << ", when=" << std::ctime(&time_t) << "}";
    return os;
}

// ============================================================================
// IMPLEMENTAÇaO DA CLASSE ActivityLog
// ============================================================================

/**
 * @brief Adiciona uma nova atividade ao log
 * @param act Atividade a ser adicionada ao histórico
 * @details A atividade é movida para o vetor interno usando move semantics
 *          para melhor performance. Mantém a ordem cronológica de inserçao.
 */
void ActivityLog::add(Activity act) {
    activities_.push_back(std::move(act));
}

/**
 * @brief Retorna todas as atividades do log
 * @return Referência constante para o vetor contendo todas as atividades
 * @details O vetor retornado está na ordem de inserçao (cronológica).
 *          Nao modifica o estado interno do ActivityLog.
 */
const std::vector<Activity>& ActivityLog::activities() const noexcept {
    return activities_;
}

/**
 * @brief Retorna o número total de atividades no log
 * @return Número de atividades armazenadas no log
 * @details Útil para estatísticas e monitoramento do volume de atividades.
 */
std::size_t ActivityLog::size() const noexcept {
    return activities_.size();
}

/**
 * @brief Verifica se o log está vazio
 * @return true se nao há atividades no log, false caso contrário
 * @details Método de conveniência para verificar a existência de atividades
 *          sem precisar verificar o tamanho.
 */
bool ActivityLog::empty() const noexcept {
    return activities_.empty();
}

/**
 * @brief Retorna a última atividade adicionada ao log
 * @return Ponteiro para a última atividade, ou nullptr se o log estiver vazio
 * @details Útil para obter a atividade mais recente sem precisar acessar
 *          todo o vetor de atividades.
 */
const Activity* ActivityLog::last() const noexcept {
    if (activities_.empty()) {
        return nullptr;
    }
    return &activities_.back();
}

/**
 * @brief Limpa todas as atividades do log
 * @details Remove todas as entradas do histórico de atividades.
 *          Operaçao irreversível - use com cuidado.
 */
void ActivityLog::clear() noexcept {
    activities_.clear();
}

} // namespace domain
} // namespace kanban