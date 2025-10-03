// Este arquivo inclui apenas os headers para garantir que os .h compilam.
#include "../include/domain/ActivityLog.h"
#include "../include/domain/Board.h"
#include "../include/domain/Card.h"
#include "../include/domain/Column.h"
#include "../include/domain/User.h"
#include "../include/interfaces/IFilter.h"
#include "../include/interfaces/IRepository.h"
#include "../include/interfaces/IService.h"
#include "../include/interfaces/IView.h"
#include "../include/persistence/FileRepository.h"
#include "persistence/MemoryRepository.h"
#include <iostream>

// Define TEST_CARD para ativar o teste
#define TEST_CARD

#ifdef TEST_CARD
// Já incluímos "domain/Card.h" acima, nao precisa incluir de novo

void testCard() {
    using namespace kanban::domain;
    
    auto card = std::make_shared<Card>("1", "Implementar Card.cpp");
    card->setDescription("Implementar todos os metodos da classe Card");
    card->setPriority(2);
    
    auto tag1 = std::make_shared<Tag>("bug", "Bug");
    auto tag2 = std::make_shared<Tag>("feature", "Feature");
    
    card->addTag(tag1);
    card->addTag(tag2);
    
    std::cout << *card << std::endl;
}
#endif

#define TEST_COLUMN

#ifdef TEST_COLUMN
void testColumn() {
    using namespace kanban::domain;
    
    auto column = std::make_shared<Column>("col1", "To Do");
    
    auto card1 = std::make_shared<Card>("1", "Fazer compras");
    auto card2 = std::make_shared<Card>("2", "Estudar C++");
    
    column->addCard(card1);
    column->addCard(card2);
    
    std::cout << "Column: " << column->name() << " tem " << column->size() << " cards\n";
    
    // Teste remover um card
    auto removed = column->removeCardById("1");
    if (removed) {
        std::cout << "Removido: " << (*removed)->title() << std::endl;
    }
    
    std::cout << "Agora tem " << column->size() << " cards\n";
}
#endif

#define TEST_ACTIVITY_LOG

#ifdef TEST_ACTIVITY_LOG
void testActivityLog() {
    using namespace kanban::domain;
    
    ActivityLog log;
    
    auto now = Clock::now();
    Activity act1("act1", "Card '1' movido de 'To Do' para 'Doing'", now);
    Activity act2("act2", "Card '2' criado em 'To Do'", now + std::chrono::seconds(10));
    
    log.add(act1);
    log.add(std::move(act2));
    
    std::cout << "ActivityLog tem " << log.size() << " atividades\n";
    
    const Activity* last = log.last();
    if (last) {
        std::cout << "Última atividade: " << *last << std::endl;
    }
    
    // Listar todas as atividades
    for (const auto& activity : log.activities()) {
        std::cout << " - " << activity << std::endl;
    }
}
#endif

// No compile_test.cpp, adicione após o TEST_ACTIVITY_LOG
#define TEST_BOARD

#ifdef TEST_BOARD
void testBoard() {
    using namespace kanban::domain;
    
    // Criar board
    auto board = std::make_shared<Board>("board1", "Projeto Principal");
    auto activityLog = std::make_shared<ActivityLog>();
    board->setActivityLog(activityLog);
    
    // Criar colunas
    auto todoCol = std::make_shared<Column>("todo", "To Do");
    auto doingCol = std::make_shared<Column>("doing", "Doing");
    auto doneCol = std::make_shared<Column>("done", "Done");
    
    board->addColumn(todoCol);
    board->addColumn(doingCol);
    board->addColumn(doneCol);
    
    // Criar cards
    auto card1 = std::make_shared<Card>("card1", "Implementar Board.cpp");
    auto card2 = std::make_shared<Card>("card2", "Testar movimentacao de cards");
    
    todoCol->addCard(card1);
    todoCol->addCard(card2);
    
    std::cout << "=== ESTADO INICIAL ===" << std::endl;
    std::cout << "Board: " << board->name() << " tem " << board->columnCount() << " colunas" << std::endl;
    for (const auto& col : board->columns()) {
        std::cout << " - " << col->name() << ": " << col->size() << " cards" << std::endl;
    }
    
    // Testar movimentacao de card
    std::cout << "\n=== MOVENDO CARD ===" << std::endl;
    try {
        board->moveCard("card1", "todo", "doing");
        std::cout << "Card movido com sucesso!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Erro ao mover card: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== ESTADO FINAL ===" << std::endl;
    for (const auto& col : board->columns()) {
        std::cout << " - " << col->name() << ": " << col->size() << " cards" << std::endl;
    }
    
    std::cout << "\n=== ATIVIDADES REGISTRADAS ===" << std::endl;
    std::cout << "Total de atividades: " << activityLog->size() << std::endl;
    for (const auto& activity : activityLog->activities()) {
        std::cout << " - " << activity.description() << std::endl;
    }
}
#endif

#define TEST_MEMORY_REPOSITORY

#ifdef TEST_MEMORY_REPOSITORY
void testMemoryRepository() {
    using namespace kanban::persistence;
    using namespace kanban::domain;
    
    MemoryRepository<Board> boardRepo;
    
    // Criar e adicionar boards
    auto board1 = std::make_shared<Board>("b1", "Projeto A");
    auto board2 = std::make_shared<Board>("b2", "Projeto B");
    
    boardRepo.add(board1);
    boardRepo.add(board2);
    
    std::cout << "=== TESTE MEMORY REPOSITORY ===" << std::endl;
    std::cout << "Total de boards: " << boardRepo.size() << std::endl;
    
    // Buscar board
    auto found = boardRepo.findById("b1");
    if (found) {
        std::cout << "Board encontrado: " << (*found)->name() << std::endl;
    }
    
    // Listar todos
    auto allBoards = boardRepo.getAll();
    std::cout << "Todos os boards:" << std::endl;
    for (const auto& board : allBoards) {
        std::cout << " - " << board->name() << std::endl;
    }
    
    // Remover
    boardRepo.remove("b1");
    std::cout << "Após remocao: " << boardRepo.size() << " boards" << std::endl;
    
    // Limpar
    boardRepo.clear();
    std::cout << "Após clear: " << boardRepo.size() << " boards" << std::endl;
}
#endif

#include "application/KanbanService.h"

// E adicione o teste
#define TEST_KANBAN_SERVICE

#ifdef TEST_KANBAN_SERVICE
void testKanbanService() {
    using namespace kanban::application;
    
    std::cout << "\n=== TESTE KANBAN SERVICE ===" << std::endl;
    
    KanbanService service;
    
    // Testar criacao de dados de exemplo
    std::cout << "Criando dados de exemplo..." << std::endl;
    service.createSampleData();
    
    // Listar boards
    auto boards = service.listBoards();
    std::cout << "Total de boards: " << boards.size() << std::endl;
    
    for (const auto& board : boards) {
        std::cout << "\nBoard: " << board->name() << " (ID: " << board->id() << ")" << std::endl;
        
        // Listar colunas do board
        auto columns = service.listColumns(board->id());
        std::cout << "Colunas:" << std::endl;
        for (const auto& column : columns) {
            std::cout << "  - " << column->name() << " (" << column->size() << " cards)" << std::endl;
            
            // Listar cards da coluna
            auto cards = service.listCards(column->id());
            for (const auto& card : cards) {
                std::cout << "    * " << card->title() << " [ID: " << card->id() << "]" << std::endl;
            }
        }
        
        // Testar movimentacao de card se houver colunas suficientes
        if (columns.size() >= 2) {
            auto fromColumn = columns[0];  // To Do
            auto toColumn = columns[1];    // Doing
            
            if (fromColumn->size() > 0) {
                auto cards = service.listCards(fromColumn->id());
                auto card = cards[0];
                
                std::cout << "\nMovendo card: '" << card->title() 
                          << "' de '" << fromColumn->name() 
                          << "' para '" << toColumn->name() << "'" << std::endl;
                
                try {
                    service.moveCard(board->id(), card->id(), fromColumn->id(), toColumn->id());
                    std::cout << "Movimento realizado com sucesso!" << std::endl;
                    
                    // Verificar estado atualizado
                    auto updatedFrom = service.listCards(fromColumn->id());
                    auto updatedTo = service.listCards(toColumn->id());
                    
                    std::cout << "Após movimento:" << std::endl;
                    std::cout << "  - " << fromColumn->name() << ": " << updatedFrom.size() << " cards" << std::endl;
                    std::cout << "  - " << toColumn->name() << ": " << updatedTo.size() << " cards" << std::endl;
                    
                } catch (const std::exception& e) {
                    std::cout << "Erro ao mover card: " << e.what() << std::endl;
                }
            }
        }
    }
    
    // Testar criacao de novo board
    std::cout << "\n--- Criando novo board ---" << std::endl;
    std::string newBoardId = service.createBoard("Meu Novo Projeto");
    std::cout << "Novo board criado com ID: " << newBoardId << std::endl;
    
    auto newBoard = service.findBoard(newBoardId);
    if (newBoard) {
        std::cout << "Board encontrado: " << (*newBoard)->name() << std::endl;
    }
}
#endif

int main() {
#ifdef TEST_CARD
    testCard();
#endif

#ifdef TEST_CARD
    testCard();
#endif

#ifdef TEST_COLUMN
    testColumn();
#endif

#ifdef TEST_ACTIVITY_LOG
    testActivityLog();
#endif

#ifdef TEST_BOARD
    testBoard();
#endif

#ifdef TEST_MEMORY_REPOSITORY
    testMemoryRepository();
#endif

#ifdef TEST_KANBAN_SERVICE
    testKanbanService();
#endif

    // Nao criamos objetos complexos aqui — apenas garantimos que os headers sao válidos.
    int x;
    std::cout << "Headers included successfully\n";
    std::cout << "\n digite qualquer numero: \n\n";
    std::cin >> x;
    return 0;
}