// Publications d'un hackathon (brochures et publicités).
//  - la publicité est dessinée avec QPainter dans une image 1080 x 1080 ;
//  - la brochure est une page HTML mise en page par QTextDocument et écrite en PDF ;
//  - le texte d'annonce se copie dans le presse-papiers.

#include "publications.h"
#include "exports.h"   // proposerOuverture()

#include <QBrush>
#include <QClipboard>
#include <QColor>
#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLinearGradient>
#include <QLocale>
#include <QMarginsF>
#include <QMessageBox>
#include <QPageLayout>
#include <QPageSize>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStandardPaths>
#include <QStringList>
#include <QTextDocument>
#include <QVBoxLayout>

// ---------------------------------------------------------------- Textes communs
static QDate lireDate(const QString &s) { return QDate::fromString(s, DATE_FMT); }

// « Du 12 au 14 mai 2025 »
static QString periode(const Comp &c) {
    const QLocale fr(QLocale::French);
    const QDate d = lireDate(c.debut), f = lireDate(c.fin);
    if (!d.isValid() || !f.isValid())
        return QString("Du %1 au %2").arg(c.debut, c.fin);
    const QString finTexte = fr.toString(f, "d MMMM yyyy");
    if (d == f)
        return QString("Le %1").arg(finTexte);
    if (d.year() == f.year() && d.month() == f.month())
        return QString("Du %1 au %2").arg(QString::number(d.day()), finTexte);
    if (d.year() == f.year())
        return QString("Du %1 au %2").arg(fr.toString(d, "d MMMM"), finTexte);
    return QString("Du %1 au %2").arg(fr.toString(d, "d MMMM yyyy"), finTexte);
}

static bool lieuConnu(const Comp &c) {
    return !c.salle.isEmpty() && c.salle != NON_DEFINI;
}

static QString places(const Comp &c) {
    if (c.mode == "Équipes")
        return QString("%1 équipes max").arg(c.max);
    return QString("%1 places").arg(c.max);
}

static QString presentation(const Comp &c) {
    const QString d = c.description.simplified();
    if (!d.isEmpty()) return d;
    return QString("Relevez le défi « %1 » et imaginez les solutions de demain.").arg(c.theme);
}

QString texteAnnonce(const Comp &c) {
    QStringList lignes;
    lignes << QString("🚀 %1 — le hackathon « %2 » arrive !").arg(c.nom, c.theme);
    lignes << QString();
    lignes << QString("📅 %1").arg(periode(c));
    if (lieuConnu(c))
        lignes << QString("📍 %1").arg(c.salle);
    lignes << QString("👥 Participation : %1 · %2").arg(c.mode.toLower(), places(c));
    lignes << QString();
    lignes << presentation(c);
    lignes << QString();
    lignes << "Inscrivez-vous dès maintenant, les places sont limitées !";
    lignes << QString("#Hackathon #HackTime #%1").arg(QString(c.theme).remove(QChar(' ')));
    return lignes.join("\n");
}

// ---------------------------------------------------------------- Publicité (image)
static QFont police(int pixels, bool gras) {
    QFont f("Segoe UI");
    f.setPixelSize(pixels);
    f.setBold(gras);
    return f;
}

QImage dessinerPublicite(const Comp &c) {
    const int W = 1080, H = 1080, M = 70;      // taille de l'image et marge
    const int GAUCHE = int(Qt::AlignLeft) | int(Qt::AlignVCenter);
    const QColor blanc(255, 255, 255);

    QImage img(W, H, QImage::Format_ARGB32_Premultiplied);
    img.fill(QColor("#0B2A6B"));
    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // fond : dégradé bleu + cercles décoratifs
    QLinearGradient fond(0, 0, W, H);
    fond.setColorAt(0.0, QColor("#0B2A6B"));
    fond.setColorAt(1.0, QColor("#1665E8"));
    p.fillRect(QRect(0, 0, W, H), QBrush(fond));
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(255, 255, 255, 16));
    p.drawEllipse(QPointF(W - 90, 150), 320, 320);
    p.drawEllipse(QPointF(W - 90, 150), 210, 210);
    p.drawEllipse(QPointF(60, H - 300), 180, 180);

    // logo (version blanche, pour fond sombre)
    const QPixmap logo(":/images/logo.png");
    if (!logo.isNull())
        p.drawPixmap(M, 60, logo.scaledToHeight(120, Qt::SmoothTransformation));

    // pastille « HACKATHON · THÈME »
    const QString etiquette = QString("HACKATHON  ·  %1").arg(c.theme.toUpper());
    const QFont fEtiquette = police(28, true);
    const QRect pastille(M, 250, QFontMetrics(fEtiquette).horizontalAdvance(etiquette) + 56, 58);
    p.setBrush(QColor("#E07527"));
    p.drawRoundedRect(pastille, 29, 29);
    p.setFont(fEtiquette);
    p.setPen(blanc);
    p.drawText(pastille, int(Qt::AlignCenter), etiquette);

    // nom de la compétition : on réduit la police jusqu'à ce que le texte tienne
    const QRect zoneTitre(M, 330, W - 2 * M, 280);
    const int drapeauxTitre = GAUCHE | int(Qt::TextWordWrap);
    int taille = 100;
    QFont fTitre = police(taille, true);
    while (taille > 44) {
        fTitre.setPixelSize(taille);
        const QRect r = QFontMetrics(fTitre).boundingRect(zoneTitre, drapeauxTitre, c.nom);
        if (r.height() <= zoneTitre.height() && r.width() <= zoneTitre.width()) break;
        taille -= 4;
    }
    fTitre.setPixelSize(taille);
    p.setFont(fTitre);
    p.drawText(zoneTitre, drapeauxTitre, c.nom);

    // dates
    p.setFont(police(46, true));
    p.setPen(QColor("#FFC58A"));
    p.drawText(QRect(M, 626, W - 2 * M, 62), GAUCHE, periode(c));

    // phrase de présentation (3 lignes au plus)
    QString resume = presentation(c);
    if (resume.size() > 130) resume = resume.left(127) + QString("…");
    p.setFont(police(28, false));
    p.setPen(QColor(230, 238, 255));
    p.drawText(QRect(M, 700, W - 2 * M, 116),
               int(Qt::AlignLeft) | int(Qt::AlignTop) | int(Qt::TextWordWrap), resume);

    // trois cartes d'information
    const QStringList titres = {"MODE", "PLACES", "LIEU"};
    const QStringList valeurs = {c.mode, places(c), lieuConnu(c) ? c.salle : QString("À venir")};
    const int ecart = 20, largeur = (W - 2 * M - 2 * ecart) / 3;
    const QFont fValeur = police(30, true);
    for (int i = 0; i < 3; ++i) {
        const QRect carte(M + i * (largeur + ecart), 832, largeur, 104);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(255, 255, 255, 34));
        p.drawRoundedRect(carte, 16, 16);
        p.setPen(QColor("#BFD3FF"));
        p.setFont(police(20, true));
        p.drawText(QRect(carte.x() + 22, carte.y() + 14, largeur - 34, 30), GAUCHE, titres[i]);
        p.setPen(blanc);
        p.setFont(fValeur);
        p.drawText(QRect(carte.x() + 22, carte.y() + 48, largeur - 34, 44), GAUCHE,
                   QFontMetrics(fValeur).elidedText(valeurs[i], Qt::ElideRight, largeur - 34));
    }

    // bandeau du bas
    const QRect bandeau(0, 962, W, H - 962);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#E07527"));
    p.drawRect(bandeau);
    p.setPen(blanc);
    p.setFont(police(40, true));
    p.drawText(bandeau.adjusted(M, 0, -M, 0), GAUCHE, QString("Inscrivez-vous dès maintenant !"));
    p.setFont(police(24, false));
    p.drawText(bandeau.adjusted(M, 0, -M, 0), int(Qt::AlignRight) | int(Qt::AlignVCenter),
               QString("Places limitées"));
    p.end();
    return img;
}

// ---------------------------------------------------------------- Brochure (PDF)
static QString ligneInfo(const QString &titre, const QString &valeur) {
    return QString("<tr><td bgcolor='#F3F6FC' width='30%'><b>%1</b></td><td>%2</td></tr>")
        .arg(titre.toHtmlEscaped(), valeur.toHtmlEscaped());
}

bool ecrireBrochure(const Comp &c, const QString &chemin) {
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QPdfWriter pdf(&fichier);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageOrientation(QPageLayout::Portrait);
    pdf.setPageMargins(QMarginsF(0, 0, 0, 0), QPageLayout::Millimeter);  // QTextDocument ajoute déjà 2 cm
    pdf.setTitle(QString("Brochure — %1").arg(c.nom));
    pdf.setCreator("HackTime");

    const QLocale fr(QLocale::French);
    const QDate d = lireDate(c.debut), f = lireDate(c.fin);
    const int jours = (d.isValid() && f.isValid() && f >= d) ? int(d.daysTo(f)) + 1 : 0;
    const QString tableau = "<table width='100%' cellspacing='0' cellpadding='7' border='1' "
                            "style='border-collapse:collapse; border-color:#C7D2E3;'>";
    const QString h2 = "<h2 style='color:#1665E8; margin-top:18px; margin-bottom:6px;'>%1</h2>";

    QString html;
    html += "<p align='right'><img src=':/images/Logo_HackTime.png' width='176' height='50'></p>";

    // bandeau de titre
    html += "<table width='100%' cellspacing='0' cellpadding='16'><tr><td bgcolor='#0B2A6B'>";
    html += QString("<p style='color:#FFC58A; font-size:10pt; font-weight:bold; margin:0px;'>HACKATHON · %1</p>")
                .arg(c.theme.toUpper().toHtmlEscaped());
    html += QString("<p style='color:#FFFFFF; font-size:24pt; font-weight:bold; margin-top:6px; margin-bottom:6px;'>%1</p>")
                .arg(c.nom.toHtmlEscaped());
    QString sousTitre = periode(c);
    if (lieuConnu(c)) sousTitre += QString(" — %1").arg(c.salle);
    html += QString("<p style='color:#DCE9FF; font-size:12pt; margin:0px;'>%1</p>").arg(sousTitre.toHtmlEscaped());
    html += "</td></tr></table>";

    html += h2.arg(QString("Présentation"));
    html += QString("<p style='font-size:11pt;'>%1</p>").arg(presentation(c).toHtmlEscaped());

    html += h2.arg(QString("Informations pratiques"));
    html += tableau;
    html += ligneInfo("Thème", c.theme);
    html += ligneInfo("Dates", jours > 0 ? QString("%1 (%2 jour(s))").arg(periode(c)).arg(jours) : periode(c));
    html += ligneInfo("Mode de participation", c.mode);
    html += ligneInfo("Capacité", places(c));
    html += ligneInfo("Lieu", lieuConnu(c) ? c.salle : QString("Communiqué prochainement"));
    if (!c.jury.isEmpty() && c.jury != NON_DEFINI)
        html += ligneInfo("Jury", c.jury);
    html += ligneInfo("Statut", c.statut);
    html += "</table>";

    if (jours >= 1 && jours <= 10) {
        html += h2.arg(QString("Programme"));
        html += tableau;
        for (int i = 0; i < jours; ++i) {
            QString contenu;
            if (jours == 1)
                contenu = "Accueil et lancement, développement des projets, démonstrations devant le jury et remise des prix.";
            else if (i == 0)
                contenu = "Accueil des participants, présentation du thème et des règles, formation des équipes, début du développement.";
            else if (i == jours - 1)
                contenu = "Finalisation des projets, démonstrations devant le jury, délibération et remise des prix.";
            else
                contenu = "Développement des projets, ateliers et accompagnement par les coachs.";
            html += ligneInfo(QString("Jour %1 — %2").arg(i + 1).arg(fr.toString(d.addDays(i), "dddd d MMMM")), contenu);
        }
        html += "</table>";
    }

    html += h2.arg(QString("Comment participer ?"));
    html += "<ol style='font-size:11pt;'>"
            "<li>Inscrivez-vous auprès de l'équipe d'organisation avant le début de la compétition.</li>"
            "<li>Constituez votre équipe ou rejoignez-en une sur place.</li>"
            "<li>Présentez-vous le premier jour avec votre ordinateur et vos idées.</li>"
            "</ol>";
    html += "<p align='center' style='color:#64748B; font-size:8pt; margin-top:24px;'>"
            "Document généré par HackTime — Gestion intelligente de hackathons</p>";

    QTextDocument doc;
    doc.setDefaultFont(QFont("Arial", 10));
    doc.setHtml(html);
    doc.print(&pdf);
    return true;
}

// ---------------------------------------------------------------- Fenêtre « Publications »
PublicationsDialog::PublicationsDialog(QWidget *parent, const Comp &c)
    : QDialog(parent), comp(c), affiche(dessinerPublicite(c)) {
    setWindowTitle(QString("Publications — %1").arg(comp.nom));

    QLabel *apercu = new QLabel;
    apercu->setFixedSize(430, 430);
    apercu->setPixmap(QPixmap::fromImage(affiche).scaled(430, 430, Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));

    QLabel *titre = new QLabel("<b>Publicité, brochure et annonce</b>");
    QLabel *aide = new QLabel("L'aperçu à gauche est la publicité (image 1080 × 1080, prête pour "
                              "les réseaux sociaux). La brochure est un PDF A4 d'une page avec la "
                              "présentation, les informations pratiques et le programme.");
    aide->setWordWrap(true);

    annonce = new QPlainTextEdit;
    annonce->setPlainText(texteAnnonce(comp));
    annonce->setMinimumSize(360, 190);

    QPushButton *bCopier = new QPushButton("📋  Copier le texte d'annonce");
    QPushButton *bPub = new QPushButton("🖼  Enregistrer la publicité (PNG)");
    QPushButton *bBrochure = new QPushButton("📄  Générer la brochure (PDF)");
    QPushButton *bFermer = new QPushButton("Fermer");

    connect(bCopier, &QPushButton::clicked, this, [this, bCopier]() {
        QGuiApplication::clipboard()->setText(annonce->toPlainText());
        bCopier->setText("✔  Texte copié");
    });
    connect(bPub, &QPushButton::clicked, this, [this]() { enregistrerPublicite(); });
    connect(bBrochure, &QPushButton::clicked, this, [this]() { genererBrochure(); });
    connect(bFermer, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout *droite = new QVBoxLayout;
    droite->setSpacing(8);
    droite->addWidget(titre);
    droite->addWidget(aide);
    droite->addWidget(new QLabel("Texte d'annonce (modifiable) :"));
    droite->addWidget(annonce, 1);
    droite->addWidget(bCopier);
    droite->addSpacing(10);
    droite->addWidget(bPub);
    droite->addWidget(bBrochure);
    droite->addSpacing(10);
    droite->addWidget(bFermer);

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setSpacing(18);
    lay->addWidget(apercu, 0, Qt::AlignTop);
    lay->addLayout(droite, 1);
}

// Documents/C-001_Hack_for_a_Better_Future + suffixe
QString PublicationsDialog::cheminParDefaut(const QString &suffixe) const {
    QString nom;
    for (const QChar ch : comp.nom)
        nom += ch.isLetterOrNumber() ? ch : QChar('_');
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
           + "/" + comp.id + "_" + nom + suffixe;
}

void PublicationsDialog::enregistrerPublicite() {
    QString chemin = QFileDialog::getSaveFileName(this, "Enregistrer la publicité",
                                                  cheminParDefaut("_publicite.png"), "Image PNG (*.png)");
    if (chemin.isEmpty()) return;
    if (!chemin.endsWith(".png", Qt::CaseInsensitive)) chemin += ".png";
    if (!affiche.save(chemin, "PNG")) {
        QMessageBox::warning(this, "Publicité", "Impossible d'enregistrer l'image à cet emplacement.");
        return;
    }
    proposerOuverture(this, chemin);
}

void PublicationsDialog::genererBrochure() {
    QString chemin = QFileDialog::getSaveFileName(this, "Générer la brochure",
                                                  cheminParDefaut("_brochure.pdf"), "Document PDF (*.pdf)");
    if (chemin.isEmpty()) return;
    if (!chemin.endsWith(".pdf", Qt::CaseInsensitive)) chemin += ".pdf";
    if (!ecrireBrochure(comp, chemin)) {
        QMessageBox::warning(this, "Brochure",
                             "Impossible d'écrire le fichier (est-il déjà ouvert dans un autre programme ?).");
        return;
    }
    proposerOuverture(this, chemin);
}
