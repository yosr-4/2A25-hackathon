#ifndef AFFECTATION_H
#define AFFECTATION_H

#include "employe.h"
#include "tache.h"

#include <QList>
#include <QString>

// Métiers innovants du module :
//  1. suivi automatique de la charge de travail, avec alerte en cas de surcharge ;
//  2. affectation intelligente des employés aux tâches selon leurs compétences
//     et leur disponibilité.

struct Alerte
{
    int idEmploye;
    bool indisponible;   // l'employé n'est pas actif mais a des tâches
    QString message;
};

struct LigneRapport
{
    Tache tache;
    bool affectee;
    QString employe;     // nom de l'employé retenu
    int pourcentage;     // charge de l'employé après affectation
    QString raison;      // explication si la tâche reste sans solution
};

class Affectation
{
public:
    // Heures de tâches affectées à un employé.
    static int charge(int idEmploye, const QList<Tache> &taches);
    static bool enSurcharge(const Employe &e, const QList<Tache> &taches);
    static QList<Alerte> alertes(const QList<Employe> &employes, const QList<Tache> &taches);

    // Affecte les tâches libres (ou toutes les tâches si toutReaffecter est vrai).
    static QList<LigneRapport> affecterAutomatiquement(bool toutReaffecter);

    // Retire aux employés en alerte juste ce qu'il faut de tâches, puis les réaffecte.
    static QList<LigneRapport> reequilibrer();

private:
    static QList<LigneRapport> repartir(const QList<Employe> &employes, QList<Tache> &taches);
};

#endif // AFFECTATION_H
