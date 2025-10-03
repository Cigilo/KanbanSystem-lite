/**
 * @file IView.h
 * @brief Declaraçao da interface IView para a camada de apresentaçao do sistema Kanban
 * @details Este header define a interface mínima que as camadas de apresentaçao
 *          (CLI, GUI, Web, etc.) devem implementar para exibir informações do
 *          sistema Kanban aos usuários. Segue o padrao MVP (Model-View-Presenter)
 *          ou MVC (Model-View-Controller) para separaçao clara de responsabilidades.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace kanban {
namespace domain {
    // Forward declarations para reduzir acoplamento
    class Board;
    class Column;
    class Card;
}

namespace interfaces {

// ============================================================================
// INTERFACE IView
// ============================================================================

/**
 * @brief Interface mínima para camadas de apresentaçao do sistema Kanban
 * @details A interface IView define o contrato que todas as implementações
 *          de visualizaçao (CLI, GUI, Web UI) devem seguir para exibir
 *          informações do sistema Kanban aos usuários. Esta interface
 *          promove:
 * 
 *          - Separaçao clara entre lógica de apresentaçao e lógica de negócio
 *          - Capacidade de trocar a interface de usuário sem alterar o domínio
 *          - Testabilidade da camada de apresentaçao através de mocks
 *          - Manutençao simplificada através de contrato bem definido
 * 
 *          Padrões de design aplicados:
 *          - Observer: a view é notificada sobre mudanças no modelo
 *          - Strategy: diferentes implementações de view para diferentes UI
 *          - Dependency Inversion: a camada de aplicaçao depende da abstraçao IView
 */
class IView {
public:
    /**
     * @brief Destrutor virtual padrao
     * @details Garante a correta destruiçao de implementações concretas
     *          (CLIView, GUIView, etc.) através de ponteiros da interface base.
     */
    virtual ~IView() = default;

    // ============================================================================
    // MÉTODOS DE EXIBIÇaO DE MENSAGENS
    // ============================================================================

    /**
     * @brief Exibe uma mensagem genérica de informaçao ao usuário
     * @param msg Mensagem a ser exibida
     * @details Utilizado para comunicar informações gerais, confirmações
     *          de operações bem-sucedidas e notificações do sistema.
     *          Exemplos:
     *          - "Board criado com sucesso"
     *          - "Card movido para 'Doing'"
     *          - "Dados de exemplo carregados"
     */
    virtual void showMessage(const std::string& msg) const = 0;

    /**
     * @brief Exibe uma mensagem de erro ao usuário
     * @param msg Mensagem de erro a ser exibida
     * @details Utilizado para comunicar condições de erro, validações
     *          falhas e operações que nao puderam ser completadas.
     *          Exemplos:
     *          - "Board nao encontrado"
     *          - "Coluna de origem nao existe"
     *          - "Erro ao salvar dados"
     */
    virtual void showError(const std::string& msg) const = 0;

    // ============================================================================
    // MÉTODOS DE ATUALIZAÇaO DE VISUALIZAÇaO
    // ============================================================================

    /**
     * @brief Atualiza a visualizaçao da lista de boards
     * @param boards Vector contendo shared_ptr para todos os boards a serem exibidos
     * @details Chamado quando a lista de boards precisa ser atualizada na UI.
     *          A implementaçao deve renderizar os boards de forma apropriada
     *          para a tecnologia de UI (lista textual, grid gráfico, etc.).
     */
    virtual void displayBoards(const std::vector<std::shared_ptr<domain::Board>>& boards) const = 0;

    /**
     * @brief Atualiza a visualizaçao da lista de colunas
     * @param columns Vector contendo shared_ptr para todas as colunas a serem exibidas
     * @details Chamado quando a lista de colunas de um board precisa ser atualizada.
     *          A implementaçao deve renderizar as colunas de forma que reflita
     *          o fluxo de trabalho Kanban (ex.: colunas lado a lado).
     */
    virtual void displayColumns(const std::vector<std::shared_ptr<domain::Column>>& columns) const = 0;

    /**
     * @brief Atualiza a visualizaçao da lista de cards
     * @param cards Vector contendo shared_ptr para todos os cards a serem exibidos
     * @details Chamado quando a lista de cards de uma coluna precisa ser atualizada.
     *          A implementaçao deve renderizar os cards de forma clara, mostrando
     *          informações relevantes como título, prioridade e tags.
     */
    virtual void displayCards(const std::vector<std::shared_ptr<domain::Card>>& cards) const = 0;

    // ============================================================================
    // NOTAS DE IMPLEMENTAÇaO
    // ============================================================================
    // Observaçao: Esta interface foi mantida mínima intencionalmente para
    // permitir máxima flexibilidade nas implementações de UI. Implementações
    // concretas podem adicionar métodos auxiliares específicos da tecnologia
    // (ex.: showWelcome(), showDemoHeader() na CLIView) sem violar o contrato.
};

} // namespace interfaces
} // namespace kanban