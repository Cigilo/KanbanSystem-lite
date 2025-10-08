#include <QApplication>
#include <QStyleFactory>
#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Configurar aplicação
    app.setApplicationName("Kanban System Lite");
    app.setApplicationVersion("3.0");
    app.setOrganizationName("Universidade");
    app.setOrganizationDomain("universidade.edu");
    
    // Configurar estilo visual
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Paleta de cores personalizada
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    // Criar e mostrar janela principal
    kanban::gui::MainWindow mainWindow;
    mainWindow.show();
    
    return app.exec();
}