#include "hackathon.h"
#include "connection.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Base de données du module Employés (SQLite par défaut, voir connection.cpp).
    // Ouverte avant la fenêtre : la page Employés lit la base dès sa création.
    QString erreur;
    if (!Connection::ouvrir(&erreur)) {
        QMessageBox::critical(nullptr, "HackTime",
                              "Connexion à la base de données impossible.\n\n" + erreur);
        return 1;
    }

    hackathon HT;

    // Charger le style depuis les ressources
    QFile f(":/style.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    // Mettre le logo comme icône de la fenêtre
    HT.setWindowIcon(QIcon(":/images/icon.png"));

    HT.show();
    return QApplication::exec();
}
