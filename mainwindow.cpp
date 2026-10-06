#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // إعدادات الجدول
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels({"Nom de l'équipe", "Compétition", "Chef d'équipe", "Membres", "Date d'inscription", "Statut"});
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEnabled(true);

    // تحميل الفرق وعرض الصفحة الأولى
    initialiserToutesLesEquipes();
    chargerPage(1);

    // ربط ضغطة السطر في الجدول لعرض التفاصيل
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTeamSelected);

    // ربط أزرار التنقل بين الصفحات
    connect(ui->btn1, &QPushButton::clicked, this, [=](){ chargerPage(1); });
    connect(ui->btn2, &QPushButton::clicked, this, [=](){ chargerPage(2); });
    connect(ui->btn3, &QPushButton::clicked, this, [=](){ chargerPage(3); });
    connect(ui->btn4, &QPushButton::clicked, this, [=](){ chargerPage(4); });

    connect(ui->btnPrev, &QPushButton::clicked, this, [=](){ chargerPage(qMax(1, currentPage - 1)); });
    connect(ui->btnNext, &QPushButton::clicked, this, [=](){ chargerPage(qMin(4, currentPage + 1)); });

    // ربط بارة البحث
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::filtrerEquipes);

    // ربط القوائم المنسدلة للفلترة والترتيب
    connect(ui->comboBoxCompetition, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filtrerParComboBox);
    connect(ui->comboBoxStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filtrerParComboBox);
    connect(ui->comboBoxTrier, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filtrerParComboBox);

    // ربط زر إضافة فريق جديد
    connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::on_btnAjouter_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// دالة تحميل الصفحة وعرض الفرق
void MainWindow::chargerPage(int page) {
    int totalPages = 4;
    if (page < 1 || page > totalPages) return;
    currentPage = page;

    int itemsPerPage = 8;
    int startIndex = (currentPage - 1) * itemsPerPage;
    int endIndex = qMin(startIndex + itemsPerPage, toutesLesEquipes.size());
    int currentCount = endIndex - startIndex;

    ui->tableWidget->setRowCount(currentCount);
    equipesAffichees.clear();

    for (int i = 0; i < currentCount; ++i) {
        Equipe eq = toutesLesEquipes[startIndex + i];
        equipesAffichees.append(eq);

        QLabel *lblNom = new QLabel(eq.nom);
        lblNom->setStyleSheet("color: #1E293B; font-weight: bold; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 0, lblNom);

        QLabel *lblComp = new QLabel(eq.competition);
        lblComp->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 1, lblComp);

        QLabel *lblChef = new QLabel(eq.chef);
        lblChef->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 2, lblChef);

        QWidget *membresWidget = new QWidget();
        QHBoxLayout *membresLayout = new QHBoxLayout(membresWidget);
        QLabel *lblMembres = new QLabel(eq.membres);
        lblMembres->setStyleSheet("color: #475569; font-weight: bold; min-width: 35px;");

        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, 6);
        progressBar->setValue(eq.membres.section('/', 0, 0).toInt());
        progressBar->setTextVisible(false);
        progressBar->setStyleSheet("QProgressBar { border: none; background: #F1F5F9; border-radius: 3px; max-height: 8px; } QProgressBar::chunk { background-color: #2563EB; border-radius: 3px; }");

        membresLayout->addWidget(lblMembres);
        membresLayout->addWidget(progressBar);
        membresLayout->setContentsMargins(2, 0, 5, 0);
        ui->tableWidget->setCellWidget(i, 3, membresWidget);

        QLabel *lblDate = new QLabel(eq.date);
        lblDate->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 4, lblDate);

        QLabel *lblStatut = new QLabel(eq.statut);
        if (eq.statut == "Active") {
            lblStatut->setStyleSheet("color: #10B981; font-weight: bold; padding-left: 5px;");
        } else {
            lblStatut->setStyleSheet("color: #F59E0B; font-weight: bold; padding-left: 5px;");
        }
        ui->tableWidget->setCellWidget(i, 5, lblStatut);
    }
}

// دالة عرض تفاصيل الفريق عند النقر
void MainWindow::onTeamSelected(int row, int column) {
    Q_UNUSED(column);

    if (row < 0 || row >= equipesAffichees.size()) return;

    Equipe eq = equipesAffichees[row];

    QString details = QString("Nom de l'équipe : %1\n"
                              "Compétition : %2\n"
                              "Chef d'équipe : %3\n"
                              "Membres : %4\n"
                              "Date d'inscription : %5\n"
                              "Statut : %6")
                          .arg(eq.nom)
                          .arg(eq.competition)
                          .arg(eq.chef)
                          .arg(eq.membres)
                          .arg(eq.date)
                          .arg(eq.statut);

    QMessageBox::information(this, "Détails de l'équipe", details);
}

// دالة فتح نافذة إضافة فريق جديد عند الضغط على الزر
void MainWindow::on_btnAjouter_clicked() {
    QDialog dialog(this);
    dialog.setWindowTitle("Ajouter une équipe");
    dialog.resize(380, 320);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();

    QLineEdit *txtNom = new QLineEdit(&dialog);
    QLineEdit *txtCompetition = new QLineEdit(&dialog);
    QLineEdit *txtChef = new QLineEdit(&dialog);
    QLineEdit *txtMembres = new QLineEdit(&dialog);
    txtMembres->setText("4/4");
    QLineEdit *txtDate = new QLineEdit(&dialog);
    txtDate->setText("05/10/2026");

    QComboBox *comboStatut = new QComboBox(&dialog);
    comboStatut->addItems({"Active", "Incomplète"});

    formLayout->addRow("Nom de l'équipe :", txtNom);
    formLayout->addRow("Compétition :", txtCompetition);
    formLayout->addRow("Chef d'équipe :", txtChef);
    formLayout->addRow("Membres :", txtMembres);
    formLayout->addRow("Date :", txtDate);
    formLayout->addRow("Statut :", comboStatut);

    mainLayout->addLayout(formLayout);

    QPushButton *btnEnregistrer = new QPushButton("Enregistrer", &dialog);
    btnEnregistrer->setStyleSheet("background-color: #2563EB; color: white; font-weight: bold; padding: 8px; border-radius: 4px;");
    mainLayout->addWidget(btnEnregistrer);

    connect(btnEnregistrer, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        QString nom = txtNom->text().trimmed();
        QString competition = txtCompetition->text().trimmed();
        QString chef = txtChef->text().trimmed();
        QString membres = txtMembres->text().trimmed();
        QString date = txtDate->text().trimmed();
        QString statut = comboStatut->currentText();

        if (!nom.isEmpty() && !competition.isEmpty() && !chef.isEmpty()) {
            Equipe nouvelleEq = {nom, competition, chef, membres, date, statut};
            toutesLesEquipes.prepend(nouvelleEq);
            chargerPage(currentPage);
            QMessageBox::information(this, "Succès", "L'équipe a été ajoutée avec succès !");
        } else {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir au moins le nom, la compétition et le chef d'équipe !");
        }
    }
}

// دالة البحث والفلترة عبر الـ QLineEdit
void MainWindow::filtrerEquipes(const QString &text) {
    if (text.isEmpty()) {
        chargerPage(currentPage);
        return;
    }

    QList<Equipe> equipesFiltrees;
    for (int i = 0; i < toutesLesEquipes.size(); ++i) {
        const Equipe &eq = toutesLesEquipes.at(i);
        if (eq.nom.contains(text, Qt::CaseInsensitive) ||
            eq.chef.contains(text, Qt::CaseInsensitive) ||
            eq.competition.contains(text, Qt::CaseInsensitive)) {
            equipesFiltrees.append(eq);
        }
    }

    ui->tableWidget->setRowCount(equipesFiltrees.size());
    equipesAffichees = equipesFiltrees;

    for (int i = 0; i < equipesFiltrees.size(); ++i) {
        Equipe eq = equipesFiltrees[i];

        QLabel *lblNom = new QLabel(eq.nom);
        lblNom->setStyleSheet("color: #1E293B; font-weight: bold; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 0, lblNom);

        QLabel *lblComp = new QLabel(eq.competition);
        lblComp->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 1, lblComp);

        QLabel *lblChef = new QLabel(eq.chef);
        lblChef->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 2, lblChef);

        QWidget *membresWidget = new QWidget();
        QHBoxLayout *membresLayout = new QHBoxLayout(membresWidget);
        QLabel *lblMembres = new QLabel(eq.membres);
        lblMembres->setStyleSheet("color: #475569; font-weight: bold; min-width: 35px;");

        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, 6);
        progressBar->setValue(eq.membres.section('/', 0, 0).toInt());
        progressBar->setTextVisible(false);
        progressBar->setStyleSheet("QProgressBar { border: none; background: #F1F5F9; border-radius: 3px; max-height: 8px; } QProgressBar::chunk { background-color: #2563EB; border-radius: 3px; }");

        membresLayout->addWidget(lblMembres);
        membresLayout->addWidget(progressBar);
        membresLayout->setContentsMargins(2, 0, 5, 0);
        ui->tableWidget->setCellWidget(i, 3, membresWidget);

        QLabel *lblDate = new QLabel(eq.date);
        lblDate->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 4, lblDate);

        QLabel *lblStatut = new QLabel(eq.statut);
        if (eq.statut == "Active") {
            lblStatut->setStyleSheet("color: #10B981; font-weight: bold; padding-left: 5px;");
        } else {
            lblStatut->setStyleSheet("color: #F59E0B; font-weight: bold; padding-left: 5px;");
        }
        ui->tableWidget->setCellWidget(i, 5, lblStatut);
    }
}

// دالة الفلترة والترتيب عبر القوائم المنسدلة (ComboBox)
void MainWindow::filtrerParComboBox() {
    QString compFiltre = ui->comboBoxCompetition->currentText();
    QString statutFiltre = ui->comboBoxStatus->currentText();
    QString triFiltre = ui->comboBoxTrier->currentText();

    QList<Equipe> equipesFiltrees;
    for (int i = 0; i < toutesLesEquipes.size(); ++i) {
        const Equipe &eq = toutesLesEquipes.at(i);
        bool matchComp = (compFiltre == "Toutes" || eq.competition == compFiltre);
        bool matchStatut = (statutFiltre == "Tous" || eq.statut.toLower() == statutFiltre.toLower() || (statutFiltre == "Incomplète" && eq.statut == "Incomplète"));

        if (matchComp && matchStatut) {
            equipesFiltrees.append(eq);
        }
    }

    if (triFiltre == "Nom") {
        std::sort(equipesFiltrees.begin(), equipesFiltrees.end(), [](const Equipe &a, const Equipe &b) {
            return a.nom < b.nom;
        });
    } else if (triFiltre == "Membres") {
        std::sort(equipesFiltrees.begin(), equipesFiltrees.end(), [](const Equipe &a, const Equipe &b) {
            return a.membres.section('/', 0, 0).toInt() > b.membres.section('/', 0, 0).toInt();
        });
    } else if (triFiltre == "Date d'inscription") {
        std::sort(equipesFiltrees.begin(), equipesFiltrees.end(), [](const Equipe &a, const Equipe &b) {
            return a.date < b.date;
        });
    }

    ui->tableWidget->setRowCount(equipesFiltrees.size());
    equipesAffichees = equipesFiltrees;

    for (int i = 0; i < equipesFiltrees.size(); ++i) {
        Equipe eq = equipesFiltrees[i];

        QLabel *lblNom = new QLabel(eq.nom);
        lblNom->setStyleSheet("color: #1E293B; font-weight: bold; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 0, lblNom);

        QLabel *lblComp = new QLabel(eq.competition);
        lblComp->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 1, lblComp);

        QLabel *lblChef = new QLabel(eq.chef);
        lblChef->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 2, lblChef);

        QWidget *membresWidget = new QWidget();
        QHBoxLayout *membresLayout = new QHBoxLayout(membresWidget);
        QLabel *lblMembres = new QLabel(eq.membres);
        lblMembres->setStyleSheet("color: #475569; font-weight: bold; min-width: 35px;");

        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, 6);
        progressBar->setValue(eq.membres.section('/', 0, 0).toInt());
        progressBar->setTextVisible(false);
        progressBar->setStyleSheet("QProgressBar { border: none; background: #F1F5F9; border-radius: 3px; max-height: 8px; } QProgressBar::chunk { background-color: #2563EB; border-radius: 3px; }");

        membresLayout->addWidget(lblMembres);
        membresLayout->addWidget(progressBar);
        membresLayout->setContentsMargins(2, 0, 5, 0);
        ui->tableWidget->setCellWidget(i, 3, membresWidget);

        QLabel *lblDate = new QLabel(eq.date);
        lblDate->setStyleSheet("color: #475569; padding-left: 5px;");
        ui->tableWidget->setCellWidget(i, 4, lblDate);

        QLabel *lblStatut = new QLabel(eq.statut);
        if (eq.statut == "Active") {
            lblStatut->setStyleSheet("color: #10B981; font-weight: bold; padding-left: 5px;");
        } else {
            lblStatut->setStyleSheet("color: #F59E0B; font-weight: bold; padding-left: 5px;");
        }
        ui->tableWidget->setCellWidget(i, 5, lblStatut);
    }
}

// قائمة الفرق وتعبئة قائمة المسابقات أوتوماتيكياً
void MainWindow::initialiserToutesLesEquipes() {
    toutesLesEquipes.append({"Matrix ReLoaded", "Cyber Security Jam", "Mariem Khedhri", "5/5", "25/10/2026", "Active"});
    toutesLesEquipes.append({"Bit Shifters", "IoT Challenge", "Omar Sghaier", "4/4", "26/10/2026", "Active"});
    toutesLesEquipes.append({"Kernel Panic", "DevOps Cup", "Nizar Chatti", "2/5", "27/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Syntax Error", "FinTech Hack", "Syrine Guesmi", "4/4", "28/10/2026", "Active"});
    toutesLesEquipes.append({"Null Pointer", "AI CodeSprint", "Alaaeddine Chebbi", "3/6", "29/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Segment Fault", "WebDev Masters", "Hiba Rezgui", "5/5", "30/10/2026", "Active"});
    toutesLesEquipes.append({"Deep Learning", "AI CodeSprint", "Bechir Tlili", "4/4", "31/10/2026", "Active"});
    toutesLesEquipes.append({"Quantum Hack", "Cloud Architecture", "Aziz Ferjani", "3/5", "01/11/2026", "Incomplète"});

    toutesLesEquipes.append({"Code Wizards", "Hack for a Better Future", "Ahmed Ben Ali", "5/5", "01/10/2026", "Active"});
    toutesLesEquipes.append({"Health Hackerz", "HealthTech Innovation", "Sara Trabelsi", "4/4", "02/10/2026", "Active"});
    toutesLesEquipes.append({"Edu Innovators", "EduTech Solutions", "Omar Khaled", "3/6", "03/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Smart Minds", "Smart City Challenge", "Meriem Ayari", "5/5", "04/10/2026", "Active"});
    toutesLesEquipes.append({"Aero Tech", "AeroTech for Tomorrow", "Yassine Jabri", "2/3", "05/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Green Energy", "Smart City Challenge", "Hamza Triki", "3/6", "16/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Alpha Squad", "AI CodeSprint", "Rim Riahi", "4/4", "06/10/2026", "Active"});
    toutesLesEquipes.append({"Beta Coders", "WebDev Masters", "Mehdi Ben Salah", "3/5", "07/10/2026", "Active"});
    toutesLesEquipes.append({"Cyber Ninjas", "Cyber Security Jam", "Amine Gharbi", "5/5", "08/10/2026", "Active"});
    toutesLesEquipes.append({"Data Miners", "AI CodeSprint", "Nour Chabbouh", "2/4", "09/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Cloud Nine", "Cloud Architecture", "Salma Kallel", "6/6", "10/10/2026", "Active"});
    toutesLesEquipes.append({"Pixel Perfect", "UI/UX Design Jam", "Malek Triki", "3/3", "11/10/2026", "Active"});
    toutesLesEquipes.append({"Byte Force", "IoT Challenge", "Skander Ben Ammar", "4/5", "12/10/2026", "Active"});
    toutesLesEquipes.append({"RoboTix", "Robotics Cup", "Farah Mejri", "5/5", "13/10/2026", "Active"});
    toutesLesEquipes.append({"Web Warriors", "WebDev Masters", "Zied Dridi", "2/4", "14/10/2026", "Incomplète"});
    toutesLesEquipes.append({"AlgoRhythm", "Competitive Programming", "Youssef Boughanmi", "3/3", "15/10/2026", "Active"});
    toutesLesEquipes.append({"InnoVibe", "FinTech Hack", "Chaima Sassi", "4/4", "17/10/2026", "Active"});
    toutesLesEquipes.append({"Neural Net", "AI CodeSprint", "Anas Ben Romdhane", "5/5", "18/10/2026", "Active"});
    toutesLesEquipes.append({"Fast Track", "Logistics Hack", "Wassim Jlassi", "3/4", "19/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Blue Lock", "Cyber Security Jam", "Rayen Loukil", "4/4", "20/10/2026", "Active"});
    toutesLesEquipes.append({"EcoHack", "Green Tech", "Emna Bouazizi", "3/5", "21/10/2026", "Incomplète"});
    toutesLesEquipes.append({"Titanium", "Hardware Jam", "Kais Mansour", "2/3", "22/10/2026", "Active"});
    toutesLesEquipes.append({"OmniDev", "FullStack Jam", "Rania Ben Hassine", "5/5", "23/10/2026", "Active"});
    toutesLesEquipes.append({"BitByBit", "Coding Marathon", "Ala Sfaxi", "4/4", "24/10/2026", "Active"});

    ui->comboBoxCompetition->clear();
    ui->comboBoxCompetition->addItem("Toutes");

    QStringList competitions;
    for (int i = 0; i < toutesLesEquipes.size(); ++i) {
        const Equipe &eq = toutesLesEquipes.at(i);
        if (!competitions.contains(eq.competition)) {
            competitions.append(eq.competition);
        }
    }
    ui->comboBoxCompetition->addItems(competitions);
}