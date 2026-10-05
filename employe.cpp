#include "employe.h"

#include <QMap>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <algorithm>

Employe::Employe()
    : id(0), statut("Actif"), heuresDispo(20)
{
}

QString Employe::codeDepuisId(int id)
{
    return QString("E-%1").arg(id, 3, 10, QChar('0'));
}

QString Employe::code() const
{
    return codeDepuisId(id);
}

QString Employe::nomComplet() const
{
    return prenom + " " + nom;
}

QString Employe::initiales() const
{
    QString s;
    if (!prenom.isEmpty())
        s += prenom.at(0).toUpper();
    if (!nom.isEmpty())
        s += nom.at(0).toUpper();
    return s;
}

bool Employe::estActif() const
{
    return statut == "Actif";
}

QStringList Employe::listeDepartements()
{
    return QStringList() << "Technique" << "Logistique" << "Communication"
                         << "Accueil" << "Ressources humaines" << "Finance";
}

QStringList Employe::listeStatuts()
{
    return QStringList() << "Actif" << "En congé" << "Inactif";
}

QStringList Employe::listeCompetences()
{
    return QStringList() << "Réseau" << "Support technique" << "Mentorat" << "Logistique"
                         << "Sécurité" << "Accueil" << "Communication" << "Audiovisuel";
}

// Minuscules sans accents : « Réseau » -> « reseau » (recherche tolérante).
QString Employe::sansAccents(const QString &texte)
{
    const QString decompose = texte.normalized(QString::NormalizationForm_D);
    QString resultat;
    for (int i = 0; i < decompose.size(); ++i) {
        const QChar c = decompose.at(i);
        if (c.category() != QChar::Mark_NonSpacing)
            resultat += c.toLower();
    }
    return resultat.trimmed();
}

// ---------------------------------------------------------------- CRUD

int Employe::prochainId()
{
    QSqlQuery q;
    if (q.exec("SELECT MAX(ID) FROM EMPLOYE") && q.next())
        return q.value(0).toInt() + 1;
    return 1;
}

bool Employe::emailExiste(const QString &email, int saufId)
{
    QSqlQuery q;
    q.prepare("SELECT COUNT(*) FROM EMPLOYE WHERE LOWER(EMAIL) = ? AND ID <> ?");
    q.addBindValue(email.trimmed().toLower());
    q.addBindValue(saufId);
    if (q.exec() && q.next())
        return q.value(0).toInt() > 0;
    return false;
}

bool Employe::ajouter(QString *erreur)
{
    id = prochainId();
    QSqlQuery q;
    q.prepare("INSERT INTO EMPLOYE (ID, NOM, PRENOM, EMAIL, POSTE, DEPARTEMENT,"
              " DATE_EMBAUCHE, STATUT, COMPETENCES, HEURES_DISPO)"
              " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    q.addBindValue(id);
    q.addBindValue(nom);
    q.addBindValue(prenom);
    q.addBindValue(email);
    q.addBindValue(poste);
    q.addBindValue(departement);
    q.addBindValue(dateEmbauche.toString("yyyy-MM-dd"));
    q.addBindValue(statut);
    q.addBindValue(competences.join(","));
    q.addBindValue(heuresDispo);
    if (!q.exec()) {
        if (erreur)
            *erreur = q.lastError().text();
        return false;
    }
    return true;
}

bool Employe::modifier(QString *erreur)
{
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYE SET NOM = ?, PRENOM = ?, EMAIL = ?, POSTE = ?, DEPARTEMENT = ?,"
              " DATE_EMBAUCHE = ?, STATUT = ?, COMPETENCES = ?, HEURES_DISPO = ?"
              " WHERE ID = ?");
    q.addBindValue(nom);
    q.addBindValue(prenom);
    q.addBindValue(email);
    q.addBindValue(poste);
    q.addBindValue(departement);
    q.addBindValue(dateEmbauche.toString("yyyy-MM-dd"));
    q.addBindValue(statut);
    q.addBindValue(competences.join(","));
    q.addBindValue(heuresDispo);
    q.addBindValue(id);
    if (!q.exec()) {
        if (erreur)
            *erreur = q.lastError().text();
        return false;
    }
    return true;
}

bool Employe::supprimer(int id)
{
    // Les tâches de l'employé supprimé redeviennent « non affectées ».
    QSqlQuery q;
    q.prepare("UPDATE TACHE SET ID_EMPLOYE = 0 WHERE ID_EMPLOYE = ?");
    q.addBindValue(id);
    q.exec();

    q.prepare("DELETE FROM EMPLOYE WHERE ID = ?");
    q.addBindValue(id);
    return q.exec();
}

QList<Employe> Employe::tous()
{
    QList<Employe> liste;
    QSqlQuery q;
    q.exec("SELECT ID, NOM, PRENOM, EMAIL, POSTE, DEPARTEMENT, DATE_EMBAUCHE, STATUT,"
           " COMPETENCES, HEURES_DISPO FROM EMPLOYE ORDER BY ID");
    while (q.next()) {
        Employe e;
        e.id = q.value(0).toInt();
        e.nom = q.value(1).toString();
        e.prenom = q.value(2).toString();
        e.email = q.value(3).toString();
        e.poste = q.value(4).toString();
        e.departement = q.value(5).toString();
        e.dateEmbauche = QDate::fromString(q.value(6).toString(), "yyyy-MM-dd");
        e.statut = q.value(7).toString();
        const QStringList morceaux = q.value(8).toString().split(',');
        for (int i = 0; i < morceaux.size(); ++i) {
            const QString c = morceaux.at(i).trimmed();
            if (!c.isEmpty())
                e.competences << c;
        }
        e.heuresDispo = q.value(9).toInt();
        liste << e;
    }
    return liste;
}

// ------------------------------------------------------ Métiers de base

namespace {

int comparerTexte(const QString &a, const QString &b)
{
    return QString::localeAwareCompare(a.toLower(), b.toLower());
}

int comparer(const Employe &a, const Employe &b, Employe::Tri tri)
{
    switch (tri) {
    case Employe::ParNom:         return comparerTexte(a.nom, b.nom);
    case Employe::ParPrenom:      return comparerTexte(a.prenom, b.prenom);
    case Employe::ParEmail:       return comparerTexte(a.email, b.email);
    case Employe::ParPoste:       return comparerTexte(a.poste, b.poste);
    case Employe::ParDepartement: return comparerTexte(a.departement, b.departement);
    case Employe::ParStatut:      return comparerTexte(a.statut, b.statut);
    case Employe::ParDate:
        if (a.dateEmbauche == b.dateEmbauche)
            return 0;
        return a.dateEmbauche < b.dateEmbauche ? -1 : 1;
    case Employe::ParId:
    default:
        return a.id - b.id;
    }
}

struct OrdreEmploye {
    Employe::Tri tri;
    bool croissant;
    bool operator()(const Employe &a, const Employe &b) const
    {
        int c = comparer(a, b, tri);
        if (c == 0)
            c = a.id - b.id;
        return croissant ? c < 0 : c > 0;
    }
};

bool statPlusGrande(const QPair<QString, int> &a, const QPair<QString, int> &b)
{
    if (a.second != b.second)
        return a.second > b.second;
    return QString::localeAwareCompare(a.first, b.first) < 0;
}

bool texteAvant(const QString &a, const QString &b)
{
    return QString::localeAwareCompare(a, b) < 0;
}

} // namespace

// Recherche (nom, prénom ou poste) + filtres + tri.
QList<Employe> Employe::filtrer(const QList<Employe> &liste, const QString &recherche,
                                const QString &departement, const QString &poste,
                                const QString &statut, Tri tri, bool croissant)
{
    const QString q = sansAccents(recherche);
    QList<Employe> resultat;
    for (int i = 0; i < liste.size(); ++i) {
        const Employe &e = liste.at(i);
        if (!departement.isEmpty() && e.departement != departement)
            continue;
        if (!poste.isEmpty() && e.poste != poste)
            continue;
        if (!statut.isEmpty() && e.statut != statut)
            continue;
        if (!q.isEmpty()) {
            const QString cible = sansAccents(e.nom + " " + e.prenom + " " + e.nom + " " + e.poste);
            if (!cible.contains(q))
                continue;
        }
        resultat << e;
    }
    OrdreEmploye ordre;
    ordre.tri = tri;
    ordre.croissant = croissant;
    std::sort(resultat.begin(), resultat.end(), ordre);
    return resultat;
}

// Nombre d'employés par département ou par poste, du plus grand au plus petit.
QList<QPair<QString, int> > Employe::statistiques(const QList<Employe> &liste, bool parPoste)
{
    QMap<QString, int> compte;
    for (int i = 0; i < liste.size(); ++i) {
        const Employe &e = liste.at(i);
        compte[parPoste ? e.poste : e.departement] += 1;
    }
    QList<QPair<QString, int> > resultat;
    for (QMap<QString, int>::const_iterator it = compte.constBegin(); it != compte.constEnd(); ++it)
        resultat << qMakePair(it.key(), it.value());
    std::sort(resultat.begin(), resultat.end(), statPlusGrande);
    return resultat;
}

QStringList Employe::valeursDistinctes(const QList<Employe> &liste, bool postes)
{
    QStringList valeurs;
    for (int i = 0; i < liste.size(); ++i) {
        const QString v = postes ? liste.at(i).poste : liste.at(i).departement;
        if (!v.isEmpty() && !valeurs.contains(v))
            valeurs << v;
    }
    std::sort(valeurs.begin(), valeurs.end(), texteAvant);
    return valeurs;
}
