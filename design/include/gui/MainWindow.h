#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QFrame>
#include <QWidget>
#include <QTextEdit>
#include <QInputDialog>
#include <QDateTime>
#include <map>
#include <memory>

// ADICIONE ESTES INCLUDES:
#include <QGroupBox>
#include <QCheckBox>
#include <QComboBox>
#include <set>

#include "application/KanbanService.h"
#include "gui/ColumnWidget.h"

namespace kanban {
namespace gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onColumnMoved(const QString& fromColumnId, const QString& toColumnId);
    void createNewBoard();
    void createNewColumn();
    void refreshBoards();
    void onBoardSelected(int index);
    void showAbout();
    void onCardMoved(const QString& cardId, const QString& fromColumnId, const QString& toColumnId);
    void onCardAdded(const QString& columnId, const QString& title);
    void onCardReordered(const QString& columnId, const QString& cardId, int newIndex);

private:
    void setupUI();
    void setupConnections();
    void setupMenuBar();
    void loadSampleData();
    void refreshCurrentBoard(bool forceRebuild = false);
    void refreshSpecificColumns(const std::string& fromColumnId, const std::string& toColumnId);
    void clearBoardTab();
    void refreshActivityLog();
    void updateStatistics();

    // ADICIONE ESTAS DECLARAÇÕES DOS NOVOS MÉTODOS:
    void setupFilterPanel();
    void applyFilters();
    void clearFilters();
    void refreshFilterTags();
    bool cardMatchesFilter(std::shared_ptr<domain::Card> card);

    // Serviço de aplicação
    std::unique_ptr<application::KanbanService> service_;

    // Componentes da UI
    QTabWidget *boardsTabWidget_;
    QListWidget *boardsListWidget_;
    QPushButton *createBoardButton_;
    QPushButton *createColumnButton_;
    QLineEdit *boardNameLineEdit_;
    QLabel *statusLabel_;
    QTextEdit *activityLogTextEdit_;
    QPushButton *refreshActivityLogButton_;
    QLabel *statsLabel_;

    // ADICIONE ESTES NOVOS COMPONENTES:
    QGroupBox *filterGroup_;
    QComboBox *tagFilterCombo_;
    QCheckBox *highPriorityFilter_;
    QCheckBox *mediumPriorityFilter_;
    QCheckBox *lowPriorityFilter_;
    QPushButton *applyFilterButton_;
    QPushButton *clearFilterButton_;

    // Estado dos filtros
    QString currentTagFilter_;
    std::set<int> currentPriorityFilters_;

    // Mapeamento de widgets por board
    std::map<std::string, std::map<std::string, ColumnWidget*>> columnWidgetsByBoard_;
    std::string currentBoardId_;
};

} // namespace gui
} // namespace kanban

#endif // MAINWINDOW_H