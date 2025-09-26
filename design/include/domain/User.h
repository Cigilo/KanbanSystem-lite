#pragma once

#include <string>
#include <ostream>

namespace kanban {
namespace domain {

// Representa um usuário local (não há autenticação remota neste escopo)
class User {
public:
    using Id = std::string;

    // Construtor explícito (passando por referência para evitar cópia)
    explicit User(const Id& id, const std::string& name) noexcept;

    // Defaulted special members (std::string suporta cópia/move)
    User(const User&) = default;
    User(User&&) noexcept = default;
    User& operator=(const User&) = default;
    User& operator=(User&&) noexcept = default;
    ~User() = default;

    // Accessors
    const Id& id() const noexcept;
    const std::string& name() const noexcept;
    void setName(const std::string& name);

    // Operadores úteis para comparações e logs
    bool operator==(const User& other) const noexcept;
    bool operator!=(const User& other) const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const User& u);

private:
    Id id_;
    std::string name_;
    // Nota: esta classe NÃO é thread-safe por padrão. Se houver concorrência entre threads,
    // proteja leituras/escritas a 'name_' com mutex na camada superior.
};

} // namespace domain
} // namespace kanban
