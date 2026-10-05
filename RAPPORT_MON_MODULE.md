# Module : Gestion des Employés

> Projet : Smart Hackathon Management — application « HackTime » (équipe Elite Hackers)
> Technologie : C++ / Qt (Qt Widgets + Qt SQL + Qt PrintSupport), interface décrite dans Qt Designer (`gestionemployes.ui`).
> Projet Qt commun : `hackathon.pro` (kit Qt 6.12.0 MinGW 64-bit, qmake). Le module est intégré comme page `PEmp` de la fenêtre principale `hackathon` (pile `SWHackTime`), à côté du module Gestion des Compétitions.
> Ce document est fondé uniquement sur le code présent dans le dossier du module.

---

## 1. Fichiers du module

| Fichier | Rôle |
|---|---|
| `employe.h` / `employe.cpp` | Classe entité `Employe` : attributs, CRUD (`ajouter`, `modifier`, `supprimer`, `tous`), contrôle d'unicité de l'e-mail (`emailExiste`), recherche + filtres + tri (`filtrer`), statistiques (`statistiques`), référentiels (`listeDepartements`, `listeStatuts`, `listeCompetences`), recherche sans accents (`sansAccents`). |
| `tache.h` / `tache.cpp` | Classe `Tache` : tâches de l'événement rattachées à une compétition (`ajouter`, `supprimer`, `affecter`, `toutDesaffecter`, `toutes`, `listeCompetitions`). Support des métiers innovants. |
| `affectation.h` / `affectation.cpp` | Classe `Affectation` + structures `Alerte` et `LigneRapport` : métiers innovants (calcul de charge, détection de surcharge, alertes, algorithme d'affectation intelligente, rééquilibrage automatique). |
| `statswidget.h` / `statswidget.cpp` | Widget personnalisé `StatsWidget` (hérite de `QWidget`) : graphique de statistiques dessiné avec `QPainter` (barres horizontales + anneau de répartition). Promu dans le `.ui` (`<customwidget>`). |
| `gestionemployes.h` / `gestionemployes.cpp` | Classe `GestionEmployes` (hérite de `QWidget`, page `PEmp` de la fenêtre principale `hackathon`) : logique de l'interface (signaux/slots, remplissage des tableaux, pagination, contrôles de saisie, exports PDF/CSV, notifications, menus) ; applique la feuille de style et la palette claire du module à sa page uniquement ; signal `navigationDemandee` pour la barre latérale. |
| `gestionemployes.ui` | Interface graphique complète (Qt Designer) : barre latérale, barre du haut, en-tête, indicateurs, 4 onglets, carte « Détails », formulaire. |
| `connection.h` / `connection.cpp` | Classe `Connection` : ouverture de la base (`ouvrir`), création des tables SQLite (`creerTables`), chargement des données de démonstration (`chargerDonneesDemo`). Bloc Oracle/ODBC fourni en commentaire. |
| `main.cpp` (commun) | Point d'entrée de l'application intégrée : ouverture de la connexion BD (`Connection::ouvrir`) avant la création de la fenêtre `hackathon`, chargement de la feuille de style globale `style.qss`, icône. |
| `resources.qrc` | Fichier de ressources Qt (préfixe `/`). |
| `resources/style.qss` | Feuille de style (couleurs, boutons, cartes, onglets, tableaux, menus, barres de défilement). |
| `resources/logo_sidebar.png`, `resources/logo_icone.png` | Logo HackTime (barre latérale) et icône de la fenêtre. |
| `resources/fleche_bas.png`, `resources/fleche_haut.png` | Flèches des listes déroulantes et des `QSpinBox`. |
| `sql/script_oracle.sql` | Script `CREATE TABLE` pour Oracle (tables `EMPLOYE` et `TACHE`). |
| `LISEZ-MOI.txt` | Notice d'ouverture et de configuration (SQLite par défaut, procédure Oracle). |
| `hackathon.pro` (commun) | Projet qmake commun : `QT += widgets sql printsupport`, sources des deux modules, `hackathon.ui` + `gestionemployes.ui`, `ressources.qrc` + `resources.qrc`. |

---

## 2. Entité et attributs

- **Classe C++ :** `Employe` (`employe.h`)
- **Table en base :** `EMPLOYE`
- **Table liée (même module) :** `TACHE` (classe `Tache`)

### 2.1 Attributs de `EMPLOYE`

| Attribut (C++) | Colonne SQL | Type C++ | Type SQL (Oracle / SQLite) | Contraintes | Contrôle de saisie dans l'UI |
|---|---|---|---|---|---|
| `id` | `ID` | `int` | `NUMBER` / `INTEGER` | PK | Champ `fId` en lecture seule ; valeur générée par `Employe::prochainId()` (`SELECT MAX(ID) FROM EMPLOYE` + 1) ; affichée au format `E-001` (`codeDepuisId`). |
| `nom` | `NOM` | `QString` | `VARCHAR2(40)` / `VARCHAR(40)` | NOT NULL | `fNom` : `maxLength` = 40 ; obligatoire (« Saisissez le nom. ») ; espaces supprimés (`trimmed()`). |
| `prenom` | `PRENOM` | `QString` | `VARCHAR2(40)` / `VARCHAR(40)` | NOT NULL | `fPrenom` : `maxLength` = 40 ; obligatoire (« Saisissez le prénom. »). |
| `email` | `EMAIL` | `QString` | `VARCHAR2(80)` / `VARCHAR(80)` | NOT NULL, UNIQUE | `fEmail` : `maxLength` = 80 ; obligatoire ; expression régulière `^[^\s@]+@[^\s@]+\.[^\s@]{2,}$` ; unicité vérifiée par `Employe::emailExiste()` (comparaison en minuscules, en excluant l'employé en cours de modification) ; converti en minuscules avant enregistrement. |
| `poste` | `POSTE` | `QString` | `VARCHAR2(50)` / `VARCHAR(50)` | NOT NULL | `fPoste` : `QComboBox` éditable (suggestions = postes déjà existants), `maxLength` = 50 ; obligatoire (« Saisissez le poste. »). |
| `departement` | `DEPARTEMENT` | `QString` | `VARCHAR2(40)` / `VARCHAR(40)` | NOT NULL | `fDepartement` : liste fermée — Technique, Logistique, Communication, Accueil, Ressources humaines, Finance (`Employe::listeDepartements()`). |
| `dateEmbauche` | `DATE_EMBAUCHE` | `QDate` | `VARCHAR2(10)` / `VARCHAR(10)` (texte au format `AAAA-MM-JJ`) | NOT NULL | `fDate` : `QDateEdit` au format `dd/MM/yyyy` avec calendrier ; `setMaximumDate(QDate::currentDate())` ; contrôle « La date d'embauche ne peut pas être dans le futur. » |
| `statut` | `STATUT` | `QString` | `VARCHAR2(20)` / `VARCHAR(20)` | NOT NULL ; valeurs autorisées par l'application : `Actif`, `En congé`, `Inactif` (pas de contrainte `CHECK` en base) | `fStatut` : liste fermée (`Employe::listeStatuts()`). |
| `competences` | `COMPETENCES` | `QStringList` | `VARCHAR2(200)` / `VARCHAR(200)` (liste séparée par des virgules) | facultatif | 8 cases à cocher : Réseau, Support technique, Mentorat, Logistique, Sécurité, Accueil, Communication, Audiovisuel. |
| `heuresDispo` | `HEURES_DISPO` | `int` | `NUMBER` / `INTEGER` | NOT NULL, DEFAULT 20 | `fDispo` : `QSpinBox` de 0 à 72, suffixe « h », valeur par défaut 20. |

> Remarque (citée dans `LISEZ-MOI.txt`) : les champs « Compétences » et « Heures disponibles » ne figurent pas dans les attributs du cahier des spécifications ; ils ont été ajoutés pour le suivi de charge et l'affectation intelligente.

### 2.2 Attributs de `TACHE`

| Attribut (C++) | Colonne SQL | Type C++ | Type SQL (Oracle / SQLite) | Contraintes | Contrôle de saisie dans l'UI |
|---|---|---|---|---|---|
| `id` | `ID` | `int` | `NUMBER` / `INTEGER` | PK | Généré par `SELECT MAX(ID) FROM TACHE` + 1 ; affiché `T-01`. |
| `titre` | `TITRE` | `QString` | `VARCHAR2(80)` / `VARCHAR(80)` | NOT NULL | `tacheTitre` : `maxLength` = 80 ; obligatoire (« Saisissez l'intitulé de la tâche. »). |
| `competition` | `COMPETITION` | `QString` | `VARCHAR2(60)` / `VARCHAR(60)` | NOT NULL | `tacheCompetition` : liste fermée (Hack for a Better Future, HealthTech Innovation, Smart City Challenge, AgriTech for Tomorrow). |
| `competence` | `COMPETENCE` | `QString` | `VARCHAR2(40)` / `VARCHAR(40)` | NOT NULL | `tacheCompetence` : liste des 8 compétences. |
| `heures` | `HEURES` | `int` | `NUMBER` / `INTEGER` | NOT NULL | `tacheHeures` : `QSpinBox` de 1 à 72, défaut 6, suffixe « h ». |
| `idEmploye` | `ID_EMPLOYE` | `int` | `NUMBER` / `INTEGER` | NOT NULL, DEFAULT 0 (0 = non affectée) ; **pas de clé étrangère** | Liste déroulante « Affectée à » dans le tableau des tâches. |

### 2.3 Script SQL exact (`sql/script_oracle.sql`)

```sql
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
```

Les mêmes tables sont créées automatiquement en SQLite par `Connection::creerTables()` (`CREATE TABLE IF NOT EXISTS …`, types `INTEGER` / `VARCHAR`).

---

## 3. Utilisateurs / rôles concernés

Le menu utilisateur (bouton `boutonUtilisateur`, en haut à droite, `GestionEmployes::preparerBarreHaute()`) propose deux rôles exclusifs (`QActionGroup`) :

| Rôle | Libellé du bouton | Ce que le code permet |
|---|---|---|
| Administrateur (par défaut) | « 👤  Admin  ▾ » | Toutes les fonctionnalités du module. |
| Responsable RH | « 👤  Resp. RH  ▾ » | Exactement les mêmes fonctionnalités. |

**[PARTIEL]** Le changement de rôle modifie uniquement le libellé du bouton, affiche le message « Connecté en tant que … » et renseigne le nom de l'auteur dans l'en-tête du PDF exporté (« exporté le … par Administrateur / Responsable RH »). **Aucune restriction de droits** n'est appliquée selon le rôle, et **il n'y a pas de vérification des identifiants** **[NON IMPLÉMENTÉ]** : dans l'application intégrée, l'accès passe par la page de connexion commune (`PLoginCompet`, module Compétitions), dont le bouton « Entrer » ouvre l'application sans contrôler le nom d'utilisateur ni le mot de passe.

Le même menu contient l'action « Réinitialiser les données de démonstration » (accessible quel que soit le rôle).

---

## 4. Fonctionnalités de base (CRUD)

### 4.1 Ajouter un employé — **implémenté**

- **Méthodes :** `GestionEmployes::enregistrerEmploye()` (mode ajout quand `m_edition == 0`) → `Employe::ajouter()` (`employe.cpp`) ; identifiant calculé par `Employe::prochainId()`.
- **Déclencheur :** bouton « Enregistrer » du formulaire « ＋  Ajouter un employé ».
- **Requêtes SQL :**
  ```sql
  SELECT MAX(ID) FROM EMPLOYE
  SELECT COUNT(*) FROM EMPLOYE WHERE LOWER(EMAIL) = ? AND ID <> ?
  INSERT INTO EMPLOYE (ID, NOM, PRENOM, EMAIL, POSTE, DEPARTEMENT, DATE_EMBAUCHE, STATUT, COMPETENCES, HEURES_DISPO)
  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
  ```
  (requêtes préparées `QSqlQuery::prepare` + `addBindValue`).
- **Contrôles de saisie** (dans cet ordre ; le message s'affiche en rouge dans le label `formErreur` sous le formulaire et le curseur est placé sur le champ fautif) :
  - « Saisissez le nom. »
  - « Saisissez le prénom. »
  - « Saisissez l'adresse e-mail. »
  - « Adresse e-mail invalide. Exemple : prenom.nom@hacktime.tn »
  - « Cette adresse e-mail est déjà utilisée par un autre employé. »
  - « Saisissez le poste. »
  - « La date d'embauche ne peut pas être dans le futur. »
- **Messages de retour** (notification temporaire « toast » de 4 s en bas de la fenêtre, `GestionEmployes::message()`, et non une `QMessageBox`) :
  - succès : « <Prénom Nom> a été ajouté (E-0xx). »
  - erreur SQL : « Enregistrement impossible : <erreur SQL> »
- Après l'ajout, la liste se positionne sur la page contenant le nouvel employé, et une nouvelle alerte de surcharge éventuelle est signalée (`signalerNouvellesAlertes`).

### 4.2 Consulter (affichage) — **implémenté**

- **Méthodes :** `Employe::tous()` ; `GestionEmployes::rafraichirListe()` (tableau paginé), `GestionEmployes::rafraichirDetails()` (fiche détaillée), `GestionEmployes::selectionnerEmploye()`.
- **Requête SQL :**
  ```sql
  SELECT ID, NOM, PRENOM, EMAIL, POSTE, DEPARTEMENT, DATE_EMBAUCHE, STATUT, COMPETENCES, HEURES_DISPO
  FROM EMPLOYE ORDER BY ID
  ```
- **Affichage :** tableau `tableEmployes` (colonnes « ID », « Nom », « Prénom », « E-mail », « Poste », « Département », « Embauche », « Statut », « Actions »), **8 lignes par page** (`LIGNES_PAR_PAGE`), boutons « ‹ » / « › », pied de liste « Affichage de X à Y sur N employés ». Le statut est affiché sous forme de pastille colorée (vert = Actif, orange = En congé, gris = Inactif). Un « ⚠ » s'ajoute au nom d'un employé en surcharge.
- Un clic sur une ligne affiche la fiche dans la carte « 👤  Détails de l'employé » (initiales, nom complet, ID, statut, e-mail, poste, département, date d'embauche, compétences, charge « X h / Y h (état) » avec barre de progression).
- Messages en cas de liste vide : « Aucun employé enregistré. Ajoutez le premier avec le formulaire. » / « Aucun employé ne correspond à cette recherche. »

### 4.3 Modifier — **implémenté**

- **Méthodes :** `GestionEmployes::modifierEmploye(int id)` → `remplirFormulaire(e)` (pré-remplissage) ; `GestionEmployes::enregistrerEmploye()` (mode modification quand `m_edition != 0`) → `Employe::modifier()`.
- **Déclencheurs :** icône « ✏️ » de la colonne « Actions », double-clic sur une ligne, ou bouton « Modifier » de la carte Détails. Le titre du formulaire devient « ✏️  Modifier <Prénom Nom> » et le bouton « Effacer » devient « Annuler ».
- **Requête SQL :**
  ```sql
  UPDATE EMPLOYE SET NOM = ?, PRENOM = ?, EMAIL = ?, POSTE = ?, DEPARTEMENT = ?,
         DATE_EMBAUCHE = ?, STATUT = ?, COMPETENCES = ?, HEURES_DISPO = ?
  WHERE ID = ?
  ```
- **Contrôles :** identiques à l'ajout (l'unicité de l'e-mail exclut l'employé modifié : `emailExiste(email, m_edition)`).
- **Message :** « <Prénom Nom> a été enregistré. » ; signalement d'une nouvelle surcharge si la modification en crée une (ex. baisse des heures disponibles, passage « En congé »).

### 4.4 Supprimer — **implémenté**

- **Méthodes :** `GestionEmployes::supprimerEmploye(int id)` → `Employe::supprimer(int id)`.
- **Déclencheurs :** icône « 🗑️ » de la colonne « Actions » ou bouton « Supprimer » de la carte Détails.
- **Confirmation :** `QMessageBox` (icône Warning) titre « Supprimer un employé », texte « Supprimer <Prénom Nom> ? », texte informatif « La fiche E-0xx sera supprimée définitivement. » + « Ses tâches (n) redeviendront non affectées. » si besoin ; boutons « Annuler » (par défaut) et « Supprimer ».
- **Requêtes SQL :**
  ```sql
  UPDATE TACHE SET ID_EMPLOYE = 0 WHERE ID_EMPLOYE = ?
  DELETE FROM EMPLOYE WHERE ID = ?
  ```
- **Messages :** « <Prénom Nom> a été supprimé. » / « La suppression a échoué. Réessayez. »

### 4.5 CRUD des tâches (entité `TACHE`, support des métiers innovants) — **[PARTIEL]**

| Opération | Statut | Méthode | SQL |
|---|---|---|---|
| Ajouter | implémenté | `GestionEmployes::ajouterTache()` → `Tache::ajouter()` | `SELECT MAX(ID) FROM TACHE` puis `INSERT INTO TACHE (ID, TITRE, COMPETITION, COMPETENCE, HEURES, ID_EMPLOYE) VALUES (?, ?, ?, ?, ?, ?)` |
| Consulter | implémenté | `Tache::toutes()` ; `GestionEmployes::rafraichirAffectation()` | `SELECT ID, TITRE, COMPETITION, COMPETENCE, HEURES, ID_EMPLOYE FROM TACHE ORDER BY ID` |
| Modifier l'affectation | implémenté | `GestionEmployes::changerAffectation()` → `Tache::affecter()` | `UPDATE TACHE SET ID_EMPLOYE = ? WHERE ID = ?` |
| Modifier titre / compétition / heures | **[NON IMPLÉMENTÉ]** | — | — |
| Supprimer | implémenté (sans confirmation) | `GestionEmployes::supprimerTache()` → `Tache::supprimer()` | `DELETE FROM TACHE WHERE ID = ?` |

Messages : « Tâche T-xx ajoutée. Elle attend une affectation. », « Saisissez l'intitulé de la tâche. », « Tâche supprimée. », « La tâche n'a pas pu être ajoutée. Réessayez. », « L'affectation n'a pas pu être enregistrée. Réessayez. »

---

## 5. Fonctionnalités avancées — métiers basiques

### 5.1 Recherche — **implémenté**

- **Méthodes :** `Employe::filtrer()` + `Employe::sansAccents()` (`employe.cpp`) ; appelées par `GestionEmployes::listeFiltree()` / `rafraichirListe()`.
- **Champs recherchés :** nom, prénom et poste (concaténés).
- **Mode :** **recherche en direct** (signal `textChanged`, sans bouton). Deux champs synchronisés : `rechercheHaut` dans la barre du haut (« 🔍   Rechercher un employé par nom ou par poste… ») et `recherche` dans l'onglet Liste (« Rechercher un employé… »). La recherche est insensible à la casse **et aux accents** (normalisation Unicode NFD, suppression des signes diacritiques : « reseau » trouve « Réseau »).
- **Filtres combinables :** listes déroulantes « Département », « Poste », « Statut » (valeur « Tous » par défaut), remplies dynamiquement à partir des données (`rafraichirFiltres`).

```cpp
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
```

### 5.2 Tri — **implémenté**

- **Méthodes :** `Employe::filtrer()` (tri par `std::sort` avec le foncteur `OrdreEmploye`, comparaison `QString::localeAwareCompare`), `GestionEmployes::trierParColonne()`.
- **Critères :**
  - liste déroulante « Trier par » (`triCombo`) : ID, Nom, Poste, Département, Date d'embauche ;
  - clic sur l'en-tête de colonne : ID, Nom, Prénom, E-mail, Poste, Département, Embauche, Statut ; un second clic inverse l'ordre (croissant / décroissant), avec indicateur de tri dans l'en-tête.
- À égalité, l'ordre est départagé par l'ID (tri stable et reproductible).

### 5.3 Export PDF — **implémenté**

- **Méthode :** `GestionEmployes::exporterPdf()` (`gestionemployes.cpp`).
- **Classes Qt :** `QFileDialog` (choix du fichier, nom proposé `employes-hacktime.pdf`), `QTextDocument` (contenu HTML), `QPrinter` (`QPrinter::HighResolution`, `PdfFormat`, A4 **paysage**).
- **Fonctionnement :** le document reprend **la liste actuellement filtrée et triée** : titre « HackTime - Liste des employés », ligne « N employé(s) - exporté le JJ/MM/AAAA par <rôle> », puis un tableau (ID, Nom, Prénom, E-mail, Poste, Département, Date d'embauche, Statut) avec en-tête bleu foncé `#0b1b58` et lignes alternées.
- **Messages :** « Liste exportée en PDF : <chemin> » ; « Aucun employé à exporter avec ces filtres. » ; « L'export PDF a échoué. Vérifiez que le dossier est accessible. »

### 5.4 Export Excel — **implémenté (format CSV)**

- **Méthode :** `GestionEmployes::exporterExcel()`.
- **Classes Qt :** `QFileDialog`, `QFile`.
- **Fonctionnement :** génère un fichier **`.csv`** (nom proposé `employes-hacktime.csv`) avec séparateur `;`, encodage UTF-8 avec BOM (`\xEF\xBB\xBF`) et champs entre guillemets, ce qu'Excel ouvre directement avec les bonnes colonnes et les accents. Même contenu filtré/trié que le PDF. Ce n'est **pas** un fichier `.xlsx` natif.
- **Messages :** « Liste exportée pour Excel : <chemin> » ; « Impossible d'écrire le fichier. Fermez-le dans Excel s'il est ouvert, puis réessayez. »

### 5.5 Statistiques — **implémenté**

- **Méthodes :** `Employe::statistiques(liste, parPoste)` (comptage via `QMap`, tri décroissant), `GestionEmployes::rafraichirStatistiques()`, `StatsWidget::paintEvent()`.
- **Bibliothèque :** **aucune bibliothèque externe — pas de Qt Charts**. Le graphique est dessiné à la main avec `QPainter` dans le widget personnalisé `StatsWidget`.
- **Type de graphique :** à gauche, **barres horizontales** (libellé, barre arrondie colorée proportionnelle au maximum, valeur) ; à droite, **anneau de répartition** (diagramme en anneau / *donut*) avec le total au centre et le mot « employés ».
- **Critères :** boutons segmentés « Par département » / « Par poste ». Les statistiques portent sur **tous** les employés (les filtres de la liste ne s'appliquent pas).

### 5.6 Indicateurs (KPI) — **implémenté**

`GestionEmployes::rafraichirIndicateurs()` met à jour 4 cartes : « Total des employés », « Employés actifs », « Départements » (nombre de départements distincts), « En surcharge » (affiché en rouge s'il est > 0).

### 5.7 Pagination — **implémenté**

`GestionEmployes::changerPage()` et `rafraichirListe()` : 8 employés par page, boutons « ‹ » / « › » désactivés aux extrémités, infobulle « Page X sur Y ».

---

## 6. Fonctionnalités métiers innovantes

### 6.1 Suivi automatique de la charge de travail avec alertes de surcharge — **implémenté**

- **Problème résolu :** pendant un hackathon, un même employé peut recevoir trop de tâches (ou des tâches alors qu'il est en congé / inactif) sans que l'organisateur s'en rende compte.
- **Données utilisées :** `EMPLOYE.HEURES_DISPO`, `EMPLOYE.STATUT`, `TACHE.HEURES`, `TACHE.ID_EMPLOYE`.
- **Fonctionnement :**
  1. `Affectation::charge(idEmploye, taches)` additionne les heures des tâches affectées à l'employé.
  2. `Affectation::alertes()` parcourt les employés ayant une charge > 0 :
     - employé **non actif** (« En congé » ou « Inactif ») avec des tâches → alerte « X est « En congé » mais a encore N h de tâches affectées. » ;
     - employé actif dont la charge > heures disponibles → alerte « X est en surcharge : N h affectées pour M h disponibles. »
  3. La charge est recalculée après chaque action (ajout/modification d'employé, changement d'affectation, ajout/suppression de tâche) via `rafraichirTout()`.
  4. `GestionEmployes::signalerNouvellesAlertes(avant)` compare les alertes avant/après l'action et affiche une notification rouge « Alerte : … » dès qu'une **nouvelle** surcharge apparaît.
- **Résultat affiché :**
  - cloche « 🔔 N » en rouge dans la barre du haut, avec un menu listant les alertes (« ⚠  … ») ; un clic ouvre l'onglet Charge et sélectionne l'employé ; sinon « Aucune alerte : personne n'est en surcharge. » ;
  - KPI « En surcharge » ; titre d'onglet « ⏱  Charge de travail (N) » ;
  - dans l'onglet Charge : cadre rouge « N alertes de charge » + liste, ou cadre vert « Aucune surcharge : toutes les charges tiennent dans les heures disponibles. » ;
  - tableau trié du plus chargé au moins chargé, avec barre de progression colorée (vert ; **orange à partir de 85 %** ; rouge en surcharge) et pastille d'état : « Libre », « Équilibré », « Presque plein », « Surcharge », « Indisponible » (fonction `etatCharge`) ;
  - « ⚠ » à côté du nom dans la liste et barre de charge dans la carte Détails.
- **Méthodes / fichiers :** `Affectation::charge`, `enSurcharge`, `alertes` (`affectation.cpp`) ; `GestionEmployes::rafraichirCharge`, `rafraichirIndicateurs`, `signalerNouvellesAlertes`, `reglerBarre`, `etatCharge` (`gestionemployes.cpp`).

### 6.2 Affectation intelligente des employés aux tâches — **implémenté**

- **Problème résolu :** répartir automatiquement les tâches des compétitions entre les employés selon leurs compétences et leur disponibilité, sans surcharger personne.
- **Données utilisées :** compétences, statut et heures disponibles des employés ; compétence demandée, durée et affectation actuelle des tâches.
- **Algorithme** (`Affectation::repartir`) :
  1. Calculer la charge actuelle de chaque employé.
  2. Pour chaque tâche non affectée, compter les **candidats** : employés **actifs** possédant la compétence demandée.
  3. Trier les tâches de la **plus contrainte** à la moins contrainte : moins de candidats d'abord, puis la plus longue, puis l'ID (heuristique « la plus contrainte d'abord »).
  4. Pour chaque tâche, retenir parmi les candidats ceux dont la charge **après affectation ne dépasse pas** leurs heures disponibles.
  5. Choisir celui dont le **taux de charge après affectation est le plus faible** ; à égalité, l'employé **le moins polyvalent** (moins de compétences), pour garder les profils polyvalents libres.
  6. Enregistrer l'affectation en base (`Tache::affecter` → `UPDATE TACHE SET ID_EMPLOYE = ? WHERE ID = ?`).
  7. Si aucune solution : la tâche reste libre avec une raison (« aucun employé actif n'a la compétence « … » » ou « les employés compétents n'ont plus assez d'heures disponibles »).

```cpp
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
```

- **Deux modes :** bouton « Affecter les tâches libres » (`affecterAutomatiquement(false)` : seules les tâches non affectées) et bouton « Tout réaffecter » (`affecterAutomatiquement(true)` : `UPDATE TACHE SET ID_EMPLOYE = 0` puis répartition complète).
- **Résultat affiché :** compte rendu (`QTextBrowser rapportVue`) : « N tâches affectées, M sans solution » puis une ligne par tâche « T-07 → Prénom Nom (12 h, charge à 50 %) » ou, en rouge, « T-09 non affectée : <raison>. » ; le tableau des tâches, la charge et les KPI sont mis à jour.
- **Affectation manuelle assistée :** dans la colonne « Affectée à », la liste déroulante propose d'abord « Non affectée », puis les employés actifs compétents avec leur charge « Prénom Nom (x/y h) », puis, après un séparateur, les autres employés marqués « - hors profil ».
- **Méthodes / fichiers :** `Affectation::affecterAutomatiquement`, `repartir` (`affectation.cpp`) ; `GestionEmployes::affecterTachesLibres`, `toutReaffecter`, `changerAffectation`, `rafraichirAffectation` (`gestionemployes.cpp`).

### 6.3 Rééquilibrage automatique de la charge — **implémenté**

- **Problème résolu :** corriger d'un clic les surcharges et les tâches détenues par des employés absents.
- **Algorithme** (`Affectation::reequilibrer`) :
  1. Calculer la liste des alertes.
  2. Employé indisponible (non actif) : **toutes** ses tâches sont libérées.
  3. Employé en surcharge : calculer l'excès (charge − heures disponibles) ; retirer **la plus petite tâche qui suffit** à repasser sous la limite ; s'il n'y en a pas, retirer la plus longue et recommencer (garde-fou de 100 itérations).
  4. Relancer `repartir()` pour réaffecter les tâches libérées aux employés compétents et disponibles.
- **Résultat affiché :** compte rendu dans l'onglet Affectation ; notification « Charge rééquilibrée : plus aucune surcharge. » ou, en rouge, « Charge rééquilibrée. Tâches sans solution : N (voir l'onglet Affectation). »
- **Déclencheur :** bouton « Rééquilibrer automatiquement » dans le cadre d'alerte de l'onglet « ⏱  Charge de travail ».
- **Méthodes / fichiers :** `Affectation::reequilibrer` (`affectation.cpp`), `GestionEmployes::reequilibrer` (`gestionemployes.cpp`).

---

## 7. Base de données

- **SGBD :**
  - **SQLite** par défaut (driver `QSQLITE`) : fichier `hacktime_employes.db` créé à côté de l'exécutable (`QCoreApplication::applicationDirPath()`), tables créées automatiquement et données de démonstration chargées au premier lancement (14 employés, 13 tâches).
  - **Oracle** prévu en option via **ODBC** (driver `QODBC`, source `Source_Projet2A`) : bloc fourni **en commentaire** dans `Connection::ouvrir()` ; il faut exécuter `sql/script_oracle.sql` puis utiliser « Réinitialiser les données de démonstration ».
- **Méthode de connexion :** `QSqlDatabase::addDatabase(...)` dans `Connection::ouvrir()`, appelée depuis `main.cpp` ; en cas d'échec, `QMessageBox::critical` « Connexion à la base de données impossible. » et arrêt du programme.
- **Accès aux données :** `QSqlQuery` avec requêtes préparées (`prepare` / `addBindValue`) ; le chargement des données de démonstration est fait dans une **transaction** (`transaction` / `commit` / `rollback`).
- **Tables utilisées :** `EMPLOYE`, `TACHE`.
- **Relations :**
  - `TACHE.ID_EMPLOYE` → `EMPLOYE.ID` : relation logique (une tâche est affectée à 0 ou 1 employé ; un employé a 0..n tâches), **sans contrainte `FOREIGN KEY`** en base ; la valeur `0` signifie « non affectée ». L'intégrité est gérée par le code (`Employe::supprimer` remet les tâches à 0 avant de supprimer l'employé).
  - `TACHE.COMPETITION` : nom de compétition stocké en **texte** (liste fixe `Tache::listeCompetitions()`), **non relié** à une table `COMPETITION` du module « Gestion des Compétitions » **[NON IMPLÉMENTÉ]**.
  - Aucune autre clé étrangère vers les autres modules (Équipes, Participants, Jury…).

---

## 8. Interfaces graphiques (GUI)

Le module est **une page de la fenêtre principale** de l'application intégrée (classe `GestionEmployes`, fichier `gestionemployes.ui`, page `PEmp` de la pile `SWHackTime` dans `hackathon.ui`). On y accède après la page de connexion, par le bouton « 💼   Employés » de la barre latérale du module Compétitions. La page est composée de zones fixes et de **4 onglets**, plus des boîtes de dialogue.

### 8.1 Structure générale (commune à tous les onglets)

L'interface de Gestion des Employés comporte, **à gauche**, une barre latérale bleu foncé (dégradé) affichant le logo HackTime et les boutons de navigation « 🏠    Tableau de bord », « 🏆    Compétitions », « 👥    Équipes », « 👤    Participants », « 💼    Employés » (actif, surligné en bleu), « 🚩    Challenges », « ⚖️    Jury », « 📊    Rapports », « ⚙️    Paramètres », ainsi que le slogan « Des idées aujourd'hui, les solutions de demain » en bas. Le bouton « Compétitions » ramène au module Gestion des Compétitions ; les autres boutons (modules pas encore intégrés) affichent le message « Le module « … » n'est pas encore implémenté. » **[NON IMPLÉMENTÉ]**.

**En haut**, une barre blanche contient la barre de recherche « 🔍   Rechercher un employé par nom ou par poste… », la cloche des notifications « 🔔 » (infobulle « Alertes de charge », avec le nombre d'alertes en rouge) et le profil de l'utilisateur « 👤  Admin  ▾ » (menu : « Administrateur », « Responsable RH », « Réinitialiser les données de démonstration »).

**Au centre**, une carte d'en-tête présente l'icône 💼, le titre « Gestion des Employés » (le mot « Employés » en bleu) et le sous-titre « Ajoutez, suivez et affectez les employés mobilisés sur votre hackathon. », suivie de quatre cartes d'indicateurs : « Total des employés », « Employés actifs », « Départements », « En surcharge ». En dessous, une carte à onglets : « 📋  Liste des employés », « 📊  Statistiques », « ⏱  Charge de travail », « 🎯  Affectation intelligente ».

**À droite**, une colonne (largeur 388 px, défilante) contient la carte « 👤  Détails de l'employé » et la carte de formulaire « ＋  Ajouter un employé ».

- Capture : `capture_01_vue_generale.png`
- Légende proposée : « Figure X : Interface graphique du module Gestion des Employés »

### 8.2 Onglet « 📋  Liste des employés » (`ongletListe`)

- **Haut de l'onglet :** titre « Liste des employés » et, à droite, les boutons « Exporter en PDF » et « Exporter en Excel ».
- **Barre de filtres :** « Recherche (nom ou poste) » (champ « Rechercher un employé… »), « Département », « Poste », « Statut » (listes avec « Tous »), « Trier par » (ID, Nom, Poste, Département, Date d'embauche).
- **Centre :** tableau `tableEmployes` avec les colonnes **« ID », « Nom », « Prénom », « E-mail », « Poste », « Département », « Embauche », « Statut », « Actions »** ; ID en bleu gras (E-001), statut en pastille colorée, actions « ✏️ » (Modifier) et « 🗑️ » (Supprimer), « ⚠ » après le nom en cas de surcharge ; en-têtes cliquables pour le tri.
- **Bas :** pied « Affichage de 1 à 8 sur 14 employés » et pagination « ‹ » « 1 » « › ».
- **Fonctionnalités accessibles :** §4.2 Consulter, §4.3 Modifier, §4.4 Supprimer, §5.1 Recherche + filtres, §5.2 Tri, §5.3 Export PDF, §5.4 Export Excel, §5.7 Pagination.
- Capture : `capture_02_liste_employes.png` — « Figure X : Liste des employés avec recherche, filtres et tri »

### 8.3 Onglet « 📊  Statistiques » (`ongletStatistiques`)

- **Haut :** titre « Nombre d'employés » et, à droite, deux boutons segmentés exclusifs « Par département » (coché par défaut) et « Par poste ».
- **Centre :** widget `StatsWidget` : à gauche les barres horizontales (une couleur par catégorie, valeur à droite), à droite l'anneau de répartition avec le total et le mot « employés ». Message si vide : « Ajoutez des employés pour afficher les statistiques. »
- **Fonctionnalités accessibles :** §5.5 Statistiques.
- Captures : `capture_08_statistiques_departement.png` — « Figure X : Statistiques des employés par département » ; `capture_09_statistiques_poste.png` — « Figure X : Statistiques des employés par poste »

### 8.4 Onglet « ⏱  Charge de travail » (`ongletCharge`)

- **Haut :** titre « Charge de travail pendant l'événement » et texte explicatif « La charge de chaque employé est recalculée à chaque changement d'affectation : heures de tâches affectées comparées à ses heures disponibles. Au-delà de 100 %, une alerte est levée. »
- **Zone d'alerte :** cadre rouge (`cadreAlerte`) « N alertes de charge » avec la liste des messages et le bouton « Rééquilibrer automatiquement » ; ou cadre vert (`cadreOk`) « Aucune surcharge : toutes les charges tiennent dans les heures disponibles. »
- **Centre :** tableau `tableCharge` avec les colonnes **« Employé », « Poste », « Charge », « Heures », « État », « Tâches affectées »** (barre de progression verte/orange/rouge, « 24 h / 24 h », pastille d'état, codes des tâches « T-01 (10 h), … » avec infobulle détaillée), trié du plus chargé au moins chargé.
- **Fonctionnalités accessibles :** §6.1 Suivi de charge et alertes, §6.3 Rééquilibrage automatique.
- Captures : `capture_10_charge_alerte.png` — « Figure X : Suivi de la charge de travail avec alerte de surcharge » ; `capture_11_charge_reequilibree.png` — « Figure X : Charge de travail après rééquilibrage automatique »

### 8.5 Onglet « 🎯  Affectation intelligente » (`ongletAffectation`)

- **Haut :** titre « Affectation aux tâches et compétitions » ; à droite les boutons « Affecter les tâches libres » et « Tout réaffecter » ; texte explicatif « Chaque tâche est attribuée à un employé actif qui possède la compétence demandée, en choisissant celui dont la charge restera la plus faible, sans jamais dépasser ses heures disponibles. »
- **Compte rendu :** zone `rapportVue` (affichée après une affectation automatique ou un rééquilibrage).
- **Centre :** tableau `tableTaches` avec les colonnes **« ID », « Tâche », « Compétition », « Compétence », « Heures », « Affectée à »** et une dernière colonne sans titre contenant l'icône « 🗑️ » ; la compétence est affichée en pastille bleue ; « Affectée à » est une liste déroulante (« Non affectée », employés compétents, puis « - hors profil »).
- **Bas :** formulaire d'ajout de tâche : « Nouvelle tâche » (« Intitulé de la tâche »), « Compétition », « Compétence », « Heures » (1–72 h) et bouton « Ajouter la tâche » (touche Entrée acceptée).
- **Fonctionnalités accessibles :** §4.5 CRUD des tâches, §6.2 Affectation intelligente (automatique et manuelle assistée).
- Captures : `capture_12_affectation_avant.png` — « Figure X : Tâches à affecter aux employés » ; `capture_13_affectation_rapport.png` — « Figure X : Résultat de l'affectation intelligente des employés aux tâches » ; `capture_14_affectation_manuelle.png` — « Figure X : Affectation manuelle assistée par compétence »

### 8.6 Carte « 👤  Détails de l'employé » (colonne de droite, `carteDetails`)

- Bloc identité : avatar rond avec les initiales, nom complet, « ID : E-0xx », pastille de statut.
- Lignes : « ✉️  E-mail », « 💼  Poste », « 🏢  Département », « 📅  Embauche », « 🛠️  Compétences », « ⏱  Charge » (« X h / Y h  (état) », en rouge si surcharge) + barre de progression.
- Boutons « Modifier » et « Supprimer ». Message si aucune sélection : « Sélectionnez un employé dans la liste pour voir sa fiche. »
- **Fonctionnalités accessibles :** §4.2, §4.3, §4.4, §6.1.
- Capture : `capture_03_details_employe.png` — « Figure X : Fiche détaillée d'un employé »

### 8.7 Carte formulaire « ＋  Ajouter un employé » / « ✏️  Modifier … » (colonne de droite, `carteFormulaire`)

- Champs : « ID » (lecture seule), « Nom * », « Prénom * », « E-mail * » (« prenom.nom@hacktime.tn »), « Poste * » (« Ex. Technicien réseau »), « Département », « Date d'embauche * », « Statut ».
- Groupe « Pour le suivi de charge et l'affectation » : « Compétences » (cases Réseau, Support technique, Mentorat, Logistique, Sécurité, Accueil, Communication, Audiovisuel) et « Heures disponibles » (0–72 h).
- Message d'erreur en rouge sous le formulaire ; boutons « Enregistrer » et « Effacer » (devient « Annuler » en modification).
- **Fonctionnalités accessibles :** §4.1 Ajouter, §4.3 Modifier.
- Captures : `capture_04_formulaire_ajout.png` — « Figure X : Formulaire d'ajout d'un employé » ; `capture_05_erreur_saisie.png` — « Figure X : Contrôle de saisie du formulaire employé » ; `capture_06_formulaire_modification.png` — « Figure X : Modification d'un employé »

### 8.8 Boîtes de dialogue et notifications

| Élément | Type | Contenu exact |
|---|---|---|
| Confirmation de suppression | `QMessageBox` (Warning) | Titre « Supprimer un employé » ; « Supprimer <Prénom Nom> ? » ; boutons « Annuler » / « Supprimer » |
| Réinitialisation | `QMessageBox` (Warning) | Titre « Réinitialiser les données » ; « Rétablir les données de démonstration ? » ; « Tous les employés et toutes les tâches actuels seront remplacés. » ; boutons « Annuler » / « Réinitialiser » |
| Erreur de connexion | `QMessageBox::critical` | « Connexion à la base de données impossible. » + erreur |
| Export PDF / Excel | `QFileDialog::getSaveFileName` | « Exporter la liste en PDF » / « Exporter la liste pour Excel » |
| Menu des alertes | `QMenu` sous la cloche | « ⚠  … » par alerte |
| Notifications | `QLabel` temporaire (toast 4 s) | Bleu foncé (succès) ou rouge (erreur / alerte) |

- Captures : `capture_07_confirmation_suppression.png` — « Figure X : Confirmation de suppression d'un employé » ; `capture_15_notification_alerte.png` — « Figure X : Notification d'alerte de surcharge » ; `capture_16_export_pdf.png` — « Figure X : Liste des employés exportée en PDF » ; `capture_17_export_excel.png` — « Figure X : Liste des employés exportée vers Excel »

---

## 9. Charte graphique appliquée

### 9.1 Couleurs réellement utilisées (`resources/style.qss`, `gestionemployes.cpp`, `statswidget.cpp`)

| Usage | Couleur(s) |
|---|---|
| Texte principal / infobulles / toast succès | `#0f1a38` |
| Couleur d'accent (boutons primaires, onglet actif, bouton « Employés » actif, sélection, focus des champs) | `#1a5ae8` (survol `#1249c7`) |
| Barre latérale (dégradé vertical) | `#112a85` → `#0b1b58` |
| En-tête du tableau PDF / titre PDF | `#0b1b58` |
| Fond général | `#edf1f9` ; cartes `white` ; fonds secondaires `#f3f6fc` ; bordures `#e1e7f2`, `#d5ddec` |
| Texte discret | `#65708b` |
| Succès / Actif / « Équilibré » | `#1c9552` sur `#dcf5e5` ; carte « Employés actifs » `#1fa35b` |
| Avertissement / « En congé » / « Presque plein » | `#a85f00` sur `#fff0dc` ; barre `#ff8c1a` ; carte « En surcharge » `#ff9a1f` |
| Erreur / surcharge / « Supprimer » | `#d33a3a` sur `#fde5e5` ; toast erreur `#b02a2a` |
| Information | `#1a5ae8` sur `#e2ebff` |
| Palette du graphique | `#1a5ae8`, `#ff8c1a`, `#1fa35b`, `#7a4be0`, `#0fa3b1`, `#e0508f`, `#c9a400`, `#6b7691`, `#d33a3a`, `#3fb0ff`, `#8a5a2b`, `#4a9b2f` |

### 9.2 Polices

- **Segoe UI**, 10 pt (`setFont(QFont("Segoe UI", 10))` dans `GestionEmployes::preparerStyle()`, la feuille de style globale de l'application impose aussi Segoe UI 13 px) ; titres 21 pt / 13 pt gras, indicateurs 19 pt gras (QSS).
- **Arial** 9 pt pour le PDF exporté.
- Palette claire imposée à la page Employés (elle reste claire même si Windows est en mode sombre).

### 9.3 Conformité avec la charte du projet (`#202C46`, `#325E9F`, `#E07527`)

**Non conforme** : aucune de ces trois valeurs n'apparaît dans le code. Le module reprend le même principe (bleu nuit + bleu + orange) mais avec d'autres teintes : bleu nuit `#0f1a38` / `#0b1b58` au lieu de `#202C46`, bleu `#1a5ae8` au lieu de `#325E9F`, orange `#ff8c1a` / `#ff9a1f` au lieu de `#E07527`. Une harmonisation dans `style.qss` (et les quelques couleurs codées en dur dans `gestionemployes.cpp` / `statswidget.cpp`) est nécessaire (voir §13).

---

## 10. Exigences non fonctionnelles respectées

| Exigence | Exemple concret dans le code |
|---|---|
| **Ergonomie** | Champs obligatoires marqués « * » ; message d'erreur précis et curseur placé sur le champ fautif ; recherche en direct tolérante aux accents ; double-clic pour modifier ; confirmation avant suppression avec bouton « Annuler » par défaut ; notifications temporaires non bloquantes ; infobulles sur les boutons d'action ; pagination de 8 lignes. |
| **Performance** | Les données sont chargées une fois (`Employe::tous()`) puis recherche, filtres, tri et statistiques sont calculés en mémoire ; seule la page visible du tableau est construite ; `QSignalBlocker` évite les rafraîchissements en cascade. |
| **Sécurité** | Toutes les requêtes avec paramètres sont préparées (`prepare` + `addBindValue`) → protection contre l'injection SQL ; confirmation des actions destructrices. **[PARTIEL]** : pas d'authentification ni de contrôle des droits par rôle ; identifiants Oracle à écrire en clair dans `connection.cpp`. |
| **Fiabilité** | Unicité de l'e-mail vérifiée dans l'UI et en base (`UNIQUE`) ; date d'embauche bornée à aujourd'hui ; chargement des données de démonstration en transaction avec `rollback` ; suppression d'un employé qui libère ses tâches ; l'affectation automatique ne dépasse jamais les heures disponibles ; messages d'échec SQL affichés. |
| **Portabilité** | Base SQLite embarquée sans installation, ou Oracle via ODBC en changeant quelques lignes ; export CSV lisible par tout tableur ; palette claire forcée indépendante du thème Windows. |
| **Maintenabilité** | Séparation en couches : entités (`Employe`, `Tache`), logique métier (`Affectation`), accès BD (`Connection`), interface (`gestionemployes.ui` + `GestionEmployes`) ; style centralisé dans `style.qss` ; référentiels centralisés (`listeDepartements`, `listeStatuts`, `listeCompetences`) ; code commenté en français. |
| **Évolutivité** | Ajout d'un critère de tri via l'énumération `Employe::Tri` ; ajout d'un département / d'une compétence dans une seule liste ; boutons de navigation déjà prévus pour les autres modules ; script Oracle fourni pour l'intégration à la base commune. |

---

## 11. Lien avec les ODD

- **ODD 8 – Travail décent et croissance économique :** le suivi automatique de la charge, les alertes de surcharge et le rééquilibrage évitent qu'un employé dépasse ses heures disponibles ou reçoive des tâches pendant un congé.
- **ODD 9 – Industrie, innovation et infrastructure :** le module numérise la gestion du personnel de l'événement et intègre un algorithme d'affectation automatique fondé sur les compétences.
- **ODD 4 – Éducation de qualité :** lien indirect — le module gère les compétences des employés et l'affectation de tâches de « Mentorat » (encadrement des équipes en mentorat) pendant les compétitions.
- **ODD 17 – Partenariats pour la réalisation des objectifs :** lien indirect — les exports PDF et CSV facilitent le partage des informations sur le personnel avec les autres organisateurs et partenaires du hackathon.

---

## 12. Liste des captures d'écran à fournir

Ordre recommandé dans le rapport :

- [ ] `capture_01_vue_generale.png` — fenêtre complète, onglet « Liste des employés », un employé sélectionné (barre latérale, barre du haut, indicateurs, détails et formulaire visibles).
- [ ] `capture_02_liste_employes.png` — onglet Liste avec une recherche saisie (ex. « tech ») et/ou un filtre actif, indicateur de tri visible dans l'en-tête.
- [ ] `capture_03_details_employe.png` — carte « Détails de l'employé » d'un employé avec des tâches (barre de charge visible).
- [ ] `capture_04_formulaire_ajout.png` — formulaire « Ajouter un employé » rempli (compétences cochées) avant « Enregistrer ».
- [ ] `capture_05_erreur_saisie.png` — message d'erreur de saisie (ex. « Adresse e-mail invalide… » ou « Cette adresse e-mail est déjà utilisée… »).
- [ ] `capture_06_formulaire_modification.png` — formulaire en mode « ✏️  Modifier … » avec le bouton « Annuler ».
- [ ] `capture_07_confirmation_suppression.png` — boîte « Supprimer un employé » (avec la mention des tâches libérées).
- [ ] `capture_08_statistiques_departement.png` — onglet Statistiques, « Par département ».
- [ ] `capture_09_statistiques_poste.png` — onglet Statistiques, « Par poste ».
- [ ] `capture_10_charge_alerte.png` — onglet Charge de travail avec le cadre rouge d'alerte et une ligne « Surcharge » / « Indisponible » (ex. affecter une tâche supplémentaire à un employé déjà plein, ou passer un employé chargé en « En congé »).
- [ ] `capture_11_charge_reequilibree.png` — même onglet après « Rééquilibrer automatiquement » (cadre vert).
- [ ] `capture_12_affectation_avant.png` — onglet Affectation intelligente avec des tâches « Non affectée ».
- [ ] `capture_13_affectation_rapport.png` — après « Affecter les tâches libres » : compte rendu visible.
- [ ] `capture_14_affectation_manuelle.png` — liste déroulante « Affectée à » ouverte (employés compétents + « hors profil »).
- [ ] `capture_15_notification_alerte.png` — menu de la cloche « 🔔 N » ouvert et/ou notification rouge « Alerte : … ».
- [ ] `capture_16_export_pdf.png` — PDF exporté ouvert dans un lecteur PDF.
- [ ] `capture_17_export_excel.png` — fichier CSV exporté ouvert dans Excel.
- [ ] (optionnel) `capture_18_menu_utilisateur.png` — menu du profil (rôles + réinitialisation).

---

## 13. Points en suspens

1. **Projet compilé dans l'application commune** (`hackathon.pro`) : l'intégration n'a pas encore été compilée ni testée dans Qt Creator — à vérifier (compilation, navigation Compétitions ↔ Employés, rendu des styles).
2. **Rôles [PARTIEL] :** Administrateur / Responsable RH sont purement cosmétiques (aucune restriction de droits) ; **pas d'authentification [NON IMPLÉMENTÉ]**.
3. **Navigation [PARTIEL] :** seuls « Compétitions » et « Employés » sont reliés ; les autres boutons de la barre latérale affichent « Le module « … » n'est pas encore implémenté. »
4. **Intégration BD avec les autres modules [NON IMPLÉMENTÉ] :** `TACHE.COMPETITION` est un texte issu d'une liste fixe, non relié à la table des compétitions ; aucune clé étrangère (`TACHE.ID_EMPLOYE` utilise la valeur 0 au lieu de `NULL` + `FOREIGN KEY`).
5. **Schéma SQL à améliorer :** `DATE_EMBAUCHE` stockée en `VARCHAR2(10)` au lieu de `DATE` ; pas de contrainte `CHECK` sur `STATUT` ; identifiants calculés par `MAX(ID) + 1` au lieu d'une séquence Oracle.
6. **Oracle non activé par défaut :** la connexion ODBC est en commentaire ; la base utilisée réellement est SQLite.
7. **Suppression non transactionnelle :** `Employe::supprimer()` ignore le résultat de `UPDATE TACHE …` et n'utilise pas de transaction.
8. **Tâches :** pas de modification du titre / de la compétition / des heures **[NON IMPLÉMENTÉ]** ; suppression d'une tâche sans confirmation.
9. **Recherche :** le texte d'aide indique « nom ou poste » mais la recherche porte aussi sur le prénom ; l'e-mail n'est pas recherché.
10. **Tri :** la liste « Trier par » propose 5 critères alors que le clic sur les en-têtes en propose 8 (Prénom, E-mail, Statut seulement par en-tête).
11. **Statistiques :** limitées au nombre d'employés par département / par poste, calculées sur tous les employés (sans tenir compte des filtres) ; pas de statistique sur les statuts ou la charge.
12. **Export « Excel » :** produit un `.csv`, pas un `.xlsx` (à préciser dans le rapport).
13. **Messages :** les erreurs et confirmations courantes sont affichées par notification temporaire (toast) ou label rouge, pas par `QMessageBox` (seules la suppression, la réinitialisation et l'erreur de connexion utilisent `QMessageBox`).
14. **Charte graphique non conforme** aux couleurs du projet `#202C46`, `#325E9F`, `#E07527` (voir §9.3).
15. **Attributs hors cahier des spécifications :** « Compétences » et « Heures disponibles » ont été ajoutés ; à mentionner/justifier dans le cahier.
