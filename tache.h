#ifndef TACHE_H
#define TACHE_H

#include <QList>
#include <QString>
#include <QStringList>

// Tâche à réaliser pendant l'événement, rattachée à une compétition.
class Tache
{
public:
    Tache();

    int id;
    QString titre;
    QString competition;
    QString competence;   // compétence demandée
    int heures;
    int idEmploye;        // 0 = non affectée

    QString code() const; // T-01

    bool ajouter();
    static bool supprimer(int id);
    static bool affecter(int idTache, int idEmploye);
    static bool toutDesaffecter();
    static QList<Tache> toutes();

    static QStringList listeCompetitions();
};

#endif // TACHE_H
