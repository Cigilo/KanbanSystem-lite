#pragma once

#include "../interfaces/IRepository.h"
#include <string>
#include <fstream>
#include <optional>
#include <stdexcept>

namespace kanban {
namespace persistence {

// Exceção específica para erros de persistência em arquivo.
// Usada para encapsular falhas de I/O, parsing ou escrita.
class FileRepositoryException : public std::runtime_error {
public:
    explicit FileRepositoryException(const std::string& what)
        : std::runtime_error(what) {}
};

// Repositório genérico baseado em arquivo (ex.: JSON/texto).
// Observação: apenas interface aqui; implementação ficará no .cpp (Etapa 2).
template<typename T, typename Id = std::string>
class FileRepository : public interfaces::IRepository<T, Id> {
public:
    explicit FileRepository(const std::string& path);
    ~FileRepository();

    void add(const std::shared_ptr<T>& item) override;
    void remove(const Id& id) override;
    std::vector<std::shared_ptr<T>> getAll() const override;
    std::optional<std::shared_ptr<T>> findById(const Id& id) const override;

private:
    std::string path_; // caminho do arquivo de persistência
    // RAII: streams ou manipuladores de arquivo serão usados no .cpp
};

} // namespace persistence
} // namespace kanban
