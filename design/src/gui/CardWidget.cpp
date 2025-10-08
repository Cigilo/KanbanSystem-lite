#include "gui/CardWidget.h"
#include "gui/CardDialog.h"
#include "gui/ColumnWidget.h"
#include <QStyle>
#include <QFontMetrics>

namespace kanban {
namespace gui {

CardWidget::CardWidget(std::shared_ptr<domain::Card> card, QWidget *parent)
    : QWidget(parent), card_(card)
{
    // Permite que o stylesheet pinte o background
    setAttribute(Qt::WA_StyledBackground, true);

    // Aplica o estilo diretamente neste widget (sem seletor)
    setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                            stop:0 #3a3a3a, stop:1 #2b2b2b);"
        "border: transparent;"        
        "border-radius: 6px;"
        "padding: 8px;"
    );
    setupUI();
}


void CardWidget::setupUI() {
    // Configuração visual do card
    setMinimumSize(200, 80);
    setMaximumWidth(250);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);



    // Layout principal
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    // Título do card
    titleLabel_ = new QLabel(QString::fromStdString(card_->title()));
    titleLabel_->setStyleSheet(
        "QLabel {"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "   color: #ffffffff;"
        "   background-color: transparent;"
        "}"
    );
    titleLabel_->setWordWrap(true);
    titleLabel_->setMaximumHeight(40);
    layout->addWidget(titleLabel_);

    // Descrição (se existir)
    if (card_->description().has_value() && !card_->description()->empty()) {
        QString description = QString::fromStdString(card_->description().value());
        
        // Limitar descrição se for muito longa
        QFontMetrics metrics(font());
        QString elidedText = metrics.elidedText(description, Qt::ElideRight, 220);
        
        descriptionLabel_ = new QLabel(elidedText);
        descriptionLabel_->setStyleSheet(
            "QLabel {"
            "   font-size: 11px;"
            "   color: #c2c2c2ff;"
            "   background-color: transparent;"
            "}"
        );
        descriptionLabel_->setWordWrap(true);
        descriptionLabel_->setMaximumHeight(30);
        layout->addWidget(descriptionLabel_);
    } else {
        descriptionLabel_ = nullptr;
    }

    // INICIALIZAR tagsLayout_
    tagsLayout_ = new QHBoxLayout();
    tagsLayout_->setContentsMargins(0, 4, 0, 4);
    tagsLayout_->setSpacing(4);
    updateTagsDisplay();
    
    // CORREÇÃO: Usar this->layout() em vez de layout()
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->insertLayout(mainLayout->count() - 1, tagsLayout_);
    }

    // Rodapé com prioridade
    QHBoxLayout *footerLayout = new QHBoxLayout();
    
    // Prioridade
    QString priorityText;
    QColor priorityColor;
    
    switch (card_->priority()) {
        case 0: priorityText = "⚪ Baixa"; priorityColor = QColor("#28a745"); break;
        case 1: priorityText = "🟡 Média"; priorityColor = QColor("#ffc107"); break;
        case 2: priorityText = "🔴 Alta"; priorityColor = QColor("#dc3545"); break;
        default: priorityText = QString("⚡ %1").arg(card_->priority()); priorityColor = QColor("#6c757d");
    }
    
    priorityLabel_ = new QLabel(priorityText);
    priorityLabel_->setStyleSheet(
        QString("QLabel {"
                "   font-size: 10px;"
                "   color: %1;"
                "   background-color: transparent;"
                "   font-weight: bold;"
                "}").arg(priorityColor.name())
    );
    
    footerLayout->addWidget(priorityLabel_);
    footerLayout->addStretch();
    
    // Botões mover para cima/baixo
    moveUpButton_ = new QPushButton("⬆");
    moveUpButton_->setMaximumWidth(28);
    moveUpButton_->setToolTip("Mover para cima");
    moveUpButton_->setStyleSheet("QPushButton { border: none; background: transparent; }");
    connect(moveUpButton_, &QPushButton::clicked, this, [this]() {
        emit moveUpRequested(QString::fromStdString(card_->id()));
    });
    footerLayout->addWidget(moveUpButton_);

    moveDownButton_ = new QPushButton("⬇");
    moveDownButton_->setMaximumWidth(28);
    moveDownButton_->setToolTip("Mover para baixo");
    moveDownButton_->setStyleSheet("QPushButton { border: none; background: transparent; }");
    connect(moveDownButton_, &QPushButton::clicked, this, [this]() {
        emit moveDownRequested(QString::fromStdString(card_->id()));
    });
    footerLayout->addWidget(moveDownButton_);
    
    // ID do card (pequeno)
    QLabel *idLabel = new QLabel(QString("#%1").arg(QString::fromStdString(card_->id()).mid(0, 6)));
    idLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 9px;"
        "   color: #adb5bd;"
        "   background-color: transparent;"
        "}"
    );
    footerLayout->addWidget(idLabel);
    
    layout->addLayout(footerLayout);
}

void CardWidget::updateTagsDisplay() {
    // Limpar tags existentes
    QLayoutItem* item;
    while ((item = tagsLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Adicionar tags atuais
    if (card_) {
        auto tags = card_->tags();
        for (const auto& tag : tags) {
            QLabel *tagLabel = new QLabel(QString::fromStdString(tag->name()));
            tagLabel->setStyleSheet(
                "QLabel {"
                "   background-color: #e3f2fd;"
                "   color: #1976d2;"
                "   padding: 2px 6px;"
                "   border-radius: 8px;"
                "   font-size: 9px;"
                "   border: 1px solid #90caf9;"
                "}"
            );
            tagLabel->setMaximumHeight(16);
            tagsLayout_->addWidget(tagLabel);
        }
    }
    
    // Spacer para empurrar para esquerda
    tagsLayout_->addStretch();
}

void CardWidget::updateUI() {
    if (!card_) {
        return;
    }
    
    // Atualizar título
    titleLabel_->setText(QString::fromStdString(card_->title()));
    
    // Gerenciar descrição
    bool hasDescription = card_->description().has_value() && !card_->description()->empty();
    
    if (hasDescription) {
        QString description = QString::fromStdString(card_->description().value());
        QFontMetrics metrics(font());
        QString elidedText = metrics.elidedText(description, Qt::ElideRight, 220);
        
        if (!descriptionLabel_) {
            // Criar descriptionLabel_ se não existir
            descriptionLabel_ = new QLabel(elidedText);
            descriptionLabel_->setStyleSheet(
                "QLabel {"
                "   font-size: 11px;"
                "   color: #ffffffff;"
                "   background-color: transparent;"
                "}"
            );
            descriptionLabel_->setWordWrap(true);
            descriptionLabel_->setMaximumHeight(30);
            // Inserir após o titleLabel_ e antes do footer
            QVBoxLayout* vLayout = qobject_cast<QVBoxLayout*>(this->layout());
            if (vLayout) {
                vLayout->insertWidget(1, descriptionLabel_);
            }
        } else {
            descriptionLabel_->setText(elidedText);
        }
    } else {
        // Se não há descrição, remove o label se existir
        if (descriptionLabel_) {
            this->layout()->removeWidget(descriptionLabel_);
            delete descriptionLabel_;
            descriptionLabel_ = nullptr;
        }
    }
    
    // Atualizar prioridade
    QString priorityText;
    QColor priorityColor;
    switch (card_->priority()) {
        case 0: priorityText = "⚪ Baixa"; priorityColor = QColor("#28a745"); break;
        case 1: priorityText = "🟡 Média"; priorityColor = QColor("#ffc107"); break;
        case 2: priorityText = "🔴 Alta"; priorityColor = QColor("#dc3545"); break;
        default: priorityText = QString("⚡ %1").arg(card_->priority()); priorityColor = QColor("#6c757d");
    }
    priorityLabel_->setText(priorityText);
    priorityLabel_->setStyleSheet(
        QString("QLabel {"
                "   font-size: 10px;"
                "   color: %1;"
                "   background-color: transparent;"
                "   font-weight: bold;"
                "}").arg(priorityColor.name())
    );

    // Atualizar tags
    updateTagsDisplay();
}

void CardWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition_ = event->pos();
    }
    QWidget::mousePressEvent(event);
}

void CardWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }
    
    if ((event->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    // Criar operação de drag
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    
    // Incluir informações completas do card
    QString cardData = QString::fromStdString(card_->id());
    mimeData->setText(cardData);
    mimeData->setData("application/x-card-id", QByteArray::fromStdString(card_->id()));
    mimeData->setData("application/x-card-title", QByteArray::fromStdString(card_->title()));
    
    // Tentar obter a coluna pai
    QWidget* parent = parentWidget();
    while (parent && !parent->inherits("ColumnWidget")) {
        parent = parent->parentWidget();
    }
    
    if (parent) {
        ColumnWidget* columnWidget = qobject_cast<ColumnWidget*>(parent);
        if (columnWidget) {
            mimeData->setData("application/x-from-column", 
                            QByteArray::fromStdString(columnWidget->getColumnId()));
        }
    }
    
    drag->setMimeData(mimeData);
    
    // Visual do drag
    QPixmap pixmap = grab();
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());
    
    // CORREÇÃO: Remover variável não utilizada
    drag->exec(Qt::MoveAction);
}

void CardWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    
    if (!card_) {
        return;
    }
    
    CardDialog dialog(card_);
    if (dialog.exec() == QDialog::Accepted) {
        // Atualizar card com novos dados
        card_->setTitle(dialog.getTitle().toStdString());
        card_->setDescription(dialog.getDescription().toStdString());
        card_->setPriority(dialog.getPriority());
        
        // Atualizar tags a partir do dialog
        card_->clearTags();
        QStringList tags = dialog.getTags();
        for (const QString& t : tags) {
            auto tag = std::make_shared<domain::Tag>(t.toStdString(), t.toStdString());
            card_->addTag(tag);
        }

        // Atualizar UI de forma segura
        updateUI();
    }
}

} // namespace gui
} // namespace kanban