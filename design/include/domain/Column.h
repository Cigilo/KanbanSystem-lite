#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace kanban {
namespace domain {

class Card;

// Representa uma coluna (List) dentro de um Board.
class Column {
public:
    using Id = std::string;

    // Construtor explícito — evita conversões implícitas e passa por referência.
    explicit Column(const Id& id, const std::string& name) noexcept;

    // Special members defaulted (shared_ptr semantics allow copy/move por padrão).
    Column(const Column&) = default;
    Column(Column&&) noexcept = default;
    Column& operator=(const Column&) = default;
    Column& operator=(Column&&) noexcept = default;
    ~Column() = default;

    // Accessors
    const Id& id() const noexcept;
    const std::string& name() const noexcept;
    void setName(const std::string& name);

    // Cards management
    // Adiciona um card ao final por padrão. Recebe shared_ptr por const& para evitar cópia do controle de referência.
    void addCard(const std::shared_ptr<Card>& card);

    // Insere o card na posição especificada (0 = início). Se index for maior que size(), insere no final.
    void insertCardAt(std::size_t index, const std::shared_ptr<Card>& card);

    // Remove um card por id e retorna a instância removida; std::nullopt se não existir.
    std::optional<std::shared_ptr<Card>> removeCardById(const Id& cardId);

    // Retorna referência const ao container interno (somente leitura).
    const std::vector<std::shared_ptr<Card>>& cards() const noexcept;

    // find: retorna std::nullopt se não encontrado.
    std::optional<std::shared_ptr<Card>> findCard(const Id& cardId) const noexcept;

    // Utilitários
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    bool hasCard(const Id& cardId) const noexcept;
    void clear() noexcept;

private:
    Id id_;
    std::string name_;
    // Ordem dos cards é preservada; o vector guarda shared_ptr para permitir múltiplas referências.
    std::vector<std::shared_ptr<Card>> cards_;
    // Nota: esta classe NÃO é thread-safe por padrão. Se a aplicação tiver acesso concorrente
    // a coluna (ex.: GUI thread + background), proteja mutações com std::mutex na camada apropriada.
};

} // namespace domain
} // namespace kanban
