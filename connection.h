#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

// Connexion à la base de données.
// Par défaut : SQLite (fichier hacktime_employes.db créé à côté de l'exécutable).
// Pour Oracle, voir le commentaire dans connection.cpp et sql/script_oracle.sql.
class Connection
{
public:
    static bool ouvrir(QString *erreur = nullptr);
    static bool chargerDonneesDemo();

private:
    static bool creerTables();
};

#endif // CONNECTION_H
