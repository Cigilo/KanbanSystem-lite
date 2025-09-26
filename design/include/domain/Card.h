#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <chrono>
#include <ostream>

namespace kanban {
namespace domain {

using TimePoint = std::chrono::system_clock::time_point;
using Clock = std::chrono::system_clock;

// Representa uma etiqueta (tag) aplicável a um card.
class Tag {
public:
    // Construtor explícito — evita conversões implícitas.
    explicit Tag(const std::string& id, const std::string& name) noexcept;

    // Defaulted copy/move
    Tag(const Tag&) = default;
    Tag(Tag&&) noexcept = default;
    Tag& operator=(const Tag&) = default;
    Tag& operator=(Tag&&) noexcept = default;
    ~Tag() = default;

    const std::string& id() const noexcept;
    const std::string& name() const noexcept;
    void setName(const std::string& name);

    // Utilitário para logs/tests
    friend std::ostream& operator<<(std::ostream& os, const Tag& t);

private:
    std::string id_;   // identificador (poderá ser UUID no futuro)
    std::string name_; // nome da etiqueta
};


// Representa um cartão (task) no Kanban.
class Card {
public:
    // Construtor explícito, passando strings por referência para evitar cópia.
    explicit Card(const std::string& id, const std::string& title);

    // Defaulted copy/move (strings e time_point suportam isso).
    Card(const Card&) = default;
    Card(Card&&) noexcept = default;
    Card& operator=(const Card&) = default;
    Card& operator=(Card&&) noexcept = default;
    ~Card() = default;

    // Getters / setters (validação onde aplicável)
    const std::string& id() const noexcept;
    const std::string& title() const noexcept;
    void setTitle(const std::string& title);

    const std::optional<std::string>& description() const noexcept;
    void setDescription(const std::string& desc);

    // prioridade, data de criação/atualização
    void setPriority(int p) noexcept;
    int priority() const noexcept;

    TimePoint createdAt() const noexcept;
    TimePoint updatedAt() const noexcept;

    // tags: adicionar/remover/consultar. Recebe shared_ptr por const& para evitar cópias do controle de referência.
    void addTag(const std::shared_ptr<Tag>& tag);
    bool removeTagById(const std::string& tagId) noexcept;
    bool hasTag(const std::string& tagId) const noexcept;
    void clearTags() noexcept;

    // Retorna referência const ao vetor interno (somente leitura — evita cópia).
    const std::vector<std::shared_ptr<Tag>>& tags() const noexcept;

    // operadores úteis
    bool operator==(const Card& other) const noexcept;
    // Ordenação por prioridade (maior prioridade primeiro). Em caso de empate, ordena por createdAt.
    bool operator<(const Card& other) const noexcept;

    // Atualiza updatedAt para agora (ajuda na implementação de setters).
    void touchUpdated() noexcept;

    // Facilita logs/tests.
    friend std::ostream& operator<<(std::ostream& os, const Card& c);

private:
    std::string id_;
    std::string title_;
    std::optional<std::string> description_;
    int priority_ = 0;
    TimePoint createdAt_;
    TimePoint updatedAt_;
    std::vector<std::shared_ptr<Tag>> tags_;

    // Nota: esta classe NÃO é thread-safe por padrão. Se precisar de acesso concorrente,
    // proteja as mutações com mutex na camada superior (ou adicione mutex aqui).
};

} // namespace domain
} // namespace kanban
