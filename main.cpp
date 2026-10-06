#include "hackathon.h"
#include "mainwindow.h"
#include "connection.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QPalette>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(0xF8FAFC));
    lightPalette.setColor(QPalette::WindowText, QColor(0x0F172A));
    lightPalette.setColor(QPalette::Base, QColor(0xFFFFFF));
    lightPalette.setColor(QPalette::AlternateBase, QColor(0xF1F5F9));
    lightPalette.setColor(QPalette::Text, QColor(0x1E293B));
    lightPalette.setColor(QPalette::Button, QColor(0xFFFFFF));
    lightPalette.setColor(QPalette::ButtonText, QColor(0x1E293B));
    lightPalette.setColor(QPalette::Highlight, QColor(0x2563EB));
    lightPalette.setColor(QPalette::HighlightedText, QColor(0xFFFFFF));
    a.setPalette(lightPalette);

    QString erreur;
    if (!Connection::ouvrir(&erreur)) {
        QMessageBox::critical(nullptr, "HackTime",
                              "Connexion à la base de données impossible.\n\n" + erreur);
        return 1;
    }

    hackathon HT;

    QFile f(":/style.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    HT.setWindowIcon(QIcon(":/images/icon.png"));

    HT.show();
    return a.exec();
}
