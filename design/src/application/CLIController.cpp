/**
 * @file CLIController.cpp
 * @brief Implementação do controlador CLI (modo interativo)
 * @details
 * Implementa o parsing básico de comandos e delega ações para o
 * KanbanService. Erros são reportados através da CLIView.
 */

#include "application/CLIController.h"
#include <iostream>
#include <sstream>

namespace kanban {
namespace application {

CLIController::CLIController(KanbanService& service, CLIView& view) noexcept
    : service_(service), view_(view) {}

void CLIController::run() {
    view_.showMessage("Modo interativo CLI iniciado. Digite 'help' para ver os comandos. (Digite 'exit' para sair)");
    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break; // EOF
        auto firstNonSpace = line.find_first_not_of(" \t\r\n");
        if (firstNonSpace == std::string::npos) continue;
        std::string cmdline = line.substr(firstNonSpace);
        std::istringstream iss(cmdline);
        std::string cmd;
        iss >> cmd;
        if (cmd == "exit") {
            break;
        } else if (cmd == "help") {
            showHelp();
        } else if (cmd == "create-board") {
            auto pos = cmdline.find_first_not_of(' ', cmd.size());
            std::string name = "";
            if (pos != std::string::npos) name = cmdline.substr(pos);
            handleCreateBoard(name);
        } else if (cmd == "move-card") {
            auto pos = cmdline.find_first_not_of(' ', cmd.size());
            std::string args = "";
            if (pos != std::string::npos) args = cmdline.substr(pos);
            handleMoveCard(args);
        } else if (cmd == "list-boards") {
            handleListBoards();
        } else {
            view_.showError("Comando desconhecido. Digite 'help' para ver os comandos.");
        }
    }
}

void CLIController::handleCreateBoard(const std::string& args) {
    if (args.empty()) {
        view_.showError("Uso: create-board <nome do board>");
        return;
    }
    try {
        std::string id = service_.createBoard(args);
        view_.showMessage("Board criado: '" + args + "' (ID: " + id + ")");
    } catch (const std::exception& e) {
        view_.showError(std::string("Falha ao criar board: ") + e.what());
    }
}

void CLIController::handleMoveCard(const std::string& args) {
    std::istringstream iss(args);
    std::string boardId, cardId, fromColumnId, toColumnId;
    iss >> boardId >> cardId >> fromColumnId >> toColumnId;
    if (boardId.empty() || cardId.empty() || fromColumnId.empty() || toColumnId.empty()) {
        view_.showError("Uso: move-card <boardId> <cardId> <fromColumnId> <toColumnId>");
        return;
    }
    try {
        service_.moveCard(boardId, cardId, fromColumnId, toColumnId);
        view_.showMessage("Card movido com sucesso: " + cardId);
    } catch (const std::exception& e) {
        view_.showError(std::string("Falha ao mover card: ") + e.what());
    }
}

void CLIController::handleListBoards() {
    auto boards = service_.listBoards();
    view_.displayBoards(boards);
}

void CLIController::showHelp() const {
    std::cout << "Comandos disponiveis:\n";
    std::cout << "  create-board <nome do board>    - Cria um novo quadro e imprime o ID\n";
    std::cout << "  move-card <boardId> <cardId> <fromColumnId> <toColumnId> - Move um card entre colunas\n";
    std::cout << "  list-boards                     - Lista todos os boards\n";
    std::cout << "  help                            - Mostra esta ajuda\n";
    std::cout << "  exit                            - Sai do programa\n";
}

} // namespace application
} // namespace kanban
