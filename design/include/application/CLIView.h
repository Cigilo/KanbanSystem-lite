/**
 * @file CLIView.h
 * @brief Implementaço da interface de linha de comando (CLI) do sistema Kanban
 * @details Esta classe implementa a interface IView para fornecer uma interface
 *          de usuário via terminal, exibindo mensagens, erros e listas de entidades
 *          do sistema de forma formatada e amigável.
 */

#pragma once

#include "../interfaces/IView.h"
#include <iostream>
#include <memory>

namespace kanban {
namespace application {

/**
 * @brief View de linha de comando para o sistema Kanban
 * @details Implementa a interface IView para renderizar a saída do sistema
 *          no terminal. Fornece métodos para exibir mensagens, erros e listas
 *          de boards, columns e cards de forma organizada e legível.
 * 
 *          A CLIView é responsável por toda a apresentaço visual do sistema
 *          no modo console, incluindo formataço, cores (se suportadas) e
 *          organizaço hierárquica dos dados.
 */
class CLIView : public interfaces::IView {
public:
    /**
     * @brief Construtor padro da CLIView
     * @details Inicializa a view de linha de comando com configurações padro.
     *          No requer parâmetros especiais para inicializaço.
     */
    CLIView() = default;

    /**
     * @brief Destrutor padro da CLIView
     * @details Garante a limpeza adequada dos recursos da view.
     */
    ~CLIView() = default;

    // ============================================================================
    // IMPLEMENTAÇO DA INTERFACE IView
    // ============================================================================

    /**
     * @brief Exibe uma mensagem informativa para o usuário
     * @param msg Mensagem a ser exibida
     * @details Formata e exibe mensagens de informaço de forma clara e destacada.
     *          Ideal para confirmacões de operacões bem-sucedidas e informacões gerais.
     */
    void showMessage(const std::string& msg) const override;

    /**
     * @brief Exibe uma mensagem de erro para o usuário
     * @param msg Mensagem de erro a ser exibida
     * @details Exibe erros de forma destacada, idealmente com formataço que
     *          chame a atenço do usuário para problemas que requerem aço.
     */
    void showError(const std::string& msg) const override;

    /**
     * @brief Exibe uma lista de boards do sistema
     * @param boards Vector contendo shared_ptr para todos os boards a serem exibidos
     * @details Renderiza uma lista formatada de boards, mostrando ID, nome
     *          e estatísticas básicas como número de colunas para cada board.
     */
    void displayBoards(const std::vector<std::shared_ptr<domain::Board>>& boards) const override;

    /**
     * @brief Exibe uma lista de columns de um board
     * @param columns Vector contendo shared_ptr para todas as columns a serem exibidas
     * @details Renderiza columns em formato de lista ou tabela, mostrando
     *          nome da coluna e quantidade de cards contidos em cada uma.
     */
    void displayColumns(const std::vector<std::shared_ptr<domain::Column>>& columns) const override;

    /**
     * @brief Exibe uma lista de cards de uma column
     * @param cards Vector contendo shared_ptr para todos os cards a serem exibidos
     * @details Apresenta cards de forma organizada, mostrando título, prioridade,
     *          descriço (se disponível) e tags associadas a cada card.
     */
    void displayCards(const std::vector<std::shared_ptr<domain::Card>>& cards) const override;

    // ============================================================================
    // MÉTODOS ADICIONAIS PARA MELHOR UX DO CLI
    // ============================================================================

    /**
     * @brief Exibe a mensagem de boas-vindas do sistema
     * @details Mostra um cabeçalho atraente com o nome do sistema, verso
     *          e informações básicas de uso ao iniciar a aplicaço.
     */
    void showWelcome() const;

    /**
     * @brief Exibe o cabeçalho da demonstraço
     * @details Renderiza um cabeçalho especial para modos de demonstraço
     *          ou dados de exemplo, destacando que se trata de conteúdo
     *          ilustrativo do sistema.
     */
    void showDemoHeader() const;

    /**
     * @brief Exibe o rodapé da demonstraço
     * @details Mostra informações finais após a exibiço de dados de
     *          demonstraço, como instruções para continuar usando o sistema.
     */
    void showDemoFooter() const;
};

} // namespace application
} // namespace kanban