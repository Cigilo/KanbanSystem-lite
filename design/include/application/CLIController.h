/**
 * @file CLIController.h
 * @brief Controlador do modo interativo da CLI
 * @details
 * Esta classe encapsula o loop interativo do modo CLI. Ela recebe
 * referências para o serviço da aplicação (KanbanService) e para a view
 * (CLIView) e fornece comandos de alto nível para demonstração e uso.
 *
 * Objetivos principais:
 *  - Separar a lógica de parsing de comandos da função main
 *  - Fornecer uma API simples e facilmente testável para a CLI
 *  - Manter mensagens e tratamento de erro consistente com a view
 */
#pragma once

#include "application/KanbanService.h"
#include "application/CLIView.h"
#include <string>

namespace kanban {
namespace application {

class CLIController {
public:
    /**
     * @brief Constrói um controlador CLI
     * @param service Referência para o KanbanService (camada de aplicação)
     * @param view Referência para a view de linha de comando
     */
    CLIController(KanbanService& service, CLIView& view) noexcept;

    /**
     * @brief Inicia o loop interativo (bloqueante)
     * @details Lê comandos da entrada padrão até o usuário digitar "exit".
     */
    void run();

private:
    KanbanService& service_;
    CLIView& view_;

    // Handlers de comandos (separados para facilitar testes e manutenção)
    void handleCreateBoard(const std::string& args);
    void handleMoveCard(const std::string& args);
    void handleListBoards();
    void showHelp() const;
};

} // namespace application
} // namespace kanban
