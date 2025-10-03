/**
 * @file FileRepository.h
 * @brief Declaraçao do repositório baseado em arquivo para persistência de dados
 * @details Este header define a classe FileRepository, que implementa a interface
 *          IRepository usando arquivos como meio de persistência. Fornece uma
 *          base para implementações concretas que podem usar formatos como JSON,
 *          XML ou texto para armazenar entidades do sistema Kanban.
 */

#pragma once

#include "../interfaces/IRepository.h"
#include <string>
#include <fstream>
#include <optional>
#include <stdexcept>

namespace kanban {
namespace persistence {

// ============================================================================
// CLASSE FileRepositoryException
// ============================================================================

/**
 * @brief Exceçao específica para erros de persistência em arquivo
 * @details Esta exceçao especializa std::runtime_error para encapsular
 *          falhas específicas de operações de I/O em arquivos, como:
 *          - Erros de abertura/leitura/escrita de arquivos
 *          - Problemas de parsing de formato (JSON, XML, etc.)
 *          - Corrupçao de dados ou formato inválido
 *          - Falhas de serializaçao/desserializaçao
 */
class FileRepositoryException : public std::runtime_error {
public:
    /**
     * @brief Construtor da exceçao FileRepositoryException
     * @param what Mensagem descritiva do erro ocorrido
     */
    explicit FileRepositoryException(const std::string& what)
        : std::runtime_error(what) {}
};

// ============================================================================
// TEMPLATE FileRepository
// ============================================================================

/**
 * @brief Repositório genérico baseado em arquivo para persistência
 * @tparam T Tipo da entidade armazenada no repositório
 * @tparam Id Tipo do identificador da entidade (padrao: std::string)
 * @details Implementa a interface IRepository usando arquivos como
 *          meio de persistência. Esta classe serve como base para
 *          implementações concretas que definirao o formato específico
 *          do arquivo (JSON, XML, texto, binário, etc.).
 * 
 *          Características principais:
 *          - Persistência durável em arquivo
 *          - Implementaçao da interface IRepository
 *          - Uso de RAII para gerenciamento de recursos de arquivo
 *          - Tratamento específico de erros de I/O
 * 
 * @note A implementaçao concreta dos métodos será fornecida no .cpp
 *       (Etapa 2 do desenvolvimento). Este header define apenas a interface.
 */
template<typename T, typename Id = std::string>
class FileRepository : public interfaces::IRepository<T, Id> {
public:
    /**
     * @brief Construtor do FileRepository
     * @param path Caminho do arquivo de persistência
     * @details Inicializa o repositório com o caminho do arquivo
     *          onde os dados serao armazenados. O arquivo será
     *          aberto e processado conforme necessário durante
     *          as operações do repositório.
     */
    explicit FileRepository(const std::string& path);
    
    /**
     * @brief Destrutor do FileRepository
     * @details Garante o fechamento adequado do arquivo e liberaçao
     *          de recursos usando princípios RAII.
     */
    ~FileRepository();

    // ============================================================================
    // IMPLEMENTAÇaO DA INTERFACE IRepository
    // ============================================================================

    /**
     * @brief Adiciona um item ao repositório e persiste no arquivo
     * @param item Shared pointer para o item a ser adicionado
     * @throws FileRepositoryException Se:
     *         - Já existir item com o mesmo ID
     *         - Ocorrer erro de I/O ao escrever no arquivo
     *         - Falha na serializaçao do item
     */
    void add(const std::shared_ptr<T>& item) override;

    /**
     * @brief Remove um item do repositório e atualiza o arquivo
     * @param id ID do item a ser removido
     * @throws FileRepositoryException Se:
     *         - O item nao existir
     *         - Ocorrer erro de I/O ao escrever no arquivo
     *         - Falha na serializaçao dos dados restantes
     */
    void remove(const Id& id) override;

    /**
     * @brief Retorna todos os itens do repositório lidos do arquivo
     * @return Vector contendo shared_ptr para todos os itens
     * @throws FileRepositoryException Se:
     *         - Ocorrer erro de I/O ao ler o arquivo
     *         - Falha na desserializaçao dos dados
     *         - Formato do arquivo estiver corrompido
     */
    std::vector<std::shared_ptr<T>> getAll() const override;

    /**
     * @brief Busca um item específico pelo ID no arquivo
     * @param id ID do item a ser encontrado
     * @return Optional contendo shared_ptr para o item se encontrado,
     *         ou std::nullopt se nao existir
     * @throws FileRepositoryException Se:
     *         - Ocorrer erro de I/O ao ler o arquivo
     *         - Falha na desserializaçao dos dados
     */
    std::optional<std::shared_ptr<T>> findById(const Id& id) const override;

private:
    std::string path_; ///< @brief Caminho do arquivo de persistência
    
    // ============================================================================
    // NOTAS DE IMPLEMENTAÇaO
    // ============================================================================
    // RAII: streams ou manipuladores de arquivo serao usados no .cpp
    // A implementaçao concreta será fornecida na Etapa 2 do desenvolvimento
};

} // namespace persistence
} // namespace kanban