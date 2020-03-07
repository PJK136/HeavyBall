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

#ifndef CONTENEUR_H
#define CONTENEUR_H

#include <QGroupBox>
#include <QGridLayout>
#include "boule.h"

class Conteneur : public QGroupBox
{
    Q_OBJECT
public:
    explicit Conteneur(QWidget *parent = 0);
    virtual void setColumnCount(unsigned int column_count);
    virtual void ajouterBoule(Boule *boule);
    virtual void ajusterLayout();
    virtual bool hasBoules();
    virtual QString toString();
    virtual void deplacerBoules(Conteneur *b);
signals:
    void conteneurChanged();
public slots:

protected:
    virtual void showEvent(QShowEvent *);
    virtual void dragEnterEvent(QDragEnterEvent *);
    virtual void dragLeaveEvent(QDragLeaveEvent *);
    virtual void dropEvent(QDropEvent *);
    QGridLayout m_layout;
    unsigned int m_column_count;
    Boule *m_dragged_boule;
    Conteneur *m_dragged_boule_conteneur;
};

#endif // CONTENEUR_H
