#include "gui/MainWindow.h"
#include "gui/ColumnWidget.h"
#include "gui/CardWidget.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QDebug>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QDateTime>

namespace kanban {
namespace gui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), 
      service_(std::make_unique<application::KanbanService>()) {
    
    setupUI();
    setupConnections();
    setupMenuBar();
    loadSampleData();
    refreshBoards();
    
    statusLabel_->setText("✅ Sistema Kanban carregado com sucesso");
}

MainWindow::~MainWindow() {
    clearBoardTab();
}

void MainWindow::setupUI() {
    // Configuração da janela principal
    setWindowTitle("Kanban System Lite - v3.0");
    setMinimumSize(1200, 800);
    
    // Widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Layout principal
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // ===== PAINEL LATERAL ESQUERDO =====
    QWidget *leftPanel = new QWidget;
    leftPanel->setMaximumWidth(300);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

     // ===== PAINEL DE FILTROS =====
    setupFilterPanel();
    
    // Adicione o painel de filtros ao layout principal
    mainLayout->insertWidget(1, filterGroup_); // Entre leftPanel e boardsTabWidget_

    // Título
    QLabel *titleLabel = new QLabel("🎯 Kanban Boards");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    
    // Lista de boards
    boardsListWidget_ = new QListWidget;
    boardsListWidget_->setStyleSheet("QListWidget { border: 1px solid #ccc; border-radius: 5px; }");
    
    // Criar novo board
    QLabel *newBoardLabel = new QLabel("Criar Novo Board:");
    boardNameLineEdit_ = new QLineEdit;
    boardNameLineEdit_->setPlaceholderText("Nome do board...");
    createBoardButton_ = new QPushButton("➕ Criar Board");
    createBoardButton_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 8px; border-radius: 4px; }");
    
    // Botão para criar coluna
    QLabel *newColumnLabel = new QLabel("Gerenciar Colunas:");
    createColumnButton_ = new QPushButton("➕ Adicionar Coluna");
    createColumnButton_->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 8px; border-radius: 4px; margin-top: 5px; }");
    
    // Adicionar ao layout esquerdo
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(boardsListWidget_);
    leftLayout->addWidget(newBoardLabel);
    leftLayout->addWidget(boardNameLineEdit_);
    leftLayout->addWidget(createBoardButton_);
    leftLayout->addWidget(newColumnLabel);
    leftLayout->addWidget(createColumnButton_);
    leftLayout->addStretch();

    // ===== ÁREA PRINCIPAL CENTRAL =====
    boardsTabWidget_ = new QTabWidget;
    boardsTabWidget_->setTabPosition(QTabWidget::North);
    boardsTabWidget_->setMovable(true);

    // ===== PAINEL LATERAL DIREITO (ATIVIDADES) =====
    QWidget *rightPanel = new QWidget;
    rightPanel->setMaximumWidth(350);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // Título do painel de atividades
    QLabel *activityTitle = new QLabel("📝 Histórico de Atividades");
    activityTitle->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    
    // Botão para atualizar atividades
    refreshActivityLogButton_ = new QPushButton("🔄 Atualizar Histórico");
    refreshActivityLogButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1976D2;"
        "}"
    );
    
    // Área de texto para o histórico
    activityLogTextEdit_ = new QTextEdit;
    activityLogTextEdit_->setReadOnly(true);
    activityLogTextEdit_->setStyleSheet(
        "QTextEdit {"
        "   background-color: #f8f9fa;"
        "   color: #212529;"
        "   border: 1px solid #dee2e6;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "   font-size: 12px;"
        "}"
    );
    // Garantir cor do texto via palette (em caso de stylesheet global conflitando)
    {
        QPalette pal = activityLogTextEdit_->palette();
        pal.setColor(QPalette::Text, QColor("#212529"));
        activityLogTextEdit_->setPalette(pal);
    }
    activityLogTextEdit_->setPlaceholderText("Nenhuma atividade registrada ainda...");
    
    // Estatísticas rápidas
    QLabel *statsTitle = new QLabel("📊 Estatísticas Rápidas");
    statsTitle->setStyleSheet("font-size: 14px; font-weight: bold; margin-top: 15px; margin-bottom: 5px;");
    
    statsLabel_ = new QLabel("Selecione um board para ver estatísticas...");
    statsLabel_->setStyleSheet("font-size: 11px; color: #666; margin: 5px;");
    statsLabel_->setWordWrap(true);
    
    // Adicionar ao layout direito
    rightLayout->addWidget(activityTitle);
    rightLayout->addWidget(refreshActivityLogButton_);
    rightLayout->addWidget(activityLogTextEdit_);
    rightLayout->addWidget(statsTitle);
    rightLayout->addWidget(statsLabel_);
    rightLayout->addStretch();

    // ===== ADICIONAR TODOS OS PAINÉIS AO LAYOUT PRINCIPAL =====
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(boardsTabWidget_, 1);
    mainLayout->addWidget(rightPanel);

    // Barra de status
    statusLabel_ = new QLabel("Pronto - Sistema Kanban carregado com sucesso");
    statusBar()->addWidget(statusLabel_);
    
    // Configurar tamanhos relativos
    leftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    rightPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    boardsTabWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::setupFilterPanel() {
    filterGroup_ = new QGroupBox("🎯 Filtros Avançados");
    filterGroup_->setMaximumWidth(280);
    QVBoxLayout *filterLayout = new QVBoxLayout(filterGroup_);
    
    // Filtro por Tags
    QLabel *tagFilterLabel = new QLabel("Filtrar por Tag:");
    tagFilterCombo_ = new QComboBox;
    tagFilterCombo_->setEditable(true);
    tagFilterCombo_->setPlaceholderText("Selecione ou digite uma tag...");
    tagFilterCombo_->addItem("Todas as tags", "");
    
    // Filtro por Prioridade
    QLabel *priorityFilterLabel = new QLabel("Filtrar por Prioridade:");
    highPriorityFilter_ = new QCheckBox("🔴 Alta Prioridade");
    mediumPriorityFilter_ = new QCheckBox("🟡 Média Prioridade");
    lowPriorityFilter_ = new QCheckBox("⚪ Baixa Prioridade");
    
    // Botões de ação
    applyFilterButton_ = new QPushButton("✅ Aplicar Filtros");
    clearFilterButton_ = new QPushButton("🧹 Limpar Filtros");
    
    applyFilterButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
    );
    
    clearFilterButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #ff9800;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f57c00;"
        "}"
    );
    
    // Layout
    filterLayout->addWidget(tagFilterLabel);
    filterLayout->addWidget(tagFilterCombo_);
    filterLayout->addWidget(priorityFilterLabel);
    filterLayout->addWidget(highPriorityFilter_);
    filterLayout->addWidget(mediumPriorityFilter_);
    filterLayout->addWidget(lowPriorityFilter_);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(applyFilterButton_);
    filterLayout->addWidget(clearFilterButton_);
    filterLayout->addStretch();
    
    // Conexões
    connect(applyFilterButton_, &QPushButton::clicked, this, &MainWindow::applyFilters);
    connect(clearFilterButton_, &QPushButton::clicked, this, &MainWindow::clearFilters);
}

// NOVO MÉTODO: Aplicar filtros
void MainWindow::applyFilters() {
    // Obter filtro de tag
    currentTagFilter_ = tagFilterCombo_->currentText().trimmed();
    if (currentTagFilter_ == "Todas as tags") {
        currentTagFilter_.clear();
    }
    
    // Obter filtros de prioridade
    currentPriorityFilters_.clear();
    if (highPriorityFilter_->isChecked()) currentPriorityFilters_.insert(2);
    if (mediumPriorityFilter_->isChecked()) currentPriorityFilters_.insert(1);
    if (lowPriorityFilter_->isChecked()) currentPriorityFilters_.insert(0);
    
    // Se nenhum filtro de prioridade selecionado, mostrar todas
    if (currentPriorityFilters_.empty()) {
        currentPriorityFilters_ = {0, 1, 2};
    }
    
    // Aplicar filtros
    refreshCurrentBoard(false);
    
    statusLabel_->setText("✅ Filtros aplicados");
}

// NOVO MÉTODO: Limpar filtros
void MainWindow::clearFilters() {
    currentTagFilter_.clear();
    currentPriorityFilters_ = {0, 1, 2};
    
    tagFilterCombo_->setCurrentIndex(0);
    highPriorityFilter_->setChecked(false);
    mediumPriorityFilter_->setChecked(false);
    lowPriorityFilter_->setChecked(false);
    
    refreshCurrentBoard(false);
    statusLabel_->setText("🧹 Filtros limpos");
}

// NOVO MÉTODO: Verificar se card corresponde aos filtros
bool MainWindow::cardMatchesFilter(std::shared_ptr<domain::Card> card) {
    if (!card) return true; // Se não há card, não filtrar
    
    // Se não há filtros ativos, mostrar tudo
    if (currentTagFilter_.isEmpty() && currentPriorityFilters_.empty()) {
        return true;
    }
    
    // Verificar filtro de prioridade
    if (!currentPriorityFilters_.empty()) {
        if (currentPriorityFilters_.find(card->priority()) == currentPriorityFilters_.end()) {
            return false;
        }
    }
    
    // Verificar filtro de tag
    if (!currentTagFilter_.isEmpty()) {
        bool hasMatchingTag = false;
        for (const auto& tag : card->tags()) {
            if (QString::fromStdString(tag->name()).contains(currentTagFilter_, Qt::CaseInsensitive)) {
                hasMatchingTag = true;
                break;
            }
        }
        if (!hasMatchingTag) {
            return false;
        }
    }
    
    return true;
}

// NOVO MÉTODO: Atualizar lista de tags para filtro
void MainWindow::refreshFilterTags() {
    if (currentBoardId_.empty()) return;
    
    QString currentText = tagFilterCombo_->currentText();
    tagFilterCombo_->clear();
    tagFilterCombo_->addItem("Todas as tags", "");
    
    try {
        auto boardOpt = service_->findBoard(currentBoardId_);
        if (!boardOpt) return;
        
        std::set<QString> uniqueTags;
        auto board = *boardOpt;
        
        // Coletar todas as tags únicas do board
        for (const auto& column : board->columns()) {
            for (const auto& card : column->cards()) {
                for (const auto& tag : card->tags()) {
                    uniqueTags.insert(QString::fromStdString(tag->name()));
                }
            }
        }
        
        // Adicionar tags ao combobox
        for (const auto& tag : uniqueTags) {
            tagFilterCombo_->addItem(tag, tag);
        }
        
        // Restaurar seleção anterior se possível
        int index = tagFilterCombo_->findText(currentText);
        if (index >= 0) {
            tagFilterCombo_->setCurrentIndex(index);
        }
        
    } catch (const std::exception& e) {
        qDebug() << "Erro ao atualizar tags de filtro:" << e.what();
    }
}

void MainWindow::setupMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Menu Arquivo
    QMenu *fileMenu = menuBar->addMenu("&Arquivo");
    
    QAction *newBoardAction = new QAction("&Novo Board", this);
    newBoardAction->setShortcut(QKeySequence::New);
    connect(newBoardAction, &QAction::triggered, this, &MainWindow::createNewBoard);
    fileMenu->addAction(newBoardAction);
    
    QAction *newColumnAction = new QAction("Nova &Coluna", this);
    connect(newColumnAction, &QAction::triggered, this, &MainWindow::createNewColumn);
    fileMenu->addAction(newColumnAction);
    
    fileMenu->addSeparator();
    
    QAction *quitAction = new QAction("&Sair", this);
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(quitAction);

    // Menu Ajuda
    QMenu *helpMenu = menuBar->addMenu("&Ajuda");
    QAction *aboutAction = new QAction("&Sobre", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpMenu->addAction(aboutAction);
}

void MainWindow::setupConnections() {
    connect(createBoardButton_, &QPushButton::clicked, this, &MainWindow::createNewBoard);
    connect(createColumnButton_, &QPushButton::clicked, this, &MainWindow::createNewColumn);
    connect(boardsListWidget_, &QListWidget::currentRowChanged, this, &MainWindow::onBoardSelected);
    connect(boardNameLineEdit_, &QLineEdit::returnPressed, this, &MainWindow::createNewBoard);
    connect(refreshActivityLogButton_, &QPushButton::clicked, this, &MainWindow::refreshActivityLog);
}

void MainWindow::loadSampleData() {
    try {
        service_->createSampleData();
        statusLabel_->setText("📊 Dados de exemplo carregados");
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao carregar dados: " + QString(e.what()));
    }
}

void MainWindow::createNewBoard() {
    QString boardName = boardNameLineEdit_->text().trimmed();
    if (boardName.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Por favor, insira um nome para o board.");
        return;
    }

    try {
        std::string boardId = service_->createBoard(boardName.toStdString());
        boardNameLineEdit_->clear();
        refreshBoards();
        
        // CORREÇÃO: Encontra o índice do novo board e seleciona
        auto boards = service_->listBoards();
        int newIndex = -1;
        for (int i = 0; i < static_cast<int>(boards.size()); ++i) {
            if (boards[i]->id() == boardId) {
                newIndex = i;
                break;
            }
        }
        
        if (newIndex != -1) {
            // CORREÇÃO: Seleciona o novo board na lista
            boardsListWidget_->setCurrentRow(newIndex);
            
            // CORREÇÃO: Força a atualização imediata do board selecionado
            currentBoardId_ = boardId;
            refreshCurrentBoard(true);
            
            statusLabel_->setText("✅ Board criado: " + boardName);
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro", QString("Erro ao criar board: ") + e.what());
        statusLabel_->setText("❌ Erro ao criar board");
    }
}

void MainWindow::createNewColumn() {
    if (currentBoardId_.empty()) {
        QMessageBox::warning(this, "Aviso", "Selecione um board primeiro.");
        return;
    }

    bool ok;
    QString columnName = QInputDialog::getText(this, 
                                              "Nova Coluna", 
                                              "Nome da coluna:", 
                                              QLineEdit::Normal, 
                                              "", 
                                              &ok);
    
    if (ok && !columnName.isEmpty()) {
        try {
            std::string columnId = service_->addColumn(currentBoardId_, columnName.toStdString());
            
            // CORREÇÃO: Força recriação completa da visualização
            refreshCurrentBoard(true);
            
            statusLabel_->setText("✅ Coluna criada: " + columnName);
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Erro", QString("Erro ao criar coluna: ") + e.what());
            statusLabel_->setText("❌ Erro ao criar coluna");
        }
    }
}

void MainWindow::refreshBoards() {
    boardsListWidget_->clear();
    auto boards = service_->listBoards();
    
    for (const auto& board : boards) {
        QString boardText = QString("📋 %1\n   🗂️ %2 colunas")
                           .arg(QString::fromStdString(board->name()))
                           .arg(board->columnCount());
        boardsListWidget_->addItem(boardText);
    }
}

void MainWindow::onBoardSelected(int index) {
    if (index < 0) {
        return;
    }

    try {
        auto boards = service_->listBoards();
        if (index < static_cast<int>(boards.size())) {
            auto board = boards[index];
            std::string newBoardId = board->id();
            
            // CORREÇÃO: Só atualiza se for um board diferente
            if (currentBoardId_ != newBoardId) {
                currentBoardId_ = newBoardId;
                refreshCurrentBoard(true);
            }
            
            // ADICIONE ESTA PARTE: Conectar os sinais columnMoved dos ColumnWidgets existentes
            if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
                auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
                for (auto& pair : currentColumnWidgets) {
                    ColumnWidget* columnWidget = pair.second;
                    // Conectar o sinal columnMoved se ainda não estiver conectado
                    connect(columnWidget, &ColumnWidget::columnMoved,
                            this, &MainWindow::onColumnMoved, Qt::UniqueConnection);
                    connect(columnWidget, &ColumnWidget::cardReordered, this, &MainWindow::onCardReordered);
                }
            }
            
            QString boardName = QString::fromStdString(board->name());
            statusLabel_->setText("📊 Visualizando board: " + boardName);
            
            // Atualizar estatísticas
            updateStatistics();
        }
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao carregar board: " + QString(e.what()));
    }
}

void MainWindow::onCardReordered(const QString& columnId, const QString& cardId, int newIndex) {
    try {
        service_->moveCardWithinColumn(currentBoardId_, 
                                      columnId.toStdString(), 
                                      cardId.toStdString(), 
                                      static_cast<std::size_t>(newIndex));
        
        // Atualizar apenas a coluna específica (performance)
        if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
            auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
            auto it = currentColumnWidgets.find(columnId.toStdString());
                if (it != currentColumnWidgets.end()) {
                    it->second->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); });
                }
        }
        
        statusLabel_->setText("🔄 Card reordenado na coluna");
        
        // Atualizar estatísticas
        updateStatistics();
        
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao reordenar card: " + QString(e.what()));
        QMessageBox::warning(this, "Erro de Reordenação", 
                            "Não foi possível reordenar o card: " + QString(e.what()));
    }
}

void MainWindow::onColumnMoved(const QString& fromColumnId, const QString& toColumnId) {
    if (fromColumnId == toColumnId) return;
    
    try {
        // Lógica para reordenar colunas no serviço
        // Você precisará adicionar um método no KanbanService para isso
        service_->moveColumn(currentBoardId_, fromColumnId.toStdString(), toColumnId.toStdString());
        refreshCurrentBoard(true);
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao mover coluna: " + QString(e.what()));
    }
}

void MainWindow::onCardMoved(const QString& cardId, const QString& fromColumnId, const QString& toColumnId) {
    try {
        if (currentBoardId_.empty() || toColumnId.isEmpty()) {
            statusLabel_->setText("❌ Selecione um board e uma coluna de destino");
            return;
        }
        
        std::string fromColumn = fromColumnId.toStdString();
        
        // CORREÇÃO: Se a coluna de origem não foi fornecida, encontrar automaticamente
        if (fromColumn.empty()) {
            auto columns = service_->listColumns(currentBoardId_);
            for (const auto& column : columns) {
                auto cardOpt = column->findCard(cardId.toStdString());
                if (cardOpt.has_value()) {
                    fromColumn = column->id();
                    break;
                }
            }
        }
        
        // CORREÇÃO: Verificar se o card já está na coluna de destino
        if (!fromColumn.empty()) {
            auto toColumnOpt = service_->findBoard(currentBoardId_);
            if (toColumnOpt.has_value()) {
                auto board = *toColumnOpt;
                auto toColumn = board->findColumn(toColumnId.toStdString());
                if (toColumn.has_value()) {
                    auto existingCard = (*toColumn)->findCard(cardId.toStdString());
                    if (existingCard.has_value()) {
                        // Card já está na coluna de destino
                        statusLabel_->setText("ℹ️ Card já está na coluna de destino");
                        return;
                    }
                }
            }
        }
        
        if (!fromColumn.empty() && !toColumnId.isEmpty()) {
            service_->moveCard(currentBoardId_, 
                              cardId.toStdString(), 
                              fromColumn, 
                              toColumnId.toStdString());
            
            // CORREÇÃO: Atualizar apenas as colunas envolvidas
            refreshSpecificColumns(fromColumn, toColumnId.toStdString());
            
            statusLabel_->setText("✅ Card movido com sucesso");
            
        } else {
            statusLabel_->setText("❌ Não foi possível determinar as colunas para mover o card");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro", QString("Erro ao mover card: ") + e.what());
        statusLabel_->setText("❌ Erro ao mover card: " + QString(e.what()));
    }
}

void MainWindow::refreshSpecificColumns(const std::string& fromColumnId, const std::string& toColumnId) {
    if (currentBoardId_.empty()) return;
    
    try {
        // CORREÇÃO: Atualizar apenas as colunas envolvidas no movimento
        if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
            auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
            
            // Atualizar coluna de origem
            auto fromIt = currentColumnWidgets.find(fromColumnId);
            if (fromIt != currentColumnWidgets.end()) {
                fromIt->second->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); });
            }
            
            // Atualizar coluna de destino
            auto toIt = currentColumnWidgets.find(toColumnId);
            if (toIt != currentColumnWidgets.end()) {
                toIt->second->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); });
            }
        }
        
        // Atualizar estatísticas
        updateStatistics();
        
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao atualizar colunas: " + QString(e.what()));
    }
}

void MainWindow::onCardAdded(const QString& columnId, const QString& title) {
    try {
        std::string newCardId = service_->addCard(currentBoardId_, 
                                                 columnId.toStdString(), 
                                                 title.toStdString());
        
        // CORREÇÃO: Atualiza apenas a coluna específica
        if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
            auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
            auto it = currentColumnWidgets.find(columnId.toStdString());
            if (it != currentColumnWidgets.end()) {
                it->second->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); });
            }
        }
        
        statusLabel_->setText("✅ Novo card criado: " + title);
        
        // Atualizar estatísticas
        updateStatistics();
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erro", QString("Erro ao criar card: ") + e.what());
        statusLabel_->setText("❌ Erro ao criar card");
    }
}

void MainWindow::refreshCurrentBoard(bool forceRebuild) {
    if (currentBoardId_.empty()) return;
    
    try {
        // CORREÇÃO: Verifica se o board ainda existe
        auto boardOpt = service_->findBoard(currentBoardId_);
        if (!boardOpt.has_value()) {
            statusLabel_->setText("❌ Board não encontrado");
            return;
        }
        
        auto columns = service_->listColumns(currentBoardId_);
        
        // CORREÇÃO: Verifica se o tab atual existe, se não, cria
        //int currentTabIndex = boardsTabWidget_->currentIndex();
        QString currentBoardName = QString::fromStdString((*boardOpt)->name());
        
        bool tabExists = false;
        for (int i = 0; i < boardsTabWidget_->count(); ++i) {
            if (boardsTabWidget_->tabText(i) == currentBoardName) {
                tabExists = true;
                break;
            }
        }
        
    if (!tabExists) {
    // Só cria novo tab se não existir
    QWidget *tabWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(tabWidget);
    
    // Título do board
    QLabel *titleLabel = new QLabel("📊 Board: " + currentBoardName);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Área rolável para colunas
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    QWidget *columnsContainer = new QWidget;
    QHBoxLayout *columnsLayout = new QHBoxLayout(columnsContainer);
    columnsLayout->setAlignment(Qt::AlignLeft);
    columnsLayout->setSpacing(15);
    columnsLayout->setContentsMargins(10, 10, 10, 10);
    
    // Limpa widgets antigos
    if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
        auto& oldWidgets = columnWidgetsByBoard_[currentBoardId_];
        for (auto& pair : oldWidgets) {
            delete pair.second;
        }
        oldWidgets.clear();
    }
    
    // Recria o mapa para este board
    columnWidgetsByBoard_[currentBoardId_] = std::map<std::string, ColumnWidget*>();
    
    // Adiciona todas as colunas
    for (const auto& column : columns) {
        ColumnWidget* columnWidget = new ColumnWidget(column);
        
        // Conectar sinais
        connect(columnWidget, &ColumnWidget::cardMoved, 
                this, &MainWindow::onCardMoved);
        connect(columnWidget, &ColumnWidget::cardAdded, 
                this, &MainWindow::onCardAdded);
        connect(columnWidget, &ColumnWidget::columnMoved,
                this, &MainWindow::onColumnMoved);
        connect(columnWidget, &ColumnWidget::cardReordered, this,
             &MainWindow::onCardReordered);
        
        columnsLayout->addWidget(columnWidget);
        columnWidgetsByBoard_[currentBoardId_][column->id()] = columnWidget;
    }
    
    columnsLayout->addStretch();
    scrollArea->setWidget(columnsContainer);
    mainLayout->addWidget(scrollArea);
    
    int newTabIndex = boardsTabWidget_->addTab(tabWidget, currentBoardName);
    boardsTabWidget_->setCurrentIndex(newTabIndex);
    
} else if (forceRebuild) {
    // Se o tab existe e forceRebuild é true, apenas atualiza o conteúdo
    // Encontra o tab existente
    QWidget* existingTab = nullptr;
    for (int i = 0; i < boardsTabWidget_->count(); ++i) {
        if (boardsTabWidget_->tabText(i) == currentBoardName) {
            existingTab = boardsTabWidget_->widget(i);
            break;
        }
    }
    
    if (existingTab) {
        // Encontra o container de colunas e atualiza
        QScrollArea* scrollArea = existingTab->findChild<QScrollArea*>();
        if (scrollArea) {
            QWidget* columnsContainer = scrollArea->widget();
            if (columnsContainer) {
                QHBoxLayout* columnsLayout = qobject_cast<QHBoxLayout*>(columnsContainer->layout());
                if (columnsLayout) {
                    // Limpa colunas existentes
                    QLayoutItem* item;
                    while ((item = columnsLayout->takeAt(0)) != nullptr) {
                        if (item->widget()) {
                            item->widget()->deleteLater();
                        }
                        delete item;
                    }
                    
                    // Limpa widgets antigos
                    if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
                        auto& oldWidgets = columnWidgetsByBoard_[currentBoardId_];
                        for (auto& pair : oldWidgets) {
                            delete pair.second;
                        }
                        oldWidgets.clear();
                    }
                    
                    // Recria o mapa para este board
                    columnWidgetsByBoard_[currentBoardId_] = std::map<std::string, ColumnWidget*>();
                    
                    // Adiciona todas as colunas
                    for (const auto& column : columns) {
                        ColumnWidget* columnWidget = new ColumnWidget(column);
                        
                        // Conectar sinais
                        connect(columnWidget, &ColumnWidget::cardMoved, 
                                this, &MainWindow::onCardMoved);
                        connect(columnWidget, &ColumnWidget::cardAdded, 
                                this, &MainWindow::onCardAdded);
                        connect(columnWidget, &ColumnWidget::columnMoved,
                                this, &MainWindow::onColumnMoved);
                        connect(columnWidget, &ColumnWidget::cardReordered, this, 
                            &MainWindow::onCardReordered);
                        
                        columnsLayout->addWidget(columnWidget);
                        columnWidgetsByBoard_[currentBoardId_][column->id()] = columnWidget;
                    }
                    
                    columnsLayout->addStretch();
                }
            }
        }
    }
} else {
            // Apenas atualiza as colunas existentes
            auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
            for (const auto& column : columns) {
                auto it = currentColumnWidgets.find(column->id());
                if (it != currentColumnWidgets.end()) {
                        it->second->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); });
                    } else {
                    // Se encontrou uma coluna nova, adiciona ao layout
                    ColumnWidget* columnWidget = new ColumnWidget(column);
                    
                    connect(columnWidget, &ColumnWidget::cardMoved, 
                            this, &MainWindow::onCardMoved);
                    connect(columnWidget, &ColumnWidget::cardAdded, 
                            this, &MainWindow::onCardAdded);
                    connect(columnWidget, &ColumnWidget::cardReordered, 
                        this, &MainWindow::onCardReordered);
                    
                    // Encontra o container de colunas e adiciona
                    QWidget* currentTab = boardsTabWidget_->currentWidget();
                    if (currentTab) {
                        QScrollArea* scrollArea = currentTab->findChild<QScrollArea*>();
                        if (scrollArea) {
                            QWidget* columnsContainer = scrollArea->widget();
                            if (columnsContainer) {
                                QHBoxLayout* columnsLayout = qobject_cast<QHBoxLayout*>(columnsContainer->layout());
                                if (columnsLayout) {
                                    // Remove o stretch temporariamente
                                    QLayoutItem* stretch = columnsLayout->takeAt(columnsLayout->count() - 1);
                                    columnsLayout->addWidget(columnWidget);
                                    if (stretch) {
                                        columnsLayout->addItem(stretch);
                                    }
                                    
                                    currentColumnWidgets[column->id()] = columnWidget;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Atualizar histórico de atividades
        refreshActivityLog();
        
        // Atualizar estatísticas
        updateStatistics();
        // APÓS recriar os CardWidgets, aplicar filtros
        if (!currentTagFilter_.isEmpty() || !currentPriorityFilters_.empty()) {
        // Aplicar filtros de forma mais simples - recriando toda a UI
        // Isso é menos eficiente, mas funciona por enquanto
        if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
            auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
            for (auto& pair : currentColumnWidgets) {
                ColumnWidget* columnWidget = pair.second;
                columnWidget->refreshCards([this](std::shared_ptr<kanban::domain::Card> c){ return this->cardMatchesFilter(c); }); // Isso vai recriar os cards com filtro
            }
        }
    }
        
        // Atualizar lista de tags para filtro
        refreshFilterTags();
        
    } catch (const std::exception& e) {
        statusLabel_->setText("❌ Erro ao atualizar board: " + QString(e.what()));
    }
}

void MainWindow::clearBoardTab() {
    // CORREÇÃO: Limpa apenas os widgets do board atual
    if (columnWidgetsByBoard_.find(currentBoardId_) != columnWidgetsByBoard_.end()) {
        auto& currentColumnWidgets = columnWidgetsByBoard_[currentBoardId_];
        for (auto& pair : currentColumnWidgets) {
            delete pair.second;
        }
        currentColumnWidgets.clear();
    }
}

void MainWindow::refreshActivityLog() {
    try {
        // Primeiro, limpar completamente o documento do QTextEdit
        // Isso remove texto e quaisquer imagens ou recursos embutidos
        if (activityLogTextEdit_) {
            // limpar conteúdo e resource cache do documento
            activityLogTextEdit_->clear();
            if (activityLogTextEdit_->document()) {
                activityLogTextEdit_->document()->clear();
                activityLogTextEdit_->setHtml(QString());
            }
        }

        if (currentBoardId_.empty()) {
            activityLogTextEdit_->setPlainText("Selecione um board para ver o histórico.");
            return;
        }
        
        auto board = service_->findBoard(currentBoardId_);
        if (!board.has_value()) {
            activityLogTextEdit_->setPlainText("Board não encontrado.");
            return;
        }
        
        auto activityLog = (*board)->activityLog();
        if (!activityLog) {
            activityLogTextEdit_->setPlainText("Nenhum histórico disponível.");
            return;
        }
        
        activityLogTextEdit_->clear();
        
        auto activities = activityLog->activities();
        
        if (activities.empty()) {
            activityLogTextEdit_->setPlainText("Nenhuma atividade registrada ainda.\n\n"
                                             "As atividades aparecerão aqui quando você:\n"
                                             "• Mover cards entre colunas\n"
                                             "• Criar novos cards\n"
                                             "• Fizer outras alterações");
            return;
        }
        
        // Ordenar por timestamp (mais recente primeiro)
        std::vector<domain::Activity> sortedActivities(activities.begin(), activities.end());
        std::sort(sortedActivities.begin(), sortedActivities.end(),
                 [](const domain::Activity& a, const domain::Activity& b) {
                     return a.when() > b.when();
                 });
        
        for (const auto& activity : sortedActivities) {
            auto time_t = std::chrono::system_clock::to_time_t(activity.when());
            QString timeStr = QDateTime::fromSecsSinceEpoch(time_t).toString("dd/MM/yyyy hh:mm:ss");
            
            QString activityText = QString("🕒 %1\n   %2\n")
                .arg(timeStr)
                .arg(QString::fromStdString(activity.description()));
            
            activityLogTextEdit_->append(activityText);
        }
        
    } catch (const std::exception& e) {
        activityLogTextEdit_->setPlainText("❌ Erro ao carregar atividades: " + QString(e.what()));
    }
}

void MainWindow::updateStatistics() {
    try {
        if (currentBoardId_.empty()) {
            statsLabel_->setText("Selecione um board para ver estatísticas...");
            return;
        }
        
        auto board = service_->findBoard(currentBoardId_);
        if (!board.has_value()) {
            statsLabel_->setText("Board não encontrado.");
            return;
        }
        
        auto columns = service_->listColumns(currentBoardId_);
        int totalCards = 0;
        int totalColumns = columns.size();
        QString columnStats;
        
        for (const auto& column : columns) {
            int columnCardCount = column->size();
            totalCards += columnCardCount;
            columnStats += QString("• %1: %2 cards\n")
                          .arg(QString::fromStdString(column->name()))
                          .arg(columnCardCount);
        }
        
        QString statsText = QString(
            "📊 Estatísticas do Board:\n\n"
            "🏷️ Total de Colunas: %1\n"
            "🎴 Total de Cards: %2\n\n"
            "📋 Distribuição:\n%3"
        ).arg(totalColumns).arg(totalCards).arg(columnStats);
        
        statsLabel_->setText(statsText);
        
    } catch (const std::exception& e) {
        statsLabel_->setText("❌ Erro ao carregar estatísticas: " + QString(e.what()));
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "Sobre Kanban System Lite",
        "<h3>Kanban System Lite v3.0</h3>"
        "<p>Sistema de gerenciamento de tarefas baseado em Kanban</p>"
        "<p><b>Funcionalidades:</b></p>"
        "<ul>"
        "<li>📋 Gerenciamento de múltiplos boards</li>"
        "<li>🗂️ Colunas personalizáveis</li>"
        "<li>🎴 Cartões com título e descrição</li>"
        "<li>📊 Visualização em tempo real</li>"
        "<li>📝 Histórico de atividades</li>"
        "<li>🚀 Drag & drop entre colunas</li>"
        "</ul>"
        "<p><b>Tecnologias:</b> C++17, Qt6, CMake</p>"
        "<p><b>Desenvolvido para:</b> Trabalho de POO - Universidade</p>");
}

} // namespace gui
} // namespace kanban