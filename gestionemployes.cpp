#include "gestionemployes.h"
#include "ui_gestionemployes.h"

#include "connection.h"
#include "statswidget.h"

#include <QAction>
#include <QActionGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPair>
#include <QPalette>
#include <QPrinter>
#include <QProgressBar>
#include <QPushButton>
#include <QRegularExpression>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTimer>
#include <QToolButton>
#include <algorithm>

// ======================================================================
//  Petites fonctions utilitaires pour l'affichage
// ======================================================================
namespace {

const int LIGNES_PAR_PAGE = 8;

// Colonnes de la liste des employés
enum { ColId, ColNom, ColPrenom, ColEmail, ColPoste, ColDepartement, ColDate, ColStatut, ColActions };

QToolButton *boutonIcone(const QString &icone, const QString &infobulle)
{
    QToolButton *b = new QToolButton;
    b->setObjectName("icone");
    b->setText(icone);
    b->setToolTip(infobulle);
    b->setCursor(Qt::PointingHandCursor);
    return b;
}

QString typeStatut(const QString &statut)
{
    if (statut == "Actif")
        return "actif";
    if (statut == "En congé")
        return "conge";
    return "inactif";
}

QString stylePastille(const QString &type)
{
    QString fond = "#e5e9f1";
    QString encre = "#5d6880";
    if (type == "actif") {
        fond = "#dcf5e5"; encre = "#1c9552";
    } else if (type == "conge") {
        fond = "#fff0dc"; encre = "#a85f00";
    } else if (type == "info") {
        fond = "#e2ebff"; encre = "#1a5ae8";
    } else if (type == "danger") {
        fond = "#fde5e5"; encre = "#d33a3a";
    }
    return QString("background:%1;color:%2;border-radius:8px;padding:4px 12px;font-weight:bold;")
        .arg(fond, encre);
}

QLabel *pastille(const QString &libelle, const QString &type)
{
    QLabel *label = new QLabel(libelle);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(stylePastille(type));
    return label;
}

// Place un widget dans une cellule de tableau, aligné à gauche.
QWidget *dansCellule(QWidget *w)
{
    QWidget *conteneur = new QWidget;
    QHBoxLayout *l = new QHBoxLayout(conteneur);
    l->setContentsMargins(8, 4, 8, 4);
    l->addWidget(w);
    l->addStretch();
    return conteneur;
}

QTableWidgetItem *cellule(const QString &contenu)
{
    QTableWidgetItem *item = new QTableWidgetItem(contenu);
    item->setToolTip(contenu);
    return item;
}

// Barre de charge : verte, orange à partir de 85 %, rouge en surcharge.
void reglerBarre(QProgressBar *barre, int heures, int dispo, bool actif)
{
    int pourcentage = 0;
    if (dispo > 0)
        pourcentage = heures * 100 / dispo;
    else if (heures > 0)
        pourcentage = 100;

    QString couleur = "#1c9552";
    if (heures > dispo || (!actif && heures > 0))
        couleur = "#d33a3a";
    else if (pourcentage >= 85)
        couleur = "#ff8c1a";

    barre->setRange(0, 100);
    barre->setValue(qMin(100, pourcentage));
    barre->setTextVisible(false);
    barre->setFixedHeight(12);
    barre->setStyleSheet(QString("QProgressBar{border:none;background:#eef2f9;border-radius:6px;}"
                                 "QProgressBar::chunk{background:%1;border-radius:6px;}").arg(couleur));
}

// État de charge d'un employé : libellé + type de pastille.
QString etatCharge(const Employe &e, int heures, QString *type)
{
    if (!e.estActif()) {
        *type = heures > 0 ? "danger" : "inactif";
        return heures > 0 ? QString("Indisponible") : e.statut;
    }
    if (heures > e.heuresDispo) {
        *type = "danger";
        return "Surcharge";
    }
    if (e.heuresDispo > 0 && heures * 100 / e.heuresDispo >= 85) {
        *type = "conge";
        return "Presque plein";
    }
    if (heures == 0) {
        *type = "info";
        return "Libre";
    }
    *type = "actif";
    return "Équilibré";
}

void remplirCombo(QComboBox *combo, const QStringList &valeurs, const QString &courant)
{
    const QSignalBlocker bloqueur(combo);
    combo->clear();
    combo->addItem("Tous");
    combo->addItems(valeurs);
    const int index = combo->findText(courant);
    combo->setCurrentIndex(index > 0 ? index : 0);
}

QString valeurFiltre(const QComboBox *combo)
{
    return combo->currentIndex() > 0 ? combo->currentText() : QString();
}

bool chargePlusForte(const QPair<double, int> &a, const QPair<double, int> &b)
{
    if (a.first != b.first)
        return a.first > b.first;
    return a.second < b.second;
}

QString champCsv(const QString &valeur)
{
    QString v = valeur;
    v.replace("\"", "\"\"");
    return "\"" + v + "\"";
}

// Palette claire imposée : la page garde ses couleurs même si
// Windows est réglé en mode sombre.
QPalette paletteClaire()
{
    QPalette p;
    p.setColor(QPalette::Window, QColor("#edf1f9"));
    p.setColor(QPalette::WindowText, QColor("#0f1a38"));
    p.setColor(QPalette::Base, QColor("#ffffff"));
    p.setColor(QPalette::AlternateBase, QColor("#f3f6fc"));
    p.setColor(QPalette::Text, QColor("#0f1a38"));
    p.setColor(QPalette::Button, QColor("#ffffff"));
    p.setColor(QPalette::ButtonText, QColor("#0f1a38"));
    p.setColor(QPalette::ToolTipBase, QColor("#0f1a38"));
    p.setColor(QPalette::ToolTipText, QColor("#ffffff"));
    p.setColor(QPalette::Highlight, QColor("#1a5ae8"));
    p.setColor(QPalette::HighlightedText, QColor("#ffffff"));
    p.setColor(QPalette::Disabled, QPalette::Text, QColor("#9aa4bd"));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#9aa4bd"));
    p.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#9aa4bd"));
    return p;
}

} // namespace

// ======================================================================
//  Construction de la fenêtre
// ======================================================================
GestionEmployes::GestionEmployes(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GestionEmployes),
      m_rapportVisible(false),
      m_selection(0),
      m_edition(0),
      m_page(0),
      m_tri(Employe::ParId),
      m_croissant(true),
      m_statsParPoste(false),
      m_utilisateur("Administrateur")
{
    // Toute l'interface est dessinée dans gestionemployes.ui (Qt Designer).
    ui->setupUi(this);

    // Ce que le formulaire ne décrit pas : style, navigation, menus, listes
    // fournies par les classes métier, largeur des colonnes et signaux.
    preparerStyle();
    preparerNavigation();
    preparerBarreHaute();
    preparerOngletListe();
    preparerOngletStatistiques();
    preparerOngletCharge();
    preparerOngletAffectation();
    preparerCarteDetails();
    preparerCarteFormulaire();

    // Message temporaire affiché en bas de la fenêtre
    m_toast = new QLabel(ui->centre);
    m_toast->hide();
    m_toastTimer = new QTimer(this);
    m_toastTimer->setSingleShot(true);
    connect(m_toastTimer, &QTimer::timeout, m_toast, &QWidget::hide);

    m_employes = Employe::tous();
    if (!m_employes.isEmpty())
        m_selection = m_employes.first().id;
    rafraichirTout();
    remplirFormulaire(nullptr);
}

GestionEmployes::~GestionEmployes()
{
    delete ui;
}

// ---------------------------------------------------------------- Style et navigation
// La feuille de style et la palette claire du module ne s'appliquent qu'à cette
// page : le module Compétitions garde le style global chargé dans main.cpp.
void GestionEmployes::preparerStyle()
{
    setPalette(paletteClaire());
    setFont(QFont("Segoe UI", 10));

    QFile qss(":/resources/style.qss");
    if (qss.open(QIODevice::ReadOnly))
        setStyleSheet(QString::fromUtf8(qss.readAll()));
}

void GestionEmployes::preparerNavigation()
{
    const QList<QPair<QPushButton *, QString> > boutons = {
        {ui->boutonTableauBord, "Tableau de bord"}, {ui->boutonCompetitions, "Compétitions"},
        {ui->boutonEquipes, "Équipes"}, {ui->boutonParticipants, "Participants"},
        {ui->boutonChallenges, "Challenges"}, {ui->boutonJury, "Jury"},
        {ui->boutonRapports, "Rapports"}, {ui->boutonParametres, "Paramètres"}};
    for (int i = 0; i < boutons.size(); ++i) {
        const QString module = boutons.at(i).second;
        connect(boutons.at(i).first, &QPushButton::clicked, this, [this, module]() {
            emit navigationDemandee(module);
        });
    }
}

// ---------------------------------------------------------------- Barre du haut
void GestionEmployes::preparerBarreHaute()
{
    // Cloche : alertes de surcharge
    m_menuAlertes = new QMenu(this);
    ui->cloche->setMenu(m_menuAlertes);

    // Utilisateur connecté : Administrateur ou Responsable RH
    QMenu *menu = new QMenu(this);
    QActionGroup *groupe = new QActionGroup(menu);
    const QStringList roles = QStringList() << "Administrateur" << "Responsable RH";
    for (int i = 0; i < roles.size(); ++i) {
        const QString role = roles.at(i);
        QAction *action = menu->addAction(role);
        action->setCheckable(true);
        action->setChecked(i == 0);
        groupe->addAction(action);
        connect(action, &QAction::triggered, this, [this, role]() {
            m_utilisateur = role;
            ui->boutonUtilisateur->setText(role == "Administrateur" ? "👤  Admin  ▾" : "👤  Resp. RH  ▾");
            message("Connecté en tant que " + role + ".");
        });
    }
    menu->addSeparator();
    QAction *reinitialiser = menu->addAction("Réinitialiser les données de démonstration");
    connect(reinitialiser, &QAction::triggered, this, &GestionEmployes::reinitialiserDonnees);
    ui->boutonUtilisateur->setMenu(menu);

    connect(ui->rechercheHaut, &QLineEdit::textChanged, this, [this](const QString &valeur) {
        {
            const QSignalBlocker bloqueur(ui->recherche);
            ui->recherche->setText(valeur);
        }
        ui->onglets->setCurrentIndex(0);
        m_page = 0;
        rafraichirListe();
    });
}

// ---------------------------------------------------------------- Onglet « Liste »
void GestionEmployes::preparerOngletListe()
{
    ui->triCombo->addItem("ID", int(Employe::ParId));
    ui->triCombo->addItem("Nom", int(Employe::ParNom));
    ui->triCombo->addItem("Poste", int(Employe::ParPoste));
    ui->triCombo->addItem("Département", int(Employe::ParDepartement));
    ui->triCombo->addItem("Date d'embauche", int(Employe::ParDate));

    // Largeur des colonnes et tri par clic sur l'en-tête
    QHeaderView *enTete = ui->tableEmployes->horizontalHeader();
    enTete->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    enTete->setSectionResizeMode(QHeaderView::ResizeToContents);
    enTete->setSectionResizeMode(ColEmail, QHeaderView::Stretch);
    enTete->setSectionResizeMode(ColPoste, QHeaderView::Stretch);
    enTete->setSectionsClickable(true);
    enTete->setSortIndicatorShown(true);

    // Signaux
    connect(ui->boutonPdf, &QPushButton::clicked, this, &GestionEmployes::exporterPdf);
    connect(ui->boutonExcel, &QPushButton::clicked, this, &GestionEmployes::exporterExcel);
    connect(ui->recherche, &QLineEdit::textChanged, this, [this](const QString &valeur) {
        {
            const QSignalBlocker bloqueur(ui->rechercheHaut);
            ui->rechercheHaut->setText(valeur);
        }
        m_page = 0;
        rafraichirListe();
    });
    QComboBox *filtres[] = {ui->filtreDepartement, ui->filtrePoste, ui->filtreStatut};
    for (int i = 0; i < 3; ++i) {
        connect(filtres[i], QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) {
            m_page = 0;
            rafraichirListe();
        });
    }
    connect(ui->triCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        m_tri = Employe::Tri(ui->triCombo->itemData(index).toInt());
        m_croissant = true;
        rafraichirListe();
    });
    connect(enTete, &QHeaderView::sectionClicked, this, &GestionEmployes::trierParColonne);
    connect(ui->tableEmployes, &QTableWidget::itemSelectionChanged, this, [this]() {
        const QList<QTableWidgetItem *> selection = ui->tableEmployes->selectedItems();
        if (selection.isEmpty())
            return;
        QTableWidgetItem *premier = ui->tableEmployes->item(selection.first()->row(), ColId);
        if (premier)
            selectionnerEmploye(premier->data(Qt::UserRole).toInt());
    });
    connect(ui->tableEmployes, &QTableWidget::cellDoubleClicked, this, [this](int ligne, int) {
        QTableWidgetItem *premier = ui->tableEmployes->item(ligne, ColId);
        if (!premier)
            return;
        const int id = premier->data(Qt::UserRole).toInt();
        QTimer::singleShot(0, this, [this, id]() { modifierEmploye(id); });
    });
    connect(ui->pagePrecedente, &QPushButton::clicked, this, [this]() { changerPage(-1); });
    connect(ui->pageSuivante, &QPushButton::clicked, this, [this]() { changerPage(1); });
}

// ---------------------------------------------------------------- Onglet « Statistiques »
void GestionEmployes::preparerOngletStatistiques()
{
    connect(ui->statsDepartement, &QPushButton::clicked, this, [this]() {
        m_statsParPoste = false;
        rafraichirStatistiques();
    });
    connect(ui->statsPoste, &QPushButton::clicked, this, [this]() {
        m_statsParPoste = true;
        rafraichirStatistiques();
    });
}

// ---------------------------------------------------------------- Onglet « Charge de travail »
void GestionEmployes::preparerOngletCharge()
{
    QHeaderView *enTete = ui->tableCharge->horizontalHeader();
    enTete->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    enTete->setSectionResizeMode(QHeaderView::ResizeToContents);
    enTete->setSectionResizeMode(2, QHeaderView::Fixed);
    enTete->resizeSection(2, 190);
    enTete->setSectionResizeMode(5, QHeaderView::Stretch);

    connect(ui->boutonEquilibrer, &QPushButton::clicked, this, &GestionEmployes::reequilibrer);
}

// ---------------------------------------------------------------- Onglet « Affectation intelligente »
void GestionEmployes::preparerOngletAffectation()
{
    ui->rapportVue->hide();

    QHeaderView *enTete = ui->tableTaches->horizontalHeader();
    enTete->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    enTete->setSectionResizeMode(QHeaderView::ResizeToContents);
    enTete->setSectionResizeMode(1, QHeaderView::Stretch);
    enTete->setSectionResizeMode(5, QHeaderView::Fixed);
    enTete->resizeSection(5, 250);

    ui->tacheCompetition->addItems(Tache::listeCompetitions());
    ui->tacheCompetence->addItems(Employe::listeCompetences());

    connect(ui->boutonAffecterLibres, &QPushButton::clicked, this, &GestionEmployes::affecterTachesLibres);
    connect(ui->boutonToutReaffecter, &QPushButton::clicked, this, &GestionEmployes::toutReaffecter);
    connect(ui->boutonAjouterTache, &QPushButton::clicked, this, &GestionEmployes::ajouterTache);
    connect(ui->tacheTitre, &QLineEdit::returnPressed, this, &GestionEmployes::ajouterTache);
}

// ---------------------------------------------------------------- Colonne de droite
void GestionEmployes::preparerCarteDetails()
{
    connect(ui->boutonModifier, &QPushButton::clicked, this, [this]() { modifierEmploye(m_selection); });
    connect(ui->boutonSupprimer, &QPushButton::clicked, this, [this]() { supprimerEmploye(m_selection); });
}

void GestionEmployes::preparerCarteFormulaire()
{
    ui->fPoste->lineEdit()->setPlaceholderText("Ex. Technicien réseau");
    ui->fPoste->lineEdit()->setMaxLength(50);
    ui->fDepartement->addItems(Employe::listeDepartements());
    ui->fStatut->addItems(Employe::listeStatuts());

    // Cases du formulaire : leur libellé doit rester identique aux
    // compétences de Employe::listeCompetences().
    m_fCompetences << ui->caseReseau << ui->caseSupport << ui->caseMentorat << ui->caseLogistique
                   << ui->caseSecurite << ui->caseAccueil << ui->caseCommunication << ui->caseAudiovisuel;

    ui->formErreur->hide();

    connect(ui->boutonEnregistrer, &QPushButton::clicked, this, &GestionEmployes::enregistrerEmploye);
    connect(ui->fEffacer, &QPushButton::clicked, this, &GestionEmployes::effacerFormulaire);
}

// ======================================================================
//  Mise à jour de l'affichage
// ======================================================================
const Employe *GestionEmployes::employe(int id) const
{
    for (int i = 0; i < m_employes.size(); ++i) {
        if (m_employes.at(i).id == id)
            return &m_employes.at(i);
    }
    return nullptr;
}

QList<Employe> GestionEmployes::listeFiltree() const
{
    return Employe::filtrer(m_employes, ui->recherche->text(), valeurFiltre(ui->filtreDepartement),
                            valeurFiltre(ui->filtrePoste), valeurFiltre(ui->filtreStatut), m_tri, m_croissant);
}

QList<int> GestionEmployes::idsEnAlerte() const
{
    QList<int> ids;
    const QList<Alerte> liste = Affectation::alertes(m_employes, m_taches);
    for (int i = 0; i < liste.size(); ++i)
        ids << liste.at(i).idEmploye;
    return ids;
}

void GestionEmployes::rafraichirTout()
{
    m_employes = Employe::tous();
    m_taches = Tache::toutes();
    if (!employe(m_selection))
        m_selection = m_employes.isEmpty() ? 0 : m_employes.first().id;

    rafraichirFiltres();
    rafraichirIndicateurs();
    rafraichirListe();
    rafraichirStatistiques();
    rafraichirCharge();
    rafraichirAffectation();
    rafraichirDetails();

    if (m_edition == 0)
        ui->fId->setText(Employe::codeDepuisId(Employe::prochainId()));
}

void GestionEmployes::rafraichirFiltres()
{
    QStringList departements = Employe::listeDepartements();
    const QStringList utilises = Employe::valeursDistinctes(m_employes, false);
    for (int i = 0; i < utilises.size(); ++i) {
        if (!departements.contains(utilises.at(i)))
            departements << utilises.at(i);
    }
    const QStringList postes = Employe::valeursDistinctes(m_employes, true);

    remplirCombo(ui->filtreDepartement, departements, ui->filtreDepartement->currentText());
    remplirCombo(ui->filtrePoste, postes, ui->filtrePoste->currentText());
    remplirCombo(ui->filtreStatut, Employe::listeStatuts(), ui->filtreStatut->currentText());

    // Suggestions de postes dans le formulaire (le texte saisi est conservé)
    const QString saisie = ui->fPoste->currentText();
    {
        const QSignalBlocker bloqueur(ui->fPoste);
        ui->fPoste->clear();
        ui->fPoste->addItems(postes);
        ui->fPoste->setCurrentIndex(-1);
        ui->fPoste->setEditText(saisie);
    }
}

void GestionEmployes::rafraichirIndicateurs()
{
    int actifs = 0;
    for (int i = 0; i < m_employes.size(); ++i) {
        if (m_employes.at(i).estActif())
            ++actifs;
    }
    const QList<Alerte> liste = Affectation::alertes(m_employes, m_taches);
    const int nombre = liste.size();

    ui->kpiTotal->setText(QString::number(m_employes.size()));
    ui->kpiActifs->setText(QString::number(actifs));
    ui->kpiDepartements->setText(QString::number(Employe::valeursDistinctes(m_employes, false).size()));
    ui->kpiSurcharge->setText(QString::number(nombre));
    ui->kpiSurcharge->setStyleSheet(nombre > 0 ? "color:#d33a3a;" : "");

    // Cloche et menu des alertes
    ui->cloche->setText(nombre > 0 ? QString("🔔 %1").arg(nombre) : QString("🔔"));
    ui->cloche->setStyleSheet(nombre > 0 ? "QToolButton{color:#d33a3a;}" : "");
    m_menuAlertes->clear();
    if (liste.isEmpty()) {
        QAction *aucune = m_menuAlertes->addAction("Aucune alerte : personne n'est en surcharge.");
        aucune->setEnabled(false);
    }
    for (int i = 0; i < liste.size(); ++i) {
        const int id = liste.at(i).idEmploye;
        QAction *action = m_menuAlertes->addAction("⚠  " + liste.at(i).message);
        connect(action, &QAction::triggered, this, [this, id]() {
            ui->onglets->setCurrentIndex(2);
            selectionnerEmploye(id);
        });
    }
    ui->onglets->setTabText(2, nombre > 0 ? QString("⏱  Charge de travail (%1)").arg(nombre)
                                        : QString("⏱  Charge de travail"));
}

void GestionEmployes::rafraichirListe()
{
    static const Employe::Tri TRIS[] = {Employe::ParId, Employe::ParNom, Employe::ParPrenom, Employe::ParEmail,
                                        Employe::ParPoste, Employe::ParDepartement, Employe::ParDate,
                                        Employe::ParStatut};

    const QList<Employe> lignes = listeFiltree();
    const int total = lignes.size();
    const int pages = qMax(1, (total + LIGNES_PAR_PAGE - 1) / LIGNES_PAR_PAGE);
    m_page = qBound(0, m_page, pages - 1);
    const int debut = m_page * LIGNES_PAR_PAGE;
    const int fin = qMin(total, debut + LIGNES_PAR_PAGE);

    QFont gras = ui->tableEmployes->font();
    gras.setBold(true);

    {
        const QSignalBlocker bloqueur(ui->tableEmployes);
        ui->tableEmployes->clearContents();
        ui->tableEmployes->setRowCount(fin - debut);
        int ligneSelection = -1;

        for (int i = debut; i < fin; ++i) {
            const Employe &e = lignes.at(i);
            const int ligne = i - debut;
            const int id = e.id;

            QTableWidgetItem *code = cellule(e.code());
            code->setData(Qt::UserRole, id);
            code->setForeground(QColor("#1a5ae8"));
            code->setFont(gras);
            ui->tableEmployes->setItem(ligne, ColId, code);

            QTableWidgetItem *nom = cellule(e.nom);
            if (e.estActif() && Affectation::enSurcharge(e, m_taches)) {
                nom->setText(e.nom + "  ⚠");
                nom->setToolTip("En surcharge");
            }
            ui->tableEmployes->setItem(ligne, ColNom, nom);
            ui->tableEmployes->setItem(ligne, ColPrenom, cellule(e.prenom));
            QTableWidgetItem *email = cellule(e.email);
            email->setForeground(QColor("#65708b"));
            ui->tableEmployes->setItem(ligne, ColEmail, email);
            ui->tableEmployes->setItem(ligne, ColPoste, cellule(e.poste));
            ui->tableEmployes->setItem(ligne, ColDepartement, cellule(e.departement));
            ui->tableEmployes->setItem(ligne, ColDate, cellule(e.dateEmbauche.toString("dd/MM/yyyy")));
            ui->tableEmployes->setItem(ligne, ColStatut, new QTableWidgetItem);
            ui->tableEmployes->setCellWidget(ligne, ColStatut, dansCellule(pastille(e.statut, typeStatut(e.statut))));

            // Boutons Modifier / Supprimer (l'action est différée : le tableau
            // ne doit pas être reconstruit pendant le clic sur un de ses boutons)
            QWidget *actions = new QWidget;
            QHBoxLayout *la = new QHBoxLayout(actions);
            la->setContentsMargins(4, 0, 4, 0);
            la->setSpacing(2);
            QToolButton *modifier = boutonIcone("✏️", "Modifier " + e.nomComplet());
            QToolButton *supprimer = boutonIcone("🗑️", "Supprimer " + e.nomComplet());
            la->addWidget(modifier);
            la->addWidget(supprimer);
            la->addStretch();
            ui->tableEmployes->setItem(ligne, ColActions, new QTableWidgetItem);
            ui->tableEmployes->setCellWidget(ligne, ColActions, actions);
            connect(modifier, &QToolButton::clicked, this, [this, id]() {
                QTimer::singleShot(0, this, [this, id]() { modifierEmploye(id); });
            });
            connect(supprimer, &QToolButton::clicked, this, [this, id]() {
                QTimer::singleShot(0, this, [this, id]() { supprimerEmploye(id); });
            });

            if (id == m_selection)
                ligneSelection = ligne;
        }

        if (ligneSelection >= 0)
            ui->tableEmployes->selectRow(ligneSelection);
        else
            ui->tableEmployes->clearSelection();
    }

    // Indicateur de tri dans l'en-tête
    int colonneTri = 0;
    for (int c = 0; c < 8; ++c) {
        if (TRIS[c] == m_tri)
            colonneTri = c;
    }
    ui->tableEmployes->horizontalHeader()->setSortIndicator(colonneTri, m_croissant ? Qt::AscendingOrder
                                                                         : Qt::DescendingOrder);

    // Pied de liste
    if (total == 0) {
        ui->piedListe->setText(m_employes.isEmpty()
                                 ? "Aucun employé enregistré. Ajoutez le premier avec le formulaire."
                                 : "Aucun employé ne correspond à cette recherche.");
    } else {
        ui->piedListe->setText(QString("Affichage de %1 à %2 sur %3 employé%4")
                                 .arg(debut + 1).arg(fin).arg(total).arg(QString(total > 1 ? "s" : "")));
    }
    ui->pageCourante->setText(QString("%1").arg(m_page + 1));
    ui->pageCourante->setToolTip(QString("Page %1 sur %2").arg(m_page + 1).arg(pages));
    ui->pagePrecedente->setEnabled(m_page > 0);
    ui->pageSuivante->setEnabled(m_page < pages - 1);
}

void GestionEmployes::rafraichirStatistiques()
{
    ui->stats->setDonnees(Employe::statistiques(m_employes, m_statsParPoste));
}

void GestionEmployes::rafraichirCharge()
{
    const QList<Alerte> liste = Affectation::alertes(m_employes, m_taches);
    ui->cadreAlerte->setVisible(!liste.isEmpty());
    ui->cadreOk->setVisible(liste.isEmpty());
    if (!liste.isEmpty()) {
        QString html = QString("<b>%1 alerte%2 de charge</b><ul style=\"margin-top:4px;\">")
                           .arg(liste.size()).arg(QString(liste.size() > 1 ? "s" : ""));
        for (int i = 0; i < liste.size(); ++i)
            html += "<li>" + liste.at(i).message.toHtmlEscaped() + "</li>";
        html += "</ul>";
        ui->texteAlerte->setText(html);
    }

    // Employés les plus chargés en premier
    QList<QPair<double, int> > ordre;
    for (int i = 0; i < m_employes.size(); ++i) {
        const Employe &e = m_employes.at(i);
        const int heures = Affectation::charge(e.id, m_taches);
        double taux = 0.0;
        if (e.heuresDispo > 0)
            taux = double(heures) / double(e.heuresDispo);
        else if (heures > 0)
            taux = 1000.0;
        ordre << qMakePair(taux, i);
    }
    std::sort(ordre.begin(), ordre.end(), chargePlusForte);

    ui->tableCharge->clearContents();
    ui->tableCharge->setRowCount(ordre.size());
    QFont gras = ui->tableCharge->font();
    gras.setBold(true);

    for (int ligne = 0; ligne < ordre.size(); ++ligne) {
        const Employe &e = m_employes.at(ordre.at(ligne).second);
        const int heures = Affectation::charge(e.id, m_taches);

        QTableWidgetItem *nom = cellule(e.nomComplet());
        nom->setFont(gras);
        ui->tableCharge->setItem(ligne, 0, nom);
        QTableWidgetItem *poste = cellule(e.poste);
        poste->setForeground(QColor("#65708b"));
        ui->tableCharge->setItem(ligne, 1, poste);

        QProgressBar *barre = new QProgressBar;
        reglerBarre(barre, heures, e.heuresDispo, e.estActif());
        QWidget *conteneur = new QWidget;
        QHBoxLayout *lb = new QHBoxLayout(conteneur);
        lb->setContentsMargins(8, 0, 8, 0);
        lb->addWidget(barre);
        ui->tableCharge->setItem(ligne, 2, new QTableWidgetItem);
        ui->tableCharge->setCellWidget(ligne, 2, conteneur);

        QTableWidgetItem *duree = cellule(QString("%1 h / %2 h").arg(heures).arg(e.heuresDispo));
        duree->setFont(gras);
        ui->tableCharge->setItem(ligne, 3, duree);

        QString type;
        const QString etat = etatCharge(e, heures, &type);
        ui->tableCharge->setItem(ligne, 4, new QTableWidgetItem);
        ui->tableCharge->setCellWidget(ligne, 4, dansCellule(pastille(etat, type)));

        QStringList resume;
        QStringList detail;
        for (int t = 0; t < m_taches.size(); ++t) {
            const Tache &tache = m_taches.at(t);
            if (tache.idEmploye != e.id)
                continue;
            resume << QString("%1 (%2 h)").arg(tache.code()).arg(tache.heures);
            detail << QString("%1  %2 (%3 h)").arg(tache.code(), tache.titre).arg(tache.heures);
        }
        QTableWidgetItem *taches = new QTableWidgetItem(resume.isEmpty() ? QString("Aucune") : resume.join(", "));
        taches->setToolTip(detail.join("\n"));
        if (resume.isEmpty())
            taches->setForeground(QColor("#65708b"));
        ui->tableCharge->setItem(ligne, 5, taches);
    }
}

void GestionEmployes::rafraichirAffectation()
{
    ui->tableTaches->clearContents();
    ui->tableTaches->setRowCount(m_taches.size());

    for (int ligne = 0; ligne < m_taches.size(); ++ligne) {
        const Tache &t = m_taches.at(ligne);
        const int idTache = t.id;

        ui->tableTaches->setItem(ligne, 0, cellule(t.code()));
        ui->tableTaches->setItem(ligne, 1, cellule(t.titre));
        ui->tableTaches->setItem(ligne, 2, cellule(t.competition));
        ui->tableTaches->setItem(ligne, 3, new QTableWidgetItem);
        ui->tableTaches->setCellWidget(ligne, 3, dansCellule(pastille(t.competence, "info")));
        ui->tableTaches->setItem(ligne, 4, cellule(QString("%1 h").arg(t.heures)));

        // Liste déroulante : d'abord les employés actifs qui ont la compétence
        QComboBox *choix = new QComboBox;
        const int nonAffectee = 0;
        choix->addItem("Non affectée", nonAffectee);
        QList<int> autres;
        for (int i = 0; i < m_employes.size(); ++i) {
            const Employe &e = m_employes.at(i);
            if (e.estActif() && e.competences.contains(t.competence)) {
                choix->addItem(QString("%1 (%2/%3 h)").arg(e.nomComplet())
                                   .arg(Affectation::charge(e.id, m_taches)).arg(e.heuresDispo), e.id);
            } else {
                autres << i;
            }
        }
        if (!autres.isEmpty() && choix->count() > 1)
            choix->insertSeparator(choix->count());
        for (int i = 0; i < autres.size(); ++i) {
            const Employe &e = m_employes.at(autres.at(i));
            choix->addItem(QString("%1 (%2/%3 h) - hors profil").arg(e.nomComplet())
                               .arg(Affectation::charge(e.id, m_taches)).arg(e.heuresDispo), e.id);
        }
        const int position = choix->findData(t.idEmploye);
        choix->setCurrentIndex(position >= 0 ? position : 0);

        QWidget *conteneur = new QWidget;
        QHBoxLayout *lc = new QHBoxLayout(conteneur);
        lc->setContentsMargins(6, 4, 6, 4);
        lc->addWidget(choix);
        ui->tableTaches->setItem(ligne, 5, new QTableWidgetItem);
        ui->tableTaches->setCellWidget(ligne, 5, conteneur);
        connect(choix, QOverload<int>::of(&QComboBox::activated), this, [this, choix, idTache](int index) {
            const int idEmploye = choix->itemData(index).toInt();
            QTimer::singleShot(0, this, [this, idTache, idEmploye]() { changerAffectation(idTache, idEmploye); });
        });

        QToolButton *supprimer = boutonIcone("🗑️", "Supprimer la tâche " + t.code());
        ui->tableTaches->setItem(ligne, 6, new QTableWidgetItem);
        ui->tableTaches->setCellWidget(ligne, 6, dansCellule(supprimer));
        connect(supprimer, &QToolButton::clicked, this, [this, idTache]() {
            QTimer::singleShot(0, this, [this, idTache]() { supprimerTache(idTache); });
        });
    }

    // Compte rendu de la dernière affectation automatique
    ui->rapportVue->setVisible(m_rapportVisible);
    if (!m_rapportVisible)
        return;
    if (m_rapport.isEmpty()) {
        ui->rapportVue->setHtml("Toutes les tâches sont déjà affectées.");
        return;
    }
    int reussies = 0;
    QString lignes;
    for (int i = 0; i < m_rapport.size(); ++i) {
        const LigneRapport &r = m_rapport.at(i);
        if (r.affectee) {
            ++reussies;
            lignes += QString("<li>%1 → %2 (%3 h, charge à %4 %)</li>")
                          .arg(r.tache.code(), r.employe.toHtmlEscaped())
                          .arg(r.tache.heures).arg(r.pourcentage);
        } else {
            lignes += QString("<li style=\"color:#d33a3a;\">%1 non affectée : %2.</li>")
                          .arg(r.tache.code(), r.raison.toHtmlEscaped());
        }
    }
    const int echecs = m_rapport.size() - reussies;
    QString titre = QString("<b>%1 tâche%2 affectée%2").arg(reussies).arg(QString(reussies > 1 ? "s" : ""));
    if (echecs > 0)
        titre += QString(", %1 sans solution").arg(echecs);
    titre += "</b>";
    ui->rapportVue->setHtml(titre + "<ul style=\"margin-top:4px;\">" + lignes + "</ul>");
}

void GestionEmployes::rafraichirDetails()
{
    const Employe *e = employe(m_selection);
    ui->detVide->setVisible(e == nullptr);
    ui->detContenu->setVisible(e != nullptr);
    if (!e)
        return;

    const int heures = Affectation::charge(e->id, m_taches);
    QString type;
    const QString etat = etatCharge(*e, heures, &type);

    ui->detAvatar->setText(e->initiales());
    ui->detNom->setText(e->nomComplet());
    ui->detId->setText("ID : " + e->code());
    ui->detStatut->setText(e->statut);
    ui->detStatut->setStyleSheet(stylePastille(typeStatut(e->statut)));
    ui->detEmail->setText(e->email);
    ui->detPoste->setText(e->poste);
    ui->detDepartement->setText(e->departement);
    ui->detDate->setText(e->dateEmbauche.toString("dd/MM/yyyy"));
    ui->detCompetences->setText(e->competences.isEmpty() ? QString("Aucune renseignée") : e->competences.join(", "));
    ui->detCharge->setText(QString("%1 h / %2 h  (%3)").arg(heures).arg(e->heuresDispo).arg(etat));
    ui->detCharge->setStyleSheet(type == "danger" ? "color:#d33a3a;font-weight:bold;" : "");
    reglerBarre(ui->detBarre, heures, e->heuresDispo, e->estActif());
}

void GestionEmployes::remplirFormulaire(const Employe *e)
{
    ui->formErreur->hide();
    m_edition = e ? e->id : 0;
    ui->formTitre->setText(e ? "✏️  Modifier " + e->nomComplet() : QString("＋  Ajouter un employé"));
    ui->fEffacer->setText(e ? "Annuler" : "Effacer");

    ui->fId->setText(e ? e->code() : Employe::codeDepuisId(Employe::prochainId()));
    ui->fNom->setText(e ? e->nom : QString());
    ui->fPrenom->setText(e ? e->prenom : QString());
    ui->fEmail->setText(e ? e->email : QString());
    ui->fPoste->setCurrentIndex(-1);
    ui->fPoste->setEditText(e ? e->poste : QString());

    if (e && ui->fDepartement->findText(e->departement) < 0)
        ui->fDepartement->addItem(e->departement);
    ui->fDepartement->setCurrentIndex(e ? ui->fDepartement->findText(e->departement) : 0);
    ui->fStatut->setCurrentIndex(e ? qMax(0, ui->fStatut->findText(e->statut)) : 0);

    ui->fDate->setMaximumDate(QDate::currentDate());
    ui->fDate->setDate(e && e->dateEmbauche.isValid() ? e->dateEmbauche : QDate::currentDate());
    ui->fDispo->setValue(e ? e->heuresDispo : 20);
    for (int i = 0; i < m_fCompetences.size(); ++i)
        m_fCompetences.at(i)->setChecked(e && e->competences.contains(m_fCompetences.at(i)->text()));
}

void GestionEmployes::message(const QString &contenu, bool erreur)
{
    m_toast->setText(contenu);
    m_toast->setStyleSheet(QString("background:%1;color:white;border-radius:10px;"
                                   "padding:12px 18px;font-weight:bold;")
                               .arg(QString(erreur ? "#b02a2a" : "#0f1a38")));
    m_toast->adjustSize();
    QWidget *centre = ui->centre;
    m_toast->move((centre->width() - m_toast->width()) / 2, centre->height() - m_toast->height() - 26);
    m_toast->raise();
    m_toast->show();
    m_toastTimer->start(4000);
}

void GestionEmployes::signalerNouvellesAlertes(const QList<int> &avant)
{
    const QList<Alerte> liste = Affectation::alertes(m_employes, m_taches);
    for (int i = 0; i < liste.size(); ++i) {
        if (!avant.contains(liste.at(i).idEmploye)) {
            message("Alerte : " + liste.at(i).message, true);
            return;
        }
    }
}

// ======================================================================
//  Employés : ajouter, consulter, modifier, supprimer
// ======================================================================
void GestionEmployes::selectionnerEmploye(int id)
{
    m_selection = id;
    rafraichirDetails();

    // Surligne la ligne correspondante si elle est sur la page affichée
    const QSignalBlocker bloqueur(ui->tableEmployes);
    for (int ligne = 0; ligne < ui->tableEmployes->rowCount(); ++ligne) {
        QTableWidgetItem *premier = ui->tableEmployes->item(ligne, ColId);
        if (premier && premier->data(Qt::UserRole).toInt() == id) {
            ui->tableEmployes->selectRow(ligne);
            return;
        }
    }
    ui->tableEmployes->clearSelection();
}

void GestionEmployes::modifierEmploye(int id)
{
    const Employe *e = employe(id);
    if (!e)
        return;
    selectionnerEmploye(id);
    remplirFormulaire(e);
    ui->fNom->setFocus();
}

void GestionEmployes::effacerFormulaire()
{
    remplirFormulaire(nullptr);
}

void GestionEmployes::enregistrerEmploye()
{
    Employe e;
    e.id = m_edition;
    e.nom = ui->fNom->text().trimmed();
    e.prenom = ui->fPrenom->text().trimmed();
    e.email = ui->fEmail->text().trimmed().toLower();
    e.poste = ui->fPoste->currentText().trimmed();
    e.departement = ui->fDepartement->currentText();
    e.dateEmbauche = ui->fDate->date();
    e.statut = ui->fStatut->currentText();
    e.heuresDispo = ui->fDispo->value();
    for (int i = 0; i < m_fCompetences.size(); ++i) {
        if (m_fCompetences.at(i)->isChecked())
            e.competences << m_fCompetences.at(i)->text();
    }

    // Contrôles de saisie
    static const QRegularExpression formatEmail("^[^\\s@]+@[^\\s@]+\\.[^\\s@]{2,}$");
    QString erreur;
    QWidget *champ = nullptr;
    if (e.nom.isEmpty()) {
        erreur = "Saisissez le nom.";
        champ = ui->fNom;
    } else if (e.prenom.isEmpty()) {
        erreur = "Saisissez le prénom.";
        champ = ui->fPrenom;
    } else if (e.email.isEmpty()) {
        erreur = "Saisissez l'adresse e-mail.";
        champ = ui->fEmail;
    } else if (!formatEmail.match(e.email).hasMatch()) {
        erreur = "Adresse e-mail invalide. Exemple : prenom.nom@hacktime.tn";
        champ = ui->fEmail;
    } else if (Employe::emailExiste(e.email, m_edition)) {
        erreur = "Cette adresse e-mail est déjà utilisée par un autre employé.";
        champ = ui->fEmail;
    } else if (e.poste.isEmpty()) {
        erreur = "Saisissez le poste.";
        champ = ui->fPoste;
    } else if (!e.dateEmbauche.isValid() || e.dateEmbauche > QDate::currentDate()) {
        erreur = "La date d'embauche ne peut pas être dans le futur.";
        champ = ui->fDate;
    }
    if (!erreur.isEmpty()) {
        ui->formErreur->setText(erreur);
        ui->formErreur->show();
        champ->setFocus();
        return;
    }

    const QList<int> avant = idsEnAlerte();
    const bool modification = m_edition != 0;
    QString erreurSql;
    const bool ok = modification ? e.modifier(&erreurSql) : e.ajouter(&erreurSql);
    if (!ok) {
        message("Enregistrement impossible : " + erreurSql, true);
        return;
    }

    const QString nomComplet = e.nomComplet();
    const QString code = e.code();
    m_selection = e.id;
    m_edition = 0;
    rafraichirTout();
    remplirFormulaire(nullptr);

    // Affiche la page de la liste où se trouve l'employé enregistré
    const QList<Employe> lignes = listeFiltree();
    for (int i = 0; i < lignes.size(); ++i) {
        if (lignes.at(i).id == m_selection) {
            m_page = i / LIGNES_PAR_PAGE;
            rafraichirListe();
            break;
        }
    }

    message(modification ? nomComplet + " a été enregistré."
                         : nomComplet + " a été ajouté (" + code + ").");
    signalerNouvellesAlertes(avant);
}

void GestionEmployes::supprimerEmploye(int id)
{
    const Employe *e = employe(id);
    if (!e)
        return;
    const QString nomComplet = e->nomComplet();

    int nbTaches = 0;
    for (int i = 0; i < m_taches.size(); ++i) {
        if (m_taches.at(i).idEmploye == id)
            ++nbTaches;
    }
    QString detail = "La fiche " + e->code() + " sera supprimée définitivement.";
    if (nbTaches > 0)
        detail += QString("\nSes tâches (%1) redeviendront non affectées.").arg(nbTaches);

    QMessageBox boite(this);
    boite.setWindowTitle("Supprimer un employé");
    boite.setIcon(QMessageBox::Warning);
    boite.setText("Supprimer " + nomComplet + " ?");
    boite.setInformativeText(detail);
    QPushButton *annuler = boite.addButton("Annuler", QMessageBox::RejectRole);
    QPushButton *confirmer = boite.addButton("Supprimer", QMessageBox::DestructiveRole);
    boite.setDefaultButton(annuler);
    boite.exec();
    if (boite.clickedButton() != confirmer)
        return;

    if (!Employe::supprimer(id)) {
        message("La suppression a échoué. Réessayez.", true);
        return;
    }
    const bool etaitEnEdition = (m_edition == id);
    if (etaitEnEdition)
        m_edition = 0;
    rafraichirTout();
    if (etaitEnEdition)
        remplirFormulaire(nullptr);
    message(nomComplet + " a été supprimé.");
}

// ======================================================================
//  Liste : pagination, tri, export
// ======================================================================
void GestionEmployes::changerPage(int decalage)
{
    m_page += decalage;
    rafraichirListe();
}

void GestionEmployes::trierParColonne(int colonne)
{
    static const Employe::Tri TRIS[] = {Employe::ParId, Employe::ParNom, Employe::ParPrenom, Employe::ParEmail,
                                        Employe::ParPoste, Employe::ParDepartement, Employe::ParDate,
                                        Employe::ParStatut};
    if (colonne < 0 || colonne > ColStatut) {
        rafraichirListe();   // colonne « Actions » : on remet simplement l'indicateur en place
        return;
    }
    if (m_tri == TRIS[colonne]) {
        m_croissant = !m_croissant;
    } else {
        m_tri = TRIS[colonne];
        m_croissant = true;
    }
    {
        const QSignalBlocker bloqueur(ui->triCombo);
        const int index = ui->triCombo->findData(int(m_tri));
        if (index >= 0)
            ui->triCombo->setCurrentIndex(index);
    }
    rafraichirListe();
}

void GestionEmployes::exporterPdf()
{
    const QList<Employe> lignes = listeFiltree();
    if (lignes.isEmpty()) {
        message("Aucun employé à exporter avec ces filtres.", true);
        return;
    }
    QString chemin = QFileDialog::getSaveFileName(this, "Exporter la liste en PDF",
                                                  "employes-hacktime.pdf", "Document PDF (*.pdf)");
    if (chemin.isEmpty())
        return;
    if (!chemin.endsWith(".pdf", Qt::CaseInsensitive))
        chemin += ".pdf";

    QString html;
    html += "<h2 style=\"color:#0b1b58;\">HackTime - Liste des employés</h2>";
    html += QString("<p style=\"color:#65708b;\">%1 employé(s) - exporté le %2 par %3</p>")
                .arg(lignes.size())
                .arg(QDate::currentDate().toString("dd/MM/yyyy"), m_utilisateur.toHtmlEscaped());
    html += "<table width=\"100%\" cellspacing=\"0\" cellpadding=\"5\">";
    html += "<tr>";
    const QStringList entetes = QStringList() << "ID" << "Nom" << "Prénom" << "E-mail" << "Poste"
                                              << "Département" << "Date d'embauche" << "Statut";
    for (int i = 0; i < entetes.size(); ++i)
        html += "<th align=\"left\" bgcolor=\"#0b1b58\"><font color=\"#ffffff\">" + entetes.at(i) + "</font></th>";
    html += "</tr>";
    for (int i = 0; i < lignes.size(); ++i) {
        const Employe &e = lignes.at(i);
        const QString fond = (i % 2 == 0) ? "#f3f6fc" : "#ffffff";
        const QStringList valeurs = QStringList() << e.code() << e.nom << e.prenom << e.email << e.poste
                                                  << e.departement << e.dateEmbauche.toString("dd/MM/yyyy")
                                                  << e.statut;
        html += "<tr>";
        for (int c = 0; c < valeurs.size(); ++c)
            html += "<td bgcolor=\"" + fond + "\">" + valeurs.at(c).toHtmlEscaped() + "</td>";
        html += "</tr>";
    }
    html += "</table>";

    QPrinter imprimante(QPrinter::HighResolution);
    imprimante.setOutputFormat(QPrinter::PdfFormat);
    imprimante.setOutputFileName(chemin);
    imprimante.setPageSize(QPageSize(QPageSize::A4));
    imprimante.setPageOrientation(QPageLayout::Landscape);

    QTextDocument document;
    document.setDefaultFont(QFont("Arial", 9));
    document.setHtml(html);
    document.print(&imprimante);

    if (QFile::exists(chemin))
        message("Liste exportée en PDF : " + chemin);
    else
        message("L'export PDF a échoué. Vérifiez que le dossier est accessible.", true);
}

// Export Excel : fichier CSV (séparateur « ; », encodage UTF-8 avec BOM),
// qu'Excel ouvre directement avec les colonnes et les accents corrects.
void GestionEmployes::exporterExcel()
{
    const QList<Employe> lignes = listeFiltree();
    if (lignes.isEmpty()) {
        message("Aucun employé à exporter avec ces filtres.", true);
        return;
    }
    QString chemin = QFileDialog::getSaveFileName(this, "Exporter la liste pour Excel",
                                                  "employes-hacktime.csv", "Fichier Excel CSV (*.csv)");
    if (chemin.isEmpty())
        return;
    if (!chemin.endsWith(".csv", Qt::CaseInsensitive))
        chemin += ".csv";

    QStringList contenu;
    contenu << (QStringList() << "ID" << "Nom" << "Prénom" << "E-mail" << "Poste" << "Département"
                              << "Date d'embauche" << "Statut").join(";");
    for (int i = 0; i < lignes.size(); ++i) {
        const Employe &e = lignes.at(i);
        contenu << (QStringList() << champCsv(e.code()) << champCsv(e.nom) << champCsv(e.prenom)
                                  << champCsv(e.email) << champCsv(e.poste) << champCsv(e.departement)
                                  << champCsv(e.dateEmbauche.toString("dd/MM/yyyy")) << champCsv(e.statut))
                       .join(";");
    }

    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly)) {
        message("Impossible d'écrire le fichier. Fermez-le dans Excel s'il est ouvert, puis réessayez.", true);
        return;
    }
    fichier.write("\xEF\xBB\xBF");
    fichier.write(contenu.join("\r\n").toUtf8());
    fichier.write("\r\n");
    fichier.close();
    message("Liste exportée pour Excel : " + chemin);
}

// ======================================================================
//  Charge de travail et affectation intelligente
// ======================================================================
void GestionEmployes::affecterTachesLibres()
{
    m_rapport = Affectation::affecterAutomatiquement(false);
    m_rapportVisible = true;
    rafraichirTout();
}

void GestionEmployes::toutReaffecter()
{
    m_rapport = Affectation::affecterAutomatiquement(true);
    m_rapportVisible = true;
    rafraichirTout();
}

void GestionEmployes::reequilibrer()
{
    m_rapport = Affectation::reequilibrer();
    m_rapportVisible = true;
    rafraichirTout();

    int echecs = 0;
    for (int i = 0; i < m_rapport.size(); ++i) {
        if (!m_rapport.at(i).affectee)
            ++echecs;
    }
    if (echecs > 0)
        message(QString("Charge rééquilibrée. Tâches sans solution : %1 (voir l'onglet Affectation).").arg(echecs), true);
    else
        message("Charge rééquilibrée : plus aucune surcharge.");
}

void GestionEmployes::changerAffectation(int idTache, int idEmploye)
{
    const QList<int> avant = idsEnAlerte();
    if (!Tache::affecter(idTache, idEmploye)) {
        message("L'affectation n'a pas pu être enregistrée. Réessayez.", true);
        return;
    }
    m_rapportVisible = false;
    rafraichirTout();
    signalerNouvellesAlertes(avant);
}

void GestionEmployes::ajouterTache()
{
    Tache t;
    t.titre = ui->tacheTitre->text().trimmed();
    t.competition = ui->tacheCompetition->currentText();
    t.competence = ui->tacheCompetence->currentText();
    t.heures = ui->tacheHeures->value();
    if (t.titre.isEmpty()) {
        message("Saisissez l'intitulé de la tâche.", true);
        ui->tacheTitre->setFocus();
        return;
    }
    if (!t.ajouter()) {
        message("La tâche n'a pas pu être ajoutée. Réessayez.", true);
        return;
    }
    ui->tacheTitre->clear();
    m_rapportVisible = false;
    rafraichirTout();
    message("Tâche " + t.code() + " ajoutée. Elle attend une affectation.");
}

void GestionEmployes::supprimerTache(int idTache)
{
    if (!Tache::supprimer(idTache)) {
        message("La tâche n'a pas pu être supprimée. Réessayez.", true);
        return;
    }
    m_rapportVisible = false;
    rafraichirTout();
    message("Tâche supprimée.");
}

void GestionEmployes::reinitialiserDonnees()
{
    QMessageBox boite(this);
    boite.setWindowTitle("Réinitialiser les données");
    boite.setIcon(QMessageBox::Warning);
    boite.setText("Rétablir les données de démonstration ?");
    boite.setInformativeText("Tous les employés et toutes les tâches actuels seront remplacés.");
    QPushButton *annuler = boite.addButton("Annuler", QMessageBox::RejectRole);
    QPushButton *confirmer = boite.addButton("Réinitialiser", QMessageBox::DestructiveRole);
    boite.setDefaultButton(annuler);
    boite.exec();
    if (boite.clickedButton() != confirmer)
        return;

    if (!Connection::chargerDonneesDemo()) {
        message("La réinitialisation a échoué.", true);
        return;
    }
    m_edition = 0;
    m_page = 0;
    m_selection = 0;
    m_rapportVisible = false;
    rafraichirTout();
    remplirFormulaire(nullptr);
    message("Données de démonstration rétablies.");
}
