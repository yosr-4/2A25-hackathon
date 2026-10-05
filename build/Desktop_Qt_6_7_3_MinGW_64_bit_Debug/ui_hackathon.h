/********************************************************************************
** Form generated from reading UI file 'hackathon.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HACKATHON_H
#define UI_HACKATHON_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "donut.h"

QT_BEGIN_NAMESPACE

class Ui_hackathon
{
public:
    QWidget *centralwidget;
    QVBoxLayout *centralLayout;
    QStackedWidget *SWHackTime;
    QWidget *PComp;
    QVBoxLayout *pCompLayout;
    QStackedWidget *SWCompet;
    QWidget *PLoginCompet;
    QVBoxLayout *loginLayout;
    QHBoxLayout *loginTopLayout;
    QSpacerItem *loginTopSpacer;
    QLabel *logoLoginCompet;
    QSpacerItem *loginSpacerHaut;
    QHBoxLayout *loginCenterLayout;
    QSpacerItem *loginSpacerGauche;
    QGridLayout *loginFormLayout;
    QLabel *lblLoginCompet;
    QLineEdit *EditLoginCompet;
    QLabel *lblPWDCompet;
    QLineEdit *EditPWDCompet;
    QPushButton *BForgotCompet;
    QPushButton *BLoginCompet;
    QSpacerItem *loginSpacerDroite;
    QSpacerItem *loginSpacerBas;
    QWidget *CRUDCompet;
    QHBoxLayout *rootLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logo;
    QSpacerItem *sidebarGap;
    QPushButton *btnDashboard;
    QPushButton *btnCompetitions;
    QPushButton *btnEquipes;
    QPushButton *btnParticipants;
    QPushButton *btnChallenges;
    QPushButton *btnJury;
    QPushButton *btnRapports;
    QPushButton *btnParametres;
    QSpacerItem *sidebarSpacer;
    QLabel *tagline;
    QWidget *content;
    QVBoxLayout *contentLayout;
    QFrame *topbar;
    QHBoxLayout *topbarLayout;
    QLineEdit *topSearch;
    QSpacerItem *topbarSpacer;
    QLabel *bell;
    QSpacerItem *topbarGap;
    QLabel *admin;
    QWidget *body;
    QHBoxLayout *bodyLayout;
    QWidget *leftCol;
    QVBoxLayout *leftLayout;
    QFrame *cardHeader;
    QHBoxLayout *headerLayout;
    QLabel *headerIcon;
    QVBoxLayout *headerTextLayout;
    QLabel *lblTitle;
    QLabel *lblSub;
    QSpacerItem *headerSpacer;
    QWidget *kpiRow;
    QHBoxLayout *kpiLayout;
    QFrame *cardKpi1;
    QHBoxLayout *kpiCardLayout1;
    QLabel *kpiIcon1;
    QVBoxLayout *kpiTextLayout1;
    QLabel *kpiTitle1;
    QLabel *kpiTotal;
    QSpacerItem *kpiSpacer1;
    QFrame *cardKpi2;
    QHBoxLayout *kpiCardLayout2;
    QLabel *kpiIcon2;
    QVBoxLayout *kpiTextLayout2;
    QLabel *kpiTitle2;
    QLabel *kpiEquipes;
    QSpacerItem *kpiSpacer2;
    QFrame *cardKpi3;
    QHBoxLayout *kpiCardLayout3;
    QLabel *kpiIcon3;
    QVBoxLayout *kpiTextLayout3;
    QLabel *kpiTitle3;
    QLabel *kpiPart;
    QSpacerItem *kpiSpacer3;
    QFrame *cardKpi4;
    QHBoxLayout *kpiCardLayout4;
    QLabel *kpiIcon4;
    QVBoxLayout *kpiTextLayout4;
    QLabel *kpiTitle4;
    QLabel *kpiCours;
    QSpacerItem *kpiSpacer4;
    QFrame *cardList;
    QVBoxLayout *listLayout;
    QLabel *lblList;
    QHBoxLayout *filterLayout;
    QLineEdit *search;
    QVBoxLayout *fStatutLayout;
    QLabel *fStatutLabel;
    QComboBox *fStatut;
    QVBoxLayout *fThemeLayout;
    QLabel *fThemeLabel;
    QComboBox *fTheme;
    QVBoxLayout *fDateLayout;
    QLabel *fDateLabel;
    QComboBox *fDate;
    QTableWidget *table;
    QHBoxLayout *listBottomLayout;
    QLabel *footer;
    QSpacerItem *listBottomSpacer;
    QWidget *pagerBox;
    QHBoxLayout *pagerLayout;
    QScrollArea *rightScroll;
    QWidget *rightCol;
    QVBoxLayout *rightLayout;
    QFrame *cardDetails;
    QVBoxLayout *detailsLayout;
    QLabel *lblDetails;
    QFrame *detailHead;
    QHBoxLayout *detailHeadLayout;
    QLabel *dIco;
    QVBoxLayout *detailNameLayout;
    QLabel *dNom;
    QLabel *dId;
    QSpacerItem *detailHeadSpacer;
    QVBoxLayout *detailBadgeLayout;
    QLabel *dBadge;
    QSpacerItem *detailBadgeSpacer;
    QGridLayout *detailsGrid;
    QLabel *dThemeLabel;
    QLabel *dTheme;
    QLabel *dDebutLabel;
    QLabel *dDebut;
    QLabel *dFinLabel;
    QLabel *dFin;
    QLabel *dModeLabel;
    QLabel *dMode;
    QLabel *dMaxLabel;
    QLabel *dMax;
    QPushButton *btnDetails;
    QFrame *cardAdd;
    QVBoxLayout *addLayout;
    QLabel *lblAdd;
    QFormLayout *addFormLayout;
    QLabel *aIdLabel;
    QLabel *aId;
    QLabel *aNomLabel;
    QLineEdit *aNom;
    QLabel *aThemeLabel;
    QComboBox *aTheme;
    QLabel *aDebutLabel;
    QDateEdit *aDebut;
    QLabel *aFinLabel;
    QDateEdit *aFin;
    QLabel *aModeLabel;
    QComboBox *aMode;
    QLabel *aMaxLabel;
    QSpinBox *aMax;
    QLabel *aStatutLabel;
    QComboBox *aStatut;
    QLabel *aErr;
    QHBoxLayout *addButtonsLayout;
    QPushButton *btnSave;
    QPushButton *btnClear;
    QFrame *cardStatut;
    QVBoxLayout *statutLayout;
    QLabel *lblStatut;
    QHBoxLayout *statutRowLayout;
    Donut *donut;
    QVBoxLayout *legendLayout;
    QHBoxLayout *legendCoursLayout;
    QLabel *dotCours;
    QLabel *legNameCours;
    QSpacerItem *legendCoursSpacer;
    QLabel *legCours;
    QHBoxLayout *legendPlanifieeLayout;
    QLabel *dotPlanifiee;
    QLabel *legNamePlanifiee;
    QSpacerItem *legendPlanifieeSpacer;
    QLabel *legPlanifiee;
    QHBoxLayout *legendTermineeLayout;
    QLabel *dotTerminee;
    QLabel *legNameTerminee;
    QSpacerItem *legendTermineeSpacer;
    QLabel *legTerminee;
    QFrame *cardActions;
    QVBoxLayout *actionsLayout;
    QLabel *lblActions;
    QPushButton *btnTeams;
    QPushButton *btnViewJury;
    QPushButton *btnReports;
    QSpacerItem *rightSpacer;

    void setupUi(QMainWindow *hackathon)
    {
        if (hackathon->objectName().isEmpty())
            hackathon->setObjectName("hackathon");
        hackathon->resize(1536, 960);
        centralwidget = new QWidget(hackathon);
        centralwidget->setObjectName("centralwidget");
        centralLayout = new QVBoxLayout(centralwidget);
        centralLayout->setSpacing(0);
        centralLayout->setObjectName("centralLayout");
        centralLayout->setContentsMargins(0, 0, 0, 0);
        SWHackTime = new QStackedWidget(centralwidget);
        SWHackTime->setObjectName("SWHackTime");
        PComp = new QWidget();
        PComp->setObjectName("PComp");
        pCompLayout = new QVBoxLayout(PComp);
        pCompLayout->setSpacing(0);
        pCompLayout->setObjectName("pCompLayout");
        pCompLayout->setContentsMargins(0, 0, 0, 0);
        SWCompet = new QStackedWidget(PComp);
        SWCompet->setObjectName("SWCompet");
        PLoginCompet = new QWidget();
        PLoginCompet->setObjectName("PLoginCompet");
        loginLayout = new QVBoxLayout(PLoginCompet);
        loginLayout->setObjectName("loginLayout");
        loginLayout->setContentsMargins(40, 30, 40, 30);
        loginTopLayout = new QHBoxLayout();
        loginTopLayout->setObjectName("loginTopLayout");
        loginTopSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loginTopLayout->addItem(loginTopSpacer);

        logoLoginCompet = new QLabel(PLoginCompet);
        logoLoginCompet->setObjectName("logoLoginCompet");
        logoLoginCompet->setMinimumSize(QSize(320, 91));
        logoLoginCompet->setMaximumSize(QSize(320, 91));
        logoLoginCompet->setPixmap(QPixmap(QString::fromUtf8(":/images/Logo_HackTime.png")));
        logoLoginCompet->setScaledContents(true);

        loginTopLayout->addWidget(logoLoginCompet);


        loginLayout->addLayout(loginTopLayout);

        loginSpacerHaut = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginLayout->addItem(loginSpacerHaut);

        loginCenterLayout = new QHBoxLayout();
        loginCenterLayout->setObjectName("loginCenterLayout");
        loginSpacerGauche = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loginCenterLayout->addItem(loginSpacerGauche);

        loginFormLayout = new QGridLayout();
        loginFormLayout->setObjectName("loginFormLayout");
        loginFormLayout->setHorizontalSpacing(48);
        loginFormLayout->setVerticalSpacing(24);
        lblLoginCompet = new QLabel(PLoginCompet);
        lblLoginCompet->setObjectName("lblLoginCompet");

        loginFormLayout->addWidget(lblLoginCompet, 0, 0, 1, 1);

        EditLoginCompet = new QLineEdit(PLoginCompet);
        EditLoginCompet->setObjectName("EditLoginCompet");
        EditLoginCompet->setMinimumSize(QSize(280, 0));

        loginFormLayout->addWidget(EditLoginCompet, 0, 1, 1, 1);

        lblPWDCompet = new QLabel(PLoginCompet);
        lblPWDCompet->setObjectName("lblPWDCompet");

        loginFormLayout->addWidget(lblPWDCompet, 1, 0, 1, 1);

        EditPWDCompet = new QLineEdit(PLoginCompet);
        EditPWDCompet->setObjectName("EditPWDCompet");
        EditPWDCompet->setMinimumSize(QSize(280, 0));
        EditPWDCompet->setEchoMode(QLineEdit::Password);

        loginFormLayout->addWidget(EditPWDCompet, 1, 1, 1, 1);

        BForgotCompet = new QPushButton(PLoginCompet);
        BForgotCompet->setObjectName("BForgotCompet");
        BForgotCompet->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        BForgotCompet->setFlat(true);

        loginFormLayout->addWidget(BForgotCompet, 2, 1, 1, 1, Qt::AlignHCenter);

        BLoginCompet = new QPushButton(PLoginCompet);
        BLoginCompet->setObjectName("BLoginCompet");
        BLoginCompet->setMinimumSize(QSize(120, 0));
        BLoginCompet->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        BLoginCompet->setProperty("kind", QVariant(QString::fromUtf8("primary")));

        loginFormLayout->addWidget(BLoginCompet, 3, 1, 1, 1, Qt::AlignHCenter);


        loginCenterLayout->addLayout(loginFormLayout);

        loginSpacerDroite = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        loginCenterLayout->addItem(loginSpacerDroite);


        loginLayout->addLayout(loginCenterLayout);

        loginSpacerBas = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginLayout->addItem(loginSpacerBas);

        loginLayout->setStretch(1, 2);
        loginLayout->setStretch(3, 3);
        SWCompet->addWidget(PLoginCompet);
        CRUDCompet = new QWidget();
        CRUDCompet->setObjectName("CRUDCompet");
        rootLayout = new QHBoxLayout(CRUDCompet);
        rootLayout->setSpacing(0);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(CRUDCompet);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(262, 0));
        sidebar->setMaximumSize(QSize(262, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(4);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(14, 24, 14, 24);
        logo = new QLabel(sidebar);
        logo->setObjectName("logo");
        logo->setMinimumSize(QSize(234, 63));
        logo->setMaximumSize(QSize(234, 63));
        logo->setPixmap(QPixmap(QString::fromUtf8(":/images/logo.png")));
        logo->setScaledContents(true);

        sidebarLayout->addWidget(logo);

        sidebarGap = new QSpacerItem(20, 24, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(sidebarGap);

        btnDashboard = new QPushButton(sidebar);
        btnDashboard->setObjectName("btnDashboard");
        btnDashboard->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnDashboard->setCheckable(true);
        btnDashboard->setAutoExclusive(true);

        sidebarLayout->addWidget(btnDashboard);

        btnCompetitions = new QPushButton(sidebar);
        btnCompetitions->setObjectName("btnCompetitions");
        btnCompetitions->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnCompetitions->setCheckable(true);
        btnCompetitions->setChecked(true);
        btnCompetitions->setAutoExclusive(true);

        sidebarLayout->addWidget(btnCompetitions);

        btnEquipes = new QPushButton(sidebar);
        btnEquipes->setObjectName("btnEquipes");
        btnEquipes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnEquipes->setCheckable(true);
        btnEquipes->setAutoExclusive(true);

        sidebarLayout->addWidget(btnEquipes);

        btnParticipants = new QPushButton(sidebar);
        btnParticipants->setObjectName("btnParticipants");
        btnParticipants->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnParticipants->setCheckable(true);
        btnParticipants->setAutoExclusive(true);

        sidebarLayout->addWidget(btnParticipants);

        btnChallenges = new QPushButton(sidebar);
        btnChallenges->setObjectName("btnChallenges");
        btnChallenges->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnChallenges->setCheckable(true);
        btnChallenges->setAutoExclusive(true);

        sidebarLayout->addWidget(btnChallenges);

        btnJury = new QPushButton(sidebar);
        btnJury->setObjectName("btnJury");
        btnJury->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnJury->setCheckable(true);
        btnJury->setAutoExclusive(true);

        sidebarLayout->addWidget(btnJury);

        btnRapports = new QPushButton(sidebar);
        btnRapports->setObjectName("btnRapports");
        btnRapports->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnRapports->setCheckable(true);
        btnRapports->setAutoExclusive(true);

        sidebarLayout->addWidget(btnRapports);

        btnParametres = new QPushButton(sidebar);
        btnParametres->setObjectName("btnParametres");
        btnParametres->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnParametres->setCheckable(true);
        btnParametres->setAutoExclusive(true);

        sidebarLayout->addWidget(btnParametres);

        sidebarSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarSpacer);

        tagline = new QLabel(sidebar);
        tagline->setObjectName("tagline");

        sidebarLayout->addWidget(tagline);


        rootLayout->addWidget(sidebar);

        content = new QWidget(CRUDCompet);
        content->setObjectName("content");
        contentLayout = new QVBoxLayout(content);
        contentLayout->setSpacing(0);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(0, 0, 0, 0);
        topbar = new QFrame(content);
        topbar->setObjectName("topbar");
        topbar->setMinimumSize(QSize(0, 76));
        topbar->setMaximumSize(QSize(16777215, 76));
        topbarLayout = new QHBoxLayout(topbar);
        topbarLayout->setObjectName("topbarLayout");
        topbarLayout->setContentsMargins(22, 0, 22, 0);
        topSearch = new QLineEdit(topbar);
        topSearch->setObjectName("topSearch");
        topSearch->setMinimumSize(QSize(560, 0));
        topSearch->setMaximumSize(QSize(560, 16777215));

        topbarLayout->addWidget(topSearch);

        topbarSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        topbarLayout->addItem(topbarSpacer);

        bell = new QLabel(topbar);
        bell->setObjectName("bell");

        topbarLayout->addWidget(bell);

        topbarGap = new QSpacerItem(24, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        topbarLayout->addItem(topbarGap);

        admin = new QLabel(topbar);
        admin->setObjectName("admin");
        admin->setTextFormat(Qt::RichText);

        topbarLayout->addWidget(admin);


        contentLayout->addWidget(topbar);

        body = new QWidget(content);
        body->setObjectName("body");
        bodyLayout = new QHBoxLayout(body);
        bodyLayout->setSpacing(18);
        bodyLayout->setObjectName("bodyLayout");
        bodyLayout->setContentsMargins(18, 16, 6, 16);
        leftCol = new QWidget(body);
        leftCol->setObjectName("leftCol");
        leftLayout = new QVBoxLayout(leftCol);
        leftLayout->setSpacing(16);
        leftLayout->setObjectName("leftLayout");
        leftLayout->setContentsMargins(0, 0, 0, 0);
        cardHeader = new QFrame(leftCol);
        cardHeader->setObjectName("cardHeader");
        cardHeader->setProperty("card", QVariant(true));
        headerLayout = new QHBoxLayout(cardHeader);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(12, 12, 20, 12);
        headerIcon = new QLabel(cardHeader);
        headerIcon->setObjectName("headerIcon");
        headerIcon->setMinimumSize(QSize(72, 72));
        headerIcon->setMaximumSize(QSize(72, 72));
        headerIcon->setAlignment(Qt::AlignCenter);

        headerLayout->addWidget(headerIcon);

        headerTextLayout = new QVBoxLayout();
        headerTextLayout->setSpacing(2);
        headerTextLayout->setObjectName("headerTextLayout");
        lblTitle = new QLabel(cardHeader);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setTextFormat(Qt::RichText);

        headerTextLayout->addWidget(lblTitle);

        lblSub = new QLabel(cardHeader);
        lblSub->setObjectName("lblSub");

        headerTextLayout->addWidget(lblSub);


        headerLayout->addLayout(headerTextLayout);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);


        leftLayout->addWidget(cardHeader);

        kpiRow = new QWidget(leftCol);
        kpiRow->setObjectName("kpiRow");
        kpiLayout = new QHBoxLayout(kpiRow);
        kpiLayout->setSpacing(16);
        kpiLayout->setObjectName("kpiLayout");
        kpiLayout->setContentsMargins(0, 0, 0, 0);
        cardKpi1 = new QFrame(kpiRow);
        cardKpi1->setObjectName("cardKpi1");
        cardKpi1->setProperty("card", QVariant(true));
        kpiCardLayout1 = new QHBoxLayout(cardKpi1);
        kpiCardLayout1->setObjectName("kpiCardLayout1");
        kpiCardLayout1->setContentsMargins(14, 14, 14, 14);
        kpiIcon1 = new QLabel(cardKpi1);
        kpiIcon1->setObjectName("kpiIcon1");
        kpiIcon1->setMinimumSize(QSize(46, 46));
        kpiIcon1->setMaximumSize(QSize(46, 46));
        kpiIcon1->setProperty("role", QVariant(QString::fromUtf8("kpiIcon")));
        kpiIcon1->setAlignment(Qt::AlignCenter);

        kpiCardLayout1->addWidget(kpiIcon1);

        kpiTextLayout1 = new QVBoxLayout();
        kpiTextLayout1->setSpacing(2);
        kpiTextLayout1->setObjectName("kpiTextLayout1");
        kpiTitle1 = new QLabel(cardKpi1);
        kpiTitle1->setObjectName("kpiTitle1");
        kpiTitle1->setProperty("role", QVariant(QString::fromUtf8("kpiTitle")));

        kpiTextLayout1->addWidget(kpiTitle1);

        kpiTotal = new QLabel(cardKpi1);
        kpiTotal->setObjectName("kpiTotal");
        kpiTotal->setProperty("role", QVariant(QString::fromUtf8("kpiValue")));

        kpiTextLayout1->addWidget(kpiTotal);


        kpiCardLayout1->addLayout(kpiTextLayout1);

        kpiSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        kpiCardLayout1->addItem(kpiSpacer1);


        kpiLayout->addWidget(cardKpi1);

        cardKpi2 = new QFrame(kpiRow);
        cardKpi2->setObjectName("cardKpi2");
        cardKpi2->setProperty("card", QVariant(true));
        kpiCardLayout2 = new QHBoxLayout(cardKpi2);
        kpiCardLayout2->setObjectName("kpiCardLayout2");
        kpiCardLayout2->setContentsMargins(14, 14, 14, 14);
        kpiIcon2 = new QLabel(cardKpi2);
        kpiIcon2->setObjectName("kpiIcon2");
        kpiIcon2->setMinimumSize(QSize(46, 46));
        kpiIcon2->setMaximumSize(QSize(46, 46));
        kpiIcon2->setProperty("role", QVariant(QString::fromUtf8("kpiIcon")));
        kpiIcon2->setAlignment(Qt::AlignCenter);

        kpiCardLayout2->addWidget(kpiIcon2);

        kpiTextLayout2 = new QVBoxLayout();
        kpiTextLayout2->setSpacing(2);
        kpiTextLayout2->setObjectName("kpiTextLayout2");
        kpiTitle2 = new QLabel(cardKpi2);
        kpiTitle2->setObjectName("kpiTitle2");
        kpiTitle2->setProperty("role", QVariant(QString::fromUtf8("kpiTitle")));

        kpiTextLayout2->addWidget(kpiTitle2);

        kpiEquipes = new QLabel(cardKpi2);
        kpiEquipes->setObjectName("kpiEquipes");
        kpiEquipes->setProperty("role", QVariant(QString::fromUtf8("kpiValue")));

        kpiTextLayout2->addWidget(kpiEquipes);


        kpiCardLayout2->addLayout(kpiTextLayout2);

        kpiSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        kpiCardLayout2->addItem(kpiSpacer2);


        kpiLayout->addWidget(cardKpi2);

        cardKpi3 = new QFrame(kpiRow);
        cardKpi3->setObjectName("cardKpi3");
        cardKpi3->setProperty("card", QVariant(true));
        kpiCardLayout3 = new QHBoxLayout(cardKpi3);
        kpiCardLayout3->setObjectName("kpiCardLayout3");
        kpiCardLayout3->setContentsMargins(14, 14, 14, 14);
        kpiIcon3 = new QLabel(cardKpi3);
        kpiIcon3->setObjectName("kpiIcon3");
        kpiIcon3->setMinimumSize(QSize(46, 46));
        kpiIcon3->setMaximumSize(QSize(46, 46));
        kpiIcon3->setProperty("role", QVariant(QString::fromUtf8("kpiIcon")));
        kpiIcon3->setAlignment(Qt::AlignCenter);

        kpiCardLayout3->addWidget(kpiIcon3);

        kpiTextLayout3 = new QVBoxLayout();
        kpiTextLayout3->setSpacing(2);
        kpiTextLayout3->setObjectName("kpiTextLayout3");
        kpiTitle3 = new QLabel(cardKpi3);
        kpiTitle3->setObjectName("kpiTitle3");
        kpiTitle3->setProperty("role", QVariant(QString::fromUtf8("kpiTitle")));

        kpiTextLayout3->addWidget(kpiTitle3);

        kpiPart = new QLabel(cardKpi3);
        kpiPart->setObjectName("kpiPart");
        kpiPart->setProperty("role", QVariant(QString::fromUtf8("kpiValue")));

        kpiTextLayout3->addWidget(kpiPart);


        kpiCardLayout3->addLayout(kpiTextLayout3);

        kpiSpacer3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        kpiCardLayout3->addItem(kpiSpacer3);


        kpiLayout->addWidget(cardKpi3);

        cardKpi4 = new QFrame(kpiRow);
        cardKpi4->setObjectName("cardKpi4");
        cardKpi4->setProperty("card", QVariant(true));
        kpiCardLayout4 = new QHBoxLayout(cardKpi4);
        kpiCardLayout4->setObjectName("kpiCardLayout4");
        kpiCardLayout4->setContentsMargins(14, 14, 14, 14);
        kpiIcon4 = new QLabel(cardKpi4);
        kpiIcon4->setObjectName("kpiIcon4");
        kpiIcon4->setMinimumSize(QSize(46, 46));
        kpiIcon4->setMaximumSize(QSize(46, 46));
        kpiIcon4->setProperty("role", QVariant(QString::fromUtf8("kpiIcon")));
        kpiIcon4->setAlignment(Qt::AlignCenter);

        kpiCardLayout4->addWidget(kpiIcon4);

        kpiTextLayout4 = new QVBoxLayout();
        kpiTextLayout4->setSpacing(2);
        kpiTextLayout4->setObjectName("kpiTextLayout4");
        kpiTitle4 = new QLabel(cardKpi4);
        kpiTitle4->setObjectName("kpiTitle4");
        kpiTitle4->setProperty("role", QVariant(QString::fromUtf8("kpiTitle")));

        kpiTextLayout4->addWidget(kpiTitle4);

        kpiCours = new QLabel(cardKpi4);
        kpiCours->setObjectName("kpiCours");
        kpiCours->setProperty("role", QVariant(QString::fromUtf8("kpiValue")));

        kpiTextLayout4->addWidget(kpiCours);


        kpiCardLayout4->addLayout(kpiTextLayout4);

        kpiSpacer4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        kpiCardLayout4->addItem(kpiSpacer4);


        kpiLayout->addWidget(cardKpi4);

        kpiLayout->setStretch(0, 1);
        kpiLayout->setStretch(1, 1);
        kpiLayout->setStretch(2, 1);
        kpiLayout->setStretch(3, 1);

        leftLayout->addWidget(kpiRow);

        cardList = new QFrame(leftCol);
        cardList->setObjectName("cardList");
        cardList->setProperty("card", QVariant(true));
        listLayout = new QVBoxLayout(cardList);
        listLayout->setSpacing(12);
        listLayout->setObjectName("listLayout");
        listLayout->setContentsMargins(16, 16, 16, 12);
        lblList = new QLabel(cardList);
        lblList->setObjectName("lblList");
        lblList->setProperty("role", QVariant(QString::fromUtf8("cardTitle")));

        listLayout->addWidget(lblList);

        filterLayout = new QHBoxLayout();
        filterLayout->setSpacing(12);
        filterLayout->setObjectName("filterLayout");
        search = new QLineEdit(cardList);
        search->setObjectName("search");
        search->setMinimumSize(QSize(300, 0));

        filterLayout->addWidget(search);

        fStatutLayout = new QVBoxLayout();
        fStatutLayout->setSpacing(2);
        fStatutLayout->setObjectName("fStatutLayout");
        fStatutLabel = new QLabel(cardList);
        fStatutLabel->setObjectName("fStatutLabel");
        fStatutLabel->setProperty("role", QVariant(QString::fromUtf8("filterLabel")));

        fStatutLayout->addWidget(fStatutLabel);

        fStatut = new QComboBox(cardList);
        fStatut->setObjectName("fStatut");

        fStatutLayout->addWidget(fStatut);


        filterLayout->addLayout(fStatutLayout);

        fThemeLayout = new QVBoxLayout();
        fThemeLayout->setSpacing(2);
        fThemeLayout->setObjectName("fThemeLayout");
        fThemeLabel = new QLabel(cardList);
        fThemeLabel->setObjectName("fThemeLabel");
        fThemeLabel->setProperty("role", QVariant(QString::fromUtf8("filterLabel")));

        fThemeLayout->addWidget(fThemeLabel);

        fTheme = new QComboBox(cardList);
        fTheme->setObjectName("fTheme");

        fThemeLayout->addWidget(fTheme);


        filterLayout->addLayout(fThemeLayout);

        fDateLayout = new QVBoxLayout();
        fDateLayout->setSpacing(2);
        fDateLayout->setObjectName("fDateLayout");
        fDateLabel = new QLabel(cardList);
        fDateLabel->setObjectName("fDateLabel");
        fDateLabel->setProperty("role", QVariant(QString::fromUtf8("filterLabel")));

        fDateLayout->addWidget(fDateLabel);

        fDate = new QComboBox(cardList);
        fDate->setObjectName("fDate");

        fDateLayout->addWidget(fDate);


        filterLayout->addLayout(fDateLayout);

        filterLayout->setStretch(0, 2);
        filterLayout->setStretch(1, 1);
        filterLayout->setStretch(2, 1);
        filterLayout->setStretch(3, 1);

        listLayout->addLayout(filterLayout);

        table = new QTableWidget(cardList);
        if (table->columnCount() < 9)
            table->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        table->setObjectName("table");
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setShowGrid(false);
        table->verticalHeader()->setVisible(false);
        table->verticalHeader()->setDefaultSectionSize(50);

        listLayout->addWidget(table);

        listBottomLayout = new QHBoxLayout();
        listBottomLayout->setObjectName("listBottomLayout");
        footer = new QLabel(cardList);
        footer->setObjectName("footer");

        listBottomLayout->addWidget(footer);

        listBottomSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        listBottomLayout->addItem(listBottomSpacer);

        pagerBox = new QWidget(cardList);
        pagerBox->setObjectName("pagerBox");
        pagerLayout = new QHBoxLayout(pagerBox);
        pagerLayout->setSpacing(6);
        pagerLayout->setObjectName("pagerLayout");
        pagerLayout->setContentsMargins(0, 0, 0, 0);

        listBottomLayout->addWidget(pagerBox);


        listLayout->addLayout(listBottomLayout);

        listLayout->setStretch(2, 1);

        leftLayout->addWidget(cardList);

        leftLayout->setStretch(2, 1);

        bodyLayout->addWidget(leftCol);

        rightScroll = new QScrollArea(body);
        rightScroll->setObjectName("rightScroll");
        rightScroll->setMinimumSize(QSize(342, 0));
        rightScroll->setMaximumSize(QSize(342, 16777215));
        rightScroll->setFrameShape(QFrame::NoFrame);
        rightScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rightScroll->setWidgetResizable(true);
        rightCol = new QWidget();
        rightCol->setObjectName("rightCol");
        rightCol->setGeometry(QRect(0, 0, 330, 1260));
        rightCol->setMinimumSize(QSize(330, 0));
        rightCol->setMaximumSize(QSize(330, 16777215));
        rightLayout = new QVBoxLayout(rightCol);
        rightLayout->setSpacing(16);
        rightLayout->setObjectName("rightLayout");
        rightLayout->setContentsMargins(0, 0, 0, 0);
        cardDetails = new QFrame(rightCol);
        cardDetails->setObjectName("cardDetails");
        cardDetails->setProperty("card", QVariant(true));
        detailsLayout = new QVBoxLayout(cardDetails);
        detailsLayout->setSpacing(10);
        detailsLayout->setObjectName("detailsLayout");
        detailsLayout->setContentsMargins(16, 16, 16, 16);
        lblDetails = new QLabel(cardDetails);
        lblDetails->setObjectName("lblDetails");
        lblDetails->setProperty("role", QVariant(QString::fromUtf8("cardTitle")));

        detailsLayout->addWidget(lblDetails);

        detailHead = new QFrame(cardDetails);
        detailHead->setObjectName("detailHead");
        detailHeadLayout = new QHBoxLayout(detailHead);
        detailHeadLayout->setObjectName("detailHeadLayout");
        dIco = new QLabel(detailHead);
        dIco->setObjectName("dIco");

        detailHeadLayout->addWidget(dIco);

        detailNameLayout = new QVBoxLayout();
        detailNameLayout->setObjectName("detailNameLayout");
        dNom = new QLabel(detailHead);
        dNom->setObjectName("dNom");

        detailNameLayout->addWidget(dNom);

        dId = new QLabel(detailHead);
        dId->setObjectName("dId");

        detailNameLayout->addWidget(dId);


        detailHeadLayout->addLayout(detailNameLayout);

        detailHeadSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        detailHeadLayout->addItem(detailHeadSpacer);

        detailBadgeLayout = new QVBoxLayout();
        detailBadgeLayout->setObjectName("detailBadgeLayout");
        dBadge = new QLabel(detailHead);
        dBadge->setObjectName("dBadge");

        detailBadgeLayout->addWidget(dBadge);

        detailBadgeSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        detailBadgeLayout->addItem(detailBadgeSpacer);


        detailHeadLayout->addLayout(detailBadgeLayout);


        detailsLayout->addWidget(detailHead);

        detailsGrid = new QGridLayout();
        detailsGrid->setObjectName("detailsGrid");
        detailsGrid->setVerticalSpacing(12);
        dThemeLabel = new QLabel(cardDetails);
        dThemeLabel->setObjectName("dThemeLabel");

        detailsGrid->addWidget(dThemeLabel, 0, 0, 1, 1);

        dTheme = new QLabel(cardDetails);
        dTheme->setObjectName("dTheme");

        detailsGrid->addWidget(dTheme, 0, 1, 1, 1);

        dDebutLabel = new QLabel(cardDetails);
        dDebutLabel->setObjectName("dDebutLabel");

        detailsGrid->addWidget(dDebutLabel, 1, 0, 1, 1);

        dDebut = new QLabel(cardDetails);
        dDebut->setObjectName("dDebut");

        detailsGrid->addWidget(dDebut, 1, 1, 1, 1);

        dFinLabel = new QLabel(cardDetails);
        dFinLabel->setObjectName("dFinLabel");

        detailsGrid->addWidget(dFinLabel, 2, 0, 1, 1);

        dFin = new QLabel(cardDetails);
        dFin->setObjectName("dFin");

        detailsGrid->addWidget(dFin, 2, 1, 1, 1);

        dModeLabel = new QLabel(cardDetails);
        dModeLabel->setObjectName("dModeLabel");

        detailsGrid->addWidget(dModeLabel, 3, 0, 1, 1);

        dMode = new QLabel(cardDetails);
        dMode->setObjectName("dMode");

        detailsGrid->addWidget(dMode, 3, 1, 1, 1);

        dMaxLabel = new QLabel(cardDetails);
        dMaxLabel->setObjectName("dMaxLabel");

        detailsGrid->addWidget(dMaxLabel, 4, 0, 1, 1);

        dMax = new QLabel(cardDetails);
        dMax->setObjectName("dMax");

        detailsGrid->addWidget(dMax, 4, 1, 1, 1);


        detailsLayout->addLayout(detailsGrid);

        btnDetails = new QPushButton(cardDetails);
        btnDetails->setObjectName("btnDetails");
        btnDetails->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnDetails->setProperty("kind", QVariant(QString::fromUtf8("primary")));

        detailsLayout->addWidget(btnDetails);


        rightLayout->addWidget(cardDetails);

        cardAdd = new QFrame(rightCol);
        cardAdd->setObjectName("cardAdd");
        cardAdd->setProperty("card", QVariant(true));
        addLayout = new QVBoxLayout(cardAdd);
        addLayout->setSpacing(12);
        addLayout->setObjectName("addLayout");
        addLayout->setContentsMargins(16, 16, 16, 16);
        lblAdd = new QLabel(cardAdd);
        lblAdd->setObjectName("lblAdd");
        lblAdd->setProperty("role", QVariant(QString::fromUtf8("cardTitle")));

        addLayout->addWidget(lblAdd);

        addFormLayout = new QFormLayout();
        addFormLayout->setObjectName("addFormLayout");
        addFormLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        addFormLayout->setHorizontalSpacing(10);
        addFormLayout->setVerticalSpacing(8);
        aIdLabel = new QLabel(cardAdd);
        aIdLabel->setObjectName("aIdLabel");

        addFormLayout->setWidget(0, QFormLayout::LabelRole, aIdLabel);

        aId = new QLabel(cardAdd);
        aId->setObjectName("aId");

        addFormLayout->setWidget(0, QFormLayout::FieldRole, aId);

        aNomLabel = new QLabel(cardAdd);
        aNomLabel->setObjectName("aNomLabel");

        addFormLayout->setWidget(1, QFormLayout::LabelRole, aNomLabel);

        aNom = new QLineEdit(cardAdd);
        aNom->setObjectName("aNom");

        addFormLayout->setWidget(1, QFormLayout::FieldRole, aNom);

        aThemeLabel = new QLabel(cardAdd);
        aThemeLabel->setObjectName("aThemeLabel");

        addFormLayout->setWidget(2, QFormLayout::LabelRole, aThemeLabel);

        aTheme = new QComboBox(cardAdd);
        aTheme->setObjectName("aTheme");
        aTheme->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        aTheme->setMinimumContentsLength(8);

        addFormLayout->setWidget(2, QFormLayout::FieldRole, aTheme);

        aDebutLabel = new QLabel(cardAdd);
        aDebutLabel->setObjectName("aDebutLabel");

        addFormLayout->setWidget(3, QFormLayout::LabelRole, aDebutLabel);

        aDebut = new QDateEdit(cardAdd);
        aDebut->setObjectName("aDebut");
        aDebut->setDisplayFormat(QString::fromUtf8("dd/MM/yyyy"));
        aDebut->setCalendarPopup(true);

        addFormLayout->setWidget(3, QFormLayout::FieldRole, aDebut);

        aFinLabel = new QLabel(cardAdd);
        aFinLabel->setObjectName("aFinLabel");

        addFormLayout->setWidget(4, QFormLayout::LabelRole, aFinLabel);

        aFin = new QDateEdit(cardAdd);
        aFin->setObjectName("aFin");
        aFin->setDisplayFormat(QString::fromUtf8("dd/MM/yyyy"));
        aFin->setCalendarPopup(true);

        addFormLayout->setWidget(4, QFormLayout::FieldRole, aFin);

        aModeLabel = new QLabel(cardAdd);
        aModeLabel->setObjectName("aModeLabel");

        addFormLayout->setWidget(5, QFormLayout::LabelRole, aModeLabel);

        aMode = new QComboBox(cardAdd);
        aMode->setObjectName("aMode");
        aMode->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        aMode->setMinimumContentsLength(8);

        addFormLayout->setWidget(5, QFormLayout::FieldRole, aMode);

        aMaxLabel = new QLabel(cardAdd);
        aMaxLabel->setObjectName("aMaxLabel");

        addFormLayout->setWidget(6, QFormLayout::LabelRole, aMaxLabel);

        aMax = new QSpinBox(cardAdd);
        aMax->setObjectName("aMax");
        aMax->setMinimum(1);
        aMax->setMaximum(999);
        aMax->setValue(5);

        addFormLayout->setWidget(6, QFormLayout::FieldRole, aMax);

        aStatutLabel = new QLabel(cardAdd);
        aStatutLabel->setObjectName("aStatutLabel");

        addFormLayout->setWidget(7, QFormLayout::LabelRole, aStatutLabel);

        aStatut = new QComboBox(cardAdd);
        aStatut->setObjectName("aStatut");
        aStatut->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        aStatut->setMinimumContentsLength(8);

        addFormLayout->setWidget(7, QFormLayout::FieldRole, aStatut);


        addLayout->addLayout(addFormLayout);

        aErr = new QLabel(cardAdd);
        aErr->setObjectName("aErr");
        aErr->setWordWrap(true);

        addLayout->addWidget(aErr);

        addButtonsLayout = new QHBoxLayout();
        addButtonsLayout->setSpacing(10);
        addButtonsLayout->setObjectName("addButtonsLayout");
        btnSave = new QPushButton(cardAdd);
        btnSave->setObjectName("btnSave");
        btnSave->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnSave->setProperty("kind", QVariant(QString::fromUtf8("primary")));

        addButtonsLayout->addWidget(btnSave);

        btnClear = new QPushButton(cardAdd);
        btnClear->setObjectName("btnClear");
        btnClear->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnClear->setProperty("kind", QVariant(QString::fromUtf8("outline")));

        addButtonsLayout->addWidget(btnClear);

        addButtonsLayout->setStretch(0, 1);
        addButtonsLayout->setStretch(1, 1);

        addLayout->addLayout(addButtonsLayout);


        rightLayout->addWidget(cardAdd);

        cardStatut = new QFrame(rightCol);
        cardStatut->setObjectName("cardStatut");
        cardStatut->setProperty("card", QVariant(true));
        statutLayout = new QVBoxLayout(cardStatut);
        statutLayout->setObjectName("statutLayout");
        statutLayout->setContentsMargins(16, 16, 16, 16);
        lblStatut = new QLabel(cardStatut);
        lblStatut->setObjectName("lblStatut");
        lblStatut->setProperty("role", QVariant(QString::fromUtf8("cardTitle")));

        statutLayout->addWidget(lblStatut);

        statutRowLayout = new QHBoxLayout();
        statutRowLayout->setObjectName("statutRowLayout");
        donut = new Donut(cardStatut);
        donut->setObjectName("donut");
        donut->setMinimumSize(QSize(140, 140));
        donut->setMaximumSize(QSize(140, 140));

        statutRowLayout->addWidget(donut);

        legendLayout = new QVBoxLayout();
        legendLayout->setObjectName("legendLayout");
        legendCoursLayout = new QHBoxLayout();
        legendCoursLayout->setObjectName("legendCoursLayout");
        dotCours = new QLabel(cardStatut);
        dotCours->setObjectName("dotCours");

        legendCoursLayout->addWidget(dotCours);

        legNameCours = new QLabel(cardStatut);
        legNameCours->setObjectName("legNameCours");

        legendCoursLayout->addWidget(legNameCours);

        legendCoursSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        legendCoursLayout->addItem(legendCoursSpacer);

        legCours = new QLabel(cardStatut);
        legCours->setObjectName("legCours");

        legendCoursLayout->addWidget(legCours);


        legendLayout->addLayout(legendCoursLayout);

        legendPlanifieeLayout = new QHBoxLayout();
        legendPlanifieeLayout->setObjectName("legendPlanifieeLayout");
        dotPlanifiee = new QLabel(cardStatut);
        dotPlanifiee->setObjectName("dotPlanifiee");

        legendPlanifieeLayout->addWidget(dotPlanifiee);

        legNamePlanifiee = new QLabel(cardStatut);
        legNamePlanifiee->setObjectName("legNamePlanifiee");

        legendPlanifieeLayout->addWidget(legNamePlanifiee);

        legendPlanifieeSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        legendPlanifieeLayout->addItem(legendPlanifieeSpacer);

        legPlanifiee = new QLabel(cardStatut);
        legPlanifiee->setObjectName("legPlanifiee");

        legendPlanifieeLayout->addWidget(legPlanifiee);


        legendLayout->addLayout(legendPlanifieeLayout);

        legendTermineeLayout = new QHBoxLayout();
        legendTermineeLayout->setObjectName("legendTermineeLayout");
        dotTerminee = new QLabel(cardStatut);
        dotTerminee->setObjectName("dotTerminee");

        legendTermineeLayout->addWidget(dotTerminee);

        legNameTerminee = new QLabel(cardStatut);
        legNameTerminee->setObjectName("legNameTerminee");

        legendTermineeLayout->addWidget(legNameTerminee);

        legendTermineeSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        legendTermineeLayout->addItem(legendTermineeSpacer);

        legTerminee = new QLabel(cardStatut);
        legTerminee->setObjectName("legTerminee");

        legendTermineeLayout->addWidget(legTerminee);


        legendLayout->addLayout(legendTermineeLayout);


        statutRowLayout->addLayout(legendLayout);


        statutLayout->addLayout(statutRowLayout);


        rightLayout->addWidget(cardStatut);

        cardActions = new QFrame(rightCol);
        cardActions->setObjectName("cardActions");
        cardActions->setProperty("card", QVariant(true));
        actionsLayout = new QVBoxLayout(cardActions);
        actionsLayout->setSpacing(10);
        actionsLayout->setObjectName("actionsLayout");
        actionsLayout->setContentsMargins(16, 16, 16, 16);
        lblActions = new QLabel(cardActions);
        lblActions->setObjectName("lblActions");
        lblActions->setProperty("role", QVariant(QString::fromUtf8("cardTitle")));

        actionsLayout->addWidget(lblActions);

        btnTeams = new QPushButton(cardActions);
        btnTeams->setObjectName("btnTeams");
        btnTeams->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnTeams->setProperty("kind", QVariant(QString::fromUtf8("primary")));

        actionsLayout->addWidget(btnTeams);

        btnViewJury = new QPushButton(cardActions);
        btnViewJury->setObjectName("btnViewJury");
        btnViewJury->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnViewJury->setProperty("kind", QVariant(QString::fromUtf8("outline")));

        actionsLayout->addWidget(btnViewJury);

        btnReports = new QPushButton(cardActions);
        btnReports->setObjectName("btnReports");
        btnReports->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnReports->setProperty("kind", QVariant(QString::fromUtf8("outline")));

        actionsLayout->addWidget(btnReports);


        rightLayout->addWidget(cardActions);

        rightSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rightLayout->addItem(rightSpacer);

        rightScroll->setWidget(rightCol);

        bodyLayout->addWidget(rightScroll);

        bodyLayout->setStretch(0, 1);

        contentLayout->addWidget(body);

        contentLayout->setStretch(1, 1);

        rootLayout->addWidget(content);

        rootLayout->setStretch(1, 1);
        SWCompet->addWidget(CRUDCompet);

        pCompLayout->addWidget(SWCompet);

        SWHackTime->addWidget(PComp);

        centralLayout->addWidget(SWHackTime);

        hackathon->setCentralWidget(centralwidget);

        retranslateUi(hackathon);

        SWHackTime->setCurrentIndex(0);
        SWCompet->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(hackathon);
    } // setupUi

    void retranslateUi(QMainWindow *hackathon)
    {
        hackathon->setWindowTitle(QCoreApplication::translate("hackathon", "HackTime", nullptr));
        logoLoginCompet->setText(QString());
        lblLoginCompet->setText(QCoreApplication::translate("hackathon", "Nom d'utilisateur", nullptr));
        lblPWDCompet->setText(QCoreApplication::translate("hackathon", "Mot de passe", nullptr));
        BForgotCompet->setText(QCoreApplication::translate("hackathon", "Mot de passe oubli\303\251 ?", nullptr));
        BLoginCompet->setText(QCoreApplication::translate("hackathon", "Entrer", nullptr));
        logo->setText(QString());
        btnDashboard->setText(QCoreApplication::translate("hackathon", "\360\237\217\240   Tableau de bord", nullptr));
        btnCompetitions->setText(QCoreApplication::translate("hackathon", "\360\237\217\206   Comp\303\251titions", nullptr));
        btnEquipes->setText(QCoreApplication::translate("hackathon", "\360\237\221\245   \303\211quipes", nullptr));
        btnParticipants->setText(QCoreApplication::translate("hackathon", "\360\237\221\244   Participants", nullptr));
        btnChallenges->setText(QCoreApplication::translate("hackathon", "\360\237\232\251   Challenges", nullptr));
        btnJury->setText(QCoreApplication::translate("hackathon", "\360\237\247\221\342\200\215\342\232\226\357\270\217   Jury", nullptr));
        btnRapports->setText(QCoreApplication::translate("hackathon", "\360\237\223\212   Rapports", nullptr));
        btnParametres->setText(QCoreApplication::translate("hackathon", "\342\232\231\357\270\217   Param\303\250tres", nullptr));
        tagline->setText(QCoreApplication::translate("hackathon", "Des id\303\251es aujourd'hui,\n"
"les solutions de demain", nullptr));
        topSearch->setPlaceholderText(QCoreApplication::translate("hackathon", "\360\237\224\215  Rechercher une comp\303\251tition, une \303\251quipe, un participant\342\200\246", nullptr));
        bell->setText(QCoreApplication::translate("hackathon", "\360\237\224\224", nullptr));
        admin->setText(QCoreApplication::translate("hackathon", "\360\237\221\244  <b>Admin</b>  \342\226\276", nullptr));
        headerIcon->setText(QCoreApplication::translate("hackathon", "\360\237\217\206", nullptr));
        lblTitle->setText(QCoreApplication::translate("hackathon", "Gestion des <span style=\"color:#1665E8\">Comp\303\251titions</span>", nullptr));
        lblSub->setText(QCoreApplication::translate("hackathon", "Cr\303\251ez, g\303\251rez et suivez toutes les comp\303\251titions de votre hackathon.", nullptr));
        kpiIcon1->setText(QCoreApplication::translate("hackathon", "\360\237\217\206", nullptr));
        kpiTitle1->setText(QCoreApplication::translate("hackathon", "Total des comp\303\251titions", nullptr));
        kpiTotal->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        kpiIcon2->setText(QCoreApplication::translate("hackathon", "\360\237\221\245", nullptr));
        kpiTitle2->setText(QCoreApplication::translate("hackathon", "\303\211quipes inscrites", nullptr));
        kpiEquipes->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        kpiIcon3->setText(QCoreApplication::translate("hackathon", "\360\237\221\244", nullptr));
        kpiTitle3->setText(QCoreApplication::translate("hackathon", "Participants", nullptr));
        kpiPart->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        kpiIcon4->setText(QCoreApplication::translate("hackathon", "\360\237\232\251", nullptr));
        kpiTitle4->setText(QCoreApplication::translate("hackathon", "Comp\303\251titions en cours", nullptr));
        kpiCours->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        lblList->setText(QCoreApplication::translate("hackathon", "\360\237\223\213  Liste des comp\303\251titions", nullptr));
        search->setPlaceholderText(QCoreApplication::translate("hackathon", "\360\237\224\215  Rechercher une comp\303\251tition\342\200\246", nullptr));
        fStatutLabel->setText(QCoreApplication::translate("hackathon", "Statut", nullptr));
        fThemeLabel->setText(QCoreApplication::translate("hackathon", "Th\303\250me", nullptr));
        fDateLabel->setText(QCoreApplication::translate("hackathon", "Date (ann\303\251e)", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("hackathon", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("hackathon", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("hackathon", "Th\303\250me", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("hackathon", "Date d\303\251but", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("hackathon", "Date fin", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("hackathon", "Mode", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("hackathon", "\303\211quipes max", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("hackathon", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = table->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("hackathon", "Actions", nullptr));
        footer->setText(QCoreApplication::translate("hackathon", "Affichage de 0 \303\240 0 sur 0 comp\303\251titions", nullptr));
        lblDetails->setText(QCoreApplication::translate("hackathon", "\360\237\217\206  D\303\251tails de la comp\303\251tition", nullptr));
        dIco->setText(QCoreApplication::translate("hackathon", "\360\237\215\203", nullptr));
        dNom->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dId->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dBadge->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dThemeLabel->setText(QCoreApplication::translate("hackathon", "\360\237\217\206  Th\303\250me", nullptr));
        dTheme->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dDebutLabel->setText(QCoreApplication::translate("hackathon", "\360\237\223\205  Date d\303\251but", nullptr));
        dDebut->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dFinLabel->setText(QCoreApplication::translate("hackathon", "\360\237\223\205  Date fin", nullptr));
        dFin->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dModeLabel->setText(QCoreApplication::translate("hackathon", "\360\237\221\245  Mode", nullptr));
        dMode->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        dMaxLabel->setText(QCoreApplication::translate("hackathon", "\360\237\221\245  \303\211quipes max", nullptr));
        dMax->setText(QCoreApplication::translate("hackathon", "-", nullptr));
        btnDetails->setText(QCoreApplication::translate("hackathon", "Voir les d\303\251tails  \342\206\222", nullptr));
        lblAdd->setText(QCoreApplication::translate("hackathon", "\357\274\213  Ajouter une comp\303\251tition", nullptr));
        aIdLabel->setText(QCoreApplication::translate("hackathon", "ID", nullptr));
        aId->setText(QCoreApplication::translate("hackathon", "C-001", nullptr));
        aNomLabel->setText(QCoreApplication::translate("hackathon", "Nom *", nullptr));
        aNom->setPlaceholderText(QCoreApplication::translate("hackathon", "Nom de la comp\303\251tition", nullptr));
        aThemeLabel->setText(QCoreApplication::translate("hackathon", "Th\303\250me", nullptr));
        aDebutLabel->setText(QCoreApplication::translate("hackathon", "Date d\303\251but", nullptr));
        aFinLabel->setText(QCoreApplication::translate("hackathon", "Date fin", nullptr));
        aModeLabel->setText(QCoreApplication::translate("hackathon", "Mode", nullptr));
        aMaxLabel->setText(QCoreApplication::translate("hackathon", "\303\211quipes max", nullptr));
        aStatutLabel->setText(QCoreApplication::translate("hackathon", "Statut", nullptr));
        aErr->setText(QString());
        btnSave->setText(QCoreApplication::translate("hackathon", "Enregistrer", nullptr));
        btnClear->setText(QCoreApplication::translate("hackathon", "Effacer", nullptr));
        lblStatut->setText(QCoreApplication::translate("hackathon", "\360\237\221\245  Statut de la comp\303\251tition", nullptr));
        dotCours->setText(QCoreApplication::translate("hackathon", "\342\227\217", nullptr));
        legNameCours->setText(QCoreApplication::translate("hackathon", "En cours", nullptr));
        legCours->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        dotPlanifiee->setText(QCoreApplication::translate("hackathon", "\342\227\217", nullptr));
        legNamePlanifiee->setText(QCoreApplication::translate("hackathon", "Planifi\303\251e", nullptr));
        legPlanifiee->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        dotTerminee->setText(QCoreApplication::translate("hackathon", "\342\227\217", nullptr));
        legNameTerminee->setText(QCoreApplication::translate("hackathon", "Termin\303\251e", nullptr));
        legTerminee->setText(QCoreApplication::translate("hackathon", "0", nullptr));
        lblActions->setText(QCoreApplication::translate("hackathon", "\342\232\241  Actions rapides", nullptr));
        btnTeams->setText(QCoreApplication::translate("hackathon", "\360\237\221\245  G\303\251rer les \303\251quipes", nullptr));
        btnViewJury->setText(QCoreApplication::translate("hackathon", "\360\237\221\244  Voir le jury", nullptr));
        btnReports->setText(QCoreApplication::translate("hackathon", "\360\237\223\212  Voir les rapports", nullptr));
    } // retranslateUi

};

namespace Ui {
    class hackathon: public Ui_hackathon {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HACKATHON_H
