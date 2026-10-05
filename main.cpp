#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QIcon>
#include <QMessageBox>
#include <QPalette>

// Palette claire imposée : l'interface garde ses couleurs même si
// Windows est réglé en mode sombre.
static QPalette paletteClaire()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor("#edf1f9"));
    p.setColor(QPalette::WindowText, QColor("#0f1a38"));
    p.setColor(QPalette::Base, QColor("#ffffff"));
    p.setColor(QPalette::AlternateBase, QColor("#f3f6fc"));
    p.setColor(QPalette::Text, QColor("#0f1a38"));
    p.setColor(QPalette::Button, QColor("#ffffff"));
    p.setColor(QPalette::ButtonText, QColor("#0f1a38"));
    p.setColor(QPalette::ToolTipBase, QColor("#0f1a38"));
    p.setColor(QPalette::ToolTipText, QColor("#ffffff"));
    p.setColor(QPalette::Highlight, QColor("#1a5ae8"));
    p.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    p.setColor(QPalette::Disabled, QPalette::Text, QColor("#9aa4bd"));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#9aa4bd"));
    p.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#9aa4bd"));
    return p;
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    QApplication app(argc, argv);
    app.setApplicationName("HackTime");
    app.setStyle("Fusion");
    app.setPalette(paletteClaire());
    app.setFont(QFont("Segoe UI", 10));
    app.setWindowIcon(QIcon(":/resources/logo_icone.png"));

    QFile qss(":/resources/style.qss");
    if (qss.open(QIODevice::ReadOnly))
        app.setStyleSheet(QString::fromUtf8(qss.readAll()));

    QString erreur;
    if (!Connection::ouvrir(&erreur)) {
        QMessageBox::critical(nullptr, "HackTime",
                              "Connexion à la base de données impossible.\n\n" + erreur);
        return 1;
    }

    MainWindow fenetre;
    fenetre.showMaximized();
    return app.exec();
}
