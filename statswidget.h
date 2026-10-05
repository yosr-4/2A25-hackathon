#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QList>
#include <QPair>
#include <QString>
#include <QWidget>

// Graphique des statistiques : barres horizontales + anneau de répartition.
// Dessiné avec QPainter, sans module supplémentaire (pas besoin de Qt Charts).
class StatsWidget : public QWidget
{
public:
    explicit StatsWidget(QWidget *parent = nullptr);
    void setDonnees(const QList<QPair<QString, int> > &donnees);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<QPair<QString, int> > m_donnees;
};

#endif // STATSWIDGET_H
