// Export de la liste des compétitions.
//  - PDF   : un tableau HTML mis en page par QTextDocument puis écrit par QPdfWriter.
//  - Excel : un vrai fichier .xlsx construit par xlsx.h (aucune bibliothèque externe).

#include "exports.h"
#include "xlsx.h"

#include <QDate>
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QMarginsF>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPdfWriter>
#include <QStringList>
#include <QTextDocument>
#include <QUrl>
#include <QWidget>
#include <string>
#include <vector>

// ---------------------------------------------------------------- PDF
bool exporterPdf(const QVector<Comp> &liste, const QString &chemin, const QString &filtres) {
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;   // dossier protégé, ou fichier déjà ouvert dans un lecteur PDF

    QPdfWriter pdf(&fichier);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageOrientation(QPageLayout::Landscape);
    pdf.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);  // QTextDocument ajoute déjà 2 cm
    pdf.setTitle("Liste des compétitions");
    pdf.setCreator("HackTime");

    const QStringList entetes = {"ID", "Nom", "Thème", "Date début", "Date fin", "Mode",
                                 "Équipes max", "Salle", "Jury", "Statut"};

    QString html;
    html += "<h1 style='color:#0B2A6B; margin-bottom:2px;'>HackTime — Liste des compétitions</h1>";
    html += QString("<p style='color:#64748B; margin-top:0px;'>Exporté le %1 &nbsp;·&nbsp; %2 compétition(s)")
                .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"))
                .arg(liste.size());
    if (!filtres.isEmpty())
        html += " &nbsp;·&nbsp; " + filtres.toHtmlEscaped();
    html += "</p>";

    html += "<table width='100%' cellspacing='0' cellpadding='6' border='1' "
            "style='border-collapse:collapse; border-color:#C7D2E3;'>";
    html += "<tr>";
    for (const QString &e : entetes)
        html += "<th bgcolor='#0B2A6B' style='color:#FFFFFF;'>" + e.toHtmlEscaped() + "</th>";
    html += "</tr>";

    int ligne = 0;
    for (const Comp &c : liste) {
        const QString fond = (ligne++ % 2 == 0) ? "#FFFFFF" : "#F3F6FC";
        const QStringList cellules = {c.id, c.nom, c.theme, c.debut, c.fin, c.mode,
                                      QString::number(c.max), c.salle, c.jury, c.statut};
        html += "<tr>";
        for (int i = 0; i < cellules.size(); ++i) {
            const bool centre = (i == 0 || i == 3 || i == 4 || i == 6);
            html += QString("<td bgcolor='%1'%2>%3</td>")
                        .arg(fond, QString(centre ? " align='center'" : ""), cellules[i].toHtmlEscaped());
        }
        html += "</tr>";
    }
    html += "</table>";

    QTextDocument doc;
    doc.setDefaultFont(QFont("Arial", 9));
    doc.setHtml(html);
    doc.print(&pdf);   // gère seul les sauts de page et numérote les pages
    return true;
}

// ---------------------------------------------------------------- Excel
static xlsx::Cell celluleTexte(const QString &s) {
    return xlsx::Cell::str(s.toStdString());   // toStdString() donne de l'UTF-8
}

// Une vraie date Excel (triable, filtrable) ; si la date est illisible on garde le texte.
static xlsx::Cell celluleDate(const QString &s) {
    const QDate d = QDate::fromString(s, DATE_FMT);
    if (!d.isValid()) return celluleTexte(s);
    return xlsx::Cell::date(double(QDate(1899, 12, 30).daysTo(d)));
}

bool exporterExcel(const QVector<Comp> &liste, const QString &chemin) {
    std::vector<xlsx::Row> lignes;
    lignes.push_back({celluleTexte("ID"), celluleTexte("Nom"), celluleTexte("Thème"), celluleTexte("Date début"),
                      celluleTexte("Date fin"), celluleTexte("Mode"), celluleTexte("Équipes max"), celluleTexte("Salle"),
                      celluleTexte("Jury"), celluleTexte("Statut"), celluleTexte("Description")});
    for (const Comp &c : liste) {
        lignes.push_back({celluleTexte(c.id), celluleTexte(c.nom), celluleTexte(c.theme), celluleDate(c.debut), celluleDate(c.fin),
                          celluleTexte(c.mode), xlsx::Cell::num(c.max), celluleTexte(c.salle), celluleTexte(c.jury),
                          celluleTexte(c.statut), celluleTexte(c.description)});
    }
    const std::vector<double> largeurs = {9, 32, 24, 13, 13, 13, 13, 15, 12, 13, 60};
    const std::string octets = xlsx::build(lignes, QString("Compétitions").toStdString(), largeurs);

    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;   // dossier protégé, ou classeur déjà ouvert dans Excel
    return fichier.write(octets.data(), qint64(octets.size())) == qint64(octets.size());
}

// ---------------------------------------------------------------- Ouverture du fichier
void proposerOuverture(QWidget *parent, const QString &chemin) {
    const auto r = QMessageBox::question(parent, "Fichier enregistré",
                       QString("Fichier enregistré :\n%1\n\nVoulez-vous l'ouvrir ?")
                           .arg(QDir::toNativeSeparators(chemin)));
    if (r == QMessageBox::Yes)
        QDesktopServices::openUrl(QUrl::fromLocalFile(chemin));
}
