#include "tache.h"

#include <QSqlQuery>
#include <QVariant>

Tache::Tache()
    : id(0), heures(0), idEmploye(0)
{
}

QString Tache::code() const
{
    return QString("T-%1").arg(id, 2, 10, QChar('0'));
}

QStringList Tache::listeCompetitions()
{
    return QStringList() << "Hack for a Better Future" << "HealthTech Innovation"
                         << "Smart City Challenge" << "AgriTech for Tomorrow";
}

bool Tache::ajouter()
{
    QSqlQuery q;
    id = 1;
    if (q.exec("SELECT MAX(ID) FROM TACHE") && q.next())
        id = q.value(0).toInt() + 1;

    q.prepare("INSERT INTO TACHE (ID, TITRE, COMPETITION, COMPETENCE, HEURES, ID_EMPLOYE)"
              " VALUES (?, ?, ?, ?, ?, ?)");
    q.addBindValue(id);
    q.addBindValue(titre);
    q.addBindValue(competition);
    q.addBindValue(competence);
    q.addBindValue(heures);
    q.addBindValue(idEmploye);
    return q.exec();
}

bool Tache::supprimer(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM TACHE WHERE ID = ?");
    q.addBindValue(id);
    return q.exec();
}

bool Tache::affecter(int idTache, int idEmploye)
{
    QSqlQuery q;
    q.prepare("UPDATE TACHE SET ID_EMPLOYE = ? WHERE ID = ?");
    q.addBindValue(idEmploye);
    q.addBindValue(idTache);
    return q.exec();
}

bool Tache::toutDesaffecter()
{
    QSqlQuery q;
    return q.exec("UPDATE TACHE SET ID_EMPLOYE = 0");
}

QList<Tache> Tache::toutes()
{
    QList<Tache> liste;
    QSqlQuery q;
    q.exec("SELECT ID, TITRE, COMPETITION, COMPETENCE, HEURES, ID_EMPLOYE FROM TACHE ORDER BY ID");
    while (q.next()) {
        Tache t;
        t.id = q.value(0).toInt();
        t.titre = q.value(1).toString();
        t.competition = q.value(2).toString();
        t.competence = q.value(3).toString();
        t.heures = q.value(4).toInt();
        t.idEmploye = q.value(5).toInt();
        liste << t;
    }
    return liste;
}
