#include "affectation.h"

#include <QMap>
#include <algorithm>

int Affectation::charge(int idEmploye, const QList<Tache> &taches)
{
    int heures = 0;
    for (int i = 0; i < taches.size(); ++i) {
        if (taches.at(i).idEmploye == idEmploye)
            heures += taches.at(i).heures;
    }
    return heures;
}

bool Affectation::enSurcharge(const Employe &e, const QList<Tache> &taches)
{
    return charge(e.id, taches) > e.heuresDispo;
}

QList<Alerte> Affectation::alertes(const QList<Employe> &employes, const QList<Tache> &taches)
{
    QList<Alerte> liste;
    for (int i = 0; i < employes.size(); ++i) {
        const Employe &e = employes.at(i);
        const int heures = charge(e.id, taches);
        if (heures == 0)
            continue;
        Alerte a;
        a.idEmploye = e.id;
        if (!e.estActif()) {
            a.indisponible = true;
            a.message = QString("%1 est « %2 » mais a encore %3 h de tâches affectées.")
                            .arg(e.nomComplet(), e.statut).arg(heures);
            liste << a;
        } else if (heures > e.heuresDispo) {
            a.indisponible = false;
            a.message = QString("%1 est en surcharge : %2 h affectées pour %3 h disponibles.")
                            .arg(e.nomComplet()).arg(heures).arg(e.heuresDispo);
            liste << a;
        }
    }
    return liste;
}

namespace {

// Tâche en attente, avec le nombre d'employés capables de la prendre.
struct EnAttente {
    int index;        // position dans la liste des tâches
    int candidats;
    int heures;
};

// Les tâches les plus contraintes d'abord (peu de candidats), puis les plus longues.
bool plusContrainte(const EnAttente &a, const EnAttente &b)
{
    if (a.candidats != b.candidats)
        return a.candidats < b.candidats;
    if (a.heures != b.heures)
        return a.heures > b.heures;
    return a.index < b.index;
}

bool peutPrendre(const Employe &e, const Tache &t)
{
    return e.estActif() && e.competences.contains(t.competence);
}

} // namespace

// Algorithme d'affectation.
//  1. Les tâches qui ont le moins de candidats passent en premier.
//  2. Candidats : employés actifs qui ont la compétence demandée et dont la
//     charge, après affectation, ne dépasse pas les heures disponibles.
//  3. Choix : le taux de charge le plus bas après affectation. À égalité,
//     l'employé le moins polyvalent, pour garder les profils polyvalents libres.
QList<LigneRapport> Affectation::repartir(const QList<Employe> &employes, QList<Tache> &taches)
{
    QMap<int, int> heuresPrises;
    for (int i = 0; i < employes.size(); ++i)
        heuresPrises[employes.at(i).id] = charge(employes.at(i).id, taches);

    QList<EnAttente> attente;
    for (int i = 0; i < taches.size(); ++i) {
        if (taches.at(i).idEmploye != 0)
            continue;
        EnAttente a;
        a.index = i;
        a.heures = taches.at(i).heures;
        a.candidats = 0;
        for (int j = 0; j < employes.size(); ++j) {
            if (peutPrendre(employes.at(j), taches.at(i)))
                a.candidats += 1;
        }
        attente << a;
    }
    std::sort(attente.begin(), attente.end(), plusContrainte);

    QList<LigneRapport> rapport;
    for (int k = 0; k < attente.size(); ++k) {
        Tache &t = taches[attente.at(k).index];
        LigneRapport ligne;
        ligne.tache = t;
        ligne.affectee = false;
        ligne.pourcentage = 0;

        if (attente.at(k).candidats == 0) {
            ligne.raison = QString("aucun employé actif n'a la compétence « %1 »").arg(t.competence);
            rapport << ligne;
            continue;
        }

        int meilleur = -1;
        double meilleurTaux = 0.0;
        for (int j = 0; j < employes.size(); ++j) {
            const Employe &e = employes.at(j);
            if (!peutPrendre(e, t))
                continue;
            const int apres = heuresPrises.value(e.id) + t.heures;
            if (e.heuresDispo <= 0 || apres > e.heuresDispo)
                continue;
            const double taux = double(apres) / double(e.heuresDispo);
            const bool mieux = meilleur < 0
                || taux < meilleurTaux - 1e-9
                || (taux < meilleurTaux + 1e-9
                    && e.competences.size() < employes.at(meilleur).competences.size());
            if (mieux) {
                meilleur = j;
                meilleurTaux = taux;
            }
        }

        if (meilleur < 0) {
            ligne.raison = "les employés compétents n'ont plus assez d'heures disponibles";
            rapport << ligne;
            continue;
        }

        const Employe &choisi = employes.at(meilleur);
        t.idEmploye = choisi.id;
        heuresPrises[choisi.id] += t.heures;
        Tache::affecter(t.id, choisi.id);

        ligne.tache = t;
        ligne.affectee = true;
        ligne.employe = choisi.nomComplet();
        ligne.pourcentage = int(meilleurTaux * 100.0 + 0.5);
        rapport << ligne;
    }
    return rapport;
}

QList<LigneRapport> Affectation::affecterAutomatiquement(bool toutReaffecter)
{
    const QList<Employe> employes = Employe::tous();
    QList<Tache> taches = Tache::toutes();
    if (toutReaffecter) {
        Tache::toutDesaffecter();
        for (int i = 0; i < taches.size(); ++i)
            taches[i].idEmploye = 0;
    }
    return repartir(employes, taches);
}

QList<LigneRapport> Affectation::reequilibrer()
{
    const QList<Employe> employes = Employe::tous();
    QList<Tache> taches = Tache::toutes();
    const QList<Alerte> liste = alertes(employes, taches);

    for (int a = 0; a < liste.size(); ++a) {
        const Alerte &alerte = liste.at(a);
        int dispo = 0;
        for (int i = 0; i < employes.size(); ++i) {
            if (employes.at(i).id == alerte.idEmploye)
                dispo = employes.at(i).heuresDispo;
        }

        if (alerte.indisponible) {
            // Employé absent : toutes ses tâches sont libérées.
            for (int i = 0; i < taches.size(); ++i) {
                if (taches.at(i).idEmploye == alerte.idEmploye) {
                    taches[i].idEmploye = 0;
                    Tache::affecter(taches.at(i).id, 0);
                }
            }
            continue;
        }

        // Surcharge : on retire la plus petite tâche qui suffit à repasser
        // sous la limite ; s'il n'y en a pas, la plus longue, et on recommence.
        int garde = 0;
        while (charge(alerte.idEmploye, taches) > dispo && garde++ < 100) {
            const int exces = charge(alerte.idEmploye, taches) - dispo;
            int choix = -1;
            int plusLongue = -1;
            for (int i = 0; i < taches.size(); ++i) {
                const Tache &t = taches.at(i);
                if (t.idEmploye != alerte.idEmploye)
                    continue;
                if (plusLongue < 0 || t.heures > taches.at(plusLongue).heures)
                    plusLongue = i;
                if (t.heures >= exces && (choix < 0 || t.heures < taches.at(choix).heures))
                    choix = i;
            }
            if (choix < 0)
                choix = plusLongue;
            if (choix < 0)
                break;
            taches[choix].idEmploye = 0;
            Tache::affecter(taches.at(choix).id, 0);
        }
    }
    return repartir(employes, taches);
}
