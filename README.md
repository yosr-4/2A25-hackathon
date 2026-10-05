# HackTime — Smart Hackathon Management (équipe Elite Hackers)

Application de bureau **C++ / Qt 6** de gestion de hackathon. Cette version intègre deux modules dans une même fenêtre :

| Module | Auteur | Contenu |
|---|---|---|
| **Gestion des Compétitions** | Yosr | Page de connexion, CRUD des compétitions, filtres, détection des conflits de planning (salle / jury), export PDF et Excel (.xlsx), brochure et publicité. Données : `competitions.json`. |
| **Gestion des Employés** | Firas | CRUD des employés, recherche, tri, statistiques, export PDF et Excel (.csv), suivi de la charge de travail avec alertes, affectation intelligente des employés aux tâches, rééquilibrage automatique. Données : base SQLite `hacktime_employes.db`. |

Après la page de connexion, le bouton **« 💼 Employés »** de la barre latérale ouvre le module Employés ; le bouton **« Compétitions »** de la barre latérale du module Employés ramène au module Compétitions.

---

## 1. Prérequis (nouvel ordinateur Windows)

Le projet a été compilé et testé avec **Qt 6.12.0 + MinGW 13.1 64-bit** sous Windows 11.

1. **Git** : <https://git-scm.com/download/win> (installation par défaut).
2. **Qt** : télécharger l'installateur en ligne <https://www.qt.io/download-qt-installer> (un compte Qt gratuit est demandé).
   Dans l'écran de sélection des composants, cocher :
   - **Qt → Qt 6.x (6.12.0 de préférence) → MinGW 64-bit**
   - **Qt → Developer and Designer Tools → MinGW 13.1.0 64-bit** (le compilateur)
   - **Qt Creator** (coché par défaut)

   Aucun module Qt supplémentaire n'est nécessaire : `widgets`, `sql` et `printsupport` font partie de Qt de base, et le pilote SQLite est inclus.

Aucune base de données n'est à installer : SQLite est intégrée à Qt.

## 2. Récupérer le projet

```bash
git clone https://github.com/Vfiras/2A25-hackathon.git
cd 2A25-hackathon
```

Pour récupérer plus tard les dernières modifications :

```bash
git pull
```

(Sans Git : bouton vert **Code → Download ZIP** sur GitHub, puis décompresser.)

## 3. Lancer avec Qt Creator (méthode conseillée)

1. Ouvrir **Qt Creator** → **Fichier → Ouvrir un fichier ou projet…** → choisir **`hackathon.pro`**.
2. Dans l'écran **Configure Project**, cocher le kit **Desktop Qt 6.x MinGW 64-bit** puis **Configure Project**.
3. Cliquer sur **Exécuter** (flèche verte, ou `Ctrl+R`).
4. Sur la page de connexion, cliquer sur **« Entrer »** (les identifiants ne sont pas encore vérifiés).

## 4. Lancer en ligne de commande (optionnel)

Dans le menu Démarrer, ouvrir **« Qt 6.x (MinGW 13.1.0 64-bit) »** (terminal où Qt et le compilateur sont déjà dans le `PATH`), puis :

```bat
cd chemin\vers\2A25-hackathon
mkdir build
cd build
qmake ..\hackathon.pro "CONFIG+=release"
mingw32-make -j8
release\hackathon.exe
```

### Créer une version portable (pour un PC sans Qt)

```bat
mkdir HackTime
copy release\hackathon.exe HackTime\
windeployqt --release --no-translations HackTime\hackathon.exe
```

Le dossier `HackTime` contient alors l'exécutable et toutes les DLL nécessaires (Qt, MinGW, pilote SQLite). On peut le copier sur n'importe quel PC Windows 64 bits et lancer `hackathon.exe` directement.

## 5. Données

Les fichiers de données sont créés automatiquement **à côté de l'exécutable**, au premier lancement :

| Fichier | Module | Contenu au premier lancement |
|---|---|---|
| `competitions.json` | Compétitions | 8 compétitions de démonstration |
| `hacktime_employes.db` | Employés | 14 employés et 13 tâches de démonstration |

Avec Qt Creator, l'exécutable se trouve dans le dossier de build (par ex. `build/Desktop_Qt_6_12_0_MinGW_64_bit-Debug/debug/`). Ces fichiers ne sont pas versionnés (voir `.gitignore`).

- Pour revenir aux données de démonstration des employés : module Employés → menu **« 👤 Admin ▾ »** en haut à droite → **« Réinitialiser les données de démonstration »**.
- Pour repartir de zéro : fermer l'application et supprimer les deux fichiers ci-dessus.

### Utiliser Oracle à la place de SQLite (module Employés, optionnel)

1. Exécuter `sql/script_oracle.sql` dans SQL Developer.
2. Dans `connection.cpp`, remplacer les deux lignes SQLite par le bloc Oracle donné en commentaire (source ODBC, utilisateur, mot de passe).
3. Lancer l'application puis **« Réinitialiser les données de démonstration »** pour remplir les tables.

## 6. Structure du dépôt

```
hackathon.pro              Projet Qt (qmake) commun
main.cpp                   Point d'entrée : connexion BD, fenêtre, style global
hackathon.h/.cpp/.ui       Fenêtre principale + module Compétitions (pile SWHackTime)
competition.h, conflits.h  Modèle Compétition et détection des conflits
exports.*, xlsx.h          Export PDF / Excel du module Compétitions
publications.*, donut.h    Brochure / publicité, graphique en anneau
style.qss, ressources.qrc, images/   Style global et images (Compétitions)

gestionemployes.h/.cpp/.ui Page du module Employés (page PEmp de SWHackTime)
employe.*, tache.*         Entités Employé et Tâche (CRUD SQL)
affectation.*              Charge de travail, alertes, affectation intelligente
statswidget.*              Graphique des statistiques des employés
connection.*               Connexion à la base (SQLite par défaut, Oracle possible)
resources.qrc, resources/  Style et icônes du module Employés
sql/script_oracle.sql      Script Oracle du module Employés
LISEZ-MOI.txt, RAPPORT_MON_MODULE.md   Documentation du module Employés
```

**Ajouter un nouveau module :** créer une classe `QWidget` (comme `GestionEmployes`), l'ajouter comme nouvelle page de `SWHackTime` dans `hackathon.ui` (widget promu), puis relier son bouton de barre latérale dans `hackathon::setupNavigation()` (`hackathon.cpp`).

## 7. Dépannage

| Problème | Solution |
|---|---|
| Qt Creator ne propose aucun kit | Relancer l'installateur Qt (*Qt Maintenance Tool*) et ajouter **Qt 6.x → MinGW 64-bit** et **MinGW 13.1.0 64-bit**. |
| `Project ERROR: Unknown module(s) in QT: ...` | Le kit choisi n'est pas un kit Qt 6 complet : choisir **Desktop Qt 6.x MinGW 64-bit**. |
| « Connexion à la base de données impossible » | Le pilote SQLite manque : lancer l'application depuis Qt Creator, ou utiliser `windeployqt` (section 4) qui copie le dossier `sqldrivers`. |
| Au double-clic sur `hackathon.exe` : « Qt6Core.dll introuvable » | Normal hors de Qt Creator : créer la version portable avec `windeployqt` (section 4). |
| Accents mal affichés dans le code | Ouvrir les fichiers en **UTF-8** (réglage par défaut de Qt Creator). |
