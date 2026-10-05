-- HackTime - Module Gestion des Employés
-- À exécuter une seule fois si vous utilisez Oracle à la place de SQLite.
-- Les données de démonstration se chargent ensuite depuis l'application :
-- menu utilisateur (en haut à droite) > « Réinitialiser les données de démonstration ».

CREATE TABLE EMPLOYE (
    ID            NUMBER        PRIMARY KEY,
    NOM           VARCHAR2(40)  NOT NULL,
    PRENOM        VARCHAR2(40)  NOT NULL,
    EMAIL         VARCHAR2(80)  NOT NULL UNIQUE,
    POSTE         VARCHAR2(50)  NOT NULL,
    DEPARTEMENT   VARCHAR2(40)  NOT NULL,
    DATE_EMBAUCHE VARCHAR2(10)  NOT NULL,   -- format AAAA-MM-JJ
    STATUT        VARCHAR2(20)  NOT NULL,   -- Actif, En congé, Inactif
    COMPETENCES   VARCHAR2(200),            -- liste séparée par des virgules
    HEURES_DISPO  NUMBER        DEFAULT 20 NOT NULL
);

CREATE TABLE TACHE (
    ID          NUMBER        PRIMARY KEY,
    TITRE       VARCHAR2(80)  NOT NULL,
    COMPETITION VARCHAR2(60)  NOT NULL,
    COMPETENCE  VARCHAR2(40)  NOT NULL,
    HEURES      NUMBER        NOT NULL,
    ID_EMPLOYE  NUMBER        DEFAULT 0 NOT NULL  -- 0 = tâche non affectée
);

COMMIT;
