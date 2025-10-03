/**
 * @file CLIView.cpp
 * @brief Implementaçao da interface de linha de comando (CLI) para o sistema Kanban
 * @details Esta classe implementa a interface IView para fornecer uma interface
 *          de usuário baseada em texto no terminal, demonstrando que o domínio
 *          funciona independentemente da GUI.
 */

#include "application/CLIView.h"
#include "domain/Board.h"
#include "domain/Column.h"
#include "domain/Card.h"
#include <iomanip>

namespace kanban {
namespace application {

// ============================================================================
// MÉTODOS BÁSICOS DE INTERAÇaO COM O USUÁRIO
// ============================================================================

/**
 * @brief Exibe uma mensagem informativa para o usuário
 * @param msg Mensagem a ser exibida
 * @details Formata a mensagem com um prefixo ">> " para indicar informaçao normal
 *          e garante quebra de linha automática
 */
void CLIView::showMessage(const std::string& msg) const {
    std::cout << ">> " << msg << std::endl;
}

/**
 * @brief Exibe uma mensagem de erro para o usuário
 * @param msg Mensagem de erro a ser exibida
 * @details Utiliza stderr para mensagens de erro e formata com prefixo "ERRO: "
 *          seguindo boas práticas de separaçao de output normal e de erro
 */
void CLIView::showError(const std::string& msg) const {
    std::cerr << "ERRO: " << msg << std::endl;
}

// ============================================================================
// MÉTODOS DE EXIBIÇaO DE DADOS DO DOMÍNIO
// ============================================================================

/**
 * @brief Exibe a lista de boards disponíveis no sistema
 * @param boards Vector contendo shared_ptr para os boards a serem exibidos
 * @details Formata a saída com cabeçalho destacado e mostra para cada board:
 *          - Nome do board
 *          - ID único do board  
 *          - Número de colunas contidas no board
 *          Trata o caso de lista vazia com mensagem apropriada
 */
void CLIView::displayBoards(const std::vector<std::shared_ptr<domain::Board>>& boards) const {
    // Verifica se há boards para exibir
    if (boards.empty()) {
        std::cout << "Nenhum board encontrado." << std::endl;
        return;
    }

    // Cabeçalho formatado para a seçao de boards
    std::cout << "\n*** BOARDS DISPONIVEIS ***" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // Itera sobre cada board e exibe suas informações principais
    for (const auto& board : boards) {
        std::cout << "> " << board->name() 
                  << " (ID: " << board->id() << ")" 
                  << " - " << board->columnCount() << " colunas"
                  << std::endl;
    }
}

/**
 * @brief Exibe a lista de colunas de um board
 * @param columns Vector contendo shared_ptr para as colunas a serem exibidas
 * @details Mostra para cada coluna:
 *          - Nome da coluna
 *          - Quantidade de cards na coluna
 *          Usa formataçao diferenciada para melhor legibilidade
 */
void CLIView::displayColumns(const std::vector<std::shared_ptr<domain::Column>>& columns) const {
    // Verifica se há colunas para exibir
    if (columns.empty()) {
        std::cout << "Nenhuma coluna encontrada." << std::endl;
        return;
    }

    // Cabeçalho formatado para a seçao de colunas
    std::cout << "\n*** COLUNAS ***" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    // Itera sobre cada coluna e exibe nome e quantidade de cards
    for (const auto& column : columns) {
        std::cout << "- " << column->name() 
                  << " (" << column->size() << " cards)"
                  << std::endl;
    }
}

/**
 * @brief Exibe a lista de cards de uma coluna
 * @param cards Vector contendo shared_ptr para os cards a serem exibidos
 * @details Mostra informações detalhadas de cada card:
 *          - Título do card (obrigatório)
 *          - Descriçao (se existir)
 *          - Prioridade numérica
 *          - Lista de tags associadas (se houver)
 *          Formata com indentaçao para indicar hierarquia
 */
void CLIView::displayCards(const std::vector<std::shared_ptr<domain::Card>>& cards) const {
    // Verifica se há cards para exibir
    if (cards.empty()) {
        std::cout << "Nenhum card encontrado." << std::endl;
        return;
    }

    // Itera sobre cada card e exibe todas as informações disponíveis
    for (const auto& card : cards) {
        // Título do card (informaçao obrigatória)
        std::cout << "  * " << card->title();
        
        // Descriçao opcional - só exibe se existir
        if (card->description().has_value()) {
            std::cout << " - " << card->description().value();
        }
        
        // Prioridade do card (sempre existe, valor padrao é 0)
        std::cout << " [Prioridade: " << card->priority() << "]";
        
        // Tags do card - exibe lista se houver tags associadas
        if (!card->tags().empty()) {
            std::cout << " [Tags: ";
            // Itera sobre as tags para criar lista formatada
            for (size_t i = 0; i < card->tags().size(); ++i) {
                std::cout << card->tags()[i]->name();
                // Adiciona vírgula entre tags, exceto para a última
                if (i < card->tags().size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]";
        }
        
        // Quebra de linha ao final de cada card
        std::cout << std::endl;
    }
}

// ============================================================================
// MÉTODOS DE FORMATAÇaO E APRESENTAÇaO
// ============================================================================

/**
 * @brief Exibe o cabeçalho de boas-vindas do sistema
 * @details Mostra informações do sistema incluindo:
 *          - Nome do projeto e etapa
 *          - Data e hora da compilaçao/execuçao
 *          - Usa formataçao com bordas para destaque visual
 */
void CLIView::showWelcome() const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "KANBAN SYSTEM LITE - ETAPA 2 (CLI)" << std::endl;
    std::cout << "Data: " << __DATE__ << " | Hora: " << __TIME__ << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

/**
 * @brief Exibe o cabeçalho indicando início da demonstraçao automática
 * @details Usado para separar visualmente a demonstraçao das outras seções
 *          e fornecer feedback claro do progresso para o usuário
 */
void CLIView::showDemoHeader() const {
    std::cout << "\n*** INICIANDO DEMONSTRACAO AUTOMATICA ***" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

/**
 * @brief Exibe o rodapé indicando conclusao da demonstraçao automática
 * @details Fornece feedback claro de conclusao e separa visualmente
 *          a demonstraçao das mensagens finais do sistema
 */
void CLIView::showDemoFooter() const {
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "*** DEMONSTRACAO CONCLUIDA COM SUCESSO! ***" << std::endl;
}

} // namespace application
} // namespace kanban