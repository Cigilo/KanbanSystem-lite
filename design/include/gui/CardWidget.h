#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QApplication>
#include <QHBoxLayout>  // ADICIONE ESTE
#include <QFrame>       // ADICIONE ESTE
#include <QPushButton>

#include "domain/Card.h"

namespace kanban {
namespace gui {

class CardWidget : public QWidget {
    Q_OBJECT

public:
    explicit CardWidget(std::shared_ptr<domain::Card> card, QWidget *parent = nullptr);
    std::string getCardId() const { return card_->id(); }
    std::shared_ptr<domain::Card> getCard() const { return card_; }
    void applyFilter(bool visible);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void updateUI();
    void updateTagsDisplay();  // ADICIONE ESTE MÉTODO

    std::shared_ptr<domain::Card> card_;
    QPoint dragStartPosition_;
    QLabel *titleLabel_;
    QLabel *descriptionLabel_;
    QLabel *priorityLabel_;
    QHBoxLayout *tagsLayout_;  // ADICIONE ESTE
    QPushButton *moveUpButton_;
    QPushButton *moveDownButton_;

signals:
    void moveUpRequested(const QString& cardId);
    void moveDownRequested(const QString& cardId);
};

} // namespace gui
} // namespace kanban

#endif // CARDWIDGET_H