#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Equipe {
    QString nom;
    QString competition;
    QString chef;
    QString membres;
    QString date;
    QString statut;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void chargerPage(int page);
    void onTeamSelected(int row, int column);
    void filtrerEquipes(const QString &text);
    void filtrerParComboBox();
    void on_btnAjouter_clicked();

private:
    Ui::MainWindow *ui;
    QList<Equipe> toutesLesEquipes;
    QList<Equipe> equipesAffichees;
    int currentPage = 1;
    void initialiserToutesLesEquipes();
};

#endif // MAINWINDOW_H