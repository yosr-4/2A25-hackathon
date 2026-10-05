// Publications d'un hackathon : publicité (image PNG), brochure (PDF) et texte d'annonce.
#ifndef PUBLICATIONS_H
#define PUBLICATIONS_H

#include <QDialog>
#include <QImage>
#include <QString>
#include "competition.h"

class QPlainTextEdit;

QImage dessinerPublicite(const Comp &c);                    // affiche carrée 1080 x 1080
bool ecrireBrochure(const Comp &c, const QString &chemin);  // brochure A4 ; false si écriture impossible
QString texteAnnonce(const Comp &c);                        // texte prêt à publier

// Fenêtre « Publications » : aperçu de la publicité + boutons d'enregistrement.
class PublicationsDialog : public QDialog {
public:
    PublicationsDialog(QWidget *parent, const Comp &c);

private:
    void enregistrerPublicite();
    void genererBrochure();
    QString cheminParDefaut(const QString &suffixe) const;

    Comp comp;
    QImage affiche;
    QPlainTextEdit *annonce = nullptr;
};

#endif // PUBLICATIONS_H
