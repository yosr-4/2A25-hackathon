#ifndef HACKATHON_H
#define HACKATHON_H

#include <QMainWindow>
#include <QVector>
#include "competition.h"

QT_BEGIN_NAMESPACE
namespace Ui { class hackathon; }
QT_END_NAMESPACE

class hackathon : public QMainWindow {
    Q_OBJECT

public:
    explicit hackathon(QWidget *parent = nullptr);
    ~hackathon() override;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private slots:
    // ----- navigation entre les pages de SWCompet (slots créés par Qt Designer)
    void on_BLoginCompet_clicked();

private:
    // ----- mise en place (une fois, au démarrage)
    void setupNavigation();
    void setupTable();
    void setupFilters();
    void setupAddForm();

    // ----- formulaire d'ajout (colonne droite, sous les détails)
    void resetAddForm();
    void showAddError(const QString &msg);
    void submitAddForm();

    // ----- actions sur les compétitions
    void editCompetition(const QString &id);
    void deleteCompetition(const QString &id);
    void selectRow(const QString &id);
    void showDetails();
    void notImplemented(const QString &module);

    // ----- fonctionnalités avancées (boutons au-dessus du tableau)
    bool confirmerMalgreConflits(const Comp &c);   // détection des conflits de planning
    void showConflicts();
    void exportList(bool pdf);                     // export PDF / Excel
    void openPublications();                       // brochure et publicité
    QString filtresTexte() const;

    // ----- affichage
    void onSelect();
    void onFilter();
    void resetFilters();
    void refreshYears();
    void refresh(bool rebuildYears = true);
    void buildPager(int pages);
    void goTo(int p);
    void updateKpis();
    void updateDetails();
    QWidget *makeBadge(const QString &statut);
    QWidget *makeActions(const QString &id);

    // ----- utilitaires
    QString nextId() const;
    int indexOf(const QString &id) const;
    QVector<Comp> applyFilters() const;

    Ui::hackathon *ui;          // tous les widgets dessinés dans hackathon.ui
    QVector<Comp> data, filtered;
    int page = 0;
    QString selectedId;
};

#endif // HACKATHON_H
