#include "gui/ColumnWidget.h"
#include "gui/CardWidget.h"
#include <QInputDialog>
#include <QDebug>
#include <QPainter>
#include <QScrollBar> // ADICIONE ESTE INCLUDE
#include "gui/CardWidget.h" // ADICIONE ESTE INCLUD

namespace kanban {
namespace gui {

ColumnWidget::ColumnWidget(std::shared_ptr<domain::Column> column, QWidget *parent)
    : QFrame(parent), column_(column) {
    
    setupUI();
    refreshCards(nullptr);
}

void ColumnWidget::setupUI() {
    // Configuração visual da coluna
    setFrameStyle(QFrame::Box);
    setStyleSheet(
        "ColumnWidget {"
        "   background-color: #f8f9fa;"
        "   border: 2px solid #dee2e6;"
        "   border-radius: 8px;"
        "}"
        "ColumnWidget::title {"
        "   background-color: #e9ecef;"
        "   border-bottom: 1px solid #dee2e6;"
        "}"
    );

    setMinimumWidth(280);
    setMaximumWidth(320);
    setAcceptDrops(true);

    // Layout principal
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(8, 8, 8, 8);
    mainLayout_->setSpacing(8);

    // Cabeçalho da coluna
    QLabel *titleLabel = new QLabel(QString::fromStdString(column_->name()));
    titleLabel->setStyleSheet(
        "QLabel {"
        "   font-weight: bold;"
        "   font-size: 16px;"
        "   color: #000000;"
        "   padding: 8px;"
        "   background-color: #ffffffff;"
        "   border-radius: 4px;"
        "}"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout_->addWidget(titleLabel);

    // Contador de cards
    QLabel *countLabel = new QLabel(QString("%1 cards").arg(column_->size()));
    countLabel->setStyleSheet(
        "QLabel {"
        "   font-size: 12px;"
        "   color: #050505ff;"
        "   padding: 4px;"
        "}"
    );
    countLabel->setAlignment(Qt::AlignCenter);
    mainLayout_->addWidget(countLabel);

    // Área rolável para cards
    scrollArea_ = new QScrollArea;
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea_->setStyleSheet(
        "QScrollArea {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QScrollBar:vertical {"
        "   background-color: #f1f1f1;"
        "   width: 10px;"
        "   border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background-color: #c1c1c1;"
        "   border-radius: 5px;"
        "}"
    );

    scrollWidget_ = new QWidget;
    cardsLayout_ = new QVBoxLayout(scrollWidget_);
    cardsLayout_->setAlignment(Qt::AlignTop);
    cardsLayout_->setSpacing(6);
    cardsLayout_->setContentsMargins(2, 2, 2, 2);

    scrollArea_->setWidget(scrollWidget_);
    mainLayout_->addWidget(scrollArea_);

    // Botão para adicionar card
    addCardButton_ = new QPushButton("+ Adicionar Card");
    addCardButton_->setStyleSheet(
        "QPushButton {"
        "   background-color: #000000ff;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px;"
        "   border-radius: 4px;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5a6268;"
        "}"
    );
    connect(addCardButton_, &QPushButton::clicked, this, &ColumnWidget::addNewCard);
    mainLayout_->addWidget(addCardButton_);
}

void ColumnWidget::refreshCards(std::function<bool(std::shared_ptr<kanban::domain::Card>)> predicate) {
    // Limpar cards existentes
    QLayoutItem *item;
    while ((item = cardsLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Adicionar cards atuais
    auto cards = column_->cards();
    for (const auto& card : cards) {
        // Se existir predicado, usar para filtrar
        if (predicate) {
            try {
                if (!predicate(card)) continue;
            } catch (...) {
                // Em caso de erro no predicado, mostrar o card por seguranca
            }
        }

        CardWidget *cardWidget = new CardWidget(card);
        // Conectar sinais de mover para cima/baixo
        connect(cardWidget, &CardWidget::moveUpRequested, this, [this](const QString& cardId){
            // encontrar índice atual do card
            int currentIndex = -1;
            auto cards = column_->cards();
            for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
                if (QString::fromStdString(cards[i]->id()) == cardId) { currentIndex = i; break; }
            }
            if (currentIndex == -1) return;
            int newIndex = std::max(0, currentIndex - 1);
            emit cardReordered(QString::fromStdString(column_->id()), cardId, newIndex);
        });
        connect(cardWidget, &CardWidget::moveDownRequested, this, [this](const QString& cardId){
            int currentIndex = -1;
            auto cards = column_->cards();
            for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
                if (QString::fromStdString(cards[i]->id()) == cardId) { currentIndex = i; break; }
            }
            if (currentIndex == -1) return;
            int maxIndex = static_cast<int>(column_->size()) - 1;
            int newIndex = std::min(maxIndex, currentIndex + 1);
            emit cardReordered(QString::fromStdString(column_->id()), cardId, newIndex);
        });

        cardsLayout_->addWidget(cardWidget);
    }
}

void ColumnWidget::addNewCard() {
    bool ok;
    QString title = QInputDialog::getText(this, 
                                         "Novo Card", 
                                         "Título do card:", 
                                         QLineEdit::Normal, 
                                         "", 
                                         &ok);
    
    if (ok && !title.isEmpty()) {
        emit cardAdded(QString::fromStdString(column_->id()), title);
    }
}

void ColumnWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasFormat("application/x-column") || 
        event->mimeData()->hasFormat("application/x-card-id")) {
        dragOver_ = true;
        update();
        event->acceptProposedAction();
    }
}

void ColumnWidget::dragMoveEvent(QDragMoveEvent *event) {
    if (event->mimeData()->hasFormat("application/x-card-id")) {
        // CORREÇÃO Qt6: usar position() em vez de pos()
        dropIndicatorIndex_ = calculateDropIndex(event->position().toPoint());
        update();
        event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat("application/x-column")) {
        event->acceptProposedAction();
    }
}

void ColumnWidget::dropEvent(QDropEvent *event) {
    dragOver_ = false;
    dropIndicatorIndex_ = -1;
    update();

    if (event->mimeData()->hasFormat("application/x-column")) {
        // Movimento de coluna (já existe)
        QByteArray itemData = event->mimeData()->data("application/x-column");
        QString fromColumnId = QString::fromUtf8(itemData);
        QString toColumnId = QString::fromStdString(column_->id());
        
        emit columnMoved(fromColumnId, toColumnId);
        event->acceptProposedAction();
    }
    else if (event->mimeData()->hasFormat("application/x-card-id")) {
        QByteArray itemData = event->mimeData()->data("application/x-card-id");
        QString cardId = QString::fromUtf8(itemData);
        
        // Obter a coluna de origem do mime data
        QString fromColumnId;
        if (event->mimeData()->hasFormat("application/x-from-column")) {
            QByteArray fromColumnData = event->mimeData()->data("application/x-from-column");
            fromColumnId = QString::fromUtf8(fromColumnData);
        }

        // DIFERENCIAR: Reordenação vs Movimento entre colunas
        if (fromColumnId == QString::fromStdString(column_->id())) {
            // REORDENAÇÃO: Card está sendo movido dentro da mesma coluna
            int newIndex = calculateDropIndex(event->position().toPoint());
            if (newIndex != -1) {
                emit cardReordered(fromColumnId, cardId, newIndex);
            }
        } else {
            // MOVIMENTO ENTRE COLUNAS: Card está sendo movido para outra coluna
            emit cardMoved(cardId, fromColumnId, QString::fromStdString(column_->id()));
        }
        
        event->acceptProposedAction();
    }
}

void ColumnWidget::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    
    // Feedback visual durante o drag
    if (dragOver_) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        QColor highlightColor = QColor(66, 165, 245, 50);
        painter.fillRect(rect(), highlightColor);
        
        QPen borderPen(QColor(66, 165, 245), 2);
        painter.setPen(borderPen);
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 6, 6);
    }
    
    // Desenhar indicador de posição para reordenação (VERSÃO SIMPLIFICADA)
    if (dropIndicatorIndex_ != -1) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        QPen linePen(QColor(66, 165, 245), 3);
        painter.setPen(linePen);
        
        // CÁLCULO SIMPLES DIRETO - SEM FUNÇÃO COMPLEXA
        int headerHeight = 100;
        int cardHeight = 90;
        int scrollOffset = scrollArea_->verticalScrollBar()->value();
        int yPos = headerHeight + (dropIndicatorIndex_ * cardHeight) - scrollOffset;
        
        // Garantir que está dentro dos limites visíveis
        if (yPos >= headerHeight && yPos <= height()) {
            painter.drawLine(20, yPos, width() - 20, yPos);
            
            // Triângulo indicador (opcional)
            QPolygon triangle;
            triangle << QPoint(25, yPos - 5) << QPoint(35, yPos) << QPoint(25, yPos + 5);
            painter.setBrush(QColor(66, 165, 245));
            painter.drawPolygon(triangle);
        }
    }
}

// NOVO MÉTODO: Calcular posição precisa do indicador

int ColumnWidget::calculateDropIndex(const QPoint& pos) {
    // Converter posição global para local dentro do scrollWidget_
    QPoint localPos = scrollWidget_->mapFrom(this, pos);
    
    // Considerar scroll atual
    int scrollOffset = scrollArea_->verticalScrollBar()->value();
    int relativeY = localPos.y() + scrollOffset;
    
    // Altura do cabeçalho
    int headerHeight = 100;
    
    // Se estiver acima do primeiro card, inserir no início
    if (relativeY < headerHeight) {
        return 0;
    }
    
    // Calcular índice baseado na posição Y
    int cardHeight = 90; // Altura aproximada de um card
    int index = (relativeY - headerHeight) / cardHeight;
    
    // Limitar ao número máximo de cards
    int maxIndex = static_cast<int>(column_->size());
    if (index > maxIndex) {
        index = maxIndex;
    }
    
    return index;
}

void ColumnWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragStartPosition_ = event->pos();
    }
    QFrame::mousePressEvent(event);
}

void ColumnWidget::mouseMoveEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::LeftButton)) return;
    if ((event->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance()) return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-column", QByteArray::fromStdString(column_->id()));
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

int ColumnWidget::calculateDropIndicatorPosition(int index) {
    if (index < 0) return -1;
    
    // Cálculo simples
    int headerHeight = 100;
    int cardHeight = 90;
    int scrollOffset = scrollArea_->verticalScrollBar()->value();
    int yPos = headerHeight + (index * cardHeight) - scrollOffset;
    
    // Limites
    if (yPos < headerHeight) yPos = headerHeight;
    if (yPos > height() - 20) yPos = height() - 20;
    
    return yPos;
}

std::vector<CardWidget*> ColumnWidget::cardWidgets() const {
    std::vector<CardWidget*> widgets;
    
    // Percorrer todos os itens do layout de cards
    for (int i = 0; i < cardsLayout_->count(); ++i) {
        QLayoutItem* item = cardsLayout_->itemAt(i);
        if (item && item->widget()) {
            CardWidget* cardWidget = qobject_cast<CardWidget*>(item->widget());
            if (cardWidget) {
                widgets.push_back(cardWidget);
            }
        }
    }
    
    return widgets;
}




} // namespace gui
} // namespace kanban