#ifndef COLUMNWIDGET_H
#define COLUMNWIDGET_H

#include <QWidget>
#include <functional>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDrag>
#include "gui/CardWidget.h" // ADICIONE ESTE INCLUD
#include "domain/Column.h"

namespace kanban {
namespace gui {

class CardWidget; // Forward declaration

class ColumnWidget : public QFrame {
    Q_OBJECT

public:
    explicit ColumnWidget(std::shared_ptr<domain::Column> column, QWidget *parent = nullptr);
    std::string getColumnId() const { return column_->id(); }
    // Now accepts an optional predicate to filter which cards are displayed.
    // If predicate is empty, all cards are shown.
    void refreshCards(std::function<bool(std::shared_ptr<kanban::domain::Card>)> predicate = nullptr);
    
    // ADICIONE ESTE MÉTODO:
    std::vector<CardWidget*> cardWidgets() const;

public slots:
    void addNewCard();

signals:
    void cardMoved(const QString& cardId, const QString& fromColumnId, const QString& toColumnId);
    void cardAdded(const QString& columnId, const QString& title);
    void cardReordered(const QString& columnId, const QString& cardId, int newIndex);
    void columnMoved(const QString& fromColumnId, const QString& toColumnId);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupUI();
    int calculateDropIndex(const QPoint& pos);
    int calculateDropIndicatorPosition(int index); // ADICIONE ESTA LINHA

    std::shared_ptr<domain::Column> column_;
    QVBoxLayout *mainLayout_;
    QVBoxLayout *cardsLayout_;
    QScrollArea *scrollArea_;
    QWidget *scrollWidget_;
    QPushButton *addCardButton_;
    bool dragOver_ = false;
    QPoint dragStartPosition_;
    int dropIndicatorIndex_ = -1;
};



} // namespace gui
} // namespace kanban

#endif // COLUMNWIDGET_H