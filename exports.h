// Export de la liste des compétitions en PDF et en Excel (.xlsx).
#ifndef EXPORTS_H
#define EXPORTS_H

#include <QString>
#include <QVector>
#include "competition.h"

// « filtres » : texte affiché sous le titre du PDF (ex. « Statut : En cours »).
// Les deux fonctions renvoient false si le fichier n'a pas pu être écrit.
bool exporterPdf(const QVector<Comp> &liste, const QString &chemin, const QString &filtres);
bool exporterExcel(const QVector<Comp> &liste, const QString &chemin);

// Après un enregistrement réussi : propose d'ouvrir le fichier avec le programme par défaut.
class QWidget;
void proposerOuverture(QWidget *parent, const QString &chemin);

#endif // EXPORTS_H
