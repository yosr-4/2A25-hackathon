// Cercle « Statut de la compétition ». Dans hackathon.ui, le widget « donut »
// est promu vers cette classe (clic droit > Promote to...).
#ifndef DONUT_H
#define DONUT_H

#include <QWidget>
#include <QMap>
#include <QPainter>
#include <QPen>
#include "competition.h"

class Donut : public QWidget {
public:
    explicit Donut(QWidget *parent = nullptr) : QWidget(parent) {}
    void setCounts(const QMap<QString, int> &c) { counts = c; update(); }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        int total = 0;
        for (int v : counts) total += v;
        const int w = 16;
        QRectF r(w / 2 + 2, w / 2 + 2, width() - w - 4, height() - w - 4);
        p.setPen(QPen(QColor("#E5EAF3"), w));
        p.drawArc(r, 0, 360 * 16);
        if (total > 0) {
            int start = 90 * 16;
            for (const QString &s : STATUTS) {
                int n = counts.value(s, 0);
                if (!n) continue;
                int span = -int(double(n) / total * 360 * 16);
                QPen pen(QColor(statutDot(s)), w);
                pen.setCapStyle(Qt::FlatCap);
                p.setPen(pen);
                p.drawArc(r, start, span);
                start += span;
            }
        }
        int active = counts.value("En cours", 0) + counts.value("Terminée", 0);
        int pct = total ? qRound(double(active) / total * 100) : 0;
        p.setPen(QColor("#1E293B"));
        QFont f; f.setPointSize(15); f.setBold(true);
        p.setFont(f);
        p.drawText(QRectF(0, 45, width(), 30), Qt::AlignCenter, QString("%1%").arg(pct));
        QFont f2; f2.setPointSize(6);
        p.setFont(f2);
        p.setPen(QColor("#64748B"));
        p.drawText(QRectF(0, 72, width(), 14), Qt::AlignCenter, "Taux de participation");
    }

private:
    QMap<QString, int> counts;
};

#endif // DONUT_H
