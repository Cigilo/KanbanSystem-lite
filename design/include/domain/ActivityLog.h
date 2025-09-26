#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <utility>
#include <ostream>

namespace kanban {
namespace domain {

using TimePoint = std::chrono::system_clock::time_point;

// Registro de uma atividade (ex.: moved card X from A to B)
class Activity {
public:
    // Construtor explícito — evita conversões implícitas indesejadas.
    explicit Activity(const std::string& id,
                      const std::string& description,
                      TimePoint when) noexcept;

    // Defaulted copy/move (seguro para std::string e TimePoint)
    Activity(const Activity&) = default;
    Activity(Activity&&) noexcept = default;
    Activity& operator=(const Activity&) = default;
    Activity& operator=(Activity&&) noexcept = default;
    ~Activity() = default;

    // Getters — const e noexcept quando apropriado
    const std::string& id() const noexcept;
    const std::string& description() const noexcept;
    TimePoint when() const noexcept;

    // Operador utilitário para facilitar logs/tests
    friend std::ostream& operator<<(std::ostream& os, const Activity& a);

private:
    std::string id_;
    std::string description_;
    TimePoint when_;
};

// Registro de atividades (ActivityLog)
class ActivityLog {
public:
    ActivityLog() = default;
    ~ActivityLog() = default;

    // Adiciona uma activity. Receber por valor permite mover eficientemente:
    //   log.add(Activity(...));
    // ou
    //   Activity a(...); log.add(std::move(a));
    void add(Activity act);

    // Retorna referência const ao vetor interno (evita cópia).
    // Observação: expõe apenas leitura; se preferir evitar exposição, retorne por valor.
    const std::vector<Activity>& activities() const noexcept;

    // Utilitários adicionais
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    // Retorna ponteiro para última atividade ou nullptr se vazio (evita exceção)
    const Activity* last() const noexcept;
    void clear() noexcept;

private:
    std::vector<Activity> activities_;
};

} // namespace domain
} // namespace kanban
