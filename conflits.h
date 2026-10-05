// Détection automatique des conflits de planning.
// Règle : deux compétitions sont en conflit si leurs dates se chevauchent ET
// qu'elles utilisent la même salle ou le même jury.
#ifndef CONFLITS_H
#define CONFLITS_H

#include <QDate>
#include <QString>
#include <QStringList>
#include <QVector>
#include "competition.h"

// Une salle « À définir » ou « En ligne » ne peut pas être en conflit.
inline bool salleReservable(const QString &salle) {
    return !salle.isEmpty() && salle != NON_DEFINI && salle != EN_LIGNE;
}
inline bool juryAffecte(const QString &jury) {
    return !jury.isEmpty() && jury != NON_DEFINI;
}

// Deux périodes [début, fin] se chevauchent si chacune commence avant la fin de l'autre.
inline bool datesSeChevauchent(const Comp &a, const Comp &b) {
    const QDate ad = QDate::fromString(a.debut, DATE_FMT), af = QDate::fromString(a.fin, DATE_FMT);
    const QDate bd = QDate::fromString(b.debut, DATE_FMT), bf = QDate::fromString(b.fin, DATE_FMT);
    if (!ad.isValid() || !af.isValid() || !bd.isValid() || !bf.isValid()) return false;
    return ad <= bf && bd <= af;
}

inline bool memeSalle(const Comp &a, const Comp &b) {
    return salleReservable(a.salle) && a.salle.compare(b.salle, Qt::CaseInsensitive) == 0;
}
inline bool memeJury(const Comp &a, const Comp &b) {
    return juryAffecte(a.jury) && a.jury.compare(b.jury, Qt::CaseInsensitive) == 0;
}

// Conflits d'UNE compétition avec toutes les autres (une phrase par conflit).
// Sert avant d'enregistrer un ajout ou une modification, et pour l'alerte du tableau.
inline QStringList conflitsDe(const Comp &c, const QVector<Comp> &toutes) {
    QStringList out;
    for (const Comp &o : toutes) {
        if (o.id == c.id || !datesSeChevauchent(c, o)) continue;
        const QString autre = QString("%1 « %2 » (du %3 au %4)").arg(o.id, o.nom, o.debut, o.fin);
        if (memeSalle(c, o))
            out << QString("Salle « %1 » déjà réservée par %2").arg(c.salle, autre);
        if (memeJury(c, o))
            out << QString("« %1 » déjà affecté à %2").arg(c.jury, autre);
    }
    return out;
}

// Tous les conflits du planning (chaque paire de compétitions n'est citée qu'une fois).
inline QStringList tousLesConflits(const QVector<Comp> &toutes) {
    QStringList out;
    for (int i = 0; i < toutes.size(); ++i) {
        for (int j = i + 1; j < toutes.size(); ++j) {
            const Comp &a = toutes[i], &b = toutes[j];
            if (!datesSeChevauchent(a, b)) continue;
            const QString paire = QString("%1 « %2 » et %3 « %4 »").arg(a.id, a.nom, b.id, b.nom);
            if (memeSalle(a, b))
                out << QString("%1 : même salle (%2)").arg(paire, a.salle);
            if (memeJury(a, b))
                out << QString("%1 : même jury (%2)").arg(paire, a.jury);
        }
    }
    return out;
}

#endif // CONFLITS_H
