#include "gui/CardDialog.h"

namespace kanban {
namespace gui {

CardDialog::CardDialog(std::shared_ptr<domain::Card> card, QWidget *parent)
    : QDialog(parent), card_(card) {
    
    setupUI();
    
    // Preencher dados se estiver editando um card existente
    if (card) {
        titleEdit_->setText(QString::fromStdString(card->title()));
        if (card->description().has_value()) {
            descriptionEdit_->setPlainText(QString::fromStdString(card->description().value()));
        }
        // CORREÇÃO: Garantir que a prioridade esteja dentro dos limites
        int priority = card->priority();
        if (priority < 0) priority = 0;
        if (priority > 2) priority = 2;
        priorityCombo_->setCurrentIndex(priority);
        
        // Preencher tags existentes
        for (const auto& tag : card->tags()) {
            tagsListWidget_->addItem(QString::fromStdString(tag->name()));
        }
    }
}

void CardDialog::setupUI() {
    setWindowTitle("Editar Card");
    setMinimumSize(400, 300);
    setModal(true);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Título
    QLabel *titleLabel = new QLabel("Título:");
    titleEdit_ = new QLineEdit;
    titleEdit_->setPlaceholderText("Digite o título do card...");
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(titleEdit_);
    
    // Descrição
    QLabel *descLabel = new QLabel("Descrição:");
    descriptionEdit_ = new QTextEdit;
    descriptionEdit_->setPlaceholderText("Digite a descrição do card...");
    descriptionEdit_->setMaximumHeight(100);
    mainLayout->addWidget(descLabel);
    mainLayout->addWidget(descriptionEdit_);
    
    // Prioridade
    QLabel *priorityLabel = new QLabel("Prioridade:");
    priorityCombo_ = new QComboBox;
    priorityCombo_->addItem("⚪ Baixa");
    priorityCombo_->addItem("🟡 Média");
    priorityCombo_->addItem("🔴 Alta");
    mainLayout->addWidget(priorityLabel);
    mainLayout->addWidget(priorityCombo_);
    
    // ===== SEÇÃO DE TAGS =====
    QLabel *tagsLabel = new QLabel("🏷️ Etiquetas:");
    tagsListWidget_ = new QListWidget;
    tagsListWidget_->setMaximumHeight(80);
    
    tagsComboBox_ = new QComboBox;
    tagsComboBox_->setEditable(true);
    tagsComboBox_->setPlaceholderText("Digite uma nova etiqueta...");
    
    // Popular com tags comuns
    tagsComboBox_->addItems({"urgente", "bug", "feature", "design", "teste", "documentação"});
    
    addTagButton_ = new QPushButton("➕ Adicionar");
    removeTagButton_ = new QPushButton("➖ Remover");
    
    QHBoxLayout *tagsInputLayout = new QHBoxLayout;
    tagsInputLayout->addWidget(tagsComboBox_, 3);
    tagsInputLayout->addWidget(addTagButton_, 1);
    tagsInputLayout->addWidget(removeTagButton_, 1);
    
    mainLayout->addWidget(tagsLabel);
    mainLayout->addWidget(tagsListWidget_);
    mainLayout->addLayout(tagsInputLayout);
    
    // Conectar botões de tags
    connect(addTagButton_, &QPushButton::clicked, this, &CardDialog::addTag);
    connect(removeTagButton_, &QPushButton::clicked, this, &CardDialog::removeTag);
    
    // CORREÇÃO: Usar o lineEdit interno do ComboBox
    if (tagsComboBox_->lineEdit()) {
        connect(tagsComboBox_->lineEdit(), &QLineEdit::returnPressed, this, &CardDialog::addTag);
    }
    
    // Botões
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    okButton_ = new QPushButton("OK");
    cancelButton_ = new QPushButton("Cancelar");
    
    okButton_->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    cancelButton_->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 8px; }");
    
    connect(okButton_, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton_, &QPushButton::clicked, this, &QDialog::reject);
    
    buttonLayout->addWidget(okButton_);
    buttonLayout->addWidget(cancelButton_);
    mainLayout->addLayout(buttonLayout);
}

void CardDialog::addTag() {
    QString tagText = tagsComboBox_->currentText().trimmed();
    if (tagText.isEmpty()) return;
    
    // Verificar se já existe
    for (int i = 0; i < tagsListWidget_->count(); ++i) {
        if (tagsListWidget_->item(i)->text().compare(tagText, Qt::CaseInsensitive) == 0) {
            return; // Já existe
        }
    }
    
    // Adicionar à lista
    tagsListWidget_->addItem(tagText);
    tagsComboBox_->setCurrentText("");
}

void CardDialog::removeTag() {
    QList<QListWidgetItem*> selected = tagsListWidget_->selectedItems();
    for (QListWidgetItem* item : selected) {
        delete item;
    }
}

QString CardDialog::getTitle() const { 
    return titleEdit_->text(); 
}

QString CardDialog::getDescription() const { 
    return descriptionEdit_->toPlainText(); 
}

int CardDialog::getPriority() const { 
    // CORREÇÃO: Garantir que o índice esteja dentro dos limites
    int index = priorityCombo_->currentIndex();
    if (index < 0) index = 0;
    if (index > 2) index = 2;
    return index;
}

QStringList CardDialog::getTags() const {
    QStringList tags;
    for (int i = 0; i < tagsListWidget_->count(); ++i) {
        tags.append(tagsListWidget_->item(i)->text());
    }
    return tags;
}

} // namespace gui
} // namespace kanban