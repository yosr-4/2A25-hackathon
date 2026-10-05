#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QDate>
#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>

// Entité Employé : attributs du cahier des spécifications + méthodes CRUD
// et métiers de base (recherche, tri, statistiques).
class Employe
{
public:
    enum Tri { ParId, ParNom, ParPrenom, ParEmail, ParPoste, ParDepartement, ParDate, ParStatut };

    Employe();

    // Attributs
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString poste;
    QString departement;
    QDate dateEmbauche;
    QString statut;            // Actif, En congé, Inactif
    QStringList competences;   // utilisées par l'affectation intelligente
    int heuresDispo;           // heures disponibles pendant l'événement

    QString code() const;        // E-001
    QString nomComplet() const;  // Prénom Nom
    QString initiales() const;
    bool estActif() const;

    // CRUD
    bool ajouter(QString *erreur = nullptr);
    bool modifier(QString *erreur = nullptr);
    static bool supprimer(int id);
    static QList<Employe> tous();

    static int prochainId();
    static bool emailExiste(const QString &email, int saufId = 0);

    // Métiers de base
    static QList<Employe> filtrer(const QList<Employe> &liste, const QString &recherche,
                                  const QString &departement, const QString &poste,
                                  const QString &statut, Tri tri, bool croissant);
    static QList<QPair<QString, int> > statistiques(const QList<Employe> &liste, bool parPoste);
    static QStringList valeursDistinctes(const QList<Employe> &liste, bool postes);

    // Référentiels
    static QStringList listeDepartements();
    static QStringList listeStatuts();
    static QStringList listeCompetences();
    static QString codeDepuisId(int id);
    static QString sansAccents(const QString &texte);
};

#endif // EMPLOYE_H
