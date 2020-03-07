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

#ifndef BOULE_H
#define BOULE_H

#include <QLabel>
class Conteneur;

class Boule : public QWidget
{
    Q_OBJECT
public:
    enum {ALEATOIRE = -1, LEGERE = 9, NORMALE = 10, LOURDE = 11};
    enum Etat {BOULE, GOOD_JOB, WELL_WELL_JOB, BAD_JOB, DAMN};
    explicit Boule(QWidget *parent = 0);
    explicit Boule(Etat etat);
    explicit Boule(int num, int masse, Conteneur *conteneur = 0, QWidget *parent = 0);

    static QPixmap renderPixmaps(Etat etat, int blink, QSize size);

    void setDefaultPolicy();
    void setNum(int num);
    void setMasse(int masse);
    void setEtat(Etat etat);
    int num();
    int masse();

    QSize minimumSizeHint() const { return QSize(32, 32); }
    QSize sizeHint() const { return QSize(64, 64); }
    int heightForWidth(int w) const { return w; }
protected:
    void drawWidget(QPaintDevice *device, QSize size, bool use_cache = true);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    int m_num;
    int m_masse;
    Conteneur *m_conteneur;
    Etat m_etat;

    enum Eyes {OPENED, CLOSED};
    Eyes m_eyes;

    QPixmap m_pixmap;
    bool m_cache_invalid;

    // EASTER_EGGS
    bool m_block;
    int m_count;

protected slots:
    void updateEyes();
};

#endif // BOULE_H
