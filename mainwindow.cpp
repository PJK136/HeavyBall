/* Copyright © 2013-2014 Paul DU ²
 * This file is part of Heavy Ball.

 * Heavy Ball is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Heavy Ball is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Heavy Ball.  If not, see <http://www.gnu.org/licenses/>. */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_choix_niveau.h"
#include <QSvgWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include "resultat.h"
#include "boule.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    choix_niveau(new QWidget(this)),
    choix_niveau_ui(new Ui::ChoixNiveau),
    m_last_pesee(new QLabel(this)),
    m_label_pesees(new QLabel(this)),
    m_aleatoire(true),
    m_coefficient(2),
    m_intervalle_debut(2),
    m_intervalle_fin(12)
{
    ui->setupUi(this);
    choix_niveau_ui->setupUi(choix_niveau);
    choix_niveau_ui->gridMessage->addWidget(choix_niveau_ui->label, 0, 1, 1, 1, Qt::AlignCenter);
    choix_niveau_ui->gridMessage->addWidget(new Boule(Boule::Etat::GOOD_JOB), 0, 0, 1, 1, Qt::AlignCenter);
    choix_niveau_ui->gridMessage->addWidget(new Boule(Boule::Etat::BAD_JOB), 0, 2, 1, 1, Qt::AlignCenter);

    connect(choix_niveau_ui->niveau_1, &QPushButton::clicked, this, [this](){choisirNiveau(1);});
    connect(choix_niveau_ui->niveau_2, &QPushButton::clicked, this, [this](){choisirNiveau(2);});
    connect(choix_niveau_ui->niveau_3, &QPushButton::clicked, this, [this](){choisirNiveau(3);});
    connect(choix_niveau_ui->niveau_4, &QPushButton::clicked, this, [this](){choisirNiveau(4);});
    connect(choix_niveau_ui->niveau_perso, &QPushButton::clicked, this, [this](){choisirNiveau(-1);});

    choix_niveau_ui->spacer2->setSprite(":/img/boule.svg", QSize(100,100), 1, 1);
    choix_niveau_ui->spacer3->setSprite(":/img/boule.svg", QSize(100,100), 1, 1);
    choix_niveau_ui->spacer4->setSprite(":/img/boule.svg", QSize(100,100), 1, 1);
    choix_niveau_ui->spacer3->setOrientation(false);
    choix_niveau_ui->spacer4->setOrientation(false);
    choix_niveau_ui->spacer2->setAlignement(Qt::AlignBottom);
    choix_niveau_ui->spacer3->setAlignement(Qt::AlignRight);
    choix_niveau_ui->spacer4->setAlignement(Qt::AlignLeft);

    ui->statusBar->addWidget(m_last_pesee);
    ui->statusBar->addPermanentWidget(m_label_pesees);
    m_nombre_pesees_initial = 3;
    m_nombre_boules = 12;
    ui->plateau_1->layout()->setContentsMargins(2,0,1,0);
    ui->plateau_2->layout()->setContentsMargins(1,0,2,0);
    QSvgWidget *centre = new QSvgWidget(":/img/balance_centre.svg", centralWidget());
    centre->setMinimumSize(QSize(64, 217));
    centre->setMaximumSize(QSize(64, 16777215));
    ui->gridLayout->addWidget(centre, 7, 1, 1, 1, Qt::AlignHCenter);
    menuNiveau();
}

MainWindow::~MainWindow()
{
    for (Boule *boule : m_boules)
        delete boule;
    if (centralWidget() != ui->centralWidget)
        delete ui->centralWidget;
    delete ui;
}

void MainWindow::menuNiveau()
{
    m_label_pesees->setText("");
    m_last_pesee->setText("");
    takeCentralWidget();
    setCentralWidget(choix_niveau);
}

void MainWindow::choisirNiveau(int niveau)
{
    switch (niveau)
    {
    case 1:
        m_nombre_boules = 6;
        m_nombre_pesees_initial = 2;
        ui->actionPlus_lourde->setChecked(true);
        ui->actionN_est_pas_multiple->setChecked(true);
        break;
    case 2:
        m_nombre_boules = 9;
        m_nombre_pesees_initial = 2;
        ui->actionPlus_lourde->setChecked(true);
        ui->actionN_est_pas_multiple->setChecked(true);
        break;
    case 3:
        m_nombre_boules = 9;
        m_nombre_pesees_initial = 3;
        ui->actionAl_atoire->setChecked(true);
        ui->actionN_est_pas_multiple->setChecked(true);
        break;
    case 4:
        m_nombre_boules = 12;
        m_nombre_pesees_initial = 3;
        ui->actionAl_atoire->setChecked(true);
        ui->actionN_est_pas_multiple->setChecked(true);
        break;
    default:
        if (!askNombreBoules(false) || !askNombrePesees(false) || !askMasseDefectueuse())
            return;
        break;
    }
    initGame();
}

void MainWindow::initGame()
{
    takeCentralWidget();
    setCentralWidget(ui->centralWidget);

    for (Boule *boule : m_boules)
        delete boule;
    m_boules.clear();

    if (m_nombre_boules <= 6 || m_nombre_boules >= 12)
        ui->conteneur->setColumnCount(6);
    else
        ui->conteneur->setColumnCount(ceil(m_nombre_boules/2.f));

    for (int i = 0; i < m_nombre_boules; i++)
    {
        m_boules.append(new Boule(i+1, Boule::NORMALE, ui->conteneur));
        ui->conteneur->ajouterBoule(m_boules.last());
    }

    m_boule = qrand() % m_nombre_boules;
    if ((ui->actionAl_atoire->isChecked() && qrand() % 2) || ui->actionPlus_l_g_re->isChecked())
        m_boules[m_boule]->setMasse(Boule::LEGERE);
    else
    {
        if (ui->actionEst_un_multiple->isChecked() || (ui->actionPeut_etre_un_multiple->isChecked() && qrand() % 2)) // Si elle peut l'être seulement, alors on tire pour 1 chance 2 pour savoir
        {
            if (m_aleatoire)
            {
                if (m_intervalle_debut >= m_intervalle_fin)
                    m_intervalle_debut = m_intervalle_fin - 1;

                int coefficient = qrand() % (m_intervalle_fin - m_intervalle_debut) + m_intervalle_debut;
                m_boules[m_boule]->setMasse(Boule::NORMALE * coefficient);
            }
            else
                m_boules[m_boule]->setMasse(Boule::NORMALE * m_coefficient);
        }
        else
            m_boules[m_boule]->setMasse(Boule::LOURDE);
    }

    m_pesees_restantes = m_nombre_pesees_initial;
    ajusterPlateaux();
    resetBalance();
    m_label_pesees->setText("Pesées restantes : " + QString::number(m_pesees_restantes));
    ui->pesee->setEnabled(true);
    ui->donner_reponse->setText("Donner la réponse");
    disconnect(ui->donner_reponse, 0, this, 0);
    connect(ui->donner_reponse, SIGNAL(clicked()), this, SLOT(askReponse()));
}

void MainWindow::peser()
{
    if (!ui->plateau_1->hasBoules())
    {
        QMessageBox::critical(this, "Pas de boule", "Il n'y a pas de boule sur le plateau de gauche !");
        return;
    }
    else if (!ui->plateau_2->hasBoules())
    {
        QMessageBox::critical(this, "Pas de boule", "Il n'y a pas de boule sur le plateau de droite !");
        return;
    }
    else
    {
        m_pesees_restantes--;
        resetBalance();
        if (ui->plateau_1->operator >(*ui->plateau_2))
        {
            ui->pied_gauche_bas->changeSize(0,10);
            ui->pied_droite_haut->changeSize(0,10);
            ui->layout_gauche->invalidate();
            ui->layout_droite->invalidate();
            m_last_pesee->setText("Le plateau (" + ui->plateau_1->toString() + ") est plus lourd que le plateau (" + ui->plateau_2->toString() + ").");
        }
        else if (ui->plateau_1->operator <(*ui->plateau_2))
        {
            ui->pied_gauche_haut->changeSize(0,10);
            ui->pied_droite_bas->changeSize(0,10);
            ui->layout_gauche->invalidate();
            ui->layout_droite->invalidate();
            m_last_pesee->setText("Le plateau (" + ui->plateau_1->toString() + ") est plus léger que le plateau (" + ui->plateau_2->toString() + ").");
        }
        else
        {
            m_last_pesee->setText("Les deux plateaux (" + ui->plateau_1->toString() + ") et (" + ui->plateau_2->toString() + ") ont la même masse.");
            QMessageBox::information(this, "Masse égale", "Les deux plateaux ont la même masse !");
        }

        m_label_pesees->setText("Pesées restantes : " + QString::number(m_pesees_restantes));
        if (m_pesees_restantes <= 0)
        {
            ui->pesee->setDisabled(true);
        }
    }
}

void MainWindow::askReponse()
{
    bool ok = false;
    int reponse = QInputDialog::getInt(this, "Donner la réponse", "Boule n° :", 1, 1, m_nombre_boules, 1, &ok);
    if (!ok)
        return;

    bool masse = false;
    if (ui->actionAl_atoire->isChecked())
    {
        QMessageBox messagebox(QMessageBox::Question, "Donner la réponse", "Sa masse ?", QMessageBox::Yes|QMessageBox::No, this);
        messagebox.setButtonText(QMessageBox::Yes, "Plus légère");
        messagebox.setButtonText(QMessageBox::No, "Plus lourde");
        masse = messagebox.exec() == QMessageBox::Yes;
    }
    else
        masse = ui->actionPlus_l_g_re->isChecked();

    if (ok)
    {
        if (reponse == m_boule + 1)
        {
            if ((m_boules[m_boule]->masse() < Boule::NORMALE && masse) || (m_boules[m_boule]->masse() > Boule::NORMALE && !masse))
            {
                QString message = "En effet, la boule n°" + QString::number(m_boule+1) + QString(" est plus ") +
                        (m_boules[m_boule]->masse() < Boule::NORMALE ? "légère" : "lourde") + QString(" que les autres boules.");
                m_last_pesee->setText(message);
                Resultat resultat(Boule::GOOD_JOB, message);
                resultat.exec();
                for (Boule *boule : m_boules)
                    boule->setEtat(Boule::GOOD_JOB);
            }
            else
            {
                QString message = "C'est bien la boule n°" + QString::number(m_boule+1) + QString(" mais elle est plus ") +
                        (m_boules[m_boule]->masse() < Boule::NORMALE ? "légère" : "lourde") + QString(" que les autres boules.");
                m_last_pesee->setText(message);
                Resultat resultat(Boule::WELL_WELL_JOB, message);
                resultat.exec();
                for (Boule *boule : m_boules)
                    boule->setEtat(Boule::WELL_WELL_JOB);
            }
        }
        else
        {
            QString message = "C'était la boule n°" + QString::number(m_boule+1) + QString(" qui était plus ") +
                    (m_boules[m_boule]->masse() < Boule::NORMALE ? "légère" : "lourde") + QString(" que les autres boules.");
            m_last_pesee->setText(message);
            Resultat resultat(Boule::BAD_JOB, message);
            resultat.exec();
            for (Boule *boule : m_boules)
                boule->setEtat(Boule::BAD_JOB);
        }
        ui->donner_reponse->setText("Nouvelle partie");
        disconnect(ui->donner_reponse, 0, this, 0);
        connect(ui->donner_reponse, SIGNAL(clicked()), this, SLOT(menuNiveau()));
    }
}

bool MainWindow::askNombreBoules(bool init)
{
    bool ok = false;
    int nb = QInputDialog::getInt(this, "Nombre de boules", "Nombre de boules :", m_nombre_boules, 3, 214783647, 1, &ok);
    if (!ok)
        return false;

    m_nombre_boules = nb;
    if (m_coefficient > m_nombre_boules - 1)
    {
        m_coefficient = m_nombre_boules - 1;
        if (m_aleatoire == false && (ui->actionPeut_etre_un_multiple->isChecked() || ui->actionEst_un_multiple->isChecked()))
            QMessageBox::information(this, "Coefficient ajusté", QString("Le coefficient de la boule plus lourde a été mise sur %1.").arg(QString::number(m_coefficient)));
    }

    bool ajuste = false;
    if (m_intervalle_fin > m_nombre_boules)
    {
        m_intervalle_fin = m_nombre_boules;
        ajuste = true;
    }
    if (m_intervalle_debut > m_nombre_boules)
    {
        m_intervalle_debut = m_nombre_boules - 1;
        ajuste = true;
    }
    if (ajuste && m_aleatoire && (ui->actionPeut_etre_un_multiple->isChecked() || ui->actionEst_un_multiple->isChecked()))
        QMessageBox::information(this, "Intervalle ajusté", QString("L'intervale de la boule plus lourde a été mise sur [%1;%2[.").arg(QString::number(m_intervalle_debut)).arg(QString::number(m_intervalle_fin)));

    if (init)
        initGame();

    return true;
}

bool MainWindow::askNombrePesees(bool init)
{
    bool ok = false;
    int nb = QInputDialog::getInt(this, "Nombre de pesées initial", "Nombre de pesées initial :", m_nombre_pesees_initial, 2, 214783647, 1, &ok);
    if (!ok)
        return false;

    m_nombre_pesees_initial = nb;

    if (init)
        initGame();

    return true;
}

bool MainWindow::askMasseDefectueuse()
{
    bool ok = false;
    QString item = QInputDialog::getItem(this, "Boule défectueuse", "Boule défectueuse :", QStringList({"Aléatoire", "Plus légère", "Plus lourde"}), 0, false, &ok);
    if (!ok)
        return false;

    if (item == "Aléatoire")
        ui->actionAl_atoire->setChecked(true);
    else if (item == "Plus légère")
        ui->actionPlus_l_g_re->setChecked(true);
    else
        ui->actionPlus_lourde->setChecked(true);

    return true;
}

void MainWindow::setResetBalance(bool reset)
{
    if (reset)
    {
        connect(ui->plateau_1, SIGNAL(conteneur_changed()), this, SLOT(resetBalance()));
        connect(ui->plateau_2, SIGNAL(conteneur_changed()), this, SLOT(resetBalance()));
    }
    else
    {
        disconnect(ui->plateau_1,SIGNAL(conteneur_changed()), this, SLOT(resetBalance()));
        disconnect(ui->plateau_2,SIGNAL(conteneur_changed()), this, SLOT(resetBalance()));
    }
}

void MainWindow::setAleatoire(bool checked)
{
    if (checked)
    {
        if (ui->actionPlus_l_g_re->isChecked())
            ui->actionPlus_l_g_re->setChecked(false);
        if (ui->actionPlus_lourde->isChecked())
            ui->actionPlus_lourde->setChecked(false);
        initGame();
    }
    else if (!ui->actionAl_atoire->isChecked() && !ui->actionPlus_l_g_re->isChecked() && !ui->actionPlus_lourde->isChecked())
        ui->actionAl_atoire->setChecked(true);
}

void MainWindow::setPlusLegere(bool checked)
{
    if (checked)
    {
        if (ui->actionAl_atoire->isChecked())
            ui->actionAl_atoire->setChecked(false);
        if (ui->actionPlus_lourde->isChecked())
            ui->actionPlus_lourde->setChecked(false);
        initGame();
    }
    else if (!ui->actionAl_atoire->isChecked() && !ui->actionPlus_l_g_re->isChecked() && !ui->actionPlus_lourde->isChecked())
        ui->actionPlus_l_g_re->setChecked(true);
}

void MainWindow::setPlusLourde(bool checked)
{
    if (checked)
    {
        if (ui->actionAl_atoire->isChecked())
            ui->actionAl_atoire->setChecked(false);
        if (ui->actionPlus_l_g_re->isChecked())
            ui->actionPlus_l_g_re->setChecked(false);
        initGame();
    }
    else if (!ui->actionAl_atoire->isChecked() && !ui->actionPlus_l_g_re->isChecked() && !ui->actionPlus_lourde->isChecked())
        ui->actionPlus_lourde->setChecked(true);
}

void MainWindow::setNotMultiple(bool checked)
{
    if (checked)
    {
        if (ui->actionPeut_etre_un_multiple->isChecked())
            ui->actionPeut_etre_un_multiple->setChecked(false);
        if (ui->actionEst_un_multiple->isChecked())
            ui->actionEst_un_multiple->setChecked(false);
        ui->actionD_finir_coefficient->setEnabled(false);
        initGame();
    }
    else if (!ui->actionN_est_pas_multiple->isChecked() && !ui->actionPeut_etre_un_multiple->isChecked() && !ui->actionEst_un_multiple->isChecked())
            ui->actionN_est_pas_multiple->setChecked(true);
}

void MainWindow::setCanBeMultiple(bool checked)
{
    if (checked)
    {
        if (ui->actionN_est_pas_multiple->isChecked())
            ui->actionN_est_pas_multiple->setChecked(false);
        if (ui->actionEst_un_multiple->isChecked())
            ui->actionEst_un_multiple->setChecked(false);
        ui->actionD_finir_coefficient->setEnabled(true);
        initGame();
    }
    else if (!ui->actionN_est_pas_multiple->isChecked() && !ui->actionPeut_etre_un_multiple->isChecked() && !ui->actionEst_un_multiple->isChecked())
            ui->actionPeut_etre_un_multiple->setChecked(true);
}

void MainWindow::setMustBeMultiple(bool checked)
{
    if (checked)
    {
        if (ui->actionN_est_pas_multiple->isChecked())
            ui->actionN_est_pas_multiple->setChecked(false);
        if (ui->actionPeut_etre_un_multiple->isChecked())
            ui->actionPeut_etre_un_multiple->setChecked(false);
        ui->actionD_finir_coefficient->setEnabled(true);
        initGame();
    }
    else if (!ui->actionN_est_pas_multiple->isChecked() && !ui->actionPeut_etre_un_multiple->isChecked() && !ui->actionEst_un_multiple->isChecked())
            ui->actionEst_un_multiple->setChecked(true);
}

void MainWindow::askCoefficient()
{
    QMessageBox::StandardButton aleatoire = QMessageBox::question(this, "Définir le coefficient", "Voulez-vous un coefficient aléatoire ?", QMessageBox::Yes|QMessageBox::No);
    if (aleatoire == QMessageBox::Yes)
    {
        m_aleatoire = true;
        bool ok = false;
        int debut = QInputDialog::getInt(this, "Définir le coefficient", "Début de l'intervalle (compris) :", m_intervalle_debut, 2, m_nombre_boules-1, 1, &ok);
        if (!ok)
        {
            m_intervalle_debut = 2;
            m_intervalle_fin = m_nombre_boules;
            QMessageBox::information(this, "Définir de la coefficient", QString("L'intervalle est mise par défaut sur : [2;%1[.").arg(QString::number(m_nombre_boules)));
            return;
        }

        m_intervalle_debut = debut;
        int fin = QInputDialog::getInt(this, "Définir le coefficient", "Fin de l'intervalle (exclus) :", m_intervalle_fin, m_intervalle_debut+1, m_nombre_boules, 1, &ok);
        if (!ok)
            m_intervalle_fin = m_nombre_boules;
        else
            m_intervalle_fin = fin;

        QMessageBox::information(this, "Définir de la coefficient", QString("L'intervalle est mise sur : [%1;%2[.").arg(QString::number(m_intervalle_debut)).arg(QString::number(m_intervalle_fin)));
        initGame();
    }
    else
    {
        m_aleatoire = false;
        bool ok = false;
        int coefficient = QInputDialog::getInt(this, "Définir le coefficient", "Coefficient ?", m_coefficient, 2, m_nombre_boules-1, 1, &ok);
        if (ok)
        {
            m_coefficient = coefficient;
            initGame();
        }
    }
}

void MainWindow::resetBalance()
{
    ui->pied_gauche_haut->changeSize(20,40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pied_gauche_bas->changeSize(20,40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pied_droite_haut->changeSize(20,40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->pied_droite_bas->changeSize(20,40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->layout_gauche->invalidate();
    ui->layout_droite->invalidate();
    ui->centralWidget->layout()->invalidate();
}

void MainWindow::ajusterPlateaux()
{
    int max = qMax(ui->plateau_1->getOptimalHeight(), ui->plateau_2->getOptimalHeight());
    ui->plateau_1->setFixedHeight(max);
    ui->plateau_2->setFixedHeight(max);
}

void MainWindow::viderGauche()
{
    ui->plateau_1->deplacerBoules(ui->conteneur);
}

void MainWindow::viderDroit()
{
    ui->plateau_2->deplacerBoules(ui->conteneur);
}

void MainWindow::aPropos()
{
    QMessageBox msgBox(this);
    msgBox.setIconPixmap(QPixmap(":/img/boule.svg"));
    msgBox.setWindowTitle("À propos de Heavy Ball");
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("<h1 align='center'><strong>Heavy Ball<br/>Version 2.2.1</strong></h1>"\
                   "<p align='center'><strong>Heavy Ball</strong> est un jeu dont le but est de trouver la boule qui est plus légère ou plus lourde parmi les autres.<br/><br />"\
                   "Logiciel conçu en <strong>C++</strong> avec le framework <strong>Qt 5.9</strong>.<br/><br/>"
                   "Pour toute suggesstion ou rapport de bug, vous pouvez adresser un mail à <a href='mailto:du.paul136@gmail.com'>du.paul136@gmail.com</a></p>"\
                   "<p align='center'>Copyright © 2013-2017 Paul DU pour le programme<br/>Clément MARTINEZ pour les graphismes<p>"\
                   "<p align='justify'>Ce programme est un logiciel libre ; vous pouvez le redistribuer ou le modifier suivant les termes de la GNU General Public License telle que publiée par la Free Software Foundation ; soit la version 3 de la licence, soit (à votre gré) toute version ultérieure.<br/>"\
                   "Ce programme est distribué dans l'espoir qu'il sera utile, mais SANS AUCUNE GARANTIE ; pas même la garantie implicite de COMMERCIABILISABILITÉ ni d'ADÉQUATION à UN OBJECTIF PARTICULIER.</p>"\
                   "<p align='center'>Consultez la <em>GNU General Public License</em> pour plus de détails.</p>");
    msgBox.exec();
}



