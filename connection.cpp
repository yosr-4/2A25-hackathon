#include "connection.h"

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

bool Connection::ouvrir(QString *erreur)
{
    // ---- SQLite (par défaut : aucun réglage à faire) -------------------
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/hacktime_employes.db");

    // ---- Oracle : remplacer les deux lignes ci-dessus par ---------------
    // QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    // db.setDatabaseName("Source_Projet2A");   // nom de la source ODBC
    // db.setUserName("utilisateur");
    // db.setPassword("mot_de_passe");
    // puis exécuter sql/script_oracle.sql une fois dans SQL Developer.

    if (!db.open()) {
        if (erreur)
            *erreur = db.lastError().text();
        return false;
    }

    if (db.driverName() == "QSQLITE") {
        const bool nouvelleBase = !db.tables().contains("EMPLOYE", Qt::CaseInsensitive);
        if (!creerTables()) {
            if (erreur)
                *erreur = db.lastError().text();
            return false;
        }
        if (nouvelleBase)
            chargerDonneesDemo();
    }
    return true;
}

bool Connection::creerTables()
{
    QSqlQuery q;
    bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS EMPLOYE ("
        " ID INTEGER PRIMARY KEY,"
        " NOM VARCHAR(40) NOT NULL,"
        " PRENOM VARCHAR(40) NOT NULL,"
        " EMAIL VARCHAR(80) NOT NULL UNIQUE,"
        " POSTE VARCHAR(50) NOT NULL,"
        " DEPARTEMENT VARCHAR(40) NOT NULL,"
        " DATE_EMBAUCHE VARCHAR(10) NOT NULL,"
        " STATUT VARCHAR(20) NOT NULL,"
        " COMPETENCES VARCHAR(200),"
        " HEURES_DISPO INTEGER DEFAULT 20 NOT NULL)");
    ok = ok && q.exec(
        "CREATE TABLE IF NOT EXISTS TACHE ("
        " ID INTEGER PRIMARY KEY,"
        " TITRE VARCHAR(80) NOT NULL,"
        " COMPETITION VARCHAR(60) NOT NULL,"
        " COMPETENCE VARCHAR(40) NOT NULL,"
        " HEURES INTEGER NOT NULL,"
        " ID_EMPLOYE INTEGER DEFAULT 0 NOT NULL)");
    return ok;
}

namespace {

struct EmployeDemo {
    int id;
    const char *nom, *prenom, *email, *poste, *departement, *date, *statut, *competences;
    int heures;
};

struct TacheDemo {
    int id;
    const char *titre, *competition, *competence;
    int heures, idEmploye;
};

const EmployeDemo EMPLOYES[] = {
    {1, "Ben Salah", "Amine", "amine.bensalah@hacktime.tn", "Responsable logistique", "Logistique", "2021-03-15", "Actif", "Logistique,Sécurité", 24},
    {2, "Trabelsi", "Ines", "ines.trabelsi@hacktime.tn", "Chargée de communication", "Communication", "2022-09-01", "Actif", "Communication,Audiovisuel", 20},
    {3, "Gharbi", "Youssef", "youssef.gharbi@hacktime.tn", "Technicien réseau", "Technique", "2020-01-20", "Actif", "Réseau,Support technique", 24},
    {4, "Mansouri", "Salma", "salma.mansouri@hacktime.tn", "Agent d'accueil", "Accueil", "2023-06-12", "Actif", "Accueil,Communication", 18},
    {5, "Jebali", "Karim", "karim.jebali@hacktime.tn", "Mentor technique", "Technique", "2019-11-04", "Actif", "Mentorat,Support technique", 20},
    {6, "Hammami", "Nour", "nour.hammami@hacktime.tn", "Chargée de recrutement", "Ressources humaines", "2018-05-02", "Actif", "Accueil,Logistique", 16},
    {7, "Chaabane", "Mehdi", "mehdi.chaabane@hacktime.tn", "Technicien réseau", "Technique", "2024-02-19", "Actif", "Réseau,Sécurité", 24},
    {8, "Bouazizi", "Rania", "rania.bouazizi@hacktime.tn", "Comptable", "Finance", "2021-10-11", "En congé", "Logistique", 16},
    {9, "Khelifi", "Omar", "omar.khelifi@hacktime.tn", "Agent de sécurité", "Logistique", "2022-04-25", "Actif", "Sécurité,Accueil", 24},
    {10, "Sassi", "Leila", "leila.sassi@hacktime.tn", "Technicienne audiovisuel", "Communication", "2023-01-09", "Actif", "Audiovisuel,Support technique", 20},
    {11, "Dridi", "Hatem", "hatem.dridi@hacktime.tn", "Agent d'accueil", "Accueil", "2025-03-03", "Actif", "Accueil", 18},
    {12, "Ayari", "Sarra", "sarra.ayari@hacktime.tn", "Mentor technique", "Technique", "2024-09-16", "Actif", "Mentorat", 16},
    {13, "Zouari", "Fares", "fares.zouari@hacktime.tn", "Agent logistique", "Logistique", "2020-07-27", "Inactif", "Logistique", 20},
    {14, "Mejri", "Yasmine", "yasmine.mejri@hacktime.tn", "Chargée de communication", "Communication", "2026-01-12", "Actif", "Communication", 20}
};

const TacheDemo TACHES[] = {
    {1, "Installer le réseau Wi-Fi des salles", "Hack for a Better Future", "Réseau", 10, 3},
    {2, "Surveiller la connexion pendant les épreuves", "HealthTech Innovation", "Réseau", 12, 3},
    {3, "Dépanner les postes des équipes", "Hack for a Better Future", "Support technique", 8, 3},
    {4, "Accueillir et enregistrer les participants", "Hack for a Better Future", "Accueil", 8, 4},
    {5, "Encadrer les équipes en mentorat", "HealthTech Innovation", "Mentorat", 12, 5},
    {6, "Préparer les salles et le matériel", "Hack for a Better Future", "Logistique", 10, 1},
    {7, "Contrôler les accès et les badges", "Smart City Challenge", "Sécurité", 12, 0},
    {8, "Animer les réseaux sociaux", "HealthTech Innovation", "Communication", 8, 2},
    {9, "Filmer la cérémonie de clôture", "Smart City Challenge", "Audiovisuel", 6, 0},
    {10, "Encadrer les équipes en mentorat", "Smart City Challenge", "Mentorat", 10, 0},
    {11, "Tenir le point d'information", "AgriTech for Tomorrow", "Accueil", 10, 0},
    {12, "Gérer la distribution des repas", "AgriTech for Tomorrow", "Logistique", 8, 0},
    {13, "Assurer le support technique de nuit", "Smart City Challenge", "Support technique", 8, 0}
};

} // namespace

bool Connection::chargerDonneesDemo()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery q;
    bool ok = q.exec("DELETE FROM TACHE");
    ok = ok && q.exec("DELETE FROM EMPLOYE");

    const int nbEmployes = int(sizeof(EMPLOYES) / sizeof(EMPLOYES[0]));
    for (int i = 0; ok && i < nbEmployes; ++i) {
        const EmployeDemo &e = EMPLOYES[i];
        q.prepare("INSERT INTO EMPLOYE (ID, NOM, PRENOM, EMAIL, POSTE, DEPARTEMENT,"
                  " DATE_EMBAUCHE, STATUT, COMPETENCES, HEURES_DISPO)"
                  " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(e.id);
        q.addBindValue(QString::fromUtf8(e.nom));
        q.addBindValue(QString::fromUtf8(e.prenom));
        q.addBindValue(QString::fromUtf8(e.email));
        q.addBindValue(QString::fromUtf8(e.poste));
        q.addBindValue(QString::fromUtf8(e.departement));
        q.addBindValue(QString::fromUtf8(e.date));
        q.addBindValue(QString::fromUtf8(e.statut));
        q.addBindValue(QString::fromUtf8(e.competences));
        q.addBindValue(e.heures);
        ok = q.exec();
    }

    const int nbTaches = int(sizeof(TACHES) / sizeof(TACHES[0]));
    for (int i = 0; ok && i < nbTaches; ++i) {
        const TacheDemo &t = TACHES[i];
        q.prepare("INSERT INTO TACHE (ID, TITRE, COMPETITION, COMPETENCE, HEURES, ID_EMPLOYE)"
                  " VALUES (?, ?, ?, ?, ?, ?)");
        q.addBindValue(t.id);
        q.addBindValue(QString::fromUtf8(t.titre));
        q.addBindValue(QString::fromUtf8(t.competition));
        q.addBindValue(QString::fromUtf8(t.competence));
        q.addBindValue(t.heures);
        q.addBindValue(t.idEmploye);
        ok = q.exec();
    }

    if (ok)
        db.commit();
    else
        db.rollback();
    return ok;
}
