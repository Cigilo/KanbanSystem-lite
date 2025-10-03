#include "application/KanbanService.h"
#include "application/CLIView.h"
#include "domain/Board.h"
#include "domain/Column.h"  
#include "domain/Card.h"
#include "domain/User.h"
#include "domain/ActivityLog.h"
#include "persistence/MemoryRepository.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

// ============================================================================
// DEMONSTRAcaO DE SMART POINTERS E STL
// ============================================================================

void demonstrateSmartPointersAndSTL() {
    std::cout << "\n*** DEMONSTRACAO: SMART POINTERS E STL ***" << std::endl;
    std::cout << "===========================================" << std::endl;
    
    // 1. std::shared_ptr - compartilhamento de ownership
    std::cout << "1. std::shared_ptr - Shared Ownership:" << std::endl;
    {
        auto card = std::make_shared<kanban::domain::Card>("shared_card", "Card com shared_ptr");
        auto card2 = card; // Compartilha ownership
        std::cout << "   Use count: " << card.use_count() << " (2 referencias)" << std::endl;
    }
    
    // 2. std::unique_ptr - ownership exclusivo
    std::cout << "2. std::unique_ptr - Exclusive Ownership:" << std::endl;
    {
        auto uniqueCard = std::make_unique<kanban::domain::Card>("unique_card", "Card com unique_ptr");
        // uniqueCard nao pode ser copiado, apenas movido
        std::cout << "   Ownership exclusivo - nao pode ser copiado" << std::endl;
    }
    
    // 3. STL Containers
    std::cout << "3. STL Containers:" << std::endl;
    {
        std::vector<std::shared_ptr<kanban::domain::Card>> cards;
        cards.push_back(std::make_shared<kanban::domain::Card>("card1", "Primeiro card"));
        cards.push_back(std::make_shared<kanban::domain::Card>("card2", "Segundo card"));
        
        std::cout << "   std::vector com " << cards.size() << " cards" << std::endl;
        
        // std::map no MemoryRepository
        kanban::persistence::MemoryRepository<kanban::domain::Board> repo;
        auto board = std::make_shared<kanban::domain::Board>("map_demo", "Board Demo");
        repo.add(board);
        std::cout << "   std::map no MemoryRepository: " << repo.size() << " item" << std::endl;
    }
    
    // 4. std::optional
    std::cout << "4. std::optional:" << std::endl;
    {
        kanban::persistence::MemoryRepository<kanban::domain::Board> repo;
        auto found = repo.findById("nao_existe");
        if (!found.has_value()) {
            std::cout << "   std::optional: valor nao encontrado (has_value() = false)" << std::endl;
        }
    }
    
    std::cout << "===========================================\n" << std::endl;
}

// ============================================================================
// DEMONSTRAcaO DE TRATAMENTO DE EXCEcOES
// ============================================================================

void demonstrateExceptionHandling(kanban::application::KanbanService& service, 
                                 kanban::application::CLIView& view) {
    std::cout << "*** DEMONSTRACAO: TRATAMENTO DE EXCECOES ***" << std::endl;
    std::cout << "============================================" << std::endl;
    
    // 1. Tentativa de mover card que nao existe
    std::cout << "1. Tentativa de mover card inexistente:" << std::endl;
    try {
        service.moveCard("board_1", "card_inexistente", "todo", "doing");
        view.showError("ERRO: Excecao nao foi lancada!");
    } catch (const std::exception& e) {
        std::cout << "    Excecao capturada: " << e.what() << std::endl;
    }
    
    // 2. Tentativa de acessar board que nao existe
    std::cout << "2. Tentativa de acessar board inexistente:" << std::endl;
    try {
        auto columns = service.listColumns("board_inexistente");
        view.showError("ERRO: Excecao nao foi lancada!");
    } catch (const std::exception& e) {
        std::cout << "    Excecao capturada: " << e.what() << std::endl;
    }
    
    // 3. Tentativa de criar board com ID duplicado (via MemoryRepository)
    std::cout << "3. Tentativa de ID duplicado no MemoryRepository:" << std::endl;
    try {
        kanban::persistence::MemoryRepository<kanban::domain::Board> repo;
        auto board1 = std::make_shared<kanban::domain::Board>("same_id", "Board 1");
        auto board2 = std::make_shared<kanban::domain::Board>("same_id", "Board 2");
        
        repo.add(board1);
        repo.add(board2); // Deve lancar excecao
        
        view.showError("ERRO: Excecao nao foi lancada!");
    } catch (const kanban::persistence::MemoryRepositoryException& e) {
        std::cout << "    MemoryRepositoryException capturada: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "    Excecao std::exception capturada: " << e.what() << std::endl;
    }
    
    std::cout << "============================================\n" << std::endl;
}

// ============================================================================
// DEMONSTRAcaO DETALHADA DO DOMÍNIO
// ============================================================================

void demonstrateDomainDetails() {
    std::cout << "*** DEMONSTRACAO: DETALHES DO DOMINIO ***" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // 1. Board com ActivityLog
    std::cout << "1. Board com ActivityLog:" << std::endl;
    auto board = std::make_shared<kanban::domain::Board>("demo_board", "Board de Demonstracao");
    auto activityLog = std::make_shared<kanban::domain::ActivityLog>();
    board->setActivityLog(activityLog);
    std::cout << "   Board criado: " << board->name() << " (ID: " << board->id() << ")" << std::endl;
    
    // 2. Column management
    std::cout << "2. Gerenciamento de Column:" << std::endl;
    auto column = std::make_shared<kanban::domain::Column>("demo_col", "Coluna Demo");
    board->addColumn(column);
    std::cout << "   Column adicionada: " << column->name() << std::endl;
    std::cout << "   Numero de colunas no board: " << board->columnCount() << std::endl;
    
    // 3. Card com tags e prioridade
    std::cout << "3. Card com tags e prioridade:" << std::endl;
    auto card = std::make_shared<kanban::domain::Card>("demo_card", "Card de Demonstracao");
    card->setDescription("Este card demonstra todas as funcionalidades");
    card->setPriority(2);
    
    auto tag1 = std::make_shared<kanban::domain::Tag>("bug", "Bug");
    auto tag2 = std::make_shared<kanban::domain::Tag>("feature", "Feature");
    card->addTag(tag1);
    card->addTag(tag2);
    
    column->addCard(card);
    std::cout << "   Card criado: " << card->title() << " (Prioridade: " << card->priority() << ")" << std::endl;
    std::cout << "   Tags: " << card->tags().size() << " tags adicionadas" << std::endl;
    
    // 4. ActivityLog em acao
    std::cout << "4. ActivityLog registrando atividades:" << std::endl;
    auto activity = kanban::domain::Activity("act1", "Card criado manualmente", 
                                           std::chrono::system_clock::now());
    activityLog->add(activity);
    std::cout << "   Atividade registrada: " << activity.description() << std::endl;
    std::cout << "   Total de atividades: " << activityLog->size() << std::endl;
    
    std::cout << "=========================================\n" << std::endl;
}

// ============================================================================
// DEMONSTRAcaO COMPLETA DO SISTEMA KANBAN
// ============================================================================

void demonstrateKanbanOperations(kanban::application::KanbanService& service, 
                                kanban::application::CLIView& view) {
    using namespace std::chrono_literals;
    
    view.showDemoHeader();

    // === 1. DEMONSTRAR SMART POINTERS E STL ===
    demonstrateSmartPointersAndSTL();
    std::this_thread::sleep_for(1s);

    // === 2. DEMONSTRAR DETALHES DO DOMÍNIO ===
    demonstrateDomainDetails();
    std::this_thread::sleep_for(1s);

    // === 3. DEMONSTRAR TRATAMENTO DE EXCEcOES ===
    demonstrateExceptionHandling(service, view);
    std::this_thread::sleep_for(1s);

    // === 4. CRIAR DADOS DE EXEMPLO COM O SERVIcO ===
    view.showMessage("4. Criando dados de exemplo com KanbanService...");
    service.createSampleData();
    std::this_thread::sleep_for(1s);

    // === 5. LISTAR E MOSTRAR TODOS OS ELEMENTOS ===
    view.showMessage("5. Listando todos os elementos do sistema...");
    
    // Listar boards
    auto boards = service.listBoards();
    view.displayBoards(boards);
    std::this_thread::sleep_for(1s);

    if (boards.empty()) {
        view.showError("Nenhum board encontrado para demonstracao.");
        return;
    }

    // Usar o primeiro board para demonstracao
    auto mainBoard = boards[0];
    std::string boardId = mainBoard->id();

    // Listar colunas
    view.showMessage("6. Listando colunas do board '" + mainBoard->name() + "'...");
    auto columns = service.listColumns(boardId);
    view.displayColumns(columns);
    std::this_thread::sleep_for(1s);

    // Mostrar cards em cada coluna
    view.showMessage("7. Detalhando cards por coluna...");
    for (const auto& column : columns) {
        std::cout << "\n>> COLUNA: " << column->name() << std::endl;
        auto cards = service.listCards(column->id());
        view.displayCards(cards);
        
        // Demonstrar operadores sobrecarregados nos cards
        if (!cards.empty()) {
            std::cout << "   Operador << implementado: " << *cards[0] << std::endl;
        }
    }
    std::this_thread::sleep_for(2s);

    // === 8. DEMONSTRAR MOVIMENTAcaO DE CARD (CASO DE SUCESSO) ===
    if (columns.size() >= 2) {
        auto fromColumn = columns[0]; // To Do
        auto toColumn = columns[1];   // Doing
        
        auto cardsInTodo = service.listCards(fromColumn->id());
        if (!cardsInTodo.empty()) {
            auto cardToMove = cardsInTodo[0];
            
            view.showMessage("8. Movendo card '" + cardToMove->title() + 
                            "' de '" + fromColumn->name() + 
                            "' para '" + toColumn->name() + "'...");
            
            try {
                service.moveCard(boardId, cardToMove->id(), fromColumn->id(), toColumn->id());
                view.showMessage(" MOVIMENTO REALIZADO COM SUCESSO!");
                
                // Mostrar estado atualizado
                view.showMessage("9. Estado atualizado apos movimento:");
                auto updatedColumns = service.listColumns(boardId);
                for (const auto& col : updatedColumns) {
                    auto cards = service.listCards(col->id());
                    std::cout << "   " << col->name() << ": " << cards.size() << " cards" << std::endl;
                }
                
                // Mostrar atividades registradas
                auto board = service.findBoard(boardId);
                if (board && (*board)->activityLog()) {
                    auto activities = (*board)->activityLog()->activities();
                    if (!activities.empty()) {
                        view.showMessage("10. Ultima atividade registrada:");
                        std::cout << "   " << activities.back().description() << std::endl;
                    }
                }
                
            } catch (const std::exception& e) {
                view.showError("Falha ao mover card: " + std::string(e.what()));
            }
        }
    }
    std::this_thread::sleep_for(1s);

    // === 11. CRIAR NOVO BOARD DINAMICAMENTE ===
    view.showMessage("11. Criando novo board dinamicamente...");
    std::string newBoardId = service.createBoard("Projeto CLI Demo");
    view.showMessage(" NOVO BOARD CRIADO: 'Projeto CLI Demo' (ID: " + newBoardId + ")");
    
    // Adicionar colunas ao novo board
    auto newBoardColumns = service.listColumns(newBoardId);
    if (newBoardColumns.empty()) {
        service.addColumn(newBoardId, "Backlog");
        service.addColumn(newBoardId, "Desenvolvimento");
        service.addColumn(newBoardId, "Testes");
        service.addColumn(newBoardId, "Concluido");
        
        // Atualizar a lista de colunas apos adicionar novas
        newBoardColumns = service.listColumns(newBoardId);
    }
    
    // Adicionar cards apenas se houver colunas
    if (!newBoardColumns.empty()) {
        service.addCard(newBoardId, newBoardColumns[0]->id(), "Implementar CLI");
        service.addCard(newBoardId, newBoardColumns[0]->id(), "Escrever documentacao");
        service.addCard(newBoardId, newBoardColumns[1]->id(), "Testar sistema");
    }
    
    view.showMessage("12. RESUMO DO NOVO BOARD:");
    newBoardColumns = service.listColumns(newBoardId);
    for (const auto& col : newBoardColumns) {
        auto cards = service.listCards(col->id());
        std::cout << "   " << col->name() << ": " << cards.size() << " cards" << std::endl;
    }

    // === 13. DEMONSTRAR CONSULTAS E FILTROS ===
    view.showMessage("13. Consultas e verificacoes finais:");
    
    // Verificar se os boards foram persistidos
    auto allBoards = service.listBoards();
    std::cout << "   Total de boards no sistema: " << allBoards.size() << std::endl;
    
    // Verificar um board específico
    auto foundBoard = service.findBoard(newBoardId);
    if (foundBoard) {
        std::cout << "   Board encontrado: " << (*foundBoard)->name() << std::endl;
    }

    view.showDemoFooter();
}

// ============================================================================
// MAIN - PONTO DE ENTRADA
// ============================================================================

int main() {
    kanban::application::KanbanService service;
    kanban::application::CLIView view;

    // Mostrar cabecalho
    view.showWelcome();

    try {
        std::cout << "\n" << std::string(70, '=') << std::endl;
        std::cout << "INICIANDO DEMONSTRACAO COMPLETA DO SISTEMA KANBAN" << std::endl;
        std::cout << "Esta demonstracao mostrara:" << std::endl;
        std::cout << "1. Uso de Smart Pointers (shared_ptr, unique_ptr)" << std::endl;
        std::cout << "2. Containers STL (vector, map, optional)" << std::endl;  
        std::cout << "3. Tratamento de Excecoes (try/catch, hierarquia)" << std::endl;
        std::cout << "4. Operacoes completas do Kanban" << std::endl;
        std::cout << "5. Arquitetura em camadas funcionando" << std::endl;
        std::cout << std::string(70, '=') << std::endl;
        
        // Executar demonstracao completa
        demonstrateKanbanOperations(service, view);

        // Mensagem final
        std::cout << "\n" << std::string(70, '=') << std::endl;
        view.showMessage(" ETAPA 2 - CLI CONCLUIDA COM SUCESSO!");
        view.showMessage(" TODOS OS REQUISITOS ATENDIDOS:");
        view.showMessage("    Smart Pointers: std::shared_ptr, std::unique_ptr");
        view.showMessage("    STL: std::vector, std::map, std::optional, std::chrono");
        view.showMessage("    Excecoes: Hierarquia propria + std::exception");
        view.showMessage("    Operacoes Kanban: Criar, listar, mover cards");
        view.showMessage("    Arquitetura: Dominio independente da GUI");
        view.showMessage("    CMake: Build funcional e automatizado");
        view.showMessage(" Sistema Kanban totalmente funcional em modo texto");
        view.showMessage(" Pronto para a Etapa 3 (GUI)");
        std::cout << std::string(70, '=') << std::endl;

    } catch (const std::exception& e) {
        view.showError(" Erro durante a demonstracao: " + std::string(e.what()));
        std::cerr << "Detalhes do erro: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        view.showError(" Erro desconhecido durante a demonstracao");
        return 1;
    }

    return 0;
}