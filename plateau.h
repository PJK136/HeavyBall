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

#ifndef PLATEAU_H
#define PLATEAU_H

#include <QGroupBox>
#include <QHBoxLayout>
#include "conteneur.h"
#include "boule.h"

class Plateau : public Conteneur
{
    Q_OBJECT
public:
    explicit Plateau(QWidget *parent = 0);
    int getRowCount();
    virtual void ajusterLayout();
    int getOptimalHeight();
    void viderBoule();
    int getMasse();
    bool operator >(Plateau &b);
    bool operator <(Plateau &b);
    bool operator ==(Plateau &b);
};

#endif // PLATEAU_H
