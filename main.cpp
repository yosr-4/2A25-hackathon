#include "hackathon.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
