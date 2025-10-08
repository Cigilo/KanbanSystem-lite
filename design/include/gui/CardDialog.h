#ifndef CARDDIALOG_H
#define CARDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

// ADICIONE ESTES INCLUDES:
#include <QListWidget>
#include <QStringList>

#include "domain/Card.h"

namespace kanban {
namespace gui {

class CardDialog : public QDialog {
    Q_OBJECT

public:
    explicit CardDialog(std::shared_ptr<domain::Card> card = nullptr, QWidget *parent = nullptr);

    QString getTitle() const;
    QString getDescription() const;
    int getPriority() const;
    QStringList getTags() const;  // ADICIONE ESTE MÉTODO

private:
    void setupUI();
    void addTag();     // ADICIONE ESTE
    void removeTag();  // ADICIONE ESTE
    
    std::shared_ptr<domain::Card> card_;
    QLineEdit *titleEdit_;
    QTextEdit *descriptionEdit_;
    QComboBox *priorityCombo_;
    QPushButton *okButton_;
    QPushButton *cancelButton_;
    
    // ADICIONE ESTES:
    QListWidget *tagsListWidget_;
    QComboBox *tagsComboBox_;
    QPushButton *addTagButton_;
    QPushButton *removeTagButton_;
};

} // namespace gui
} // namespace kanban

#endif // CARDDIALOG_H