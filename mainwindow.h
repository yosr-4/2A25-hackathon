#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "affectation.h"
#include "employe.h"
#include "tache.h"

#include <QList>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QCheckBox;
class QLabel;
class QMenu;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Employés (CRUD)
    void enregistrerEmploye();
    void effacerFormulaire();
    void modifierEmploye(int id);
    void supprimerEmploye(int id);
    void selectionnerEmploye(int id);

    // Liste
    void rafraichirListe();
    void changerPage(int decalage);
    void trierParColonne(int colonne);
    void exporterPdf();
    void exporterExcel();

    // Charge et affectation
    void affecterTachesLibres();
    void toutReaffecter();
    void reequilibrer();
    void changerAffectation(int idTache, int idEmploye);
    void ajouterTache();
    void supprimerTache(int idTache);

    void reinitialiserDonnees();

private:
    // Compléments de l'interface dessinée dans mainwindow.ui :
    // menus, listes déroulantes, largeur des colonnes et signaux
    void preparerBarreHaute();
    void preparerOngletListe();
    void preparerOngletStatistiques();
    void preparerOngletCharge();
    void preparerOngletAffectation();
    void preparerCarteDetails();
    void preparerCarteFormulaire();

    // Mise à jour de l'affichage
    void rafraichirTout();
    void rafraichirFiltres();
    void rafraichirIndicateurs();
    void rafraichirStatistiques();
    void rafraichirCharge();
    void rafraichirAffectation();
    void rafraichirDetails();
    void remplirFormulaire(const Employe *e);
    void message(const QString &texte, bool erreur = false);
    void signalerNouvellesAlertes(const QList<int> &avant);

    QList<Employe> listeFiltree() const;
    QList<int> idsEnAlerte() const;
    const Employe *employe(int id) const;

    // Widgets du formulaire mainwindow.ui (accès par ui->nomDuWidget)
    Ui::MainWindow *ui;

    // Données et état de l'écran
    QList<Employe> m_employes;
    QList<Tache> m_taches;
    QList<LigneRapport> m_rapport;
    bool m_rapportVisible;
    int m_selection;      // identifiant de l'employé affiché dans « Détails »
    int m_edition;        // identifiant de l'employé en cours de modification (0 = ajout)
    int m_page;
    Employe::Tri m_tri;
    bool m_croissant;
    bool m_statsParPoste;
    QString m_utilisateur;

    // Éléments créés par le code (absents du formulaire)
    QMenu *m_menuAlertes;                 // alertes de la cloche
    QList<QCheckBox *> m_fCompetences;    // cases « Compétences » du formulaire
    QLabel *m_toast;                      // message temporaire en bas de la fenêtre
    QTimer *m_toastTimer;
};

#endif // MAINWINDOW_H
