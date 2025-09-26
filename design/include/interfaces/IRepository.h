#pragma once

#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace kanban {
namespace interfaces {

// Interface genérica de repositório (ex.: para persistência em arquivo/DB).
// Usamos templates para reutilização e forte tipagem.
// Observação: esta interface NÃO é thread-safe por padrão.
template<typename T, typename Id = std::string>
class IRepository {
public:
    virtual ~IRepository() = default;

    // Adiciona um item ao repositório.
    // Deve lançar std::runtime_error (ou derivada) se já existir item com mesmo id.
    virtual void add(const std::shared_ptr<T>& item) = 0;

    // Remove um item pelo id.
    // Pode lançar std::runtime_error (ou derivada) se não existir.
    virtual void remove(const Id& id) = 0;

    // Retorna todos os itens do repositório.
    // Retorna cópia para manter encapsulamento; implementação decide a ordem.
    virtual std::vector<std::shared_ptr<T>> getAll() const = 0;

    // Busca um item por id.
    // Retorna std::nullopt se não existir.
    virtual std::optional<std::shared_ptr<T>> findById(const Id& id) const = 0;
};

} // namespace interfaces
} // namespace kanban
