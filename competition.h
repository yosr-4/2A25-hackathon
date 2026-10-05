// Modèle de données : une compétition + les listes de choix et les couleurs de statut.
#ifndef COMPETITION_H
#define COMPETITION_H

#include <QString>
#include <QStringList>

struct Comp {
    QString id, nom, theme, debut, fin, mode, statut;
    int max = 5;
};

static const QString DATE_FMT = "dd/MM/yyyy";
static const int PER_PAGE = 8;

static const QStringList THEMES = {"Environnement", "Santé", "Éducation", "Ville intelligente",
                                   "Agriculture", "Intelligence artificielle", "Développement",
                                   "Finance", "Autre"};
static const QStringList MODES = {"Équipes", "Individuel"};
static const QStringList STATUTS = {"Planifiée", "En cours", "Terminée"};

inline QString themeIcon(const QString &t) {
    if (t == "Environnement") return "🍃";
    if (t == "Santé") return "❤️";
    if (t == "Éducation") return "🎓";
    if (t == "Ville intelligente") return "🏙️";
    if (t == "Agriculture") return "🌱";
    if (t == "Intelligence artificielle") return "🤖";
    if (t == "Développement") return "</>";
    if (t == "Finance") return "🪙";
    return "⭐";
}
inline QString statutBg(const QString &s) {
    if (s == "En cours") return "#DDF3E4";
    if (s == "Terminée") return "#E5E9F0";
    return "#DCE9FF";
}
inline QString statutFg(const QString &s) {
    if (s == "En cours") return "#2E9E5B";
    if (s == "Terminée") return "#64748B";
    return "#2563EB";
}
inline QString statutDot(const QString &s) {
    if (s == "En cours") return "#2E9E5B";
    if (s == "Terminée") return "#94A3B8";
    return "#2563EB";
}

#endif // COMPETITION_H
