/* Copyright © 2013-2014 Paul DU
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
class ChoixNiveau;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void menuNiveau();
    void choisirNiveau(int niveau);
    void initGame();
    void peser();
    void askReponse();
    bool askNombreBoules(bool init = true);
    bool askNombrePesees(bool init = true);
    bool askMasseDefectueuse();
    void setResetBalance(bool reset);
    void setAleatoire(bool checked);
    void setPlusLegere(bool checked);
    void setPlusLourde(bool checked);
    void setNotMultiple(bool checked);
    void setCanBeMultiple(bool checked);
    void setMustBeMultiple(bool checked);
    void askCoefficient();
    void ajusterPlateaux();
    void resetBalance();
    void viderGauche();
    void viderDroit();
    void aPropos();

protected:
    Ui::MainWindow *ui;
    QWidget *choix_niveau;
    Ui::ChoixNiveau *choix_niveau_ui;
    int m_nombre_boules;
    QVector<class Boule*> m_boules;
    class QLabel *m_last_pesee;
    class QLabel *m_label_pesees;
    int m_nombre_pesees_initial;
    int m_pesees_restantes;
    int m_boule;
    bool m_aleatoire;
    int m_coefficient;
    int m_intervalle_debut;
    int m_intervalle_fin;
};

#endif // MAINWINDOW_H

